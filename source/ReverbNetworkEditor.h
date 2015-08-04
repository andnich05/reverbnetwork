/*
* ReverbNetworkEditor: Vst Plug-in editor (GUI)
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

#ifndef REVERBNETWORKEDITOR_H
#define REVERBNETWORKEDITOR_H

#include "pluginterfaces/vst/ivstplugview.h"
#include "public.sdk/source/vst/vstguieditor.h"
#include "pluginterfaces/vst/ivstcontextmenu.h"

#include "XmlPresetReadWrite.h"
#include "ReverbNetworkEnums.h"


namespace VSTGUI {
	class GuiBaseAPModule;
	class GuiGraphicsView;
	class GuiCustomSplashScreen;
	class GuiCustomRowColumnView;
	class GuiSignalGenerator;
	class GuiWorkspaceView;
}

namespace Steinberg {
namespace Vst {

class ReverbNetworkEditor :
	public VSTGUIEditor,
	public CControlListener

{
public:
	ReverbNetworkEditor(void* controller);
	~ReverbNetworkEditor();

	// Data to save when the editor is closed (closed means destroyed!)
	typedef struct EditorUserData{
		std::string presetName;
		std::vector<std::string> moduleNames;
		XmlPresetReadWrite::GraphicsView graphicsView;
	};
	EditorUserData editorUserData;

	//---from VSTGUIEditor---------------
	bool PLUGIN_API open(void* parent, const PlatformType& platformType = kDefaultNative);
	void PLUGIN_API close();

	//---from CControlListener---------
	void valueChanged(CControl* pControl);
	char controlModifierClicked(CControl* pControl, long button);
	void controlBeginEdit(CControl* pControl);
	void controlEndEdit(CControl* pControl);

	// from IPlugView
	/** Is view sizable by user. */
	tresult PLUGIN_API canResize();
	tresult PLUGIN_API onSize(ViewRect* newSize);

	// Sync GUI with controller parameters (e.g. otherwise if the user closes and reopens the GUI the changes in the GUI would be lost)
	// Also called when users loads a VST Preset
	void updateGuiWithControllerParameters();

	// Update GUI from Controller (and/or from Processor with e.g. sample values)
	void updateEditorFromController(ParamID tag, ParamValue value);

	// Set plugin version (see version.h)
	inline void setPluginVersion(std::string version) { pluginVersion = version; }
	// Get the saved user data
	inline const EditorUserData& getUserData() const { return editorUserData; }
	// Set the user data
	inline void setUserData(const EditorUserData& userData) { editorUserData = userData; applyUserData(); }
	// Apply the loaded user data
	void applyUserData();

private:
	std::string pluginVersion;

	// Holds pointer to the module GUIs
	std::vector<GuiBaseAPModule*> apGuiModules;

	// Signal generator
	GuiSignalGenerator* signalGenerator;

	double sampleRate;
	
	GuiWorkspaceView* workspaceView; // View where the modules are placed and where they can be moved around and stuff
	CSplitView* splitView; // Contains a view with the modules and a view with the graphics view
	GuiCustomRowColumnView* mainView; // Sub view
	GuiCustomRowColumnView* viewVstOutputSelect; // View with Vst output selection and other stuff
	GuiCustomRowColumnView* viewModuleListMain; // List with modules
	GuiCustomSplashScreen* splashOverrideParametersQuery; // Splash view which asks the user if he really wants to override module parameters
	CScrollView* viewModuleScrollList;

	std::vector<bool> allpassModuleIdPool;
	// Total number of created modules = total number of times the function createModule() has been called
	uint32 totalNumberOfCreatedModules;

	// Some bitmaps
	CBitmap* knobBackground;
	CBitmap* knobBackgroundSmall;
	CBitmap* ppmOff;
	CBitmap* ppmOn;

	// The graphics view holds all the components in small form as an overview
	GuiGraphicsView* graphicsView;

	// Holds pointers to all GUI elements which have their own GUI id; the index itself is the GUI id
	// If a module is removed then the elements to which the pointers in this vector are pointing WILL NOT BE DESTROYED AUTOMATICALLY (Because of reference counter != 0)!
	// Remove the pointers out of this vector manually => close()-function
	std::vector<CControl*> guiElements;
	
	// Create the GUI for an Allpass module
	GuiBaseAPModule* createAPModule();
	// Create the sub-components
	GuiCustomRowColumnView* createMixer(const CRect& parentViewSize, const int& moduleId);
	GuiCustomRowColumnView* createQuantizer(const CRect& parentViewSize, const int& moduleId);
	GuiCustomRowColumnView* createEqualizer(const CRect& parentViewSize, const int& moduleId);
	GuiCustomRowColumnView* createAllpass(const CRect& parentViewSize, const int& moduleId);
	GuiCustomRowColumnView* createOutput(const CRect& parentViewSize, const int& moduleId);


	// Create a GUI knob group which consists of a text title, a knob which directly controls the parameter and a text edit which affects the knob
	CViewContainer* createKnobGroup(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& knobTag, const int32_t& valueEditTag,
		const double& valueEditMinValue, const double& valueEditMaxValue, const int& valueEditPrecision, const std::string& unit);

	// Create a text label with a title for a group
	CTextLabel* createGroupTitle(const VSTGUI::UTF8StringPtr title, const CCoord& width) const;

	// Create a mixer row in the mixer component
	GuiCustomRowColumnView* createMixerRow(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& idOffset);

	// Add a GUI element pointer to the vector with the GUI-ID as the index
	void addGuiElementPointer(CControl* guiElement, const int32_t& guiId);

	// Conversion function, can be a nullptr if no conversion is needed
	typedef double(*ConversionFunction)(const double&);
	void updateGuiParameter(uint32 firstParamId, uint32 lastParamId, uint32 firstGuiId, ConversionFunction functPtr);
	
	// Messages to Editor
	CMessageResult notify(CBaseObject* sender, const char* message);

	// Update values from controller/processor
	std::vector<double> lastPpmValues;
	std::vector<EqualizerStability> eqStabilityValues;

	XmlPresetReadWrite* xmlPreset; // XML preset structure
	// Apply the loaded xml preset structure to the Vst plug-in
	void setXmlPreset(const XmlPresetReadWrite::Preset& presetStruct);
	// Build the xml preset structure with all plugin-in parameters
	const XmlPresetReadWrite::Preset getXmlPreset();
	// Differ between the possible file selector styles (no other possibility right now...)
	CNewFileSelector::Style fileSelectorStyle;

	XmlPresetReadWrite::Module tempModuleParameters; // Contains the module parameters which are saved when the user clicks on "copy" in a module view
	XmlPresetReadWrite::Module defaultModuleParameters; // Contains the default parameters of a module
	// Copy parameters of specified module into the specified module structure
	void copyModuleParameters(const unsigned int& sourceModuleId, XmlPresetReadWrite::Module& m);
	// Paste the parameters from the specified structure into the specified module (can be the 'defaultModuleParamters' => set to default)
	void pasteModuleParameters(const unsigned int& destModuleId, const XmlPresetReadWrite::Module& m);

	// Initialize Graphics view with all modules, vst in- and outputs
	void initializeGraphicsView();
	// Update graphics view module (if gain 0.0 => input is unused)
	void updateGraphicsViewModule(const int& moduleId, const int& input, const double& gainValue);
	
	// Update the gain values in the all option menus (Mixer)
	void updateGainValuesInOptionMenus(const int& moduleNumber, const int& input, const double& gainValue);

	// Update stability from controller/processor
	void updateEqStability(const int& moduleNumber, const bool& stable);

	// Open module detail view when user double clicks on a module in the graphics view
	void openModuleDetailView(const int& moduleNumber, const bool& open);
};

}} // namespaces

#endif // REVERBNETWORKEDITOR_H