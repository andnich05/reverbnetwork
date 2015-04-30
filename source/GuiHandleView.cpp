#include "GuiHandleView.h"

#include "coffscreencontext.h"
#include "cbitmap.h"
#include "cframe.h"
#include "ifocusdrawing.h"
#include "controls/ccontrol.h"

#include <algorithm>
#include <assert.h>

namespace VSTGUI {

	GuiHandleView::GuiHandleView(const CRect &rect)
		: CViewContainer(rect)
	{
		backgroundOffset(0, 0);
		backgroundColor = kBlackCColor;
		setAutosizingEnabled(true);
		mousePressed = false;
		mousePressedX = 0;
		mousePressedY = 0;
	}

	// ANFASSER OBEN HINMACHEN AN DEM MAN DAS MODUL VERSCHIEBEN KANN
	CMouseEventResult GuiHandleView::onMouseDown(CPoint &where, const CButtonState& buttons)
	{
		return kMouseEventHandled;

	}

	CMouseEventResult GuiHandleView::onMouseMoved(CPoint &where, const CButtonState& buttons)
	{

		return kMouseEventHandled;
	}

	CMouseEventResult GuiHandleView::onMouseUp(CPoint &where, const CButtonState& buttons)
	{

		return kMouseEventHandled;
	}

}