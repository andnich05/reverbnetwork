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

	struct ModuleOutput {
		double gainFactor;
		bool muted;
		bool soloed;
	};

	struct VstInput {
		double gainFactor;
		bool muted;
		bool soloed;
	};

	struct SignalGeneratorInput {
		double gainFactor;
		bool muted;
		bool soloed;
	};

	struct Mixer {
		std::vector<ModuleOutput> moduleOutputs;
		std::vector<VstInput> vstInputs;
		SignalGeneratorInput signalGeneratorInput;
		std::vector<int> inputSlots;
	};

	struct Quantizer {
		bool bypass;
		int quantization;
	};

	struct Equalizer {
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
		bool limiter;
	};

	struct Allpass {
		bool bypass;
		double delay;
		bool modulationEnabled;
		int modulationSignalType;
		double modulationExcursion;
		double modulationRate;
		double decay;
		bool diffKSignNegative;
	};

	struct Output {
		bool bypass;
		double gain;
		bool limiter;
	};

	struct Module {
		std::string name;
		unsigned int id;
		double positionX;
		double positionY;
		bool isVisible;
		bool isCollapsed;
		Mixer mixerParameters;
		Quantizer quantizerParameters;
		Equalizer equalizerParameters;
		Allpass allpassParameters;
		Output outputParameters;
	};

	struct General {
		std::vector<int> vstOutputMenuIndexes;
	};

	struct GraphicsModule {
		bool isVisible;
		double positionX;
		double positionY;
	};

	struct GraphicsVstInput {
		double positionX;
		double positionY;
	};

	struct GraphicsVstOutput {
		double positionX;
		double positionY;
	};

	struct SignalGenerator {
		int signalType;
		double gain;
		int width;
		double time;
		bool autoFireEnabled;
	};

	struct GraphicsView {
		std::vector<GraphicsModule> modules;
		std::vector<GraphicsVstInput> vstInputs;
		std::vector<GraphicsVstOutput> vstOutputs;
	};

	// Main structure
	struct Preset {
		std::string name;
		std::string buildVersion;
		long int buildDate;
		long int buildTime;
		int maxModuleNumber;
		int maxVstInputs;
		int maxVstOutputs;
		std::vector<Module> modules;
		SignalGenerator signalGenerator;
		GraphicsView graphicsView;
		General generalParamters;
	};

	// Load a XML preset from disc, returns a structure with all preset parameters
	const Preset loadPreset(const char* filePath) const;
	// Save a XML preset to disc
	void savePreset(const char* filePath, const Preset& p) const;

private:
	
};

#endif // XMLPRESETREADWRITE_H