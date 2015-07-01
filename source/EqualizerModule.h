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

class EqualizerModule
{
public:

	EqualizerModule(FilterType filterType, double centerFrequency, double qFactor, double gain);
	~EqualizerModule();

	// Set the sample rate of the host
	inline void setSamplingFreq(const double& fs) { samplingFreq = fs; calculateCoefficients(); }
	// Set the center or cut-off frequency of the equalizer
	void setCenterFreq(const double& f0);
	// Set the Q factor of the equalizer
	inline void setQFactor(const double& q) { qFactor = q; oneDividedByQ = 1 / qFactor; calculateCoefficients(); }
	// Set the gain of the equalizer (has no effect when Filter Type is Low-pass or High-pass)
	inline void setGain(const double& g) { gain = g; calculateCoefficients(); };
	// Set the filter type of the equalizer
	inline void setFilterType(const FilterType& type) { filterType = type; calculateCoefficients(); }
	// Set one of the filter coefficients; works only in raw mode!
	void setFilterCoefficient(const FilterCoefficients coefficient, const double& value);
	// Process the sample by reference
	void processSample(double& sample);

private:
	// Calculate the filter coefficients
	void calculateCoefficients();

	// Sampling frequency
	double samplingFreq;
	// Center frequency f0 ("Mittenfrequenz")
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

};

#endif // EQUALIZERMODULE_H