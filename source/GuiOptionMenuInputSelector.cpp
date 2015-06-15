#include "GuiOptionMenuInputSelector.h"

namespace VSTGUI {

GuiOptionMenuInputSelector::GuiOptionMenuInputSelector(const CRect& size, CControlListener* listener, int32_t tag, CBitmap* background, CBitmap* bgWhenClick, const int32_t style)
	: COptionMenu(size, listener, tag, background, bgWhenClick, style)
{
	lastIndex = 0;
}

GuiOptionMenuInputSelector::~GuiOptionMenuInputSelector()
{
}

void GuiOptionMenuInputSelector::setValue(float val)
{
	if ((int32_t)val < 0 || (int32_t)val >= getNbEntries())
		return;

	lastIndex = currentIndex;

	currentIndex = (int32_t)val;
	if (style & (kMultipleCheckStyle & ~kCheckStyle))
	{
		CMenuItem* item = getCurrent();
		if (item)
			item->setChecked(!item->isChecked());
	}
	CParamDisplay::setValue(val);

	// to force the redraw
	setDirty();

}


}