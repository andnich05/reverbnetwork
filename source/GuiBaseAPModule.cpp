#include "GuiBaseAPModule.h"

#include "coffscreencontext.h"
#include "cbitmap.h"
#include "cframe.h"
#include "ifocusdrawing.h"
#include "controls/ccontrol.h"

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
	//this->handleRegion.offset(-1, -1);
	backgroundOffset (0, 0);
	backgroundColor = kBlackCColor;
	setAutosizingEnabled (true);
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
		viewSize = CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight()); // Save for uncollapse
		CRect newSize = this->getViewSize();
		newSize.setBottomRight(CPoint(this->getViewSize().getTopRight().x, this->getViewSize().getTopRight().y + handleSize.y));
		this->setViewSize(newSize);
	}
	else {
		CRect newSize = this->getViewSize();
		newSize.setBottomRight(CPoint(this->getViewSize().getTopLeft().x + viewSize.x, this->getViewSize().getTopLeft().y + viewSize.y));
		this->setViewSize(newSize);
	}
	collapsed = collapse;
	this->setMouseableArea(this->getViewSize());
	this->getParentView()->setDirty();
}

CMouseEventResult GuiBaseAPModule::onMouseDown(CPoint &where, const CButtonState& buttons)
{
	this->setDirty();
	editor->notify(this, kModuleWantsFocus);
	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", "mouse down");
	fclose(pFile);*/

	// where are global coordinates of the parent (?) view

	// frameToLocal changes the global coordinates (e.g. 1200) to local coordinates of this frame (e.g. 50)
	CRect handleRegion(CRect(CPoint(0, 0), CPoint(handleSize)));
	CPoint whereCopy = where;
	whereCopy.offset(-getViewSize().left, -getViewSize().top);
	// Check if the mouse click is inside the handle region
	//if (coordinates.x <= handleRegion.right && coordinates.y <= handleRegion.bottom && coordinates.x >= handleRegion.left && coordinates.y >= handleRegion.top && buttons.isLeftButton()) 

	if (whereCopy.isInside(handleRegion) && buttons.isLeftButton()) {
		//&& (mousePressedX != whereCopy.x || mousePressedY != whereCopy.y)
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

		this->getParentView()->setDirty();
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

void GuiBaseAPModule::setViewSize(const CRect &rect, bool invalid)
{
	if (rect == getViewSize())
		return;

	CRect oldSize(getViewSize());
	CView::setViewSize(rect, invalid);

	if (getAutosizingEnabled())
	{
		CCoord widthDelta = rect.getWidth() - oldSize.getWidth();
		CCoord heightDelta = rect.getHeight() - oldSize.getHeight();

		if (widthDelta != 0 || heightDelta != 0)
		{
			int32_t numSubviews = getNbViews();
			int32_t counter = 0;
			bool treatAsColumn = (getAutosizeFlags() & kAutosizeColumn) != 0;
			bool treatAsRow = (getAutosizeFlags() & kAutosizeRow) != 0;
			FOREACHSUBVIEW
				int32_t autosize = pV->getAutosizeFlags();
			CRect viewSize(pV->getViewSize());
			CRect mouseSize(pV->getMouseableArea());
			if (treatAsColumn)
			{
				if (counter)
				{
					viewSize.offset(counter * (widthDelta / (numSubviews)), 0);
					mouseSize.offset(counter * (widthDelta / (numSubviews)), 0);
				}
				viewSize.setWidth(viewSize.getWidth() + (widthDelta / (numSubviews)));
				mouseSize.setWidth(mouseSize.getWidth() + (widthDelta / (numSubviews)));
			}
			else if (widthDelta != 0 && autosize & kAutosizeRight)
			{
				viewSize.right += widthDelta;
				mouseSize.right += widthDelta;
				if (!(autosize & kAutosizeLeft))
				{
					viewSize.left += widthDelta;
					mouseSize.left += widthDelta;
				}
			}
			if (treatAsRow)
			{
				if (counter)
				{
					viewSize.offset(0, counter * (heightDelta / (numSubviews)));
					mouseSize.offset(0, counter * (heightDelta / (numSubviews)));
				}
				viewSize.setHeight(viewSize.getHeight() + (heightDelta / (numSubviews)));
				mouseSize.setHeight(mouseSize.getHeight() + (heightDelta / (numSubviews)));
			}
			else if (heightDelta != 0 && autosize & kAutosizeBottom)
			{
				viewSize.bottom += heightDelta;
				mouseSize.bottom += heightDelta;
				if (!(autosize & kAutosizeTop))
				{
					viewSize.top += heightDelta;
					mouseSize.top += heightDelta;
				}
			}
			if (viewSize != pV->getViewSize())
			{
				pV->setViewSize(viewSize);
				pV->setMouseableArea(mouseSize);
			}
			counter++;
			ENDFOREACHSUBVIEW
		}
	}
	parentSizeChanged();
}


//void GuiBaseAPModule::drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect)
//{
//	if (getDrawBackground())
//	{
//		CRect oldClip;
//		pContext->getClipRect(oldClip);
//		CRect newClip(_updateRect);
//		newClip.bound(oldClip);
//		pContext->setClipRect(newClip);
//		CRect tr(0, 0, getViewSize().getWidth(), getViewSize().getHeight());
//		getDrawBackground()->draw(pContext, tr, backgroundOffset);
//		pContext->setClipRect(oldClip);
//	}
//	else if ((backgroundColor.alpha != 255 && getTransparency()) || !getTransparency())
//	{
//		pContext->setDrawMode(kAliasing);
//		pContext->setLineWidth(1);
//		pContext->setFillColor(backgroundColor);
//		pContext->setFrameColor(CColor(100, 100, 100, 255));
//		pContext->setLineStyle(kLineSolid);
//		/*CRect r;
//		if (backgroundColorDrawStyle == kDrawFilled || (backgroundColorDrawStyle == kDrawFilledAndStroked && backgroundColor.alpha == 255))
//		{
//			r = _updateRect;
//			r.inset(-1, -1);
//		}
//		else
//		{
//			r = getViewSize();
//			r.offset(-r.left, -r.top);
//		}*/
//
//		CRect s = this->getViewSize();
//		s.setTopLeft(frameToLocal(s.getTopLeft()));
//		s.setBottomRight(frameToLocal(s.getBottomRight()));
//		s.offset(1, 0);
//		pContext->drawRect(s, backgroundColorDrawStyle);
//
//		pContext->setLineWidth(1);
//		pContext->setFillColor(CColor(30, 30, 30, 255));
//		pContext->drawRect(handleRegion, backgroundColorDrawStyle);
//	}
//}

}