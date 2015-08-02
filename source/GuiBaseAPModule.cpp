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

#include "GuiBaseAPModule.h"

#include "ReverbNetworkDefines.h"

#include "coffscreencontext.h"
#include "cbitmap.h"
#include "cframe.h"
#include "ifocusdrawing.h"
#include "controls/ccontrol.h"
//#include "cscrollview.h"

#include <algorithm>
#include <assert.h>

namespace VSTGUI {

const char* GuiBaseAPModule::kModuleWantsFocus = "Module wants focus";

GuiBaseAPModule::GuiBaseAPModule(const CRect &rect, const CRect& handleRegion, const unsigned int moduleId, CBaseObject* editor)
: CViewContainer(rect)
, handleSize(handleRegion.getWidth(), handleRegion.getHeight())
, viewSize(rect.getWidth(), getHeight())
, moduleId(moduleId)
, editor(editor)
{
	//setAutosizingEnabled (true);
	mousePressed = false;
	mousePressedX = 0;
	mousePressedY = 0;
	collapsed = false;
}

unsigned int GuiBaseAPModule::getModuleId() const {
	return moduleId;
}

void GuiBaseAPModule::collapseView(const bool& collapse) {
	// If already collapsed or not collapsed: do nothing
	if (collapse == isCollapsed()) {
		return;
	}
	if (collapse) {
		// Change the view's size
		viewSize = CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight()); // Save for uncollapse
		CRect newSize = this->getViewSize();
		newSize.setBottomRight(CPoint(this->getViewSize().getTopRight().x, this->getViewSize().getTopRight().y + handleSize.y));
		this->setViewSize(newSize);
	}
	else {
		// Expand the view
		CRect newSize = this->getViewSize();
		newSize.setBottomRight(CPoint(this->getViewSize().getTopLeft().x + viewSize.x, this->getViewSize().getTopLeft().y + viewSize.y));
		this->setViewSize(newSize);
	}
	collapsed = collapse;
	this->setMouseableArea(this->getViewSize()); // !!! Important: Update the mouseable area!
	this->getParentView()->setDirty(); // Repaint
}

CMouseEventResult GuiBaseAPModule::onMouseDown(CPoint &where, const CButtonState& buttons)
{
	// Notify the editor that the user has clicked on this module view so that the editor can bring the view in the foreground by changing the Z order of his views
	editor->notify(this, kModuleWantsFocus);

	// FrameToLocal changes the global coordinates (e.g. 1200) to local coordinates of this frame (e.g. 50) => !Parameter is a reference!
	CRect handleRegion(CRect(CPoint(0, 0), CPoint(handleSize)));
	CPoint whereCopy = where;
	// convert to relativ pos
	whereCopy.offset(-getViewSize().left, -getViewSize().top);
	// Check if the mouse click is inside the handle region
	if (whereCopy.isInside(handleRegion) && buttons.isLeftButton()) {
		mousePressed = true;
		mousePressedX = whereCopy.x;
		mousePressedY = whereCopy.y;
	}
	// If not: do the regular Vst stuff...
	else {
		// convert to relativ pos
		CPoint where2(where);
		where2.offset(-getViewSize().left, -getViewSize().top);

		FOREACHSUBVIEW_REVERSE(true)
			if (pV && pV->isVisible() && pV->getMouseEnabled() && pV->hitTest(where2, buttons))
			{
			CControl* control = dynamic_cast<CControl*> (pV);
			if (control && control->getListener() && buttons & (kAlt | kShift | kControl | kApple | kRButton))
			{
				if (control->getListener()->controlModifierClicked((CControl*)pV, buttons) != 0)
					return kMouseEventHandled;
			}
			CBaseObjectGuard crg(pV);

			if (pV->wantsFocus())
				getFrame()->setFocusView(pV);

			CMouseEventResult result = pV->onMouseDown(where2, buttons);
			if (result != kMouseEventNotHandled && result != kMouseEventNotImplemented)
			{
				if (pV->getNbReference() > 1 && result == kMouseEventHandled)
					mouseDownView = pV;
				return result;
			}
			if (!pV->getTransparency())
				return result;
			}
		ENDFOREACHSUBVIEW
			return kMouseEventNotHandled;
	}
	return kMouseEventHandled;

}

CMouseEventResult GuiBaseAPModule::onMouseMoved(CPoint &where, const CButtonState& buttons)
{
	if (mousePressed) {
		// Move the view to the new position
		this->setViewSize(CRect(CPoint(where.x - mousePressedX, where.y - mousePressedY), CPoint(this->getWidth(), this->getHeight())));

		// invalid() updates the GUI; setDirty() is similar but does not force an immediate redraw, although setDirty() is thread safe(?)

		// Don't paint the modules outside the parent view
		if (this->getViewSize().getBottomRight().y > getParentView()->getViewSize().getBottomRight().y) {
			this->setViewSize(CRect(CPoint(getViewSize().getTopLeft().x, getParentView()->getViewSize().getBottomRight().y - getViewSize().getHeight()), 
				CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
		}
		if (this->getViewSize().getBottomRight().x > getParentView()->getViewSize().getBottomRight().x) {
			this->setViewSize(CRect(CPoint(getParentView()->getViewSize().getBottomRight().x - getViewSize().getWidth(), getViewSize().getTopLeft().y), 
				CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
		}
		if (this->getViewSize().getTopLeft().x < 0.0) {
			this->setViewSize(CRect(CPoint(0, getViewSize().getTopLeft().y), CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
		}
		if (this->getViewSize().getTopLeft().y < 0.0) {
			this->setViewSize(CRect(CPoint(getViewSize().getTopLeft().x, 0), CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
		}
		
		/*FILE* pFile = fopen("E:\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(getViewSize().getBottomRight().y).c_str());
		fprintf(pFile, "y(n): %s\n", std::to_string(getParentView()->getViewSize().getBottomRight().y).c_str());
		fclose(pFile);*/

		this->setMouseableArea(this->getViewSize());
		this->getParentView()->setDirty();
	}
	// Do the regular Vst stuff
	else {
		if (mouseDownView)
		{
			CBaseObjectGuard crg(mouseDownView);

			// convert to relativ pos
			CPoint where2(where);
			where2.offset(-getViewSize().left, -getViewSize().top);
			CMouseEventResult mouseResult = mouseDownView->onMouseMoved(where2, buttons);
			if (mouseResult != kMouseEventHandled && mouseResult != kMouseEventNotImplemented)
			{
				mouseDownView = 0;
				return kMouseEventNotHandled;
			}
			return kMouseEventHandled;
		}
		return kMouseEventNotHandled;
	}
	return kMouseEventHandled;
}

CMouseEventResult GuiBaseAPModule::onMouseUp(CPoint &where, const CButtonState& buttons)
{
	// Reset
	mousePressed = false;

	// Regular Vst stuff
	if (mouseDownView)
	{
		CBaseObjectGuard crg(mouseDownView);

		// convert to relativ pos
		CPoint where2(where);
		where2.offset(-getViewSize().left, -getViewSize().top);
		mouseDownView->onMouseUp(where2, buttons);
		mouseDownView = 0;
		return kMouseEventHandled;
	}
	return kMouseEventNotHandled;
}

//-----------------------------------------------------------------------------
/**
* @param pContext the context which to use to draw the background
* @param _updateRect the area which to draw
*/
void GuiBaseAPModule::drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect)
{
	if (getDrawBackground())
	{
		CRect oldClip;
		pContext->getClipRect(oldClip);
		CRect newClip(_updateRect);
		newClip.bound(oldClip);
		pContext->setClipRect(newClip);
		CRect tr(0, 0, getViewSize().getWidth(), getViewSize().getHeight());
		getDrawBackground()->draw(pContext, tr, backgroundOffset);
		pContext->setClipRect(oldClip);
	}
	else if ((backgroundColor.alpha != 255 && getTransparency()) || !getTransparency())
	{
		/*FILE* pFile = fopen("E:\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(34576).c_str());
		fclose(pFile);*/
		pContext->setDrawMode(kAliasing);
		pContext->setLineWidth(1);
		pContext->setFillColor(backgroundColor);
		pContext->setFrameColor(backgroundColor);
		pContext->setLineStyle(kLineSolid);
		CRect r;
		if (backgroundColorDrawStyle == kDrawFilled || (backgroundColorDrawStyle == kDrawFilledAndStroked && backgroundColor.alpha == 255))
		{
			r = _updateRect;
			r.inset(-1, -1);
		}
		else
		{
			r = getViewSize();
			r.offset(-r.left, -r.top);
		}
		pContext->drawRect(r, backgroundColorDrawStyle);
	}

	// Custom stuff: Draw frame
	pContext->setFillColor(CColor(0, 0, 0, 0));
	pContext->setFrameColor(CCOLOR_MODULE_MAINFRAME);
	CRect rect = getViewSize();
	rect.offset(-rect.left, -rect.top);
	pContext->drawRect(rect);
}

}