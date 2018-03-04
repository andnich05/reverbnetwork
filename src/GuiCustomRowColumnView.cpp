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

#include "../include/GuiCustomRowColumnView.h"

#include "../vstgui4/vstgui/lib/cbitmap.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"

GuiCustomRowColumnView::GuiCustomRowColumnView(const VSTGUI::CRect& size, Style style, LayoutStyle layoutStyle, VSTGUI::CCoord spacing, const VSTGUI::CRect& margin) 
	: CRowColumnView(size, style, layoutStyle, spacing, margin)
	, frameWidth(0)
	, frameColor(VSTGUI::CColor(0, 0, 0, 0)) {

}

void GuiCustomRowColumnView::drawBackgroundRect(VSTGUI::CDrawContext* pContext, const VSTGUI::CRect& _updateRect) {
	CRowColumnView::drawBackgroundRect(pContext, _updateRect);

	pContext->setFillColor(VSTGUI::CColor(0, 0, 0, 0));
	pContext->setLineWidth(frameWidth);
	pContext->setFrameColor(frameColor);
	VSTGUI::CRect rect = getViewSize();
	rect.offset(-rect.left, -rect.top);
	pContext->drawRect(rect);
}
