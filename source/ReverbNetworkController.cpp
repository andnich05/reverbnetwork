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

		Parameter* parameter;

		// Parameter creation: Must be in the same exact order as in ReverbNetworkDefines.h!
		// Mixer parameters
		uint32 pidCounter = 0;
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			for (uint16 j = 0; j < MAXMODULEINPUTS; ++j) {
				std::string temp = "";
				temp.append("Allpass ");
				temp.append(std::to_string(i));
				temp.append(" - Channel Gain ");
				temp.append(std::to_string(j));
				parameter = new RangeParameter(USTRING(temp.c_str()), pidCounter, USTRING(""), 0.0, 1.0, 0.0);
				parameter->setPrecision(2);
				parameters.addParameter(parameter);
				++pidCounter;
			}
		}

		// Allpass parameters: delay and decay
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "";
			temp.append("Allpass Delay ");
			temp.append(std::to_string(i));
			parameter = new RangeParameter(USTRING(temp.c_str()), pidCounter, USTRING("sec"), 0.0, 1.0, 0.0); // ms?
			parameter->setPrecision(2);
			parameters.addParameter(parameter);
			++pidCounter;
		}
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "";
			temp.append("Allpass Decay ");
			temp.append(std::to_string(i));
			parameter = new RangeParameter(USTRING(temp.c_str()), pidCounter, USTRING("sec"), 0.0, 1.0, 0.0);
			parameter->setPrecision(2);
			parameters.addParameter(parameter);
			++pidCounter;
		}

		// Module output gain parameter
		for (uint16 i = 0; i < MAXMODULENUMBER; ++i) {
			std::string temp = "";
			temp.append("Module Output Gain ");
			temp.append(std::to_string(i));
			parameter = new RangeParameter(USTRING(temp.c_str()), pidCounter, USTRING(""), 0.0, 1.0, 0.0);
			parameter->setPrecision(2);
			parameters.addParameter(parameter);
			++pidCounter;
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
//
////------------------------------------------------------------------------
//void ADelayController::addDependentView(ADelayEditor* view)
//{
//	viewsArray.add(view);
//}
//
////------------------------------------------------------------------------
//void ADelayController::removeDependentView(ADelayEditor* view)
//{
//	for (int32 i = 0; i < viewsArray.total(); i++)
//	{
//		if (viewsArray.at(i) == view)
//		{
//			viewsArray.removeAt(i);
//			break;
//		}
//	}
//}
//
////------------------------------------------------------------------------
//void ADelayController::editorAttached(EditorView* editor)
//{
//	ADelayEditor* view = dynamic_cast<ADelayEditor*> (editor);
//	if (view)
//	{
//		addDependentView(view);
//	}
//}
//
////------------------------------------------------------------------------
//void ADelayController::editorRemoved(EditorView* editor)
//{
//	ADelayEditor* view = dynamic_cast<ADelayEditor*> (editor);
//	if (view)
//	{
//		removeDependentView(view);
//	}
//}
//



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
