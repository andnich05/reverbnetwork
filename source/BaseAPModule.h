#ifndef BASEAPMODULE_H
#define BASEAPMODULE_H

#include <vector>
#include <map>

class MixerModule;
class EqualizerModule;
class SchroederAllpass;
class GainModule;

class BaseAPModule
{
public:
	BaseAPModule(unsigned int sampleRate);
	~BaseAPModule();

	// Process all input channels of a module, return output sample which is a mix of the input samples
	double processModuleSamples(std::vector<double>& channelSamples);

	// Update parameters of the modules (0 to MAXMODULEINPUT-1 => Mixer input gain; M => Allpass delay; M+1 => Allpass decay; M+2 => Module output gain) => zu kompliziert
	void updateParameter(const unsigned int& pid, const double& normalizedValue);

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
};

#endif // BASEAPMODULE_H