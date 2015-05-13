#ifndef GAINMODULE_H
#define GAINMODULE_H

class GainModule
{
public:
	GainModule(double gain);
	~GainModule();

	inline void setGain(const double& g) { gain = g; }
	void processSample(double& sample);

private:
	double gain;
};

#endif // GAINMODULE_H