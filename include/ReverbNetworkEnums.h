/*
* ReverbNetworkEnums: Enumerations for the ReverbNetwork plug-in
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

#ifndef REVERBNETWORKENUMS_H
#define REVERBNETWORKENUMS_H

#include <string>

// Possible equalizer filter types
enum FilterType {
	lowPass,
	highPass,
	bandPassStop,
	lowShelf,
	highShelf,
	rawBiquad,
	numberOfFilterTypes
};

// Possible modulation signal types
enum ModulationSignalType {
	sine,
	triangle,
	numberOfSignalTypes
};

// Possible signal generator types
enum SignalGeneratorType {
	dirac,
	numberOfSignalGeneratorTypes
};

// User data for the valueToStringConverion function of the text edits (GUI)
struct valueToStringUserData{
	int precision;
	std::string unit;
};

// Message structure from Processor to Controller to Editor
struct EqualizerStability {
	int moduleNumber;
	bool isStable;
};

#endif // REVERBNETWORKENUMS_H