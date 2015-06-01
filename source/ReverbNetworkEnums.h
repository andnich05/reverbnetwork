#ifndef REVERBNETWORKENUMS_H
#define REVERBNETWORKENUMS_H

// Possible equalizer filter types
enum FilterType
{
	lowPass,
	highPass,
	bandPassStop,
	lowShelf,
	highShelf,
	numberOfFilterTypes
};

struct valueToStringUserData{
	int precision;
	const char* unit;
};

#endif // REVERBNETWORKENUMS_H