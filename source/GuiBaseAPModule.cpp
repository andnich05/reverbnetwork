#include "GuiBaseAPModule.h"

#include "coffscreencontext.h"
#include "cbitmap.h"
#include "cframe.h"
#include "ifocusdrawing.h"
#include "controls/ccontrol.h"

#include <algorithm>
#include <assert.h>

namespace VSTGUI {

GuiBaseAPModule::GuiBaseAPModule(const CRect &rect, const CRect& handleRegion)
: CViewContainer(rect)
, handleRegion(handleRegion)
{
	backgroundOffset (0, 0);
	backgroundColor = kBlackCColor;
	setAutosizingEnabled (true);
	mousePressed = false;
	mousePressedX = 0;
	mousePressedY = 0;
}

// ANFASSER OBEN HINMACHEN AN DEM MAN DAS MODUL VERSCHIEBEN KANN
CMouseEventResult GuiBaseAPModule::onMouseDown(CPoint &where, const CButtonState& buttons)
{
	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", "mouse down");
	fclose(pFile);*/

	// where are global coordinates of the parent (?) view

		// frameToLocal changes the global coordinates (e.g. 1200) to local coordinates of this frame (e.g. 50)
	
	CPoint whereCopy = where;
	frameToLocal(whereCopy);
	// Check whether the mouse click is inside the handle region
	//if (coordinates.x <= handleRegion.right && coordinates.y <= handleRegion.bottom && coordinates.x >= handleRegion.left && coordinates.y >= handleRegion.top && buttons.isLeftButton()) {
	if (whereCopy.isInside(handleRegion) && buttons.isLeftButton()) {
		mousePressed = true;
		mousePressedX = whereCopy.x;
		mousePressedY = whereCopy.y;
		
	}
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
		//this->size.moveTo(where.x - mousePressedX, where.y - mousePressedY);
		this->setViewSize(CRect(CPoint(where.x - mousePressedX, where.y - mousePressedY), CPoint(this->getWidth(), this->getHeight())));
		// invalid() updates the GUI; setDirty() is similar but does not force an immediate redraw, although setDirty() is thread safe
		this->setMouseableArea(this->getViewSize());

		//FOREACHSUBVIEW_REVERSE(true)
		//	pV->setViewSize(CRect(CPoint(where.x - mousePressedX, where.y - mousePressedY), CPoint(pV->getWidth(), pV->getHeight())));
		//	pV->setMouseableArea(pV->getViewSize());
		///*FILE* pFile = fopen("E:\\logVst.txt", "a");
		//fprintf(pFile, "y(n): %s\n", std::to_string(pV->getViewSize().bottom).c_str());
		//fclose(pFile);*/
		//ENDFOREACHSUBVIEW

		this->getParentView()->invalid();
	}
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

	mousePressed = false;

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
		pContext->setDrawMode(kAliasing);
		pContext->setLineWidth(1);
		pContext->setFillColor(backgroundColor);
		pContext->setFrameColor(CColor(100, 100, 100, 255));
		pContext->setLineStyle(kLineSolid);
		/*CRect r;
		if (backgroundColorDrawStyle == kDrawFilled || (backgroundColorDrawStyle == kDrawFilledAndStroked && backgroundColor.alpha == 255))
		{
			r = _updateRect;
			r.inset(-1, -1);
		}
		else
		{
			r = getViewSize();
			r.offset(-r.left, -r.top);
		}*/

		CRect s = this->getViewSize();
		s.setTopLeft(frameToLocal(s.getTopLeft()));
		s.setBottomRight(frameToLocal(s.getBottomRight()));
		s.offset(1, 0);
		pContext->drawRect(s, backgroundColorDrawStyle);

		pContext->setLineWidth(1);
		pContext->setFillColor(CColor(30, 30, 30, 255));
		pContext->drawRect(handleRegion, backgroundColorDrawStyle);
	}
}

}