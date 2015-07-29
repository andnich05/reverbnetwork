#ifndef REVERBNETWORKENUMS_H
#define REVERBNETWORKENUMS_H

#include <string>

// Possible equalizer filter types
enum FilterType
{
	lowPass,
	highPass,
	bandPassStop,
	lowShelf,
	highShelf,
	rawBiquad,
	numberOfFilterTypes
};

enum SignalGeneratorType
{
	dirac,
	numberOfSignalGeneratorTypes
};

struct valueToStringUserData{
	int precision;
	std::string unit;
};

struct EqualizerStability {
	int moduleNumber;
	bool isStable;
};

#endif // REVERBNETWORKENUMS_H