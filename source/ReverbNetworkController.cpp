//-----------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.0
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/adelay/source/adelaycontroller.cpp
// Created by  : Steinberg, 06/2009
// Description : 
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2013, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "ReverbNetworkController.h"
#include "reverbnetworkids.h"
#include "pluginterfaces/base/ustring.h"


#include "ReverbNetworkDefines.h"
#include "PresetReadWrite.h"


#if TARGET_OS_IPHONE
#include "interappaudio/iosEditor.h"
#endif

namespace Steinberg {
namespace Vst {

std::string ReverbNetworkController::pluginVersion = "0";

//-----------------------------------------------------------------------------
tresult PLUGIN_API ReverbNetworkController::initialize(FUnknown* context)
{

	tresult result = EditControllerEx1::initialize (context);
	if (result == kResultTrue)
	{
		// Parameter creation
		//------
		// MIXER parameters
		// Mixer input selection
		uint32 pidCounter = 0;
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			for (uint32 j = 0; j < MAXMODULEINPUTS; ++j) {
				std::string temp = "";
				temp.append("APM");
				temp.append(std::to_string(i));
				temp.append("-MXInput");
				temp.append(std::to_string(j));
				temp.append("-Select");
				StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_MIXERINPUTSELECT_FIRST + pidCounter, 0, ParameterInfo::kIsList | ParameterInfo::kIsReadOnly);
				// Not connected is the first entry
				parameter->appendString(USTRING("<Not Connected>"));
				// Module outputs come first
				for (uint32 k = 0; k < MAXMODULENUMBER; ++k) {
					std::string temp2 = "APM";
					temp2.append(std::to_string(k));
					temp2.append(" Output");
					parameter->appendString(USTRING(temp2.c_str()));
				}
				// Then the VST inputs
				for (uint32 k = 0; k < MAXVSTINPUTS; ++k) {
					std::string temp2 = "VST";
					temp2.append(std::to_string(k));
					temp2.append(" Input");
					parameter->appendString(USTRING(temp2.c_str()));
				}
				parameter->appendString(USTRING("SignalGen"));
				EditControllerEx1::parameters.addParameter(parameter);
				
				++pidCounter;
			}
		}
		// Mixer input gain 
		pidCounter = 0;
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			for (uint32 j = 0; j < MAXINPUTS; ++j) {
				std::string temp = "";
				temp.append("APM");
				temp.append(std::to_string(i));
				temp.append("MXInput");
				temp.append(std::to_string(j));
				temp.append("-Gain");
				RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_MIXERGAIN_FIRST + pidCounter, USTRING(UNIT_MIXERGAIN), MIN_MIXERGAIN, MAX_MIXERGAIN, DEF_MIXERGAIN, 0, ParameterInfo::kCanAutomate);
				EditControllerEx1::parameters.addParameter(parameter);
				++pidCounter;
			}
		}
		// Mixer input mute
		pidCounter = 0;
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			for (uint32 j = 0; j < MAXINPUTS; ++j) {
				std::string temp = "";
				temp.append("APM");
				temp.append(std::to_string(i));
				temp.append("-MXInput");
				temp.append(std::to_string(j));
				temp.append("-Mute");
				StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_MIXERINPUTMUTED_FIRST + pidCounter, 0, ParameterInfo::kCanAutomate);
				parameter->appendString(USTRING("False"));
				parameter->appendString(USTRING("True"));
				EditControllerEx1::parameters.addParameter(parameter);
				++pidCounter;
			}
		}
		// Mixer input solo
		pidCounter = 0;
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			for (uint32 j = 0; j < MAXINPUTS; ++j) {
				std::string temp = "";
				temp.append("APM");
				temp.append(std::to_string(i));
				temp.append("-MXInput");
				temp.append(std::to_string(j));
				temp.append("-Solo");
				StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_MIXERINPUTSOLOED_FIRST + pidCounter, 0, ParameterInfo::kCanAutomate);
				parameter->appendString(USTRING("False"));
				parameter->appendString(USTRING("True"));
				EditControllerEx1::parameters.addParameter(parameter);
				++pidCounter;
			}
		}
		// QUANTIZER parameters
		// Quantizer bitdepth
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-QUANT-Quantization");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_QUANTIZERBITDEPTH_FIRST + i, USTRING(UNIT_QUANTIZERBITDEPTH), MIN_QUANTIZERBITDEPTH, MAX_QUANTIZERBITDEPTH, DEF_QUANTIZERBITDEPTH, 31, ParameterInfo::kCanAutomate);
			parameter->setPrecision(0);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		// Quantizer Bypass
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-QUANT-Bypass");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_QUANTIZERBYPASS_FIRST + i, 0, ParameterInfo::kCanAutomate);
			parameter->appendString(USTRING("False"));
			parameter->appendString(USTRING("True"));
			EditControllerEx1::parameters.addParameter(parameter);
		}
		
		// -----
		// EQUALIZER parameters
		// Equalizer Filter Type
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM ";
			temp.append(std::to_string(i));
			temp.append("-EQ-FilterType");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_EQFILTERTYPE_FIRST + i, 0, ParameterInfo::kIsList);
			parameter->appendString(USTRING("Low Pass"));
			parameter->appendString(USTRING("High Pass"));
			parameter->appendString(USTRING("Band Pass/Stop"));
			parameter->appendString(USTRING("Low Shelf"));
			parameter->appendString(USTRING("High Shelf"));
			parameter->appendString(USTRING("Raw Biquad"));
			EditControllerEx1::parameters.addParameter(parameter);
		}
		// Equalizer Center Frequency
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-EQ-Frequency");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_EQCENTERFREQ_FIRST + i, USTRING(UNIT_EQCENTERFREQ), MIN_EQCENTERFREQ, MAX_EQCENTERFREQ, DEF_EQCENTERFREQ, 0, ParameterInfo::kCanAutomate);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		// Equalizer Q Factor
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM ";
			temp.append(std::to_string(i));
			temp.append("-EQ-QFactor");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_EQQFACTOR_FIRST + i, USTRING(UNIT_EQQFACTOR), MIN_EQQFACTOR, MAX_EQQFACTOR, DEF_EQQFACTOR, 0, ParameterInfo::kCanAutomate);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		// Equalizer Gain
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-EQ-Gain");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_EQGAIN_FIRST + i, USTRING(UNIT_EQGAIN), MIN_EQGAIN, MAX_EQGAIN, DEF_EQGAIN, 0, ParameterInfo::kCanAutomate);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		for (uint32 i = PARAM_EQCOEFFICIENTA0_FIRST; i <= PARAM_EQCOEFFICIENTA0_LAST; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i - PARAM_EQCOEFFICIENTA0_FIRST));
			temp.append("-EQ-a0");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), i, USTRING(UNIT_EQCOEFFICIENTS), MIN_EQCOEFFICIENTS, MAX_EQCOEFFICIENTS, DEF_EQCOEFFICIENTS, 0, ParameterInfo::kCanAutomate);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		for (uint32 i = PARAM_EQCOEFFICIENTA1_FIRST; i <= PARAM_EQCOEFFICIENTA1_LAST; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i - PARAM_EQCOEFFICIENTA1_FIRST));
			temp.append("-EQ-a1");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), i, USTRING(UNIT_EQCOEFFICIENTS), MIN_EQCOEFFICIENTS, MAX_EQCOEFFICIENTS, DEF_EQCOEFFICIENTS, 0, ParameterInfo::kCanAutomate);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		for (uint32 i = PARAM_EQCOEFFICIENTA2_FIRST; i <= PARAM_EQCOEFFICIENTA2_LAST; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i - PARAM_EQCOEFFICIENTA2_FIRST));
			temp.append("-EQ-a2");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), i, USTRING(UNIT_EQCOEFFICIENTS), MIN_EQCOEFFICIENTS, MAX_EQCOEFFICIENTS, DEF_EQCOEFFICIENTS, 0, ParameterInfo::kCanAutomate);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		for (uint32 i = PARAM_EQCOEFFICIENTB1_FIRST; i <= PARAM_EQCOEFFICIENTB1_LAST; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i - PARAM_EQCOEFFICIENTB1_FIRST));
			temp.append("-EQ-b1");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), i, USTRING(UNIT_EQCOEFFICIENTS), MIN_EQCOEFFICIENTS, MAX_EQCOEFFICIENTS, DEF_EQCOEFFICIENTS, 0, ParameterInfo::kCanAutomate);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		for (uint32 i = PARAM_EQCOEFFICIENTB2_FIRST; i <= PARAM_EQCOEFFICIENTB2_LAST; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i - PARAM_EQCOEFFICIENTB2_FIRST));
			temp.append("-EQ-b2");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), i, USTRING(UNIT_EQCOEFFICIENTS), MIN_EQCOEFFICIENTS, MAX_EQCOEFFICIENTS, DEF_EQCOEFFICIENTS, 0, ParameterInfo::kCanAutomate);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		// Equalizer Bypass
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-EQ-Bypass");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_EQBYPASS_FIRST + i, 0, ParameterInfo::kCanAutomate);
			parameter->appendString(USTRING("False"));
			parameter->appendString(USTRING("True"));
			EditControllerEx1::parameters.addParameter(parameter);
		}
		//-----
		// ALLPASS paramters
		// Allpass delay
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-ALLPASS-Delay");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_ALLPASSDELAY_FIRST + i, USTRING(UNIT_ALLPASSDELAY), MIN_ALLPASSDELAY, MAX_ALLPASSDELAY, DEF_ALLPASSDELAY, 0, ParameterInfo::kCanAutomate);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		// Allpass decay
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-ALLPASS-Decay");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_ALLPASSDECAY_FIRST + i, USTRING(UNIT_ALLPASSDECAY), MIN_ALLPASSDECAY, MAX_ALLPASSDECAY, DEF_ALLPASSDECAY, 0, ParameterInfo::kCanAutomate);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-AP-DiffKSign");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_ALLPASSDIFFKSIGN_FIRST + i, 0, ParameterInfo::kCanAutomate | ParameterInfo::kIsList);
			parameter->appendString(USTRING("Positive"));
			parameter->appendString(USTRING("Negative"));
			EditControllerEx1::parameters.addParameter(parameter);
		}
		// Allpass Bypass
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-ALLPASS-Bypass");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_ALLPASSBYPASS_FIRST + i, 0, ParameterInfo::kCanAutomate);
			parameter->appendString(USTRING("False"));
			parameter->appendString(USTRING("True"));
			EditControllerEx1::parameters.addParameter(parameter);
		}

		//-----
		// OUTPUT parameters
		// Gain
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-OUT-Gain");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_OUTGAIN_FIRST + i, USTRING(UNIT_OUTPUTGAIN), MIN_OUTPUTGAIN, MAX_OUTPUTGAIN, DEF_OUTPUTGAIN, 0, ParameterInfo::kCanAutomate);
			EditControllerEx1::parameters.addParameter(parameter);
		}
		// Bypass
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-OUT-Bypass");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_OUTBYPASS_FIRST + i, 0, ParameterInfo::kCanAutomate);
			parameter->appendString(USTRING("False"));
			parameter->appendString(USTRING("True"));
			EditControllerEx1::parameters.addParameter(parameter);
		}

		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(i));
			temp.append("-Visible");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_MODULEVISIBLE_FIRST + i, 0, ParameterInfo::kIsReadOnly);
			parameter->appendString(USTRING("False"));
			parameter->appendString(USTRING("True"));
			EditControllerEx1::parameters.addParameter(parameter);
		}

		//-----
		// VST output select
		for (uint32 i = 0; i < MAXVSTOUTPUTS; ++i) {
			std::string temp = "VSTOUT";
			temp.append(std::to_string(i));
			temp.append("-Select");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_GENERALVSTOUTPUTSELECT_FIRST + i, 0, ParameterInfo::kIsList);
			// Not connected is the first entry
			parameter->appendString(USTRING("<Not Connected>"));
			// Module outputs come first
			for (uint32 k = 0; k < MAXMODULENUMBER; ++k) {
				std::string temp2 = "APM";
				temp2.append(std::to_string(k));
				temp2.append(" Output");
				parameter->appendString(USTRING(temp2.c_str()));
			}
			// Then the VST inputs
			for (uint32 k = 0; k < MAXVSTINPUTS; ++k) {
				std::string temp2 = "VST";
				temp2.append(std::to_string(k));
				temp2.append(" Input");
				parameter->appendString(USTRING(temp2.c_str()));
			}
			parameter->appendString(USTRING("SignalGen"));
			EditControllerEx1::parameters.addParameter(parameter);
		}

		// Signal generator
		StringListParameter* p1 = new StringListParameter(USTRING("SIGNALGEN-Type"), PARAM_SIGNALGENERATOR_SIGNALTYPE, 0, ParameterInfo::kIsList);
		p1->appendString(USTRING("Dirac"));
		EditControllerEx1::parameters.addParameter(p1);
		EditControllerEx1::parameters.addParameter(new RangeParameter(USTRING("SIGNALGEN-Amplitude"), PARAM_SIGNALGENERATOR_AMPLITUDE, USTRING(UNIT_SIGNALGENERATOR_AMPLITUDE), MIN_SIGNALGENERATOR_AMPLITUDE, MAX_SIGNALGENERATOR_AMPLITUDE, DEF_SIGNALGENERATOR_AMPLITUDE, 0, ParameterInfo::kCanAutomate));
		EditControllerEx1::parameters.addParameter(new RangeParameter(USTRING("SIGNALGEN-Width"), PARAM_SIGNALGENERATOR_WIDTH, USTRING(UNIT_SIGNALGENERATOR_WIDTH), MIN_SIGNALGENERATOR_WIDTH, MAX_SIGNALGENERATOR_WIDTH, DEF_SIGNALGENERATOR_WIDTH, 0, ParameterInfo::kCanAutomate));
		EditControllerEx1::parameters.addParameter(new RangeParameter(USTRING("SIGNALGEN-Time"), PARAM_SIGNALGENERATOR_TIME, USTRING(UNIT_SIGNALGENERATOR_TIME), MIN_SIGNALGENERATOR_TIME, MAX_SIGNALGENERATOR_TIME, DEF_SIGNALGENERATOR_TIME, 0, ParameterInfo::kCanAutomate));
		StringListParameter* p2 = new StringListParameter(USTRING("SIGNALGEN-AutoFireOn"), PARAM_SIGNALGENERATOR_AUTOFIREENABLED, 0, ParameterInfo::kIsList);
		p2->appendString(USTRING("False"));
		p2->appendString(USTRING("True"));
		EditControllerEx1::parameters.addParameter(p2);

		// Initialize Editor user data
		editorUserData.moduleNames.resize(MAXMODULENUMBER);
		editorUserData.presetName = "";
	}
	return kResultTrue;
}

IPlugView* PLUGIN_API ReverbNetworkController::createView(const char* name)
{
	// someone wants my editor
	if (name && strcmp (name, "editor") == 0)
	{
		ReverbNetworkEditor* view = new ReverbNetworkEditor(this);
		view->setPluginVersion(pluginVersion);
		return view;
	}
	return 0;
}

//------------------------------------------------------------------------
void ReverbNetworkController::addDependentView(ReverbNetworkEditor* view)
{
	viewsArray.add(view);
}

//------------------------------------------------------------------------
void ReverbNetworkController::removeDependentView(ReverbNetworkEditor* view)
{
	for (int32 i = 0; i < viewsArray.total(); i++)
	{
		if (viewsArray.at(i) == view)
		{
			viewsArray.removeAt(i);
			break;
		}
	}
}

//------------------------------------------------------------------------
void ReverbNetworkController::editorAttached(EditorView* editor)
{
	ReverbNetworkEditor* view = dynamic_cast<ReverbNetworkEditor*> (editor);
	if (view)
	{
		view->setUserData(editorUserData);

		addDependentView(view);
	}
}

//------------------------------------------------------------------------
void ReverbNetworkController::editorRemoved(EditorView* editor)
{
	ReverbNetworkEditor* view = dynamic_cast<ReverbNetworkEditor*> (editor);
	if (view)
	{
		/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", view->getModuleNames().at(0).c_str());
		fclose(pFile);*/

		// Save the data when the Editor is closed
		editorUserData = view->getUserData();

		removeDependentView(view);
	}
}



tresult PLUGIN_API ReverbNetworkController::setComponentState(IBStream* state)
{
	// Create object
	PresetReadWrite preset;
	// Load filestream into object
	tresult result = preset.setParamterState(state);
	if (result == kResultTrue) {
		// Read the values from the file stream and update the parameters
		for (int32 i = 0; i < parameters.getParameterCount(); ++i) {
			setParamNormalized(i, preset.getNormValueByParamId(i));
			
		}
		// Update the GUI with the loaded parameters
		for (int32 i = 0; i < viewsArray.total(); i++)
		{
			if (viewsArray.at(i))
			{
				viewsArray.at(i)->updateGuiWithControllerParameters();
			}
		}
	}
	return result;
}

// Nötig?
tresult PLUGIN_API ReverbNetworkController::setParamNormalizedFromPreset(ParamID tag, ParamValue value)
{

	Parameter* parameter = getParameterObject(tag);
	if (parameter)
	{
		parameter->setNormalized(value);
		return kResultTrue;
	}
	return kResultFalse;
}

tresult PLUGIN_API ReverbNetworkController::setParamNormalized(ParamID tag, ParamValue value)
{

	Parameter* parameter = getParameterObject(tag);
	if (parameter)
	{
		parameter->setNormalized(value);
	}
	// called from host to update our parameters state
	tresult result = EditControllerEx1::setParamNormalized (tag, value);

	// Update the GUI with values coming from Processor
	//if ((tag >= PARAM_PPMUPDATE_FIRST && tag <= PARAM_PPMUPDATE_LAST) || (tag >= PARAM_EQSTABILITY_FIRST && tag <= PARAM_EQSTABILITY_LAST)) {
		for (int32 i = 0; i < viewsArray.total(); i++)
		{
			if (viewsArray.at(i))
			{
				viewsArray.at(i)->updateEditorFromController(tag, value);
			}
		}
	//}

	return result;
}

// Problems with thread safety when changing member variables of the Editor!
tresult PLUGIN_API ReverbNetworkController::notify(IMessage* message) {
	if (!message) return kMessageUnknown;
	if (strncmp(message->getMessageID(), "EqStability", 128) == 0) {
		const void* ptr;
		EqualizerStability eqStability;
		uint32 size = sizeof(eqStability);
		message->getAttributes()->getBinary(0, ptr, size);
		if (ptr) {
			eqStability = *(EqualizerStability*)ptr;
		}
		else {
			return kMessageUnknown;
		}
		for (int i = 0; i < viewsArray.total(); ++i) {
			if (viewsArray.at(i)) {
				viewsArray.at(i)->updateEditorFromController(PARAM_EQSTABILITY_FIRST + eqStability.moduleNumber, eqStability.isStable);
			}
		}
		//delete ptr;
		return kMessageNotified;
	}
	return kMessageUnknown;
	//FILE* pFile = fopen("E:\\logVst.txt", "a");
	//			fprintf(pFile, "y(n): %s\n", message->getMessageID());
	//			fclose(pFile);
	//			return kMessageNotified;
}

ParamValue PLUGIN_API ReverbNetworkController::getParamNormalized(ParamID tag)
{
	Parameter* parameter = getParameterObject(tag);
	if (parameter)
	{
		return parameter->getNormalized();
	}
	return 0.0;
}

void ReverbNetworkController::setVersion(std::string version) {
	pluginVersion = version;
}

std::string ReverbNetworkController::getVersion() {
	return pluginVersion;
}

tresult PLUGIN_API ReverbNetworkController::queryInterface(const char* iid, void** obj)
{
	QUERY_INTERFACE(iid, obj, IMidiMapping::iid, IMidiMapping)
		return EditControllerEx1::queryInterface(iid, obj);
}

tresult PLUGIN_API ReverbNetworkController::getMidiControllerAssignment(int32 busIndex, int16 channel,
	CtrlNumber midiControllerNumber, ParamID& tag)
{
	// we support for the Gain parameter all MIDI Channel but only first bus (there is only one!)
	/*if (busIndex == 0 && midiControllerNumber == kCtrlVolume)
	{
		tag = kGainId;
		return kResultTrue;
	}*/
	return kResultFalse;
}

#if TARGET_OS_IPHONE
//-----------------------------------------------------------------------------
IPlugView* PLUGIN_API ADelayController::createView (FIDString name)
{
	if (strcmp (name, ViewType::kEditor) == 0)
	{
		return new ADelayEditorForIOS (this);
	}
	return 0;
}

#endif

}} // namespaces
