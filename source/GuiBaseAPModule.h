#ifndef GUIBASEAPMODULE_H
#define GUIBASEAPMODULE_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include "../vstgui4/vstgui/lib/controls/ccontrol.h"

namespace VSTGUI {

class CRowColumnView;
class CAnimKnob;

class GuiBaseAPModule :
	public CViewContainer
{
public:
	GuiBaseAPModule(const CRect& size, CControlListener* listener);
	~GuiBaseAPModule();

protected:
	CRect  size;
	CRect  mouseableArea;


private: 
	CControlListener* listener;

	CBitmap* knobBackground;
	CRowColumnView* baseModuleView;
	// Handle view to grab and move the module with the mouse
	CViewContainer* handleView;
	// Control view which holds the individual processing modules
	CRowColumnView* controlView;
	// Holds the input mixer controls (input gain for each channel)
	CViewContainer* mixerView;
	// Holds the equalizer controls
	CViewContainer* equalizerView;
	// Holds the allpass controls (delay and decay)
	CViewContainer* allpassView;
	CAnimKnob* delayKnob;
	CAnimKnob* decayKnob;
	// Holds the output gain control
	CViewContainer* gainView;
};

}

#endif // GUIBASEAPMODULE_H
