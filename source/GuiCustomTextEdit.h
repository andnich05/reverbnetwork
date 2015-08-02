/*
* GuiCustomTextEdit: Remove strings from input
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

#ifndef GUICUSTOMTEXTEDIT_H
#define GUICUSTOMTEXTEDIT_H

#include "../vstgui4/vstgui/lib/controls/ctextedit.h"

namespace VSTGUI {
	class GuiCustomTextEdit : public CTextEdit {
	public:
		GuiCustomTextEdit(const CRect& size, CControlListener* listener, int32_t tag, UTF8StringPtr txt = 0, CBitmap* background = 0, const int32_t style = 0);

		// From CTextEdit
		virtual	void takeFocus() VSTGUI_OVERRIDE_VMETHOD;
		virtual	void looseFocus() VSTGUI_OVERRIDE_VMETHOD;

		// Set string to remove when user clicks in the view
		void setStringToTruncate(const std::string& str);

	private:
		char stringToTruncate[256];
	};

}

#endif // GUICUSTOMTEXTEDIT_H