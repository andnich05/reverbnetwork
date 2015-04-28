#ifndef REVERBNETWORKEDITOR_H
#define REVERBNETWORKEDITOR_H

#include "public.sdk/source/vst/vstguieditor.h"
#include <memory>

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

private:
	// Holds pointer to the module GUIs
	std::vector<CRowColumnView*> apGuiModules;

	CBitmap* knobBackground;
};

}} // namespaces

#endif // REVERBNETWORKEDITOR_H