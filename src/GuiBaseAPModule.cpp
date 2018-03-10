/*
* GuiBaseAPModule: Contains the module components
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

#include "../include/GuiBaseAPModule.h"

#include "../include/ReverbNetworkDefines.h"

#include "../vstgui4/vstgui/lib/coffscreencontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"
#include "../vstgui4/vstgui/lib/cframe.h"
#include "../vstgui4/vstgui/lib/ifocusdrawing.h"
#include "../vstgui4/vstgui/lib/controls/ccontrol.h"

#include <algorithm>

const char* GuiBaseAPModule::kModuleWantsFocus = "Module wants focus";

GuiBaseAPModule::GuiBaseAPModule(const VSTGUI::CRect &rect, const VSTGUI::CRect& handleRegion, const unsigned int moduleId, CBaseObject* editor)
	: CViewContainer(rect)
	, mousePressedX(0)
	, mousePressedY(0)
	, mousePressed(false)
	, handleSize(handleRegion.getWidth(), handleRegion.getHeight())
	, viewSize(rect.getWidth(), getHeight())
	, moduleId(moduleId)
	, collapsed(false)
	, editor(editor)
{

}

unsigned int GuiBaseAPModule::getModuleId() const {
	return moduleId;
}

void GuiBaseAPModule::collapseView(bool collapse) {
	// If already collapsed or not collapsed: do nothing
	if (collapse == isCollapsed()) {
		return;
	}
	if (collapse) {
		// Change the view's size
		viewSize = VSTGUI::CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight()); // Save for uncollapse
		VSTGUI::CRect newSize = this->getViewSize();
		newSize.setBottomRight(VSTGUI::CPoint(this->getViewSize().getTopRight().x, this->getViewSize().getTopRight().y + handleSize.y));
		this->setViewSize(newSize);
	}
	else {
		// Expand the view
		VSTGUI::CRect newSize = this->getViewSize();
		newSize.setBottomRight(VSTGUI::CPoint(this->getViewSize().getTopLeft().x + viewSize.x, this->getViewSize().getTopLeft().y + viewSize.y));
		this->setViewSize(newSize);
	}
	collapsed = collapse;
	this->setMouseableArea(this->getViewSize()); // !!! Important: Update the mouseable area!
	this->getParentView()->setDirty(); // Repaint
}

VSTGUI::CMouseEventResult GuiBaseAPModule::onMouseDown(VSTGUI::CPoint &where, const VSTGUI::CButtonState& buttons)
{
	// Notify the editor that the user has clicked on this module view so that the editor can bring the view in the foreground by changing the Z order of his views
	editor->notify(this, kModuleWantsFocus);

	// FrameToLocal changes the global coordinates (e.g. 1200) to local coordinates of this frame (e.g. 50) => !Parameter is a reference!
	VSTGUI::CRect handleRegion(VSTGUI::CRect(VSTGUI::CPoint(0, 0), VSTGUI::CPoint(handleSize)));
	VSTGUI::CPoint whereCopy = where;
	// convert to relativ pos
	whereCopy.offset(-getViewSize().left, -getViewSize().top);
	// Check if the mouse click is inside the handle region
	if (handleRegion.pointInside(whereCopy) && buttons.isLeftButton()) {
		mousePressed = true;
		mousePressedX = whereCopy.x;
		mousePressedY = whereCopy.y;
	}
	// If not: do the regular Vst stuff...
	else {
		return CViewContainer::onMouseDown(where, buttons);
	}
	return VSTGUI::kMouseEventHandled;

}

VSTGUI::CMouseEventResult GuiBaseAPModule::onMouseMoved(VSTGUI::CPoint &where, const VSTGUI::CButtonState& buttons)
{
	if (mousePressed) {
		// Move the view to the new position
		this->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(where.x - mousePressedX, where.y - mousePressedY), VSTGUI::CPoint(this->getWidth(), this->getHeight())));

		// invalid() updates the GUI; setDirty() is similar but does not force an immediate redraw, although setDirty() is thread safe(?)

		// Don't paint the modules outside the parent view
		if (this->getViewSize().getBottomRight().y > getParentView()->getViewSize().getBottomRight().y) {
			this->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(getViewSize().getTopLeft().x, getParentView()->getViewSize().getBottomRight().y - getViewSize().getHeight()),
				VSTGUI::CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
		}
		if (this->getViewSize().getBottomRight().x > getParentView()->getViewSize().getBottomRight().x) {
			this->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(getParentView()->getViewSize().getBottomRight().x - getViewSize().getWidth(), getViewSize().getTopLeft().y),
				VSTGUI::CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
		}
		if (this->getViewSize().getTopLeft().x < 0.0) {
			this->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(0, getViewSize().getTopLeft().y), VSTGUI::CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
		}
		if (this->getViewSize().getTopLeft().y < 0.0) {
			this->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(getViewSize().getTopLeft().x, 0), VSTGUI::CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
		}

		this->setMouseableArea(this->getViewSize());
		this->getParentView()->setDirty();
	}
	// Do the regular Vst stuff
	else {
		return CViewContainer::onMouseMoved(where, buttons);
	}
	return VSTGUI::kMouseEventHandled;
}

VSTGUI::CMouseEventResult GuiBaseAPModule::onMouseUp(VSTGUI::CPoint &where, const VSTGUI::CButtonState& buttons)
{
	// Reset
	mousePressed = false;

	return CViewContainer::onMouseUp(where, buttons);
}

void GuiBaseAPModule::drawBackgroundRect(VSTGUI::CDrawContext* pContext, const VSTGUI::CRect& _updateRect)
{
	CViewContainer::drawBackgroundRect(pContext, _updateRect);

	// Draw frame
	pContext->setFillColor(VSTGUI::CColor(0, 0, 0, 0));
	pContext->setFrameColor(CCOLOR_MODULE_MAINFRAME);
	VSTGUI::CRect rect = getViewSize();
	rect.offset(-rect.left, -rect.top);
	pContext->drawRect(rect);

}
