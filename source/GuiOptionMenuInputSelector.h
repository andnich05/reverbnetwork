#ifndef GUIOPTIONMENUINPUTSELECTOR_H
#define GUIOPTIONMENUINPUTSELECTOR_H

#include "../vstgui4/vstgui/lib/controls/coptionmenu.h"

namespace VSTGUI {

class GuiOptionMenuInputSelector :
	public COptionMenu
{
public:
	GuiOptionMenuInputSelector(const CRect& size, CControlListener* listener, int32_t tag, CBitmap* background = 0, CBitmap* bgWhenClick = 0, const int32_t style = 0);
	~GuiOptionMenuInputSelector();

	// Get the index which was selected before the current index was selected
	inline int32_t getLastIndex() { return lastIndex; }

private:
	int32_t lastIndex;

	virtual void setValue(float val) VSTGUI_OVERRIDE_VMETHOD;
};

}

#endif // GUIOPTIONMENUINPUTSELECTOR_H