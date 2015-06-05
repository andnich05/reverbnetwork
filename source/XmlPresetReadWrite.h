#ifndef XMLPRESETREADWRITE_H
#define XMLPRESETREADWRITE_H

#include <vector>

class XmlPresetReadWrite
{
public:
	XmlPresetReadWrite();
	~XmlPresetReadWrite();

	struct inputSlot {
		int optionMenuIndex;
		double gainFactor;
		bool muted;
		bool soloed;
	};

	struct mixer {
		std::vector<inputSlot> inputSlots;
	};

	struct quantizer {
		bool bypass;
		int quantization;
	};

	struct equalizer {
		bool bypass;
		int filterTypeIndex;
		double frequency;
		double qFactor;
		double gain;
	};

	struct allpass {
		bool bypass;
		double delay;
		double decay;
	};

	struct output {
		bool bypass;
		double gain;
	};

	struct module {
		std::string name;
		double positionX;
		double positionY;
		bool isVisible;
		bool isCollapsed;
		mixer mixerParamters;
		quantizer quantizerParamters;
		equalizer equalizerParameters;
		allpass allpassParameters;
		output outputParameters;
	};

	struct general {
		std::vector<int> vstOutputMenuIndexes;
	};

	struct preset {
		std::string name;
		long int buildDate;
		long int buildTime;
		int maxModuleNumber;
		int maxVstInputs;
		int maxVstOutputs;
		std::vector<module> modules;
		general generalParamters;
	};

	static const preset& loadPreset(const char* filePath);

	static preset loadedPreset;
	
};

#endif // XMLPRESETREADWRITE_H