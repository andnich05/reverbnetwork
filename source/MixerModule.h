#ifndef MIXERMODULE_H
#define MIXERMODULE_H

#include <vector>

class MixerModule
{
public:
	MixerModule(const double& defaultGain);
	~MixerModule();

	// Mix samples of each channels, return the new sample value
	double mixInputs(double* moduleInputBuffer, std::vector<double>& vstInputBuffer);
	// Set gain of each input
	void setInputGain(const int& input, const double& gain);

private:
	std::vector<double> inputGain;
};

#endif // MIXERMODULE_H