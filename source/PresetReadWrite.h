#ifndef PRESETREADWRITE_H
#define PRESETREADWRITE_H

#include <vector>
#include "base/source/fstreamer.h"

class PresetReadWrite
{
public:
	PresetReadWrite();
	~PresetReadWrite();

	// IBStream contains a filestream of a VST preset file which contains a "state" (the values) of all VST parameters
	// Read the VST parameter values from the filestream and save them ("Load Preset")
	Steinberg::tresult setParamterState(Steinberg::IBStream* state);
	// Write the saved parameter values to the filestream ("Save Preset")
	Steinberg::tresult getParamterState(Steinberg::IBStream* state) const;

	// Set a parameter (has to be done in Processor)
	inline void setNormValueByParamId(const double& normalizeValue, const unsigned long& parameterId) { 
		if (parameterId < parameterValues.size()) parameterValues[parameterId] = normalizeValue; 
	}

	// Get a parameter (has to be done in Controller)
	inline double getNormValueByParamId(const unsigned long& parameterId) const {
		if (parameterId < parameterValues.size()) return parameterValues[parameterId]; else return 0.0;
	}

private:
	// Parameter values, index is the unique id defined in ReverbNetworkDefines.h, all values are normalized
	std::vector<double> parameterValues;
};

#endif // PRESETREADWRITE_H