#ifndef XMLPRESETREADWRITE_H
#define XMLPRESETREADWRITE_H

#include <vector>

class XmlPresetReadWrite
{
public:
	XmlPresetReadWrite();
	~XmlPresetReadWrite();

	struct moduleOutput {
		double gainFactor;
		bool muted;
		bool soloed;
	};

	struct vstInput {
		double gainFactor;
		bool muted;
		bool soloed;
	};

	struct mixer {
		std::vector<moduleOutput> moduleOutputs;
		std::vector<vstInput> vstInputs;
		std::vector<int> inputSlots;
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
		unsigned int id;
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
		std::string buildVersion;
		long int buildDate;
		long int buildTime;
		int maxModuleNumber;
		int maxVstInputs;
		int maxVstOutputs;
		std::vector<module> modules;
		general generalParamters;
	};

	const preset loadPreset(const char* filePath);
	void savePreset(const char* filePath, const preset& p);

private:
	
};

#endif // XMLPRESETREADWRITE_H