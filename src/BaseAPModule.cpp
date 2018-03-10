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
}

void BaseAPModule::setSampleRate(double sampleRate) {
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

void BaseAPModule::updateMixerGain(int inputNumber, double gain) {
	mixer->setInputGain(inputNumber, gain);
}

void BaseAPModule::updateMixerMute(int inputNumber, double mute) { 
	mixer->setInputMuted(inputNumber, mute != 0.0);
}

void BaseAPModule::updateMixerSolo(int inputNumber, double solo) {
	mixer->setInputSoloed(inputNumber, solo != 0.0); 
}

void BaseAPModule::updateQuantizerQuantization(double quantization) {
	quantizer->setQuantization(quantization);
}

bool BaseAPModule::updateEqualizerFilterType(double filterType) {
	return equalizer->setFilterType((FilterType)((int)(filterType)));
}
bool BaseAPModule::updateEqualizerCenterFrequency(double freq) {
	return equalizer->setCenterFreq(freq);
}
bool BaseAPModule::updateEqualizerQFactor(double qFactor) {
	return equalizer->setQFactor(qFactor);
}
bool BaseAPModule::updateEqualizerGain(double gain) {
	return equalizer->setGain(ValueConversion::logToLinear(gain));
}

bool BaseAPModule::updateEqualizerCoefficients(double value, long int paramId) {
	if (paramId >= PARAM_EQCOEFFICIENTA0_FIRST && paramId <= PARAM_EQCOEFFICIENTA0_LAST) {
		equalizer->setFilterCoefficient(EqualizerModule::FilterCoefficients::a0, value);
	}
	else if (paramId >= PARAM_EQCOEFFICIENTA1_FIRST && paramId <= PARAM_EQCOEFFICIENTA1_LAST) {
		equalizer->setFilterCoefficient(EqualizerModule::FilterCoefficients::a1, value);
	}
	else if (paramId >= PARAM_EQCOEFFICIENTA2_FIRST && paramId <= PARAM_EQCOEFFICIENTA2_LAST) {
		equalizer->setFilterCoefficient(EqualizerModule::FilterCoefficients::a2, value);
	}
	else if (paramId >= PARAM_EQCOEFFICIENTB1_FIRST && paramId <= PARAM_EQCOEFFICIENTB1_LAST) {
		equalizer->setFilterCoefficient(EqualizerModule::FilterCoefficients::b1, value);
	}
	else if (paramId >= PARAM_EQCOEFFICIENTB2_FIRST && paramId <= PARAM_EQCOEFFICIENTB2_LAST) {
		equalizer->setFilterCoefficient(EqualizerModule::FilterCoefficients::b2, value);
	}
	return equalizer->isStable();
}

bool BaseAPModule::updateEqualizerLimiter(double limit) {
	return equalizer->enableLimiter(limit != 0.0);
}

void BaseAPModule::updateAllpassDelay(double delay) {
	allpass->setDelayTimeSec(delay / 1000.0);
}
void BaseAPModule::updateAllpassDecay(double decay) {
	allpass->setDecayTimeSec(decay / 1000.0);
}

void BaseAPModule::updateAllpassDiffKSign(bool isNegative) {
	allpass->setGainSign(isNegative);
}

void BaseAPModule::updateAllpassModulationEnabled(bool enabled) {
	allpass->setModulationEnabled(enabled);
}

void BaseAPModule::updateAllpassModulationSignalType(double signalType) {
	allpass->setModulationSignalType((ModulationSignalType)((int)(signalType)));
}

void BaseAPModule::updateAllpassModulationExcursion(double excursion) {
	allpass->setModulationExcursion(excursion);
}

void BaseAPModule::updateAllpassModulationRate(double rate) {
	allpass->setModulationRateMs(rate);
}

void BaseAPModule::updateOutputGain(double gain) {
	gainOutput->setGain(ValueConversion::logToLinear(gain));
}

void BaseAPModule::updateOutputLimiter(double limit) {
	gainOutput->enableLimiter(limit != 0.0);
}
