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

#ifndef PRESETREADWRITE_H
#define PRESETREADWRITE_H

#include <vector>
#include "base/source/fstreamer.h"


class PresetReadWrite
{
public:
	PresetReadWrite();
	~PresetReadWrite();

	// IBStream contains a filestream of a VST preset file (in binary form) which contains a "state" (the values) of all VST parameters

	// Read the VST parameter values from the filestream and save them ("Load Preset")
	Steinberg::tresult setParamterState(Steinberg::IBStream* state);
	// Write the saved parameter values to the filestream ("Save Preset")
	Steinberg::tresult getParamterState(Steinberg::IBStream* state) const;

	// Set a parameter (has to be done in Processor process function (input parameter changes))
	inline void setNormValueByParamId(const double& normalizeValue, const unsigned long& parameterId) { 
		if (parameterId < parameterValues.size()) parameterValues[parameterId] = normalizeValue; 
	}

	// Get a parameter (has to be done in Controller)
	inline double getNormValueByParamId(const unsigned long& parameterId) const {
		if (parameterId < parameterValues.size()) return parameterValues[parameterId]; else return 0.0;
	}

private:
	// All parameter values, index is the unique id defined in ReverbNetworkDefines.h, all values are normalized
	std::vector<double> parameterValues;
};

#endif // PRESETREADWRITE_H