#include "XmlPresetReadWrite.h"
#include "../pugixml-1.6/src/pugixml.hpp"
#include <string>

XmlPresetReadWrite::preset XmlPresetReadWrite::loadedPreset;

XmlPresetReadWrite::XmlPresetReadWrite() {

}


XmlPresetReadWrite::~XmlPresetReadWrite() {

}


const XmlPresetReadWrite::preset& XmlPresetReadWrite::loadPreset(const char* filePath) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filePath);
	if (result) {
		pugi::xml_node tool = doc.child("reverbNetwork").child("preset").child("buildInformation");
		loadedPreset.buildDate = (long)tool.child("date").text().as_llong();
		loadedPreset.buildTime = (long)tool.child("time").text().as_llong();
		loadedPreset.maxModuleNumber = tool.child("maxModuleNumber").text().as_int();
		loadedPreset.maxVstInputs = tool.child("maxVstInputs").text().as_int();
		loadedPreset.maxVstOutputs = tool.child("maxVstOutputs").text().as_int();

		loadedPreset.name = doc.child("reverbNetwork").child("preset").child("name").text().as_string();

		//tool = doc.child("reverbNetwork").child("preset").child("parameters");
		for (tool = doc.child("reverbNetwork").child("preset").child("parameters").child("module"); tool; tool = tool.next_sibling("module")) {
			module m;
			m.name = tool.child("name").text().as_string();
			m.id = tool.child("id").text().as_uint();
			m.positionX = tool.child("position").child("x").text().as_double();
			m.positionY = tool.child("position").child("y").text().as_double();
			m.isVisible = tool.child("isVisible").text().as_bool();
			m.isCollapsed = tool.child("isCollapsed").text().as_bool();

			mixer mix;
			for (pugi::xml_node subTool = tool.child("mixer").child("moduleOut"); subTool; subTool = subTool.next_sibling("moduleOut")) {
				moduleOutput mo;
				mo.gainFactor = subTool.child("gain").text().as_double();
				mo.muted = subTool.child("muted").text().as_bool();
				mo.soloed = subTool.child("soloed").text().as_bool();
				mix.moduleOutputs.push_back(mo);
			}
			for (pugi::xml_node subTool = tool.child("mixer").child("vstIn"); subTool; subTool = subTool.next_sibling("vstIn")) {
				vstInput vi;
				vi.gainFactor = subTool.child("gain").text().as_double();
				vi.muted = subTool.child("muted").text().as_bool();
				vi.soloed = subTool.child("soloed").text().as_bool();
				mix.vstInputs.push_back(vi);
			}
			for (pugi::xml_node subTool = tool.child("mixer").child("inputSlot"); subTool; subTool = subTool.next_sibling("inputSlot")) {
				mix.inputSlots.push_back(subTool.text().as_int());
			}
			m.mixerParamters = mix;

			quantizer q;
			q.bypass = tool.child("quantizer").child("bypass").text().as_bool();
			q.quantization = tool.child("quantizer").child("quantization").text().as_int();
			m.quantizerParamters = q;

			equalizer e;
			e.bypass = tool.child("equalizer").child("bypass").text().as_bool();
			e.filterTypeIndex = tool.child("equalizer").child("filterTypeIndex").text().as_int();
			e.frequency = tool.child("equalizer").child("frequency").text().as_double();
			e.qFactor = tool.child("equalizer").child("qFactor").text().as_double();
			e.gain = tool.child("equalizer").child("gain").text().as_double();
			m.equalizerParameters = e;

			allpass a;
			a.bypass = tool.child("allpass").child("bypass").text().as_bool();
			a.delay = tool.child("allpass").child("delay").text().as_double();
			a.decay = tool.child("allpass").child("decay").text().as_double();
			m.allpassParameters = a;

			output o;
			o.bypass = tool.child("output").child("bypass").text().as_bool();
			o.gain = tool.child("output").child("gain").text().as_double();
			m.outputParameters = o;

			loadedPreset.modules.push_back(m);
		}

		general g;
		for (tool = doc.child("reverbNetwork").child("preset").child("parameters").child("general").child("vstOutputMenuIndex"); tool; tool = tool.next_sibling("vstOutputMenuIndex")) {
			g.vstOutputMenuIndexes.push_back(tool.text().as_int());
		}
		loadedPreset.generalParamters = g;

		/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(tool.child("name").text().as_double()).c_str());
		fclose(pFile);*/
	}
	else {
		/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
		fprintf(pFile, "Loading failed: %s\n", result.description());
		fclose(pFile);*/
	}

	return loadedPreset;
}