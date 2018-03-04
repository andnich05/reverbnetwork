/*
* EqualizerModule: Biquad filter for frequency manipulation
*
* Copyright (C) 2015  Andrej Nichelmann
*                     Klaus Michael Indlekofer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "EqualizerModule.h"
#include "ValueConversion.h"
#include "ReverbNetworkDefines.h"
#include <cmath>

// Limit output values when the filter becomes unstable
//#define LIMITER

//#ifdef LIMITER
// Max und min values at double precision: http://steve.hollasch.net/cgindex/coding/ieeefloat.html
// ± ~10^(-323.3) to ~10^(308.3)
const double maxSampleValue = 1.0e100;
const double minSampleValue = -1.0e100;
//#endif

EqualizerModule::EqualizerModule(FilterType filterType, double centerFrequency, double qFactor, double gain)
	: samplingFreq(0.0)
	, centerFreq(centerFrequency)
	, qFactor(qFactor)
	, gain(gain)
	, filterType(filterType)
	, stable(false)
	, limit(true)
{
	// Initialize everything
	xn0 = 0.0;
	xn1 = 0.0;
	xn2 = 0.0;
	yn1 = 0.0;
	yn2 = 0.0;

	a0 = 0.0;
	a1 = 0.0;
	a2 = 0.0;
	b1 = 0.0;
	b2 = 0.0;
	
	K = 0.0;
	oneDividedByQ = 1 / qFactor;
	calculateCoefficients();
}

EqualizerModule::~EqualizerModule() 
{
}

// Source: Zölzer book p.136+137
// All sqrt(2) terms are being replaced by 1/qFactor!
const bool& EqualizerModule::calculateCoefficients() {
	// tan() has a discontinuity at 0.5*PI
	// Should not happen because the maximum center frequncy depends on the sampling frequency, but just in case...
	if ((centerFreq / samplingFreq) < 0.5) {
		// Calculate K
		K = tan((centerFreq / samplingFreq) * M_PI);
	}
	else {
		K = tan(0.4999 * M_PI);
	}

	// Calculate the filter coefficients
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
		// The sign of the gain parameter determines if it's a band pass or a band stop
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
		// Nothing to do here, the user has to enter the coefficients by himself
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
	return checkStability();
}

const bool& EqualizerModule::setCenterFreq(const double& f0) {
	// Once again check if f0 is not to big
	if (f0 <= ValueConversion::getMaxEqFrequency()) {
		centerFreq = f0;
	}
	else {
		centerFreq = ValueConversion::getMaxEqFrequency();
	}
	return calculateCoefficients();
}

const bool& EqualizerModule::setFilterCoefficient(const FilterCoefficients coefficient, const double& value) {
	// Only change the coefficients if the raw filter is selected in the GUI
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
	}
	return checkStability();
}

const bool& EqualizerModule::checkStability() {
	// Check if the current coefficients lead to a stable filter
	// Source: Kammeyer book p.77 / 78
	stable = false;
	if (b2 > (pow(b1, 2.0) / 4.0)) {
		if (b2 < 1.0) {
			stable = true;
		}
	}
	else if (b2 < (pow(b1, 2.0) / 4.0)) {
		if (b1 < (1.0 + b2) && b1 > (-1.0 - b2)) {
			stable = true;
		}
	}
	else if (b2 == (pow(b1, 2.0) / 4.0)) {
		if (b1 < 2.0 && b1 > -2.0) {
			stable = true;
		}
	}
	return stable;
}

void EqualizerModule::processSample(double& sample) {
	// Save current input sample
	xn0 = sample;

	// Difference Equation
	sample = a0 * sample + a1 * xn1 + a2 * xn2 - b1 * yn1 - b2 * yn2;

	if (limit) {
		if (sample > 1.0) sample = 1.0;
		if (sample < -1.0) sample = -1.0;
	}
	else {
		if (sample > maxSampleValue) sample = maxSampleValue;
		if (sample < minSampleValue) sample = minSampleValue;
	}
	
	// Shift samples
	xn2 = xn1;
	xn1 = xn0;
	yn2 = yn1;
	yn1 = sample;
}
