#include "EqualizerModule.h"
#define _USE_MATH_DEFINES
#include <cmath>

EqualizerModule::EqualizerModule(FilterType filterType, double samplingFreq, double centerFreq, double qFactor, double dBgain)
	: samplingFreq(samplingFreq)
	, centerFreq(centerFreq)
	, qFactor(qFactor)
	, dBgain(dBgain)
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

	K = tan((centerFreq / samplingFreq) * M_PI);

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
	case FilterType::bandPass: {
		double denominator = 1 + oneDividedByQ * K + pow(K, 2);
		a0 = (1 + gain * oneDividedByQ * K + pow(K, 2)) / denominator;
		a1 = (2 * (pow(K, 2) - 1)) / denominator;
		a2 = (1 - gain * oneDividedByQ * K + pow(K, 2)) / denominator;
		b1 = a1;
		b2 = (1 - oneDividedByQ * K + pow(K, 2)) / denominator;
		break;
	}
	case FilterType::bandStop: {
		double denominator = 1 + gain * oneDividedByQ * K + pow(K, 2);
		a0 = (1 + oneDividedByQ * K + pow(K, 2)) / denominator;
		a1 = (2 * (pow(K, 2) - 1)) / denominator;
		a2 = (1 - oneDividedByQ * K + pow(K, 2)) / denominator;
		b1 = a1;
		b2 = (1 - gain * oneDividedByQ  * K + pow(K, 2)) / denominator;
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
			double denominator = 1 + oneDividedByQ * sqrt(gain) * K + gain * pow(K, 2);
			a0 = (1 + oneDividedByQ * K + pow(K, 2)) / denominator;
			a1 = (2 * (pow(K, 2) - 1)) / denominator;
			a2 = (1 - oneDividedByQ * K + pow(K, 2)) / denominator;
			b1 = (2 * (gain * pow(K, 2) - 1)) / denominator;
			b2 = (1 - oneDividedByQ * sqrt(gain) * K + gain * pow(K, 2)) / denominator;
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
			double aDenominator = gain + oneDividedByQ * sqrt(gain) * K + pow(K, 2);
			double bDenominator = 1 + oneDividedByQ / sqrt(gain) * K + (pow(K, 2) / gain);
			a0 = (1 + oneDividedByQ * K + pow(K, 2)) / aDenominator;
			a1 = (2 * (pow(K, 2) - 1)) / aDenominator;
			a2 = (1 - oneDividedByQ * K + pow(K, 2)) / aDenominator;
			b1 = (2 * (pow(K, 2) / gain - 1)) / bDenominator;
			b2 = (1 - oneDividedByQ / sqrt(gain) * K + (pow(K, 2) / gain)) / bDenominator;
		}
		break;
	}
	}
	
}

void EqualizerModule::processSample(double& sample) {
	// Save current input sample
	xn0 = sample;

	// Difference Equation
	sample = a0 * sample + a1 * xn1 + a2 * xn2 - b1 * yn1 - b2 * yn2;
	
	// Shift samples
	xn2 = xn1;
	xn1 = xn0;
	yn2 = yn1;
	yn1 = sample;
}