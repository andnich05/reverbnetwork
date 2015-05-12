#include "BaseAPModule.h"
#include "MixerModule.h"
#include "EqualizerModule.h"
#include "SchroederAllpass.h"
#include "GainModule.h"
#include "ReverbNetworkDefines.h"
#include "ValueConversion.h"

#include <cmath>
#include <string>

BaseAPModule::BaseAPModule(unsigned int sampleRate)
	: sampleRate(sampleRate)
	, mixer(new MixerModule())
	, equalizer(new EqualizerModule(FilterType::lowPass, sampleRate, sampleRate / 4, 1.0, 1.0))
	, allpass(new SchroederAllpass(sampleRate, 0, 0.0))
	, gainOutput(new GainModule())
	, bypassMixer(false)
	, bypassEqualizer(false)
	, bypassAllpass(false)
	, bypassGain(false)
{
	
}

BaseAPModule::~BaseAPModule() {
	if (mixer) {
		delete mixer;
		mixer = nullptr;
	}
	if (equalizer) {
		delete equalizer;
		equalizer = nullptr;
	}
	if (allpass) {
		delete allpass;
		allpass = nullptr;
	}
	if (gainOutput) {
		delete gainOutput;
		gainOutput = nullptr;
	}
}

double BaseAPModule::processModuleSamples(std::vector<double>& channelSamples) {
	double outputSample = 0.0;

	// Mix channels
	outputSample = mixer->mixChannels(channelSamples);

	// Do equalizing
	if (!bypassEqualizer) {
		equalizer->processSample(outputSample);
	}

	// Send through the allpass
	if (!bypassAllpass) {
		allpass->doProcessing(outputSample);
	}

	// Apply output gain if sample isn't zero
	if (!bypassGain) {
		if (outputSample != 0.0) {
			gainOutput->processSample(outputSample);
		}
	}

	// Return output sample of the whole module
	return outputSample;
}

void BaseAPModule::updateMixerGain(const unsigned int& inputNumber, const double& gain) {
	mixer->setChannelGain(inputNumber, ValueConversion::logToLinear(ValueConversion::normToValueGain(gain)));
}

void BaseAPModule::updateEqualizerFilterType(const double& filterType) {
	equalizer->setFilterType((FilterType)((int)(filterType)));
}
void BaseAPModule::updateEqualizerCenterFrequency(const double& freq) {
	equalizer->setCenterFreq(ValueConversion::normToValueCenterFreq(freq));
}
void BaseAPModule::updateEqualizerQFactor(const double& qFactor) {
	equalizer->setQFactor(ValueConversion::normToValueQFactor(qFactor));
}
void BaseAPModule::updateEqualizerGain(const double& gain) {
	equalizer->setGain(ValueConversion::logToLinear(ValueConversion::normToValueGain(gain)));
}

void BaseAPModule::updateAllpassDelay(const double& delay) {
	allpass->setDelayTimeSec(ValueConversion::normToValueDelay(delay));
}
void BaseAPModule::updateAllpassDecay(const double& decay) {
	allpass->setDecayTime(ValueConversion::normToValueDecay(decay));
}

void BaseAPModule::updateOutputGain(const double& gain) {
	gainOutput->setGain(ValueConversion::logToLinear(ValueConversion::normToValueGain(gain)));
}


//#include <string>
//
//void BaseAPModule::updateParameter(const unsigned int& pid, const double& normalizedValue) {
//	if (pid >= PARAM_MIXERGAIN_FIRST && pid <= PARAM_MIXERGAIN_LAST) {
//		mixer->setChannelGain((pid - PARAM_MIXERGAIN_FIRST) % MAXMODULEINPUTS, normalizedValue); // Calculate channel
//	}
//}