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

#ifndef EQUALIZERMODULE_H
#define EQUALIZERMODULE_H

#include "ReverbNetworkEnums.h"

enum FilterCoefficients {
	a0,
	a1,
	a2,
	b1,
	b2,
	numberOfCofficients
};

class EqualizerModule {
public:
	EqualizerModule(FilterType filterType, double centerFrequency, double qFactor, double gain);
	~EqualizerModule();

	// Set the sample rate of the host; Returns true if the filter is stable
	inline const bool& setSamplingFreq(const double& fs) { samplingFreq = fs; return calculateCoefficients(); }
	// Set the center or cut-off frequency of the equalizer; Returns true if the filter is stable
	const bool& setCenterFreq(const double& f0);
	// Set the Q factor of the equalizer; Returns true if the filter is stable
	inline const bool& setQFactor(const double& q) { qFactor = q; oneDividedByQ = 1 / qFactor; return calculateCoefficients(); }
	// Set the gain of the equalizer (has no effect when Filter Type is Low-pass or High-pass); Returns true if the filter is stable
	inline const bool& setGain(const double& g) { gain = g; return calculateCoefficients(); };
	// Set the filter type of the equalizer; Returns true if the filter is stable
	inline const bool& setFilterType(const FilterType& type) { filterType = type; return calculateCoefficients(); }
	// Set one of the filter coefficients; works only in raw mode!
	// Returns true if the stability condition is met
	const bool& setFilterCoefficient(const FilterCoefficients coefficient, const double& value);
	// Process the sample by reference
	void processSample(double& sample);
	// Check whether the current filter is stable or not
	inline const bool& isStable() { return stable; }
	// Enable or disable limiter
	inline const bool& enableLimiter(const bool& enable) { this->limit = enable; return calculateCoefficients(); }

private:
	// Calculate the filter coefficients
	const bool& calculateCoefficients();
	// Check the stability condition for the biquad filter after the new coefficients have been calculated
	const bool& checkStability();

	// Sampling frequency
	double samplingFreq;
	// Center frequency fc ("Mittenfrequenz")
	double centerFreq;
	// Q factor or quality factor ("Güte")
	double qFactor;
	// Gain
	double gain;
	// Filter type to use (see enum)
	FilterType filterType;

	//--------------------------------------
	// Variables for the difference equation
	// Input samples (number states the delay in samples)
	double xn0, xn1, xn2;
	// Output samples (number states the delay in samples)
	double yn1, yn2;
	// Filter coefficients
	double a0, a1, a2, b1, b2;
	// Auxiliary variables
	double K, oneDividedByQ;
	// True if the current filter is stable; is returned by all setter functions
	bool stable;
	// True if output samples should be limited
	bool limit;
};

#endif // EQUALIZERMODULE_H