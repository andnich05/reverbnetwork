#ifndef EQUALIZERMODULE_H
#define EQUALIZERMODULE_H

#include "ReverbNetworkEnums.h"

class EqualizerModule
{
public:

	enum class FilterCoefficients {
		a0,
		a1,
		a2,
		b1,
		b2,
		numberOfCofficients
	};

	EqualizerModule(FilterType filterType, double qFactor, double gain);
	~EqualizerModule();

	inline void setSamplingFreq(const double& fs) { samplingFreq = fs; calculateCoefficients(); }
	void setCenterFreq(const double& f0);
	inline void setQFactor(const double& q) { qFactor = q; oneDividedByQ = 1 / qFactor; calculateCoefficients(); }
	inline void setGain(const double& g) { gain = g; calculateCoefficients(); };
	inline void setFilterType(const FilterType& type) { filterType = type; calculateCoefficients(); }
	void setFilterCoefficient(const FilterCoefficients& coefficient, const double& value);

	void processSample(double& sample);

private:

	//void calculateK();
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
	// Input samples; number states the delay
	double xn0, xn1, xn2;
	// Output samples; number states the delay
	double yn1, yn2;
	// Filter coefficients
	double a0, a1, a2, b1, b2;
	// Auxiliary variables
	double K, oneDividedByQ;

};

#endif // EQUALIZERMODULE_H