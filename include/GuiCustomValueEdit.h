/*
* GuiCustomValueEdit: Remove unit from input
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

#ifndef GUICUSTOMVALUEEDIT_H
#define GUICUSTOMVALUEEDIT_H

#include "../vstgui4/vstgui/lib/controls/ctextedit.h"
#include "../include/ReverbNetworkEnums.h"

class GuiCustomValueEdit : public VSTGUI::CTextEdit {

public:
	GuiCustomValueEdit(const VSTGUI::CRect& size, VSTGUI::IControlListener* listener, int32_t tag, VSTGUI::UTF8StringPtr txt = 0, VSTGUI::CBitmap* background = 0, const int32_t style = 0);

	virtual	void takeFocus() override;
	virtual	void looseFocus() override;

	void setStringToTruncate(const std::string& str, bool truncateSpaceBeforeString);

	void setValueToStringUserData(const valueToStringUserData & userData);
	const valueToStringUserData & getValueToStringUserData() const;

private:
	std::string stringToTruncate;
	valueToStringUserData userData;
};

#endif // GUICUSTOMVALUEEDIT_H