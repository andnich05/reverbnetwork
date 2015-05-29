#ifndef BASEAPMODULE_H
#define BASEAPMODULE_H

#include <vector>

class MixerModule;
class QuantizerModule;
class EqualizerModule;
class SchroederAllpass;
class GainModule;

class BaseAPModule
{
public:
	BaseAPModule(double sampleRate);
	~BaseAPModule();

	// Process all input channels of a module, return output sample which is a mix of the input samples
	//double processModuleSamples(std::vector<double>& channelSamples);
	double processSamples(double* moduleInputBuffer, std::vector<double>& vstInputBuffer);

	// Update functions; all passed values are raw VST parameters so that they have to be transformed accordingly
	//void updateParameter(const unsigned int& pid, const double& normalizedValue);
	void updateMixerGain(const unsigned int& inputNumber, const double& gain);
	inline void switchMixerBypass(const double& bypass) { bypassMixer = (bypass != 0.0); }

	void updateQuantizerQuantization(const double& quantization);
	inline void switchQuantizerBypass(const double& bypass) { bypassQuantizer = (bypass != 0.0); }

	void updateEqualizerFilterType(const double& filterType);
	void updateEqualizerCenterFrequency(const double& freq);
	void updateEqualizerQFactor(const double& qFactor);
	void updateEqualizerGain(const double& gain);
	inline void switchEqualizerBypass(const double& bypass) { bypassEqualizer = (bypass != 0.0); }

	void updateAllpassDelay(const double& delay);
	void updateAllpassDecay(const double& decay);
	inline void switchAllpassBypass(const double& bypass) { bypassAllpass = (bypass != 0.0); }

	void updateOutputGain(const double& gain);
	inline void switchOutputBypass(const double& bypass) { bypassGain = (bypass != 0.0); }

private:

	MixerModule* mixer;
	QuantizerModule* quantizer;
	EqualizerModule* equalizer;
	SchroederAllpass* allpass;
	GainModule* gainOutput;

	unsigned int sampleRate;

	// Bypass variables
	bool bypassMixer;
	bool bypassQuantizer;
	bool bypassEqualizer;
	bool bypassAllpass;
	bool bypassGain;
};

#endif // BASEAPMODULE_H