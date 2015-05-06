#ifndef REVERBNETWORKEDITOR_H
#define REVERBNETWORKEDITOR_H

#include "public.sdk/source/vst/vstguieditor.h"
#include <memory>

#include "GuiBaseAPModule.h"

namespace Steinberg {
namespace Vst {

	

class ReverbNetworkEditor :
	public VSTGUIEditor,
	public CControlListener

{
public:
	ReverbNetworkEditor(void* controller);
	~ReverbNetworkEditor();

	//---from VSTGUIEditor---------------
	bool PLUGIN_API open(void* parent, const PlatformType& platformType = kDefaultNative);
	void PLUGIN_API close();

	//---from CControlListener---------
	void valueChanged(CControl* pControl);
	char controlModifierClicked(CControl* pControl, long button);
	void controlBeginEdit(CControl* pControl);
	void controlEndEdit(CControl* pControl);

	
	// Create the GUI for a Allpass module
	void createAPModule();
	// Remove the GUI for a specified Allpass module
	void removeAPModule(uint16 moduleNumber);

	// Create a GUI knob group which consists of a text title, a knob which directly controls the parameter and a text edit which affects the knob
	// Returns the group view
	CViewContainer* createKnobGroup(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& knobTag, const int32_t& valueEditTag);

	// Create a text label with a title for a group
	CTextLabel* createGroupTitle(const VSTGUI::UTF8StringPtr title, const CCoord& width);

	CRowColumnView* createMixerRow(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& optionMenuTag, const int32_t& knobTag, const int32_t& valueEditTag);



private:
	// Holds pointer to the module GUIs
	//std::vector<GuiBaseAPModule*> apGuiModules;

	// View where the modules are placed and where they can be moved around and stuff
	CScrollView* workspaceView;

	//// True if the GUI for the module is present (= created and not removed yet)

	// True if the id (= the index of the vector) is already taken
	std::vector<bool> allpassModuleIdPool;
	// Total number of created modules = total number of times the function createModule() has been called
	uint32 totalNumberOfCreatedModules;

	CBitmap* knobBackground;
	CBitmap* knobBackgroundSmall;
	CBitmap* groupFrame;
};

}} // namespaces

#endif // REVERBNETWORKEDITOR_H