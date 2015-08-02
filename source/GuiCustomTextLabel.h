/*
* GuiCustomTextLabel: Color, fontsize and alignment settable in constructor
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

#ifndef GUICUSTOMTEXTLABEL
#define GUICUSTOMTEXTLABEL

#include "../vstgui4/vstgui/lib/controls/ctextlabel.h"
#include "../vstgui4/vstgui/lib/cfont.h"

namespace VSTGUI {
	class GuiCustomTextLabel : public CTextLabel {
	public:
		// Custom constructor with some handy stuff
		GuiCustomTextLabel(const CRect& size, UTF8StringPtr txt = 0, const CFontRef& font = kNormalFont, const CHoriTxtAlign& textAlign = CHoriTxtAlign::kCenterText, const CColor& backgroundColor = CColor(0, 0, 0, 0), const CColor& frameColor = CColor(0, 0, 0, 0));
	};

}

#endif // GUICUSTOMTEXTLABEL