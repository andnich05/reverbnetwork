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

#include "XmlPresetReadWrite.h"
#include "../pugixml-1.6/src/pugixml.hpp"
#include <string>

XmlPresetReadWrite::XmlPresetReadWrite() {

}


XmlPresetReadWrite::~XmlPresetReadWrite() {

}


const XmlPresetReadWrite::Preset XmlPresetReadWrite::loadPreset(const char* filePath) const {
	Preset loadedPreset = {}; // Initialize
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filePath);

	// Read values from XML tree
	if (result) {
		pugi::xml_node tool = doc.child("reverbNetwork").child("preset").child("buildInformation");
		loadedPreset.buildVersion = tool.child("version").text().as_string();
		loadedPreset.buildDate = (long)tool.child("date").text().as_llong();
		loadedPreset.buildTime = (long)tool.child("time").text().as_llong();
		loadedPreset.name = doc.child("reverbNetwork").child("preset").child("name").text().as_string();
		loadedPreset.maxModuleNumber = tool.child("maxModuleNumber").text().as_int();
		loadedPreset.maxVstInputs = tool.child("maxVstInputs").text().as_int();
		loadedPreset.maxVstOutputs = tool.child("maxVstOutputs").text().as_int();

		for (tool = doc.child("reverbNetwork").child("preset").child("parameters").child("module"); tool; tool = tool.next_sibling("module")) {
			Module m = {};
			m.name = tool.child("name").text().as_string();
			m.id = tool.child("id").text().as_uint();
			m.positionX = tool.child("position").child("x").text().as_double();
			m.positionY = tool.child("position").child("y").text().as_double();
			m.isVisible = tool.child("isVisible").text().as_bool();
			m.isCollapsed = tool.child("isCollapsed").text().as_bool();

			Mixer mix = {};
			for (pugi::xml_node subTool = tool.child("mixer").child("moduleOut"); subTool; subTool = subTool.next_sibling("moduleOut")) {
				ModuleOutput mo = {};
				mo.gainFactor = subTool.child("gain").text().as_double();
				mo.muted = subTool.child("muted").text().as_bool();
				mo.soloed = subTool.child("soloed").text().as_bool();
				mix.moduleOutputs.push_back(mo);
			}
			for (pugi::xml_node subTool = tool.child("mixer").child("vstIn"); subTool; subTool = subTool.next_sibling("vstIn")) {
				VstInput vi = {};
				vi.gainFactor = subTool.child("gain").text().as_double();
				vi.muted = subTool.child("muted").text().as_bool();
				vi.soloed = subTool.child("soloed").text().as_bool();
				mix.vstInputs.push_back(vi);
			}
			pugi::xml_node sgTool = tool.child("mixer").child("signalGeneratorIn");
			SignalGeneratorInput sgI = {};
			sgI.gainFactor = sgTool.child("gain").text().as_double();
			sgI.muted = sgTool.child("muted").text().as_bool();
			sgI.soloed = sgTool.child("soloed").text().as_bool();
			mix.signalGeneratorInput = sgI;

			for (pugi::xml_node subTool = tool.child("mixer").child("inputSlot"); subTool; subTool = subTool.next_sibling("inputSlot")) {
				mix.inputSlots.push_back(subTool.text().as_int());
			}
			m.mixerParamters = mix;

			Quantizer q = {};
			q.bypass = tool.child("quantizer").child("bypass").text().as_bool();
			q.quantization = tool.child("quantizer").child("quantization").text().as_int();
			m.quantizerParamters = q;

			Equalizer e = {};
			e.bypass = tool.child("equalizer").child("bypass").text().as_bool();
			e.filterTypeIndex = tool.child("equalizer").child("filterTypeIndex").text().as_int();
			e.frequency = tool.child("equalizer").child("frequency").text().as_double();
			e.qFactor = tool.child("equalizer").child("qFactor").text().as_double();
			e.gain = tool.child("equalizer").child("gain").text().as_double();
			e.a0 = tool.child("equalizer").child("a0").text().as_double();
			e.a1 = tool.child("equalizer").child("a1").text().as_double();
			e.a2 = tool.child("equalizer").child("a2").text().as_double();
			e.b1 = tool.child("equalizer").child("b1").text().as_double();
			e.b2 = tool.child("equalizer").child("b2").text().as_double();
			m.equalizerParameters = e;

			Allpass a = {};
			a.bypass = tool.child("allpass").child("bypass").text().as_bool();
			a.delay = tool.child("allpass").child("delay").text().as_double();
			a.modulationEnabled = tool.child("allpass").child("modulationEnabled").text().as_bool();
			a.modulationExcursion = tool.child("allpass").child("modulationExcursion").text().as_double();
			a.modulationRate = tool.child("allpass").child("modulationRate").text().as_double();
			a.decay = tool.child("allpass").child("decay").text().as_double();
			a.diffKSignNegative = tool.child("allpass").child("diffKSignNegative").text().as_bool();
			m.allpassParameters = a;

			Output o = {};
			o.bypass = tool.child("output").child("bypass").text().as_bool();
			o.gain = tool.child("output").child("gain").text().as_double();
			m.outputParameters = o;

			loadedPreset.modules.push_back(m);
		}

		General g = {};
		for (tool = doc.child("reverbNetwork").child("preset").child("parameters").child("general").child("vstOutputMenuIndex"); tool; tool = tool.next_sibling("vstOutputMenuIndex")) {
			g.vstOutputMenuIndexes.push_back(tool.text().as_int());
		}
		loadedPreset.generalParamters = g;

		SignalGenerator sG = {};
		tool = doc.child("reverbNetwork").child("preset").child("signalGenerator");
		sG.signalType = tool.child("signalType").text().as_int();
		sG.gain = tool.child("gain").text().as_double();
		sG.width = tool.child("width").text().as_int();
		sG.time = tool.child("time").text().as_double();
		loadedPreset.signalGenerator = sG;

		GraphicsView gV = {};
		for (tool = doc.child("reverbNetwork").child("preset").child("graphicsView").child("module"); tool; tool = tool.next_sibling("module")) {
			GraphicsModule gM = {};
			gM.isVisible = tool.child("isVisible").text().as_bool();
			gM.positionX = tool.child("position").child("x").text().as_double();
			gM.positionY = tool.child("position").child("y").text().as_double();
			gV.modules.push_back(gM);
		}
		for (tool = doc.child("reverbNetwork").child("preset").child("graphicsView").child("vstInput"); tool; tool = tool.next_sibling("vstInput")) {
			GraphicsVstInput gI = {};
			gI.positionX = tool.child("position").child("x").text().as_double();
			gI.positionY = tool.child("position").child("y").text().as_double();
			gV.vstInputs.push_back(gI);
		}
		for (tool = doc.child("reverbNetwork").child("preset").child("graphicsView").child("vstOutput"); tool; tool = tool.next_sibling("vstOutput")) {
			GraphicsVstOutput gO = {};
			gO.positionX = tool.child("position").child("x").text().as_double();
			gO.positionY = tool.child("position").child("y").text().as_double();
			gV.vstOutputs.push_back(gO);
		}
		loadedPreset.graphicsView = gV;

		/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(tool.child("name").text().as_double()).c_str());
		fclose(pFile);*/
	}
	else {
		// Error
	}

	return loadedPreset;
}

void XmlPresetReadWrite::savePreset(const char* filePath, const Preset& p) const {
	pugi::xml_document doc;
	pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
	// Add a custom header declaration in the xml file
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "UTF-8";

	// Create XML tree
	pugi::xml_node presetNode = doc.append_child("reverbNetwork").append_child("preset");

	pugi::xml_node infoNode = presetNode.append_child("buildInformation");
	infoNode.append_child("version").text().set(p.buildVersion.c_str());
	infoNode.append_child("date").text().set(p.buildDate);
	infoNode.append_child("time").text().set(p.buildTime);
	infoNode.append_child("maxModuleNumber").text().set(p.maxModuleNumber);
	infoNode.append_child("maxVstInputs").text().set(p.maxVstInputs);
	infoNode.append_child("maxVstOutputs").text().set(p.maxVstOutputs);

	presetNode.append_child("name").text().set(p.name.c_str());
	pugi::xml_node paramNode = presetNode.append_child("parameters");

	for (auto&& module : p.modules) {
		// General module parameters
		pugi::xml_node moduleNode = paramNode.append_child("module");
		moduleNode.append_child("name").text().set(module.name.c_str());
		moduleNode.append_child("id").text().set(module.id);
		moduleNode.append_child("position").append_child("x").text().set(module.positionX);
		moduleNode.child("position").append_child("y").text().set(module.positionY);
		moduleNode.append_child("isVisible").text().set(module.isVisible);
		moduleNode.append_child("isCollapsed").text().set(module.isCollapsed);

		// Mixer
		pugi::xml_node mixerNode = moduleNode.append_child("mixer");
		for (auto&& mOut : module.mixerParamters.moduleOutputs) {
			pugi::xml_node mOutNode = mixerNode.append_child("moduleOut");
			mOutNode.append_child("gain").text().set(mOut.gainFactor);
			mOutNode.append_child("muted").text().set(mOut.muted);
			mOutNode.append_child("soloed").text().set(mOut.soloed);
		}
		for (auto&& vIn : module.mixerParamters.vstInputs) {
			pugi::xml_node vInNone = mixerNode.append_child("vstIn");
			vInNone.append_child("gain").text().set(vIn.gainFactor);
			vInNone.append_child("muted").text().set(vIn.muted);
			vInNone.append_child("soloed").text().set(vIn.soloed);
		}
		pugi::xml_node sgIn = mixerNode.append_child("signalGeneratorIn");
		sgIn.append_child("gain").text().set(module.mixerParamters.signalGeneratorInput.gainFactor);
		sgIn.append_child("muted").text().set(module.mixerParamters.signalGeneratorInput.muted);
		sgIn.append_child("soloed").text().set(module.mixerParamters.signalGeneratorInput.soloed);
		for (auto&& inputSlot : module.mixerParamters.inputSlots) {
			mixerNode.append_child("inputSlot").text().set(inputSlot);
		}
		
		// Quantizer
		moduleNode.append_child("quantizer");
		moduleNode.child("quantizer").append_child("bypass").text().set(module.quantizerParamters.bypass);
		moduleNode.child("quantizer").append_child("quantization").text().set(module.quantizerParamters.quantization);

		// Equalizer
		moduleNode.append_child("equalizer");
		moduleNode.child("equalizer").append_child("bypass").text().set(module.equalizerParameters.bypass);
		moduleNode.child("equalizer").append_child("filterTypeIndex").text().set(module.equalizerParameters.filterTypeIndex);
		moduleNode.child("equalizer").append_child("frequency").text().set(module.equalizerParameters.frequency);
		moduleNode.child("equalizer").append_child("qFactor").text().set(module.equalizerParameters.qFactor);
		moduleNode.child("equalizer").append_child("gain").text().set(module.equalizerParameters.gain);
		moduleNode.child("equalizer").append_child("a0").text().set(module.equalizerParameters.a0);
		moduleNode.child("equalizer").append_child("a1").text().set(module.equalizerParameters.a1);
		moduleNode.child("equalizer").append_child("a2").text().set(module.equalizerParameters.a2);
		moduleNode.child("equalizer").append_child("b1").text().set(module.equalizerParameters.b1);
		moduleNode.child("equalizer").append_child("b2").text().set(module.equalizerParameters.b2);

		// Allpass
		moduleNode.append_child("allpass");
		moduleNode.child("allpass").append_child("bypass").text().set(module.allpassParameters.bypass);
		moduleNode.child("allpass").append_child("delay").text().set(module.allpassParameters.delay);
		moduleNode.child("allpass").append_child("modulationEnabled").text().set(module.allpassParameters.modulationEnabled);
		moduleNode.child("allpass").append_child("modulationExcursion").text().set(module.allpassParameters.modulationExcursion);
		moduleNode.child("allpass").append_child("modulationRate").text().set(module.allpassParameters.modulationRate);
		moduleNode.child("allpass").append_child("decay").text().set(module.allpassParameters.decay);
		moduleNode.child("allpass").append_child("diffKSignPositive").text().set(module.allpassParameters.diffKSignNegative);

		// Output
		moduleNode.append_child("output");
		moduleNode.child("output").append_child("bypass").text().set(module.outputParameters.bypass);
		moduleNode.child("output").append_child("gain").text().set(module.outputParameters.gain);
	}

	// General parameters
	pugi::xml_node generalNode = paramNode.append_child("general");
	for (auto&& vstOut : p.generalParamters.vstOutputMenuIndexes) {
		generalNode.append_child("vstOutputMenuIndex").text().set(vstOut);
	}

	pugi::xml_node signalGenNode = presetNode.append_child("signalGenerator");
	signalGenNode.append_child("signalType").text().set(p.signalGenerator.signalType);
	signalGenNode.append_child("gain").text().set(p.signalGenerator.gain);
	signalGenNode.append_child("width").text().set(p.signalGenerator.width);
	signalGenNode.append_child("time").text().set(p.signalGenerator.time);

	// Graphics View
	pugi::xml_node graphicsNode = presetNode.append_child("graphicsView");
	for (auto&& module : p.graphicsView.modules) {
		pugi::xml_node moduleNode = graphicsNode.append_child("module");
		moduleNode.append_child("position").append_child("x").text().set(module.positionX);
		moduleNode.child("position").append_child("y").text().set(module.positionY);
		moduleNode.append_child("isVisible").text().set(module.isVisible);
	}
	for (auto&& vstInput : p.graphicsView.vstInputs) {
		pugi::xml_node moduleNode = graphicsNode.append_child("vstInput");
		moduleNode.append_child("position").append_child("x").text().set(vstInput.positionX);
		moduleNode.child("position").append_child("y").text().set(vstInput.positionY);
	}
	for (auto&& vstOutput : p.graphicsView.vstOutputs) {
		pugi::xml_node moduleNode = graphicsNode.append_child("vstOutput");
		moduleNode.append_child("position").append_child("x").text().set(vstOutput.positionX);
		moduleNode.child("position").append_child("y").text().set(vstOutput.positionY);
	}

	// Save XML file
	bool result = false;
	const char* filenameExtensionXml = strstr(filePath, ".xml");
	const char* filenameExtensionXML = strstr(filePath, ".XML");
	if (!filenameExtensionXml && !filenameExtensionXML) {
		// No file extension specified => add one
		char tempFilepath[4096];
		strncpy(tempFilepath, filePath, 4094);
		strncat(tempFilepath, ".xml", 6);
		result = doc.save_file(tempFilepath, "\t", pugi::format_no_declaration | pugi::format_indent);
	}
	else {
		result = doc.save_file(filePath, "\t", pugi::format_no_declaration | pugi::format_indent);
	}
	if (result) {
		// Success
	}
	else {
		// Error
	}
}