#include "GuiBaseAPModule.h"
#include "ReverbNetworkDefines.h"
#include "../vstgui4/vstgui/vstgui.h"

namespace VSTGUI {


GuiBaseAPModule::GuiBaseAPModule (const CRect &rect, CControlListener* listener)
: CViewContainer(rect)
, listener(listener)
{
	backgroundOffset (0, 0);
	backgroundColor = kBlackCColor;
	setAutosizingEnabled (true);

	knobBackground = new CBitmap("knob.png");

	//CRowColumnView* baseModuleView = new CRowColumnView(CRect(0, 0, 300, 250), CRowColumnView::kRowStyle);
	baseModuleView = new CRowColumnView(CRect(0, 0, 400, 400), CRowColumnView::kRowStyle);
	baseModuleView->setBackgroundColor(kBlueCColor);

	// Handle view to grab and move the module with the mouse
	handleView = new CViewContainer(CRect(0, 0, 400, 10));
	handleView->setBackgroundColor(CColor(222, 222, 0, 255));

	// Control view which holds the individual processing modules
	controlView = new CRowColumnView(CRect(0, 0, 400, 300), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy);
	controlView->setBackgroundColor(CColor(150, 150, 150, 255));
	//controlView->addView(knob);

	// Holds the input mixer controls (input gain for each channel)
	mixerView = new CViewContainer(CRect(0, 0, 100, 300));

	// Holds the equalizer controls
	equalizerView = new CViewContainer(CRect(0, 0, 100, 300));

	// Holds the allpass controls (delay and decay)
	allpassView = new CViewContainer(CRect(0, 0, 100, 300));
	delayKnob = new CAnimKnob(CRect(CPoint(0, 0), CPoint(42, 42)), listener, 0 + PARAM_ALLPASSDELAY_FIRST, 80, 42, knobBackground);
	allpassView->addView(delayKnob);
	decayKnob = new CAnimKnob(CRect(CPoint(0, 50), CPoint(42, 42)), listener, 0 + PARAM_ALLPASSDECAY_FIRST, 80, 42, knobBackground);
	allpassView->addView(decayKnob);

	// Holds the output gain control
	gainView = new CViewContainer(CRect(0, 0, 100, 300));

	// Add process views to the control view
	controlView->addView(mixerView);
	controlView->addView(equalizerView);
	controlView->addView(allpassView);
	controlView->addView(gainView);

	baseModuleView->addView(handleView);
	baseModuleView->addView(controlView);

	this->addView(baseModuleView);
}


}