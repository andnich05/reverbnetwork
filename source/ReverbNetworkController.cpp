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

#include "ReverbNetworkEditor.h"
#include "ReverbNetworkDefines.h"

#if TARGET_OS_IPHONE
#include "interappaudio/iosEditor.h"
#endif

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
	tresult PLUGIN_API ReverbNetworkController::initialize(FUnknown* context)
{
	tresult result = EditController::initialize (context);
	if (result == kResultTrue)
	{
		//parameters.addParameter (STR16 ("Delay"), STR16 ("sec"), 0, 1, ParameterInfo::kCanAutomate, 100);
		// Map (M=MAXMODULENUMBER; I=MAXMODULEINPUT):
		// 0 to M*I-1 => Mixer input gain per channel
		// (M*I to M*I+X => Equalizer parameters)
		// M*I to M*I+M-1 => Allpass Delay
		// M*I+M to M*I+2*M-1 => Allpass Decay
		// M*I+2*M-1 to M*I+3*M-1 => Module output gain

		// Parameter creation
		//------
		// MIXER parameters
		// Mixer input selection
		uint32 pidCounter = 0;
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			for (uint16 j = 0; j < MAXMODULEINPUTS; ++j) {
				std::string temp = "";
				temp.append("Module ");
				temp.append(std::to_string(i));
				temp.append(" - Mixer Input");
				temp.append(std::to_string(j));
				temp.append("Select");
				StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_MIXERINPUTSELECT_FIRST + pidCounter);
				// Not connected is the first entry
				parameter->appendString(USTRING("<Not Connected>"));
				// Module outputs come first
				for (uint16 k = 0; k < MAXMODULENUMBER; ++k) {
					std::string temp2 = "APM";
					temp2.append(std::to_string(k));
					temp2.append(" Output");
					parameter->appendString(USTRING(temp2.c_str()));
				}
				// Then the VST inputs
				for (uint16 k = 0; k < MAXVSTINPUTS; ++k) {
					std::string temp2 = "VST";
					temp2.append(std::to_string(k));
					temp2.append(" Input");
					parameter->appendString(USTRING(temp2.c_str()));
				}
				parameters.addParameter(parameter);
				
				++pidCounter;
			}
		}
		// Mixer input gain 
		pidCounter = 0;
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			for (uint16 j = 0; j < MAXMODULEINPUTS; ++j) {
				std::string temp = "";
				temp.append("Module ");
				temp.append(std::to_string(i));
				temp.append(" - Mixer Channel Input ");
				temp.append(std::to_string(j));
				temp.append(" Gain");
				RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_MIXERGAIN_FIRST + pidCounter, USTRING("dB"), MINOUTPUTGAINDB, MAXOUTPUTGAINDB, DEFAULTOUTPUTGAINDB);
				parameters.addParameter(parameter);
				++pidCounter;
			}
		}
		// Mixer bypass switch
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "Module ";
			temp.append(std::to_string(i));
			temp.append("Mixer Bypass");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_MIXERBYPASS_FIRST + i);
			parameter->appendString(USTRING("False"));
			parameter->appendString(USTRING("True"));
			parameters.addParameter(parameter);
		}

		// -----
		// EQUALIZER parameters
		// Equalizer Filter Type
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "Module ";
			temp.append(std::to_string(i));
			temp.append("Equalizer Filter Type");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_EQFILTERTYPE_FIRST + i);
			parameter->appendString(USTRING("Low Pass"));
			parameter->appendString(USTRING("High Pass"));
			parameter->appendString(USTRING("Band Pass"));
			parameter->appendString(USTRING("Band Stop"));
			parameter->appendString(USTRING("Low Shelf"));
			parameter->appendString(USTRING("High Shelf"));
			parameters.addParameter(parameter);
		}
		// Equalizer Center Frequency
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "Module ";
			temp.append(std::to_string(i));
			temp.append(" Equalizer Center Frequency");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_EQCENTERFREQ_FIRST + i, USTRING("Hz"), MINEQCENTERFREQ, MAXEQCENTERFREQ, DEFAULTEQCENTERFREQ);
			parameters.addParameter(parameter);
		}
		// Equalizer Q Factor
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "Module ";
			temp.append(std::to_string(i));
			temp.append(" Equalizer Q Factor");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_EQQFACTOR_FIRST + i, USTRING(""), MINEQQFACTOR, MAXEQQFACTOR, DEFAULTEQQFACTOR);
			parameters.addParameter(parameter);
		}
		// Equalizer Gain
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "Module ";
			temp.append(std::to_string(i));
			temp.append(" Equalizer Gain");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_EQGAIN_FIRST + i, USTRING("dB"), MINEQGAINDB, MAXEQGAINDB, DEFAULTEQGAIN);
			parameters.addParameter(parameter);
		}
		// Equalizer Bypass
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "Module ";
			temp.append(std::to_string(i));
			temp.append("Equalizer Bypass");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_EQBYPASS_FIRST + i);
			parameter->appendString(USTRING("False"));
			parameter->appendString(USTRING("True"));
			parameters.addParameter(parameter);
		}

		//-----
		// ALLPASS paramters
		// Allpass delay
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "Module ";
			temp.append(std::to_string(i));
			temp.append(" Allpass Delay");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_ALLPASSDELAY_FIRST + i, USTRING("sec"), 0.0, MAXDELAY, DEFAULTDELAY);
			parameters.addParameter(parameter);
		}
		// Allpass decay
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "Module ";
			temp.append(std::to_string(i));
			temp.append(" Allpass Decay");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_ALLPASSDECAY_FIRST + i, USTRING("sec"), 0.0, MAXDECAY, DEFAULTDECAY);
			parameters.addParameter(parameter);
		}
		// Allpass Bypass
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "Module ";
			temp.append(std::to_string(i));
			temp.append("Allpass Bypass");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_ALLPASSBYPASS_FIRST + i);
			parameter->appendString(USTRING("False"));
			parameter->appendString(USTRING("True"));
			parameters.addParameter(parameter);
		}

		//-----
		// OUTPUT parameters
		// Gain
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "Module ";
			temp.append(std::to_string(i));
			temp.append(" Output Gain");
			RangeParameter* parameter = new RangeParameter(USTRING(temp.c_str()), PARAM_OUTGAIN_FIRST + i, USTRING(""), MINOUTPUTGAINDB, MAXOUTPUTGAINDB, DEFAULTOUTPUTGAINDB);
			parameters.addParameter(parameter);
		}
		// Bypass
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "Module ";
			temp.append(std::to_string(i));
			temp.append("Output Gain Bypass");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_OUTBYPASS_FIRST + i);
			parameter->appendString(USTRING("False"));
			parameter->appendString(USTRING("True"));
			parameters.addParameter(parameter);
		}

		//-----
		// VST output select
		pidCounter = 0;
		for (uint16 i = 0; i < MAXVSTOUTPUTS; ++i) {
			std::string temp = "VST Output ";
			temp.append(std::to_string(i));
			temp.append(" Select");
			StringListParameter* parameter = new StringListParameter(USTRING(temp.c_str()), PARAM_GENERALVSTOUTPUTSELECT_FIRST + i);
			// Not connected is the first entry
			parameter->appendString(USTRING("<Not Connected>"));
			// Module outputs come first
			for (uint16 k = 0; k < MAXMODULENUMBER; ++k) {
				std::string temp2 = "APM";
				temp2.append(std::to_string(k));
				temp2.append(" Output");
				parameter->appendString(USTRING(temp2.c_str()));
			}
			// Then the VST inputs
			for (uint16 k = 0; k < MAXVSTINPUTS; ++k) {
				std::string temp2 = "VST";
				temp2.append(std::to_string(k));
				temp2.append(" Input");
				parameter->appendString(USTRING(temp2.c_str()));
			}
			parameters.addParameter(parameter);
		}

	}
	return kResultTrue;
}

	IPlugView* PLUGIN_API ReverbNetworkController::createView(const char* name)
{
	// someone wants my editor
	if (name && strcmp (name, "editor") == 0)
	{
		ReverbNetworkEditor* view = new ReverbNetworkEditor(this);
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
		addDependentView(view);
	}
}

//------------------------------------------------------------------------
void ReverbNetworkController::editorRemoved(EditorView* editor)
{
	ReverbNetworkEditor* view = dynamic_cast<ReverbNetworkEditor*> (editor);
	if (view)
	{
		removeDependentView(view);
	}
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
