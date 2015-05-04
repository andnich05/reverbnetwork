#ifndef EQUALIZERMODULE_H
#define EQUALIZERMODULE_H

class EqualizerModule
{
public:

	// Possible filter types
	enum FilterType
	{
		lowPass,
		highPass,
		bandPass,
		bandStop,
		lowShelf,
		highShelf
	};

	EqualizerModule(unsigned int samplingFreq, unsigned int centerFreq, double qFactor, double gain);
	~EqualizerModule();

	inline void setSamplingFreq(const unsigned int& fs) { samplingFreq = fs; }
	inline void setCenterFreq(const unsigned int& f0) { centerFreq = f0; }
	inline void setQFactor(const double& q) { qFactor = q; }
	inline void setGain(const double& g) { gain = g; }
	inline void setFilterType(const FilterType& type) { filterType = type; }

	void process(double& sample);

private:

	void calculateK();
	void calculateCoefficients();

	// Sampling frequency
	unsigned int samplingFreq;
	// Center frequency f0 ("Mittenfrequenz")
	unsigned int centerFreq;
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
	// Auxiliary variable
	double K;
};

#endif // EQUALIZERMODULE_H