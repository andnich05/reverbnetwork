#ifndef BASEAPMODULE_H
#define BASEAPMODULE_H

#include <vector>
#include <map>

class MixerModule;
class EqualizerModule;
class SchroederAllpass;
class GainModule;
enum class FilterType : unsigned int;

class BaseAPModule
{
public:
	BaseAPModule(unsigned int sampleRate);
	~BaseAPModule();

	// Process all input channels of a module, return output sample which is a mix of the input samples
	double processModuleSamples(std::vector<double>& channelSamples);

	// Update functions; all passed values are normalized?
	//void updateParameter(const unsigned int& pid, const double& normalizedValue);
	void updateMixerGain(const unsigned int& inputNumber, const double& gain);
	inline void switchMixerBypass(const bool& bypass) { bypassMixer = bypass; }

	void updateEqualizerFilterType(const FilterType& filterType);
	void updateEqualizerCenterFrequency(const double& freq);
	void updateEqualizerQFactor(const double& qFactor);
	void updateEqualizerGain(const double& gain);
	inline void switchEqualizerBypass(const bool& bypass) { bypassEqualizer = bypass; }

	void updateAllpassDelay(const double& delay);
	void updateAllpassDecay(const double& decay);
	inline void switchAllpassBypass(const bool& bypass) { bypassAllpass = bypass; }

	void updateOutputGain(const double& gain);
	inline void switchOutputBypass(const bool& bypass) { bypassGain = bypass; }

private:
	// Mixes input signals into one signal with different gain values
	MixerModule* mixer;
	// Equalizer...
	EqualizerModule* equalizer;
	// Adds delay
	SchroederAllpass* allpass;
	// Gain...
	GainModule* gain;

	unsigned int sampleRate;

	// Holds either the module number (output) or the VST input to which the module input is connected (see ConnectionMatrix for mapping)
	std::vector<short> inputs;
	// Holds either the module number (input) or the VST output to which the module output is connected (see ConnectionMatrix for mapping)
	short output;

	// Bypass variables
	bool bypassMixer;
	bool bypassEqualizer;
	bool bypassAllpass;
	bool bypassGain;
};

#endif // BASEAPMODULE_H