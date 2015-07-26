#ifndef REVERBNETWORKEDITOR_H
#define REVERBNETWORKEDITOR_H

#include "pluginterfaces/vst/ivstplugview.h"
#include "public.sdk/source/vst/vstguieditor.h"
#include "pluginterfaces/vst/ivstcontextmenu.h"

#include "GuiBaseAPModule.h"
#include "XmlPresetReadWrite.h"
#include "GuiGraphicsView.h"
#include "ReverbNetworkEnums.h"
#include "GuiCustomSplashScreen.h"

namespace Steinberg {
namespace Vst {

class ReverbNetworkEditor :
	public VSTGUIEditor,
	public CControlListener,
	public IParameterFinder,
	public IContextMenuTarget

{
public:
	ReverbNetworkEditor(void* controller);
	~ReverbNetworkEditor();

	typedef struct EditorUserData{
		std::string presetName;
		std::vector<std::string> moduleNames;
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

	inline void setPluginVersion(std::string version) { pluginVersion = version; }
	inline const EditorUserData& getUserData() const { return editorUserData; }
	inline void setUserData(const EditorUserData& userData) { editorUserData = userData; applyUserData(); }
	void applyUserData();
	
	DELEGATE_REFCOUNT(VSTGUIEditor)
	tresult PLUGIN_API queryInterface(const char* iid, void** obj);

	//---from IParameterFinder---------------
	tresult PLUGIN_API findParameter(int32 xPos, int32 yPos, ParamID& resultTag);

	//---from IContextMenuTarget---------------
	tresult PLUGIN_API executeMenuItem(int32 tag);

private:
	
	//std::vector<EqualizerStability> eqStability;

	std::string pluginVersion;

	// Holds pointer to the module GUIs
	std::vector<GuiBaseAPModule*> apGuiModules;

	double sampleRate;

	// View where the modules are placed and where they can be moved around and stuff
	//CScrollView* workspaceView;
	CViewContainer* workspaceView;
	CSplitView* splitView;
	CRowColumnView* mainView;
	CRowColumnView* viewVstOutputSelect;
	CRowColumnView* viewModuleListMain;
	GuiCustomSplashScreen* splashOverrideParametersQuery;

	// True if the id (= the index of the vector) is already taken
	std::vector<bool> allpassModuleIdPool;
	// Total number of created modules = total number of times the function createModule() has been called
	uint32 totalNumberOfCreatedModules;

	CBitmap* knobBackground;
	CBitmap* knobBackgroundSmall;
	CBitmap* ppmOff;
	CBitmap* ppmOn;

	// The graphics view holds all the components in small form as an overview
	GuiGraphicsView* graphicsView;

	// Holds pointers to all GUI elements which have their own GUI id; the index itself is the GUI id
	// If a module is removed then the elements to which the pointers in this vector are pointing WILL NOT BE DESTROYED AUTOMATICALLY (Because of reference counter != 0)!
	// Remove the pointers out of this vector manually => close()-function
	//std::vector<SharedPointer<CControl>> guiElements;
	std::vector<CControl*> guiElements;
	

	// Create the GUI for a Allpass module
	GuiBaseAPModule* createAPModule();

	CRowColumnView* createMixer(const CRect& parentViewSize, const int& moduleId);
	CRowColumnView* createQuantizer(const CRect& parentViewSize, const int& moduleId);
	CRowColumnView* createEqualizer(const CRect& parentViewSize, const int& moduleId);
	CRowColumnView* createAllpass(const CRect& parentViewSize, const int& moduleId);
	CRowColumnView* createOutput(const CRect& parentViewSize, const int& moduleId);

	// Create a GUI knob group which consists of a text title, a knob which directly controls the parameter and a text edit which affects the knob
	// Returns the group view
	CViewContainer* createKnobGroup(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& knobTag, const int32_t& valueEditTag,
		const double& valueEditMinValue, const double& valueEditMaxValue, const int& valueEditPrecision, const std::string& unit);
	//CTextEditStringToValueProc textEditStringToValueFunctionPtr, CParamDisplayValueToStringProc textEditValueToStringFunctionPtr);

	// Create a text label with a title for a group
	CTextLabel* createGroupTitle(const VSTGUI::UTF8StringPtr title, const CCoord& width) const;

	CRowColumnView* createMixerRow(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& idOffset);

	// Add a GUI element pointer to the vector with the GUI-ID as the index
	void addGuiElementPointer(CControl* guiElement, const int32_t& guiId);

	// Conversion function, can be a nullptr if no conversion is needed
	typedef double(*ConversionFunction)(const double&);
	void updateGuiParameter(uint32 firstParamId, uint32 lastParamId, uint32 firstGuiId, ConversionFunction functPtr);
	
	// Messages to Editor
	CMessageResult notify(CBaseObject* sender, const char* message);

	std::vector<double> lastPpmValues;
	std::vector<EqualizerStability> eqStabilityValues;

	XmlPresetReadWrite* xmlPreset;
	// Apply the loaded xml preset structure to the Vst plug-in
	void setXmlPreset(const XmlPresetReadWrite::preset& presetStruct);
	// Build the xml preset structure with all plugin-in parameters
	const XmlPresetReadWrite::preset getXmlPreset();
	// Differ between the possible file selector styles (no other possibility right now...)
	CNewFileSelector::Style fileSelectorStyle;

	XmlPresetReadWrite::module tempModuleParameters;
	XmlPresetReadWrite::module defaultModuleParameters;
	// Copy parameters of specified module into the specified module structure
	void copyModuleParameters(const unsigned int& sourceModuleId, XmlPresetReadWrite::module& m);
	// Paste the parameters from the specified structure into the specified module (can be the 'defaultModuleParamters' => set to default)
	void pasteModuleParameters(const unsigned int& destModuleId, const XmlPresetReadWrite::module& m);

	void initializeGraphicsView();
	void updateGraphicsViewModule(const int& moduleId, const int& input, const double& gainValue);

	void updateGainValuesInOptionMenus(const int& moduleNumber, const int& input, const double& gainValue);

	void updateEqStability(const int& moduleNumber, const bool& stable);
	void openModuleDetailView(const int& moduleNumber, const bool& open);
};

}} // namespaces

#endif // REVERBNETWORKEDITOR_H