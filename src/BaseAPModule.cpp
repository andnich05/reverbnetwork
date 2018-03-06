/*
* BaseAPModule: Contains and manages all other processing components
*
* Copyright (C) 2015  Andrej Nichelmann
*                     Klaus Michael Indlekofer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../include/BaseAPModule.h"
#include "../include/MixerModule.h"
#include "../include/QuantizerModule.h"
#include "../include/EqualizerModule.h"
#include "../include/SchroederAllpass.h"
#include "../include/GainModule.h"
#include "../include/ReverbNetworkDefines.h"
#include "../include/ValueConversion.h"

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
	// Delete all components
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
	equalizer->setSamplingFreq(sampleRate);
	allpass->setSampleRate(sampleRate);
}

double BaseAPModule::processSamples(double* moduleInputBuffer, double* vstInputBuffer, double& signalGeneratorSample) const {
	// Mix channels (no bypass available)
	double outputSample = mixer->mixInputs(moduleInputBuffer, vstInputBuffer, signalGeneratorSample);

	// Quantize samples
	if (!bypassQuantizer) {
		if (outputSample != 0.0) {	
			quantizer->processSample(outputSample);
		}
	}

	// Equalize samples
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

const bool& BaseAPModule::updateEqualizerLimiter(const double& limit) {
	return equalizer->enableLimiter(limit != 0.0);
}

void BaseAPModule::updateAllpassDelay(const double& delay) {
	allpass->setDelayTimeSec(delay / 1000.0);
}
void BaseAPModule::updateAllpassDecay(const double& decay) {
	allpass->setDecayTimeSec(decay / 1000.0);
}

void BaseAPModule::updateAllpassDiffKSign(const bool& isNegative) {
	allpass->setGainSign(isNegative);
}

void BaseAPModule::updateAllpassModulationEnabled(const bool& enabled) {
	allpass->setModulationEnabled(enabled);
}

void BaseAPModule::updateAllpassModulationSignalType(const double& signalType) {
	allpass->setModulationSignalType((ModulationSignalType)((int)(signalType)));
}

void BaseAPModule::updateAllpassModulationExcursion(const double& excursion) {
	allpass->setModulationExcursion(excursion);
}

void BaseAPModule::updateAllpassModulationRate(const double& rate) {
	allpass->setModulationRateMs(rate);
}

void BaseAPModule::updateOutputGain(const double& gain) {
	gainOutput->setGain(ValueConversion::logToLinear(gain));
}

void BaseAPModule::updateOutputLimiter(const double& limit) {
	gainOutput->enableLimiter(limit != 0.0);
}