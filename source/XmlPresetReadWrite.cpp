#include "XmlPresetReadWrite.h"
#include "../pugixml-1.6/src/pugixml.hpp"
#include <string>

XmlPresetReadWrite::XmlPresetReadWrite() {

}


XmlPresetReadWrite::~XmlPresetReadWrite() {

}


const XmlPresetReadWrite::preset XmlPresetReadWrite::loadPreset(const char* filePath) {
	preset loadedPreset = {};
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filePath);
	if (result) {
		pugi::xml_node tool = doc.child("reverbNetwork").child("preset").child("buildInformation");
		loadedPreset.buildDate = (long)tool.child("date").text().as_llong();
		loadedPreset.buildTime = (long)tool.child("time").text().as_llong();
		loadedPreset.name = doc.child("reverbNetwork").child("preset").child("name").text().as_string();
		loadedPreset.maxModuleNumber = tool.child("maxModuleNumber").text().as_int();
		loadedPreset.maxVstInputs = tool.child("maxVstInputs").text().as_int();
		loadedPreset.maxVstOutputs = tool.child("maxVstOutputs").text().as_int();

		//tool = doc.child("reverbNetwork").child("preset").child("parameters");
		for (tool = doc.child("reverbNetwork").child("preset").child("parameters").child("module"); tool; tool = tool.next_sibling("module")) {
			module m = {};
			m.name = tool.child("name").text().as_string();
			m.id = tool.child("id").text().as_uint();
			m.positionX = tool.child("position").child("x").text().as_double();
			m.positionY = tool.child("position").child("y").text().as_double();
			m.isVisible = tool.child("isVisible").text().as_bool();
			m.isCollapsed = tool.child("isCollapsed").text().as_bool();

			mixer mix = {};
			for (pugi::xml_node subTool = tool.child("mixer").child("moduleOut"); subTool; subTool = subTool.next_sibling("moduleOut")) {
				moduleOutput mo = {};
				mo.gainFactor = subTool.child("gain").text().as_double();
				mo.muted = subTool.child("muted").text().as_bool();
				mo.soloed = subTool.child("soloed").text().as_bool();
				mix.moduleOutputs.push_back(mo);
			}
			for (pugi::xml_node subTool = tool.child("mixer").child("vstIn"); subTool; subTool = subTool.next_sibling("vstIn")) {
				vstInput vi = {};
				vi.gainFactor = subTool.child("gain").text().as_double();
				vi.muted = subTool.child("muted").text().as_bool();
				vi.soloed = subTool.child("soloed").text().as_bool();
				mix.vstInputs.push_back(vi);
			}
			for (pugi::xml_node subTool = tool.child("mixer").child("inputSlot"); subTool; subTool = subTool.next_sibling("inputSlot")) {
				mix.inputSlots.push_back(subTool.text().as_int());
			}
			m.mixerParamters = mix;

			quantizer q = {};
			q.bypass = tool.child("quantizer").child("bypass").text().as_bool();
			q.quantization = tool.child("quantizer").child("quantization").text().as_int();
			m.quantizerParamters = q;

			equalizer e = {};
			e.bypass = tool.child("equalizer").child("bypass").text().as_bool();
			e.filterTypeIndex = tool.child("equalizer").child("filterTypeIndex").text().as_int();
			e.frequency = tool.child("equalizer").child("frequency").text().as_double();
			e.qFactor = tool.child("equalizer").child("qFactor").text().as_double();
			e.gain = tool.child("equalizer").child("gain").text().as_double();
			m.equalizerParameters = e;

			allpass a = {};
			a.bypass = tool.child("allpass").child("bypass").text().as_bool();
			a.delay = tool.child("allpass").child("delay").text().as_double();
			a.decay = tool.child("allpass").child("decay").text().as_double();
			m.allpassParameters = a;

			output o = {};
			o.bypass = tool.child("output").child("bypass").text().as_bool();
			o.gain = tool.child("output").child("gain").text().as_double();
			m.outputParameters = o;

			loadedPreset.modules.push_back(m);
		}

		general g = {};
		for (tool = doc.child("reverbNetwork").child("preset").child("parameters").child("general").child("vstOutputMenuIndex"); tool; tool = tool.next_sibling("vstOutputMenuIndex")) {
			g.vstOutputMenuIndexes.push_back(tool.text().as_int());
		}
		loadedPreset.generalParamters = g;

		/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(tool.child("name").text().as_double()).c_str());
		fclose(pFile);*/
	}
	else {
		// Error
	}

	return loadedPreset;
}

void XmlPresetReadWrite::savePreset(const char* filePath, const preset& p) {
	pugi::xml_document doc;
	pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
	// Add a custom header declaration in the xml file
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "UTF-8";

	pugi::xml_node presetNode = doc.append_child("reverbNetwork").append_child("preset");

	pugi::xml_node infoNode = presetNode.append_child("buildInformation");
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

		// Allpass
		moduleNode.append_child("allpass");
		moduleNode.child("allpass").append_child("bypass").text().set(module.allpassParameters.bypass);
		moduleNode.child("allpass").append_child("delay").text().set(module.allpassParameters.delay);
		moduleNode.child("allpass").append_child("decay").text().set(module.allpassParameters.decay);

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