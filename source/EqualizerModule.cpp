#include "EqualizerModule.h"
#include "ValueConversion.h"
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif
#include <cmath>
#include <string>

#define LIMITER

#ifdef LIMITER
// Max und min values at double precision: http://steve.hollasch.net/cgindex/coding/ieeefloat.html
// � ~10^(-323.3) to ~10^(308.3)
const double maxSampleValue = pow(10, 300);
const double minSampleValue = -pow(10, 300);
#endif

EqualizerModule::EqualizerModule(FilterType filterType, double centerFrequency, double qFactor, double gain)
	: samplingFreq(0.0)
	, centerFreq(centerFrequency)
	, qFactor(qFactor)
	, gain(gain)
	, filterType(filterType)
{
	// Initialize everything
	xn0 = 0.0;
	xn1 = 0.0;
	xn2 = 0.0;
	yn1 = 0.0;
	yn2 = 0.0;
	
	K = 0.0;
	oneDividedByQ = 1 / qFactor;

	calculateCoefficients();
}

EqualizerModule::~EqualizerModule() 
{
}

// Source: Z�lzer book p.136+137
// Alle WURZEL(2)-Terme werden durch 1/qFactor ersetzt!
void EqualizerModule::calculateCoefficients() {
	// tan() has a discontinuity at 0.5*PI
	if ((centerFreq / samplingFreq) < 0.5) {
		K = tan((centerFreq / samplingFreq) * M_PI);
	}
	else {
		K = tan(0.4999 * M_PI);
	}

	switch (filterType) {
	case FilterType::lowPass: {
		double denominator = 1 + oneDividedByQ * K + pow(K, 2);
		a0 = (pow(K, 2)) / denominator;
		a1 = (2 * pow(K, 2)) / denominator;
		a2 = a0;
		b1 = (2 * (pow(K, 2) - 1)) / denominator;
		b2 = (1 - oneDividedByQ * K + pow(K, 2)) / denominator;
		break;
	}
	case FilterType::highPass: {
		double denominator = 1 + oneDividedByQ * K + pow(K, 2);
		a0 = 1 / denominator;
		a1 = -2 / denominator;
		a2 = a0;
		b1 = (2 * (pow(K, 2) - 1)) / denominator;
		b2 = (1 - oneDividedByQ * K + pow(K, 2)) / denominator;
		break;
	}
	case FilterType::bandPassStop: {
		if (gain >= 1.0) {
			// Band pass
			double denominator = 1 + oneDividedByQ * K + pow(K, 2);
			a0 = (1 + gain * oneDividedByQ * K + pow(K, 2)) / denominator;
			a1 = (2 * (pow(K, 2) - 1)) / denominator;
			a2 = (1 - gain * oneDividedByQ * K + pow(K, 2)) / denominator;
			b1 = a1;
			b2 = (1 - oneDividedByQ * K + pow(K, 2)) / denominator;		
		}
		else{
			// Band stop
			// Switch sign of log gain value
			double switchedGain = ValueConversion::logToLinear(-ValueConversion::linearToLog(gain));
			double denominator = 1 + switchedGain * oneDividedByQ * K + pow(K, 2);
			a0 = (1 + oneDividedByQ * K + pow(K, 2)) / denominator;
			a1 = (2 * (pow(K, 2) - 1)) / denominator;
			a2 = (1 - oneDividedByQ * K + pow(K, 2)) / denominator;
			b1 = a1;
			b2 = (1 - switchedGain * oneDividedByQ  * K + pow(K, 2)) / denominator;
		}
		
		break;
	}
	case FilterType::lowShelf: {
		// Increasing low shelf filter
		if (gain >= 1.0) {
			double denominator = 1 + oneDividedByQ * K + pow(K, 2);
			a0 = (1 + oneDividedByQ * sqrt(gain) * K + gain * pow(K, 2)) / denominator;
			a1 = (2 * (gain * pow(K, 2) - 1)) / denominator;
			a2 = (1 - oneDividedByQ * sqrt(gain) * K + gain * pow(K, 2)) / denominator;
			b1 = (2 * (pow(K, 2) - 1)) / denominator;
			b2 = (1 - oneDividedByQ * K + pow(K, 2)) / denominator;
		}
		// Decreasing low shelf filter
		else {
			// Switch sign of log gain value
			double switchedGain = ValueConversion::logToLinear(-ValueConversion::linearToLog(gain));
			double denominator = 1 + oneDividedByQ * sqrt(switchedGain) * K + switchedGain * pow(K, 2);
			a0 = (1 + oneDividedByQ * K + pow(K, 2)) / denominator;
			a1 = (2 * (pow(K, 2) - 1)) / denominator;
			a2 = (1 - oneDividedByQ * K + pow(K, 2)) / denominator;
			b1 = (2 * (switchedGain * pow(K, 2) - 1)) / denominator;
			b2 = (1 - oneDividedByQ * sqrt(switchedGain) * K + switchedGain * pow(K, 2)) / denominator;
		}
			break;
	}
	case FilterType::highShelf: {
		// Increasing high shelf filter
		if (gain >= 1.0) {
			double denominator = 1 + oneDividedByQ * K + pow(K, 2);
			a0 = (gain + oneDividedByQ * sqrt(gain) * K + pow(K, 2)) / denominator;
			a1 = (2 * (pow(K, 2) - gain)) / denominator;
			a2 = (gain - oneDividedByQ * sqrt(gain) * K + pow(K, 2)) / denominator;
			b1 = (2 * (pow(K, 2) - 1)) / denominator;
			b2 = (1 - oneDividedByQ * K + pow(K, 2)) / denominator;
		}
		// Decreasing high shelf filter
		else {
			// Switch sign of log gain value
			double switchedGain = ValueConversion::logToLinear(-ValueConversion::linearToLog(gain));
			double aDenominator = switchedGain + oneDividedByQ * sqrt(switchedGain) * K + pow(K, 2);
			double bDenominator = 1 + oneDividedByQ / sqrt(switchedGain) * K + (pow(K, 2) / switchedGain);
			a0 = (1 + oneDividedByQ * K + pow(K, 2)) / aDenominator;
			a1 = (2 * (pow(K, 2) - 1)) / aDenominator;
			a2 = (1 - oneDividedByQ * K + pow(K, 2)) / aDenominator;
			b1 = (2 * (pow(K, 2) / switchedGain - 1)) / bDenominator;
			b2 = (1 - oneDividedByQ / sqrt(switchedGain) * K + (pow(K, 2) / switchedGain)) / bDenominator;
		}
		break;
	}
	case FilterType::rawBiquad: {
		break;
	}
	default: {
		a0 = 0.0;
		a1 = 0.0;
		a2 = 0.0;
		b1 = 0.0;
		b2 = 0.0;
		break;
	}
	}
}

void EqualizerModule::setCenterFreq(const double& f0) {
	if (f0 <= ValueConversion::getMaxEqFrequency()) {
		centerFreq = f0;
		calculateCoefficients();
	}
	else {
		centerFreq = ValueConversion::getMaxEqFrequency();
	}
}

void EqualizerModule::setFilterCoefficient(const FilterCoefficients coefficient, const double& value) {
	
	if (filterType == FilterType::rawBiquad) {
		switch (coefficient) {
		case FilterCoefficients::a0:
			a0 = value;
			break;
		case FilterCoefficients::a1:
			a1 = value;
			break;
		case FilterCoefficients::a2:
			a2 = value;
			break;
		case FilterCoefficients::b1:
			b1 = value;
			break;
		case FilterCoefficients::b2:
			b2 = value;
			break;
		default:
			break;
		}
		// Biquad stability condition: http://nrlug.puhep.res.in/GLUE/Packages/engg/DSP/book/node75.html
		//if (...)
	}
}

void EqualizerModule::processSample(double& sample) {
	// Save current input sample
	xn0 = sample;

	// Difference Equation
	sample = a0 * sample + a1 * xn1 + a2 * xn2 - b1 * yn1 - b2 * yn2;

	#ifdef LIMITER
	if (sample > maxSampleValue) sample = maxSampleValue;
	if (sample < minSampleValue) sample = minSampleValue;
	#endif
	
	// Shift samples
	xn2 = xn1;
	xn1 = xn0;
	yn2 = yn1;
	yn1 = sample;
}
