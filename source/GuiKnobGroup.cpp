#include "GuiKnobGroup.h"
#include "../vstgui4/vstgui/vstgui.h"

namespace VSTGUI {

	GuiKnobGroup::GuiKnobGroup(const CRect& size, const VSTGUI::UTF8StringPtr name, const CBitmap& knobBitmap, CControlListener* listener)
: CViewContainer(size) {

	
	//CTextLabel* labelTitle = new CTextLabel(CRect(CPoint(0, 0), CPoint(this->size.width, 20)), name, 0);
	//CAnimKnob* delayKnob = new CAnimKnob(CRect(CPoint(0, labelTitle->getHeight() + 5), CPoint(knobBitmap.getWidth(), knobBitmap.getWidth())), this, numberOfAPModules + PARAM_ALLPASSDELAY_FIRST, 80, 42, knobBackground);
	//CTextEdit* delayEdit = new CTextEdit(CRect(CPoint(0, 70), CPoint(20, 20)), this, 666, "0.0");
	//delayView->addView(delayLabel);
	//delayView->addView(delayKnob);
	//delayView->addView(delayEdit);
	//allpassView->addView(delayView);
	
}



}