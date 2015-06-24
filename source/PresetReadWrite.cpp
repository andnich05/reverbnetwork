#include "PresetReadWrite.h"
#include "ReverbNetworkDefines.h"
#include "ValueConversion.h"
#include <string>

PresetReadWrite::PresetReadWrite() {
	// Initialize the values with the default values (Must be in the same order as in ReverbNetworkDefines.h)
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
		parameterValues.push_back(ValueConversion::plainToNormVstCenterFreq(DEF_EQCENTERFREQ));
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
	for (auto i = PARAM_EQBYPASS_FIRST; i <= PARAM_EQBYPASS_LAST; ++i) {
		parameterValues.push_back(DEF_EQBYPASS);
	}
	for (auto i = PARAM_ALLPASSDELAY_FIRST; i <= PARAM_ALLPASSDELAY_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormDelay(DEF_ALLPASSDELAY));
	}
	for (auto i = PARAM_ALLPASSDECAY_FIRST; i <= PARAM_ALLPASSDECAY_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormDecay(DEF_ALLPASSDECAY));
	}
	for (auto i = PARAM_ALLPASSBYPASS_FIRST; i <= PARAM_ALLPASSBYPASS_LAST; ++i) {
		parameterValues.push_back(DEF_ALLPASSBYPASS);
	}
	for (auto i = PARAM_OUTGAIN_FIRST; i <= PARAM_OUTGAIN_LAST; ++i) {
		parameterValues.push_back(ValueConversion::plainToNormOutputGain(DEF_OUTPUTGAIN));
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
	// If one of them doesn't match it means that the preset is incompatible with the actaul Build
	// ToDo: show message box for the user...
	if (maxVstInputs != MAXVSTINPUTS) return Steinberg::kResultFalse;
	if (maxVstOutputs != MAXVSTOUTPUTS) return Steinberg::kResultFalse;
	if (maxModuleNumber != MAXMODULENUMBER) return Steinberg::kResultFalse;

	for (Steinberg::uint32 i = 0; i < parameterValues.size(); ++i) {
		// Read and save the values
		if (!streamer.readDouble(parameterValues[i])) return Steinberg::kResultFalse;
	}

	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(parameterValues[PARAM_GENERALVSTOUTPUTSELECT_FIRST]).c_str());
	fclose(pFile);*/

	return Steinberg::kResultTrue;
}

Steinberg::tresult PresetReadWrite::getParamterState(Steinberg::IBStream* state) {
	Steinberg::IBStreamer streamer(state, kLittleEndian);

	// These three parameters determine whether a preset is compatible with the current Build or not
	if (!streamer.writeInt32u(MAXVSTINPUTS)) return Steinberg::kResultFalse;
	if (!streamer.writeInt32u(MAXVSTOUTPUTS)) return Steinberg::kResultFalse;
	if (!streamer.writeInt32u(MAXMODULENUMBER)) return Steinberg::kResultFalse;

	for (Steinberg::uint32 i = 0; i < parameterValues.size(); ++i) {
		// Read and save the values
		if (!streamer.writeDouble(parameterValues[i])) return Steinberg::kResultFalse;
	}

	return Steinberg::kResultTrue;
}


