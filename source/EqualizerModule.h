#ifndef EQUALIZERMODULE_H
#define EQUALIZERMODULE_H

#include "ReverbNetworkEnums.h"

class EqualizerModule
{
public:

	EqualizerModule(FilterType filterType, double samplingFreq, double centerFreq, double qFactor, double dBgain);
	~EqualizerModule();

	inline void setSamplingFreq(const double& fs) { samplingFreq = fs; calculateK(); }
	inline void setCenterFreq(const double& f0) { centerFreq = f0; calculateK(); }
	inline void setQFactor(const double& q) { qFactor = q; calculateK(); }
	inline void setGain(const double& g) { gain = g; calculateK(); };
	inline void setFilterType(const FilterType& type) { filterType = type; calculateK(); }

	void processSample(double& sample);

private:

	void calculateK();
	void calculateCoefficients();

	// Sampling frequency
	double samplingFreq;
	// Center frequency f0 ("Mittenfrequenz")
	double centerFreq;
	// Q factor or quality factor ("Güte")
	double qFactor;
	// Gain
	double dBgain;
	double gain;
	// Filter type to use (see enum)
	FilterType filterType;

	//--------------------------------------
	// Variables for the difference equation
	// Input samples; number states the delay
	double xn0, xn1, xn2;
	// Output samples; number states the delay
	double yn1, yn2;
	// Filter coefficients
	double a0, a1, a2, b1, b2;
	// Auxiliary variable
	double K;
};

#endif // EQUALIZERMODULE_H