#include "MixerModule.h"
#include "ReverbNetworkDefines.h"

MixerModule::MixerModule(const double& gain) {
	// Initialisation
	channelGain.resize(MAXMODULEINPUTS, gain);
}

MixerModule::~MixerModule() {
}

double MixerModule::mixChannels(std::vector<double>& channelSamples) {
	double output = 0.0;
	for (int i = 0; i < MAXMODULEINPUTS; ++i) {
		if (channelGain[i] != 0.0) {
			output += channelSamples[i] * channelGain[i];
		}
	}
	return output;
}

#include <string>
void MixerModule::setChannelGain(const unsigned short& channel, const double& gain) {
	channelGain[channel] = gain;	
}