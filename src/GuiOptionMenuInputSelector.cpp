/*
* GuiOptionMenuInputSelector: Saves the last selected index
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