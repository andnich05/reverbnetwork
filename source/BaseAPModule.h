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
	BaseAPModule();
	~BaseAPModule();

	// Set the sample rate of the host (important for the equalizer and the allpass)
	void setSampleRate(const double& sampleRate);

	// Process all input channels of a module, return output sample which is a mix of the input samples
	//double processModuleSamples(std::vector<double>& channelSamples);
	double processSamples(double* moduleInputBuffer, double* vstInputBuffer) const;

	// Update functions; all passed values plain values (already de-normalized by the Processor)
	//void updateParameter(const unsigned int& pid, const double& normalizedValue);
	// Mixer update functions
	void updateMixerGain(const int& inputNumber, const double& gain);
	void updateMixerMute(const int& inputNumber, const double& mute);
	void updateMixerSolo(const int& inputNumber, const double& solo);
	inline void switchMixerBypass(const double& bypass) { bypassMixer = (bypass != 0.0); }
	
	// Quantizer update functions
	void updateQuantizerQuantization(const double& quantization);
	inline void switchQuantizerBypass(const double& bypass) { bypassQuantizer = (bypass != 0.0); }

	// Equalizer update functions
	void updateEqualizerFilterType(const double& filterType);
	void updateEqualizerCenterFrequency(const double& freq);
	void updateEqualizerQFactor(const double& qFactor);
	void updateEqualizerGain(const double& gain);
	void updateEqualizerCoefficients(const double& value, const long int& paramId);
	inline void switchEqualizerBypass(const double& bypass) { bypassEqualizer = (bypass != 0.0); }

	// Allpass update functions
	void updateAllpassDelay(const double& delay);
	void updateAllpassDecay(const double& decay);
	inline void switchAllpassBypass(const double& bypass) { bypassAllpass = (bypass != 0.0); }

	// Output update functions
	void updateOutputGain(const double& gain);
	inline void switchOutputBypass(const double& bypass) { bypassGain = (bypass != 0.0); }

private:

	// Component objects
	MixerModule* mixer;
	QuantizerModule* quantizer;
	EqualizerModule* equalizer;
	SchroederAllpass* allpass;
	GainModule* gainOutput;

	// Host sample rate
	double sampleRate;

	// Bypass variables
	bool bypassMixer;
	bool bypassQuantizer;
	bool bypassEqualizer;
	bool bypassAllpass;
	bool bypassGain;
};

#endif // BASEAPMODULE_H