#ifndef GAINMODULE_H
#define GAINMODULE_H

class GainModule
{
public:
	GainModule(double gain);
	~GainModule();

	// Set output gain as a factor
	inline void setGain(const double& g) { gain = g; }
	// Process sample by reference
	void processSample (double& sample) const;

private:
	// Gain as a factor
	double gain;
};

#endif // GAINMODULE_H