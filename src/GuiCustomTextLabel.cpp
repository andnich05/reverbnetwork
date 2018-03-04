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

#include "../include/GuiCustomTextLabel.h"

GuiCustomTextLabel::GuiCustomTextLabel(const VSTGUI::CRect& size, VSTGUI::UTF8StringPtr txt, const VSTGUI::CFontRef& font, const VSTGUI::CHoriTxtAlign& textAlign, const VSTGUI::CColor& backgroundColor, const VSTGUI::CColor& frameColor)
: CTextLabel(size, txt, 0, 0) {

	setBackColor(backgroundColor);
	setFrameColor(frameColor);
	setFont(font);
	setHoriAlign(textAlign);
}
