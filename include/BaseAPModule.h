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

#ifndef BASEAPMODULE_H
#define BASEAPMODULE_H

#include <memory>
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
	void setSampleRate(double sampleRate);

	// Process all input channels of a module, all vst inputs and the signal generator, returns output sample which is a mix of the input samples
	double processSamples(double* moduleInputBuffer, double* vstInputBuffer, double& signalGeneratorSample) const;

	//---
	// Update functions; all passed values plain values (already de-normalized by the Processor)
	// Mixer update functions
	void updateMixerGain(int inputNumber, double gain);
	void updateMixerMute(int inputNumber, double mute);
	void updateMixerSolo(int inputNumber, double solo);
	inline void switchMixerBypass(double bypass) { bypassMixer = (bypass != 0.0); }
	
	// Quantizer update functions
	void updateQuantizerQuantization(double quantization);
	inline void switchQuantizerBypass(double bypass) { bypassQuantizer = (bypass != 0.0); }

	// Equalizer update functions; return true if the filter is stable
	bool updateEqualizerFilterType(double filterType);
	bool updateEqualizerCenterFrequency(double freq);
	bool updateEqualizerQFactor(double qFactor);
	bool updateEqualizerGain(double gain);
	bool updateEqualizerCoefficients(double value, long int paramId);
	bool updateEqualizerLimiter(double limit);
	inline void switchEqualizerBypass(double bypass) { bypassEqualizer = (bypass != 0.0); }

	// Allpass update functions
	void updateAllpassDelay(double delay);
	void updateAllpassDecay(double decay);
	void updateAllpassDiffKSign(bool isNegative);
	void updateAllpassModulationEnabled(bool enabled);
	void updateAllpassModulationSignalType(double signalType);
	void updateAllpassModulationExcursion(double excursion);
	void updateAllpassModulationRate(double rate);
	inline void switchAllpassBypass(double bypass) { bypassAllpass = (bypass != 0.0); }

	// Output update functions
	void updateOutputGain(double gain);
	void updateOutputLimiter(double limit);
	inline void switchOutputBypass(double bypass) { bypassGain = (bypass != 0.0); }

	//---

private:

	// Component objects
	std::unique_ptr< MixerModule > mixer;
	std::unique_ptr< QuantizerModule > quantizer;
	std::unique_ptr< EqualizerModule > equalizer;
	std::unique_ptr< SchroederAllpass > allpass;
	std::unique_ptr< GainModule > gainOutput;

	// Bypass variables
	bool bypassMixer;
	bool bypassQuantizer;
	bool bypassEqualizer;
	bool bypassAllpass;
	bool bypassGain;
};

#endif // BASEAPMODULE_H
