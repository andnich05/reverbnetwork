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


BaseAPModule::BaseAPModule()
	: mixer(new MixerModule(DEF_MIXERGAIN))
	, quantizer(new QuantizerModule(DEF_QUANTIZERBITDEPTH))
	, equalizer(new EqualizerModule(FilterType::lowPass, DEF_EQCENTERFREQ, DEF_EQQFACTOR, ValueConversion::logToLinear(DEF_EQGAIN)))
	, allpass(new SchroederAllpass(DEF_ALLPASSDELAY / 1000.0, DEF_ALLPASSDECAY / 1000.0))
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

void BaseAPModule::setSampleRate(const double& sampleRate) {
	this->sampleRate = sampleRate;
	equalizer->setSamplingFreq(sampleRate);
	allpass->setSampleRate(sampleRate);
}

//double BaseAPModule::processModuleSamples(std::vector<double>& channelSamples) {
double BaseAPModule::processSamples(double* moduleInputBuffer, double* vstInputBuffer) const {
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

void BaseAPModule::updateMixerGain(const int& inputNumber, const double& gain) {
	mixer->setInputGain(inputNumber, gain);
}

void BaseAPModule::updateMixerMute(const int& inputNumber, const double& mute) { 
	mixer->setInputMuted(inputNumber, mute != 0.0);
}

void BaseAPModule::updateMixerSolo(const int& inputNumber, const double& solo) {
	mixer->setInputSoloed(inputNumber, solo != 0.0); 
}

void BaseAPModule::updateQuantizerQuantization(const double& quantization) {
	quantizer->setQuantization(quantization);
}

const bool& BaseAPModule::updateEqualizerFilterType(const double& filterType) {
	return equalizer->setFilterType((FilterType)((int)(filterType)));
}
const bool& BaseAPModule::updateEqualizerCenterFrequency(const double& freq) {
	return equalizer->setCenterFreq(freq);
}
const bool& BaseAPModule::updateEqualizerQFactor(const double& qFactor) {
	return equalizer->setQFactor(qFactor);
}
const bool& BaseAPModule::updateEqualizerGain(const double& gain) {
	return equalizer->setGain(ValueConversion::logToLinear(gain));
}

const bool& BaseAPModule::updateEqualizerCoefficients(const double& value, const long int& paramId) {
	if (paramId >= PARAM_EQCOEFFICIENTA0_FIRST && paramId <= PARAM_EQCOEFFICIENTA0_LAST) {
		equalizer->setFilterCoefficient(FilterCoefficients::a0, value);
	}
	else if (paramId >= PARAM_EQCOEFFICIENTA1_FIRST && paramId <= PARAM_EQCOEFFICIENTA1_LAST) {
		equalizer->setFilterCoefficient(FilterCoefficients::a1, value);
	}
	else if (paramId >= PARAM_EQCOEFFICIENTA2_FIRST && paramId <= PARAM_EQCOEFFICIENTA2_LAST) {
		equalizer->setFilterCoefficient(FilterCoefficients::a2, value);
	}
	else if (paramId >= PARAM_EQCOEFFICIENTB1_FIRST && paramId <= PARAM_EQCOEFFICIENTB1_LAST) {
		equalizer->setFilterCoefficient(FilterCoefficients::b1, value);
	}
	else if (paramId >= PARAM_EQCOEFFICIENTB2_FIRST && paramId <= PARAM_EQCOEFFICIENTB2_LAST) {
		equalizer->setFilterCoefficient(FilterCoefficients::b2, value);
	}
	return equalizer->isStable();
}

void BaseAPModule::updateAllpassDelay(const double& delay) {
	allpass->setDelayTimeSec(delay / 1000.0);
}
void BaseAPModule::updateAllpassDecay(const double& decay) {
	allpass->setDecayTimeSec(decay / 1000.0);
}

void BaseAPModule::updateOutputGain(const double& gain) {
	
	gainOutput->setGain(ValueConversion::logToLinear(gain));
}