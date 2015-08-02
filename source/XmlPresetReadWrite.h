/*
* XmlPresetReadWrite: Load and save XML presets
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

#ifndef XMLPRESETREADWRITE_H
#define XMLPRESETREADWRITE_H

#include <vector>

class XmlPresetReadWrite
{
public:
	XmlPresetReadWrite();
	~XmlPresetReadWrite();


	//---Structures for all plugin components

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
		double a0;
		double a1;
		double a2;
		double b1;
		double b2;
	};

	struct allpass {
		bool bypass;
		double delay;
		bool modulationEnabled;
		double modulationExcursion;
		double modulationRate;
		double decay;
		bool diffKSignNegative;
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

	struct graphicsModule {
		bool isVisible;
		double positionX;
		double positionY;
	};

	struct graphicsVstInput {
		double positionX;
		double positionY;
	};

	struct graphicsVstOutput {
		double positionX;
		double positionY;
	};

	struct signalGenerator {
		int signalType;
		double gain;
		int width;
		double time;
	};

	struct graphicsView {
		std::vector<graphicsModule> modules;
		std::vector<graphicsVstInput> vstInputs;
		std::vector<graphicsVstOutput> vstOutputs;
	};

	// Main structure
	struct preset {
		std::string name;
		std::string buildVersion;
		long int buildDate;
		long int buildTime;
		int maxModuleNumber;
		int maxVstInputs;
		int maxVstOutputs;
		std::vector<module> modules;
		signalGenerator signalGenerator;
		graphicsView graphicsView;
		general generalParamters;
	};

	// Load a XML preset from disc, returns a structure with all preset parameters
	const preset loadPreset(const char* filePath) const;
	// Save a XML preset to disc
	void savePreset(const char* filePath, const preset& p) const;

private:
	
};

#endif // XMLPRESETREADWRITE_H