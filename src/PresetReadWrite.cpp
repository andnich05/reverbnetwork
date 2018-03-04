/*
* PresetReadWrite: Vst preset reading and writing
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

#include "PresetReadWrite.h"
#include "ReverbNetworkDefines.h"
#include "ValueConversion.h"

PresetReadWrite::PresetReadWrite() {
	// Initialize the values with the default values (Must be in the same order as in ReverbNetworkDefines.h!)
	for (auto i = PARAM_MIXERINPUTSELECT_FIRST; i <= PARAM_MIXERINPUTSELECT_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormMixerInputSelect(DEF_MIXERINPUT));
	}
	for (auto i = PARAM_MIXERGAIN_FIRST; i <= PARAM_MIXERGAIN_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormInputGain(DEF_MIXERGAIN));
	}
	for (auto i = PARAM_MIXERINPUTMUTED_FIRST; i <= PARAM_MIXERINPUTMUTED_LAST; ++i) {
		parameterValues.push_back(0.0);
	}
	for (auto i = PARAM_MIXERINPUTSOLOED_FIRST; i <= PARAM_MIXERINPUTSOLOED_LAST; ++i) {
		parameterValues.push_back(0.0);
	}
	for (auto i = PARAM_QUANTIZERBITDEPTH_FIRST; i <= PARAM_QUANTIZERBITDEPTH_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormQuantization(DEF_QUANTIZERBITDEPTH));
	}
	for (auto i = PARAM_QUANTIZERBYPASS_FIRST; i <= PARAM_QUANTIZERBYPASS_LAST; ++i) {
		parameterValues.push_back(DEF_QUANTIZERBYPASS);
	}
	for (auto i = PARAM_EQFILTERTYPE_FIRST; i <= PARAM_EQFILTERTYPE_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormFilterTypeSelect(DEF_EQFILTERTYPE));
	}
	for (auto i = PARAM_EQCENTERFREQ_FIRST; i <= PARAM_EQCENTERFREQ_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormProcCenterFreq(DEF_EQCENTERFREQ));
	}
	for (auto i = PARAM_EQQFACTOR_FIRST; i <= PARAM_EQQFACTOR_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormQFactor(DEF_EQQFACTOR));
	}
	for (auto i = PARAM_EQGAIN_FIRST; i <= PARAM_EQGAIN_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormEqGain(DEF_EQGAIN));
	}
	for (auto i = PARAM_EQCOEFFICIENTA0_FIRST; i <= PARAM_EQCOEFFICIENTA0_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormEqCoefficients(DEF_EQCOEFFICIENTS));
	}
	for (auto i = PARAM_EQCOEFFICIENTA1_FIRST; i <= PARAM_EQCOEFFICIENTA1_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormEqCoefficients(DEF_EQCOEFFICIENTS));
	}
	for (auto i = PARAM_EQCOEFFICIENTA2_FIRST; i <= PARAM_EQCOEFFICIENTA2_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormEqCoefficients(DEF_EQCOEFFICIENTS));
	}
	for (auto i = PARAM_EQCOEFFICIENTB1_FIRST; i <= PARAM_EQCOEFFICIENTB1_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormEqCoefficients(DEF_EQCOEFFICIENTS));
	}
	for (auto i = PARAM_EQCOEFFICIENTB2_FIRST; i <= PARAM_EQCOEFFICIENTB2_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormEqCoefficients(DEF_EQCOEFFICIENTS));
	}
	for (auto i = PARAM_EQLIMITER_FIRST; i <= PARAM_EQLIMITER_LAST; ++i) {
		parameterValues.push_back(DEF_EQLIMITER);
	}
	for (auto i = PARAM_EQBYPASS_FIRST; i <= PARAM_EQBYPASS_LAST; ++i) {
		parameterValues.push_back(DEF_EQBYPASS);
	}
	for (auto i = PARAM_ALLPASSDELAY_FIRST; i <= PARAM_ALLPASSDELAY_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormDelay(DEF_ALLPASSDELAY));
	}
	for (auto i = PARAM_ALLPASSDECAY_FIRST; i <= PARAM_ALLPASSDECAY_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormDecay(DEF_ALLPASSDECAY));
	}
	for (auto i = PARAM_ALLPASSDIFFKSIGN_FIRST; i <= PARAM_ALLPASSDIFFKSIGN_LAST; ++i) {
		parameterValues.push_back(DEF_ALLPASSDIFFKSIGN);
	}
	for (auto i = PARAM_ALLPASSMODENABLED_FIRST; i <= PARAM_ALLPASSMODENABLED_LAST; ++i) {
		parameterValues.push_back(DEF_ALLPASSMODENABLED);
	}
	for (auto i = PARAM_ALLPASSMODSIGNALTYPE_FIRST; i <= PARAM_ALLPASSMODSIGNALTYPE_LAST; ++i) {
		parameterValues.push_back(DEF_ALLPASSMODSIGNALTYPE);
	}
	for (auto i = PARAM_ALLPASSMODEXCURSION_FIRST; i <= PARAM_ALLPASSMODEXCURSION_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormModExcursion(DEF_ALLPASSMODEXCURSION));
	}
	for (auto i = PARAM_ALLPASSMODRATE_FIRST; i <= PARAM_ALLPASSMODRATE_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormModRate(DEF_ALLPASSMODRATE));
	}
	for (auto i = PARAM_ALLPASSBYPASS_FIRST; i <= PARAM_ALLPASSBYPASS_LAST; ++i) {
		parameterValues.push_back(DEF_ALLPASSBYPASS);
	}
	for (auto i = PARAM_OUTGAIN_FIRST; i <= PARAM_OUTGAIN_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormOutputGain(DEF_OUTPUTGAIN));
	}
	for (auto i = PARAM_OUTLIMITER_FIRST; i <= PARAM_OUTLIMITER_LAST; ++i) {
		parameterValues.push_back(DEF_OUTLIMITER);
	}
	for (auto i = PARAM_OUTBYPASS_FIRST; i <= PARAM_OUTBYPASS_LAST; ++i) {
		parameterValues.push_back(DEF_OUTPUTBYPASS);
	}
	for (auto i = PARAM_MODULEVISIBLE_FIRST; i <= PARAM_MODULEVISIBLE_LAST; ++i) {
		parameterValues.push_back(DEF_MODULEVISIBLE);
	}
	for (auto i = PARAM_GENERALVSTOUTPUTSELECT_FIRST; i <= PARAM_GENERALVSTOUTPUTSELECT_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormMixerInputSelect(DEF_OUTPUTSELECT));
	}
	parameterValues.push_back(ValueConversion::plainToNormSignalType(DEF_SIGNALGENERATOR_SIGNALTYPE));
	parameterValues.push_back(ValueConversion::plainToNormSignalAmplitude(DEF_SIGNALGENERATOR_AMPLITUDE));
	parameterValues.push_back(ValueConversion::plainToNormSignalWidth(DEF_SIGNALGENERATOR_WIDTH));
	parameterValues.push_back(ValueConversion::plainToNormSignalTime(DEF_SIGNALGENERATOR_TIME));
	parameterValues.push_back(DEF_SIGNALGENERATOR_AUTOFIREENABLED);
	parameterValues.push_back(DEF_SIGNALGENERATOR_FIRE);
}

PresetReadWrite::~PresetReadWrite() {

}

Steinberg::tresult PresetReadWrite::setParamterState(Steinberg::IBStream* state) {
	// Load the filestream containing the parameter values
	Steinberg::IBStreamer streamer(state, kLittleEndian);

	Steinberg::uint32 maxVstInputs = 0;
	Steinberg::uint32 maxVstOutputs = 0;
	Steinberg::uint32 maxModuleNumber = 0;

	// Read the first three parameters
	if (!streamer.readInt32u(maxVstInputs)) return Steinberg::kResultFalse;
	if (!streamer.readInt32u(maxVstOutputs)) return Steinberg::kResultFalse;
	if (!streamer.readInt32u(maxModuleNumber)) return Steinberg::kResultFalse;

	// Check if those three parameters (which influence the number of all other parameters) match the #defines in the current Build
	// If one of them doesn't match it means that the preset is propably incompatible with the actual Build
	// ToDo: show message box for the user...
	if (maxVstInputs != MAXVSTINPUTS) return Steinberg::kResultFalse;
	if (maxVstOutputs != MAXVSTOUTPUTS) return Steinberg::kResultFalse;
	if (maxModuleNumber != MAXMODULENUMBER) return Steinberg::kResultFalse;

	for (Steinberg::uint32 i = 0; i < parameterValues.size(); ++i) {
		// Read and save the values
		if (!streamer.readDouble(parameterValues[i])) return Steinberg::kResultFalse;
	}

	return Steinberg::kResultTrue;
}

Steinberg::tresult PresetReadWrite::getParamterState(Steinberg::IBStream* state) const {
	Steinberg::IBStreamer streamer(state, kLittleEndian);

	// These three parameters determine whether a preset is compatible with the current Build or not
	if (!streamer.writeInt32u(MAXVSTINPUTS)) return Steinberg::kResultFalse;
	if (!streamer.writeInt32u(MAXVSTOUTPUTS)) return Steinberg::kResultFalse;
	if (!streamer.writeInt32u(MAXMODULENUMBER)) return Steinberg::kResultFalse;

	for (Steinberg::uint32 i = 0; i < parameterValues.size(); ++i) {
		// Read and save every parameter value
		if (!streamer.writeDouble(parameterValues[i])) return Steinberg::kResultFalse;
	}

	return Steinberg::kResultTrue;
}


