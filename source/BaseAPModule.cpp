#include "BaseAPModule.h"
#include "MixerModule.h"
#include "QuantizerModule.h"
#include "EqualizerModule.h"
#include "SchroederAllpass.h"
#include "GainModule.h"
#include "ReverbNetworkDefines.h"
#include "ValueConversion.h"

#include <cmath>
#include <string>

BaseAPModule::BaseAPModule(double sampleRate)
	: sampleRate(sampleRate)
	, mixer(new MixerModule(DEF_MIXERGAIN))
	, quantizer(new QuantizerModule(DEF_QUANTIZERBITDEPTH))
	, equalizer(new EqualizerModule(FilterType::lowPass, sampleRate, DEF_EQCENTERFREQ, DEF_EQQFACTOR, ValueConversion::logToLinear(DEF_EQGAIN)))
	, allpass(new SchroederAllpass(sampleRate, DEF_ALLPASSDELAY, DEF_ALLPASSDECAY))
	, gainOutput(new GainModule(ValueConversion::logToLinear(DEF_OUTPUTGAIN)))
	, bypassMixer(false)
	, bypassQuantizer(false)
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
	if (quantizer) {
		delete quantizer;
		quantizer = nullptr;
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

//double BaseAPModule::processModuleSamples(std::vector<double>& channelSamples) {
double BaseAPModule::processSamples(double* moduleInputBuffer, std::vector<double>& vstInputBuffer) {
	// Mix channels
	double outputSample = mixer->mixInputs(moduleInputBuffer, vstInputBuffer);

	if (!bypassQuantizer) {
		if (outputSample != 0.0) {
			quantizer->processSample(outputSample);
		}
	}

	// Do equalizing
	if (!bypassEqualizer) {
		equalizer->processSample(outputSample);
	}
	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(outputSample).c_str());
	fclose(pFile);*/
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
	mixer->setInputGain(inputNumber, gain);
}

void BaseAPModule::updateQuantizerQuantization(const double& quantization) {
	quantizer->setQuantization(quantization);
}

void BaseAPModule::updateEqualizerFilterType(const double& filterType) {
	equalizer->setFilterType((FilterType)((int)(filterType)));
}
void BaseAPModule::updateEqualizerCenterFrequency(const double& freq) {
	equalizer->setCenterFreq(freq);
}
void BaseAPModule::updateEqualizerQFactor(const double& qFactor) {
	equalizer->setQFactor(qFactor);
}
void BaseAPModule::updateEqualizerGain(const double& gain) {
	equalizer->setGain(ValueConversion::logToLinear(gain));
	//FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	//fprintf(pFile, "y(n): %s\n", std::to_string(ValueConversion::logToLinear(ValueConversion::normToValueGain(gain))).c_str());
	////fprintf(pFile, "y(n): %s\n", std::to_string(K).c_str());
	//fclose(pFile);
}

void BaseAPModule::updateAllpassDelay(const double& delay) {
	allpass->setDelayTimeMsec(delay);
}
void BaseAPModule::updateAllpassDecay(const double& decay) {
	allpass->setDecayTime(decay);
}

void BaseAPModule::updateOutputGain(const double& gain) {
	
	gainOutput->setGain(ValueConversion::logToLinear(gain));
}


//#include <string>
//
//void BaseAPModule::updateParameter(const unsigned int& pid, const double& normalizedValue) {
//	if (pid >= PARAM_MIXERGAIN_FIRST && pid <= PARAM_MIXERGAIN_LAST) {
//		mixer->setChannelGain((pid - PARAM_MIXERGAIN_FIRST) % MAXMODULEINPUTS, normalizedValue); // Calculate channel
//	}
//}