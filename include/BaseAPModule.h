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
	void setSampleRate(const double& sampleRate);

	// Process all input channels of a module, all vst inputs and the signal generator, returns output sample which is a mix of the input samples
	double processSamples(double* moduleInputBuffer, double* vstInputBuffer, double& signalGeneratorSample) const;

	//---
	// Update functions; all passed values plain values (already de-normalized by the Processor)
	// Mixer update functions
	void updateMixerGain(const int& inputNumber, const double& gain);
	void updateMixerMute(const int& inputNumber, const double& mute);
	void updateMixerSolo(const int& inputNumber, const double& solo);
	inline void switchMixerBypass(const double& bypass) { bypassMixer = (bypass != 0.0); }
	
	// Quantizer update functions
	void updateQuantizerQuantization(const double& quantization);
	inline void switchQuantizerBypass(const double& bypass) { bypassQuantizer = (bypass != 0.0); }

	// Equalizer update functions; return true if the filter is stable
	const bool& updateEqualizerFilterType(const double& filterType);
	const bool& updateEqualizerCenterFrequency(const double& freq);
	const bool& updateEqualizerQFactor(const double& qFactor);
	const bool& updateEqualizerGain(const double& gain);
	const bool& updateEqualizerCoefficients(const double& value, const long int& paramId);
	const bool& updateEqualizerLimiter(const double& limit);
	inline void switchEqualizerBypass(const double& bypass) { bypassEqualizer = (bypass != 0.0); }

	// Allpass update functions
	void updateAllpassDelay(const double& delay);
	void updateAllpassDecay(const double& decay);
	void updateAllpassDiffKSign(const bool& isNegative);
	void updateAllpassModulationEnabled(const bool& enabled);
	void updateAllpassModulationSignalType(const double& signalType);
	void updateAllpassModulationExcursion(const double& excursion);
	void updateAllpassModulationRate(const double& rate);
	inline void switchAllpassBypass(const double& bypass) { bypassAllpass = (bypass != 0.0); }

	// Output update functions
	void updateOutputGain(const double& gain);
	void updateOutputLimiter(const double& limit);
	inline void switchOutputBypass(const double& bypass) { bypassGain = (bypass != 0.0); }

	//---

private:

	// Component objects
	MixerModule* mixer;
	QuantizerModule* quantizer;
	EqualizerModule* equalizer;
	SchroederAllpass* allpass;
	GainModule* gainOutput;

	// Bypass variables
	bool bypassMixer;
	bool bypassQuantizer;
	bool bypassEqualizer;
	bool bypassAllpass;
	bool bypassGain;
};

#endif // BASEAPMODULE_H