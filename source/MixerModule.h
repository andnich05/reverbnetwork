#ifndef MIXERMODULE_H
#define MIXERMODULE_H

#include <vector>

class MixerModule
{
public:
	MixerModule();
	~MixerModule();

	// Mix samples of each channels, return the new sample value (no reference possible?)
	double mixChannels(std::vector<double>& channelSamples);
	// Set gain of each channel
	void setChannelGain(const unsigned short& channel, const double& gain);

private:
	std::vector<double> channelGain;
};

#endif // MIXERMODULE_H