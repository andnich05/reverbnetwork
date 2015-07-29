#ifndef MIXERMODULE_H
#define MIXERMODULE_H

#include <vector>

class MixerModule
{
public:
	MixerModule(const double& defaultGain);
	~MixerModule();

	// Mix samples of each channels, return the new sample value
	double mixInputs(double* moduleInputBuffer, double* vstInputBuffer, double& signalGeneratorSample) const;
	// Set gain of an input
	inline void setInputGain(const int& input, const double& gain) { inputGain[input] = gain; }
	// Mute or unmute an input
	inline void setInputMuted(const int& input, const bool& mute) { inputMuted[input] = mute; }
	// Solo or unsolo an input
	void setInputSoloed(const int& input, const bool& solo);

private:
	std::vector<double> inputGain;
	std::vector<bool> inputMuted;
	std::vector<bool> inputSoloed;

	bool isAnyOtherInputSoloed;
};

#endif // MIXERMODULE_H