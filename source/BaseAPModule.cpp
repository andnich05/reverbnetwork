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
	, equalizer(new EqualizerModule(EqualizerModule::lowPass, sampleRate, sampleRate / 4, 1.0, 1.0))
	, allpass(new SchroederAllpass(sampleRate, 0, 0.0))
	, gain(new GainModule()) 
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
	if (gain) {
		delete gain;
		gain = nullptr;
	}
}

double BaseAPModule::processModuleSamples(std::vector<double>& channelSamples) {
	double outputSample = 0.0;
	// Mix channels
	outputSample = mixer->mixChannels(channelSamples);
	// Saving processing time => if the sample is zero there is no need in processing it (doesn't work for allpass)
	//if (outputSample == 0.0) {
		//return 0.0;
	//}

	equalizer->processSample(outputSample);
	// ...

	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", "mouse down");
	fclose(pFile);*/

	// Send through the allpass
	allpass->doProcessing(outputSample);

	// Apply output gain if sample isn't zero
	if (outputSample != 0.0) {
		gain->processSample(outputSample);
	}

	// Return output sample of the whole module
	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(outputSample).c_str());
	fclose(pFile);*/
	return outputSample;
}

#include <string>

void BaseAPModule::updateParameter(const unsigned int& pid, const double& normalizedValue) {
	if (pid <= PARAM_MODULEINGAIN_LAST) {
		mixer->setChannelGain(pid % MAXMODULEINPUTS, normalizedValue); // Calculate channel
		
	}
	else if (pid <= PARAM_ALLPASSDELAY_LAST) { // Allpass delay
		allpass->setDelayTime(ValueConversion::normToValueDelay(normalizedValue, sampleRate));
	}
	else if (pid <= PARAM_ALLPASSDECAY_LAST) { // Allpass decay
		allpass->setDecayTime(ValueConversion::normToValueDecay(normalizedValue));
	}
	else if (pid <= PARAM_MODULEOUTGAIN_LAST) { // Module output gain
		gain->setGain(normalizedValue);
	}
}