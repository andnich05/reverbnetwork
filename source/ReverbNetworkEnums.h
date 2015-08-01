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