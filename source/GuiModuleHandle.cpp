#include "GuiModuleHandle.h"

#include "coffscreencontext.h"
#include "cbitmap.h"
#include "cframe.h"
#include "ifocusdrawing.h"
#include "controls/ccontrol.h"

#include <algorithm>
#include <assert.h>

namespace VSTGUI {

GuiModuleHandle::GuiModuleHandle(const CRect &rect)
: CViewContainer(rect)
{
	backgroundOffset (0, 0);
	backgroundColor = kBlackCColor;
	setAutosizingEnabled (true);
	mousePressed = false;
	mousePressedX = 0;
	mousePressedY = 0;
}

// ANFASSER OBEN HINMACHEN AN DEM MAN DAS MODUL VERSCHIEBEN KANN
CMouseEventResult GuiModuleHandle::onMouseDown (CPoint &where, const CButtonState& buttons)
{
	

	mousePressed = true;
	mousePressedX = where.x;
	mousePressedY = where.y;

	//this->setViewSize(CRect(100, 50, 200, 200));
	//this->setBackgroundColor(kRedCColor);



	return kMouseEventHandled;


}

CMouseEventResult GuiModuleHandle::onMouseMoved(CPoint &where, const CButtonState& buttons)
{	
	if (mousePressed) {
		this->size.moveTo(where.x - mousePressedX, where.y - mousePressedY);

		// invalid() updates the GUI; setDirty() is similar but does not force an immediate redraw
		this->getParentView()->invalid();

	}

	return kMouseEventHandled;
}

CMouseEventResult GuiModuleHandle::onMouseUp(CPoint &where, const CButtonState& buttons)
{
	mousePressed = false;

	return kMouseEventHandled;
}

}