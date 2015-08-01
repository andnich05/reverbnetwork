//-----------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.0
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/adelay/source/adelaycontroller.h
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

#ifndef REVERBNETWORKCONTROLLER_H
#define REVERBNETWORKCONTROLLER_H

#include "public.sdk/source/vst/vsteditcontroller.h"

#include "ReverbNetworkEditor.h"

#if MAC
#include <TargetConditionals.h>
#endif

namespace Steinberg {
namespace Vst {

class ReverbNetworkEditor;

//-----------------------------------------------------------------------------
class ReverbNetworkController : public EditControllerEx1, public IMidiMapping
{
public:
	// Initialize the plugin
	tresult PLUGIN_API initialize (FUnknown* context);
	// Create the editor and it's view
	IPlugView* PLUGIN_API ReverbNetworkController::createView(const char* name);

#if TARGET_OS_IPHONE
	IPlugView* PLUGIN_API createView (FIDString name) override;
#endif

	static FUnknown* createInstance(void*) { return (IEditController*)new ReverbNetworkController; }

	void editorDestroyed(EditorView* editor) {} // nothing to do here
	// Called after a new editor is created (apply some saved data)
	void editorAttached(EditorView* editor);
	// Called before editor is removed, is also called when the user only CLOSES the plugin view (save some data here for when the editor is recreated)
	void editorRemoved(EditorView* editor);

	//---Internal functions-------
	void addDependentView(ReverbNetworkEditor* view);
	void removeDependentView(ReverbNetworkEditor* view);

	// Load preset and first initialization
	tresult PLUGIN_API setComponentState(IBStream* state);
	// Update a parameter
	tresult PLUGIN_API setParamNormalized(ParamID tag, ParamValue value);
	// Get a parameter
	ParamValue PLUGIN_API getParamNormalized(ParamID tag);
	tresult PLUGIN_API setParamNormalizedFromPreset(ParamID tag, ParamValue value);

	// Set plugin version (see version.h)
	static void setVersion(std::string version);
	// Get plugin version (see version.h)
	static std::string getVersion();

	// Receive a message from another component (e.g. Processor)
	tresult PLUGIN_API notify(IMessage* message);

	//...
	DELEGATE_REFCOUNT(EditController)
	tresult PLUGIN_API queryInterface(const char* iid, void** obj);
	//---from IMidiMapping-----------------
	tresult PLUGIN_API getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& tag);

private:
	TArray <ReverbNetworkEditor*> viewsArray; // For more than one editor

	static std::string pluginVersion;

	// Contains everything from the editor that has no VST parameter
	ReverbNetworkEditor::EditorUserData editorUserData;
};

}} // namespaces

#endif // REVERBNETWORKCONTROLLER_H
