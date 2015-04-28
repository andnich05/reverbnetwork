#ifndef GUIKNOBGROUP_H
#define GUIKNOBGROUP_H

#include "cviewcontainer.h"
#include "../vstgui4/vstgui/lib/controls/ccontrol.h"
#include <string>

namespace VSTGUI {

class GuiKnobGroup :
	public CViewContainer
{
public:
	GuiKnobGroup(const CRect& size, const VSTGUI::UTF8StringPtr name, const CBitmap& knobBitmap, CControlListener* listener);



};

}

#endif // GUIKNOBGROUP_H