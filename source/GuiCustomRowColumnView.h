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

namespace VSTGUI {
	class GuiCustomRowColumnView : public CRowColumnView {
	public:
		GuiCustomRowColumnView(const CRect& size, Style style = kRowStyle, LayoutStyle layoutStyle = kLeftTopEqualy, CCoord spacing = 0., const CRect& margin = CRect(0., 0., 0., 0.));

		// From CViewContainer
		virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);	///< draw the background

		// Set the width of the frame to paint around the view (frame is painted inside the view so beware of collisions with content! In such a case: Increase the margin)
		virtual inline void setFrameWidth(const int& width = 0) { frameWidth = width; }
		// Set the rounding of the frame to paint around the view (ToDo...)
		//virtual inline void setFrameRounding(const int& rounding = 0) { frameRounding = rounding; }
		// Set the color of the frame to paint around the view
		virtual inline void setFrameColor(const CColor& color = CColor(0, 0, 0, 0)) { frameColor = color; }

	private:
		int frameWidth;
		//int frameRounding;
		CColor frameColor;
	};

}

#endif GUICUSTOMROWCOLUMNVIEW_H