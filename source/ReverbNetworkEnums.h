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

struct valueToStringUserData{
	int precision;
	std::string unit;
};

#endif // REVERBNETWORKENUMS_H