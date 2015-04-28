#include "GuiBaseModuleFrame.h"

#include "coffscreencontext.h"
#include "cbitmap.h"
#include "cframe.h"
#include "ifocusdrawing.h"
#include "controls/ccontrol.h"

#include <algorithm>
#include <assert.h>

namespace VSTGUI {

GuiBaseModuleFrame::GuiBaseModuleFrame (const CRect &rect)
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
CMouseEventResult GuiBaseModuleFrame::onMouseDown (CPoint &where, const CButtonState& buttons)
{
	

	mousePressed = true;
	mousePressedX = where.x;
	mousePressedY = where.y;

	//this->setViewSize(CRect(100, 50, 200, 200));
	//this->setBackgroundColor(kRedCColor);



	return kMouseEventHandled;


}

CMouseEventResult GuiBaseModuleFrame::onMouseMoved(CPoint &where, const CButtonState& buttons)
{	
	if (mousePressed) {
		this->size.moveTo(where.x - mousePressedX, where.y - mousePressedY);

		// invalid() updates the GUI; setDirty() is similar but does not force an immediate redraw
		this->getParentView()->invalid();

		FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", "Mouse moved");
		fclose(pFile);
	}

	return kMouseEventHandled;
}

CMouseEventResult GuiBaseModuleFrame::onMouseUp(CPoint &where, const CButtonState& buttons)
{
	FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", "Mouse up");
	fclose(pFile);
	mousePressed = false;

	return kMouseEventHandled;
}

}