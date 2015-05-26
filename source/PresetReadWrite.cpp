#include "PresetReadWrite.h"
#include "ReverbNetworkDefines.h"
#include "ValueConversion.h"
#include <string>

PresetReadWrite::PresetReadWrite() {
	// Initialize the values with the default values (Must be in the same order as in ReverbNetworkDefines.h)
	for (unsigned int i = PARAM_MIXERINPUTSELECT_FIRST; i <= PARAM_MIXERINPUTSELECT_LAST; ++i) {
		parameterValues.push_back(ValueConversion::valueToNormMixerInputSelect(DEF_MIXERINPUT));
	}
	for (unsigned int i = PARAM_MIXERGAIN_FIRST; i <= PARAM_MIXERGAIN_LAST; ++i) {
		parameterValues.push_back(ValueConversion::valueToNormGain(DEF_MIXERGAIN));
	}
	for (unsigned int i = PARAM_MIXERBYPASS_FIRST; i <= PARAM_MIXERBYPASS_LAST; ++i) {
		parameterValues.push_back(DEF_MIXERBYPASS);
	}
	for (unsigned int i = PARAM_EQFILTERTYPE_FIRST; i <= PARAM_EQFILTERTYPE_LAST; ++i) {
		parameterValues.push_back(ValueConversion::valueToNormFilterTypeSelect(DEF_EQFILTERTYPE));
	}
	for (unsigned int i = PARAM_EQCENTERFREQ_FIRST; i <= PARAM_EQCENTERFREQ_LAST; ++i) {
		parameterValues.push_back(ValueConversion::valueToNormCenterFreq(DEF_EQCENTERFREQ));
	}
	for (unsigned int i = PARAM_EQQFACTOR_FIRST; i <= PARAM_EQQFACTOR_LAST; ++i) {
		parameterValues.push_back(ValueConversion::valueToNormQFactor(DEF_EQQFACTOR));
	}
	for (unsigned int i = PARAM_EQGAIN_FIRST; i <= PARAM_EQGAIN_LAST; ++i) {
		parameterValues.push_back(ValueConversion::valueToNormGain(DEF_EQGAIN));
	}
	for (unsigned int i = PARAM_EQBYPASS_FIRST; i <= PARAM_EQBYPASS_LAST; ++i) {
		parameterValues.push_back(DEF_EQBYPASS);
	}
	for (unsigned int i = PARAM_ALLPASSDELAY_FIRST; i <= PARAM_ALLPASSDELAY_LAST; ++i) {
		parameterValues.push_back(ValueConversion::valueToNormDelay(DEF_ALLPASSDELAY));
	}
	for (unsigned int i = PARAM_ALLPASSDECAY_FIRST; i <= PARAM_ALLPASSDECAY_LAST; ++i) {
		parameterValues.push_back(ValueConversion::valueToNormDecay(DEF_ALLPASSDECAY));
	}
	for (unsigned int i = PARAM_ALLPASSBYPASS_FIRST; i <= PARAM_ALLPASSBYPASS_LAST; ++i) {
		parameterValues.push_back(DEF_ALLPASSBYPASS);
	}
	for (unsigned int i = PARAM_OUTGAIN_FIRST; i <= PARAM_OUTGAIN_LAST; ++i) {
		parameterValues.push_back(ValueConversion::valueToNormGain(DEF_OUTPUTGAIN));
	}
	for (unsigned int i = PARAM_OUTBYPASS_FIRST; i <= PARAM_OUTBYPASS_LAST; ++i) {
		parameterValues.push_back(DEF_OUTPUTBYPASS);
	}
	for (unsigned int i = PARAM_MODULEVISIBLE_FIRST; i <= PARAM_MODULEVISIBLE_LAST; ++i) {
		parameterValues.push_back(DEF_MODULEVISIBLE);
	}
	for (unsigned int i = PARAM_PPMUPDATE_FIRST; i <= PARAM_PPMUPDATE_LAST; ++i) {
		parameterValues.push_back(DEF_PPMVALUE);
	}
	for (unsigned int i = PARAM_GENERALVSTOUTPUTSELECT_FIRST; i <= PARAM_GENERALVSTOUTPUTSELECT_LAST; ++i) {
		parameterValues.push_back(ValueConversion::valueToNormMixerInputSelect(DEF_OUTPUTSELECT));
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

	// These three parameters determine whether a preset is compatible with the current Build or not!
	if (!streamer.writeInt32u(MAXVSTINPUTS)) return Steinberg::kResultFalse;
	if (!streamer.writeInt32u(MAXVSTOUTPUTS)) return Steinberg::kResultFalse;
	if (!streamer.writeInt32u(MAXMODULENUMBER)) return Steinberg::kResultFalse;

	for (Steinberg::uint32 i = 0; i < parameterValues.size(); ++i) {
		// Read and save the values
		if (!streamer.writeDouble(parameterValues[i])) return Steinberg::kResultFalse;
	}

	return Steinberg::kResultTrue;
}


