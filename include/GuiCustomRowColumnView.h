/*
* GuiCustomRowColumnView: CRowColumnView with frame painting
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

#ifndef GUICUSTOMROWCOLUMNVIEW_H
#define GUICUSTOMROWCOLUMNVIEW_H

#include "../vstgui4/vstgui/lib/crowcolumnview.h"
#include "../vstgui4/vstgui/lib/ccolor.h"

class GuiCustomRowColumnView : public VSTGUI::CRowColumnView {
public:
	GuiCustomRowColumnView(const VSTGUI::CRect& size, Style style = kRowStyle, LayoutStyle layoutStyle = kLeftTopEqualy, VSTGUI::CCoord spacing = 0., const VSTGUI::CRect& margin = VSTGUI::CRect(0., 0., 0., 0.));

	// From CViewContainer
	virtual void drawBackgroundRect(VSTGUI::CDrawContext* pContext, const VSTGUI::CRect& _updateRect) override;

	// Set the width of the frame to paint around the view (frame is painted inside the view so beware of collisions with content! In such a case: Increase the margin)
	virtual inline void setFrameWidth(const int& width = 0) { frameWidth = width; }
	// Set the rounding of the frame to paint around the view (ToDo...)
	//virtual inline void setFrameRounding(const int& rounding = 0) { frameRounding = rounding; }
	// Set the color of the frame to paint around the view
	virtual inline void setFrameColor(const VSTGUI::CColor& color = VSTGUI::CColor(0, 0, 0, 0)) { frameColor = color; }

private:
	int frameWidth;
	VSTGUI::CColor frameColor;
};


#endif GUICUSTOMROWCOLUMNVIEW_H
