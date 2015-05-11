#ifndef EQUALIZERMODULE_H
#define EQUALIZERMODULE_H

class EqualizerModule
{
public:

	// Possible filter types
	enum FilterType : unsigned int
	{
		lowPass,
		highPass,
		bandPass,
		bandStop,
		lowShelf,
		highShelf
	};

	EqualizerModule(FilterType filterType, double samplingFreq, double centerFreq, double qFactor, double dBgain);
	~EqualizerModule();

	inline void setSamplingFreq(const double& fs) { samplingFreq = fs; }
	inline void setCenterFreq(const double& f0) { centerFreq = f0; }
	inline void setQFactor(const double& q) { qFactor = q; }
	void setGainInDB(const double& g);
	inline void setFilterType(const FilterType& type) { filterType = type; }

	void processSample(double& sample);

private:

	void calculateK();
	void calculateCoefficients(FilterType filterType);

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