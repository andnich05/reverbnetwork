#ifndef GAINMODULE_H
#define GAINMODULE_H

class GainModule
{
public:
	GainModule();
	~GainModule();

	inline void setGain(const double& g) { if (g >= 0.0 && g <= 1.0) gain = g; }
	void processSample(double& sample);

private:
	double gain;
};

#endif // GAINMODULE_H