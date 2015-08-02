/*
* ReverbNetworkController: Vst Plug-in controller
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
