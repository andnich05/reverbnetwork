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

#ifndef GUIOPTIONMENUINPUTSELECTOR_H
#define GUIOPTIONMENUINPUTSELECTOR_H

#include "../vstgui4/vstgui/lib/controls/coptionmenu.h"

class GuiOptionMenuInputSelector :
	public VSTGUI::COptionMenu
{
public:
	GuiOptionMenuInputSelector(const VSTGUI::CRect& size, VSTGUI::IControlListener* listener, int32_t tag, VSTGUI::CBitmap* background = 0, VSTGUI::CBitmap* bgWhenClick = 0, const int32_t style = 0);

	// Get the index which active before the current index was selected
	inline int32_t getLastIndex() const { return lastIndex; }

protected:
	virtual void setValue(float val) override;

private:
	int32_t lastIndex;
};

#endif // GUIOPTIONMENUINPUTSELECTOR_H