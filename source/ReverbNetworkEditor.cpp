#include "ReverbNetworkEditor.h"
#include "ReverbNetworkDefines.h"
#include "GuiModuleHandle.h"

namespace Steinberg {
namespace Vst {

ReverbNetworkEditor::ReverbNetworkEditor(void* controller)
: VSTGUIEditor(controller) 
, numberOfAPModules(0)
{


	
}

ReverbNetworkEditor::~ReverbNetworkEditor() {

}

bool PLUGIN_API ReverbNetworkEditor::open(void* parent, const PlatformType& platformType) {
	
	if (frame) // already attached!
	{
		return false;
	}

	knobBackground = new CBitmap("knob.png");

	CRect editorSize(0, 0, 1000, 700);
	frame = new CFrame(editorSize, parent, this);
	frame->setBackgroundColor(CColor(110, 110, 110, 255));

	workspaceView = new CScrollView(CRect(0, 0, 700, 700), CRect(0, 0, 1500, 1500), CScrollView::kHorizontalScrollbar | CScrollView::kVerticalScrollbar, 16.0);
	workspaceView->setBackgroundColor(CColor(80, 80, 80, 255));

	CTextButton* buttonAddModule = new CTextButton(CRect(CPoint(750, 200), CPoint(100, 20)), this, 'AddM', "Add Module");
	frame->addView(buttonAddModule);
	CTextButton* buttonRemoveModule = new CTextButton(CRect(CPoint(750, 230), CPoint(100, 20)), this, 'RmvM', "Remove Module");
	frame->addView(buttonRemoveModule);
	frame->addView(workspaceView);
	return true;
}

void PLUGIN_API ReverbNetworkEditor::close() {
	if (frame)
	{
		frame->forget();	// Same as 'delete frame'?
		frame = 0;
	}
}

void ReverbNetworkEditor::valueChanged(CControl* pControl) {
	switch (pControl->getTag()) {
	case 0: {
		controller->setParamNormalized(1, pControl->getValue());
		controller->performEdit(1, pControl->getValue());
		break;
	}
	case 'AddM': {
		// Make sure create function is called only one time (without it would be two times)
		if (pControl->isDirty()) {
			createAPModule();
			/*FILE* pFile = fopen("E:\\logVst.txt", "a");
			fprintf(pFile, "y(n): %s\n", "Create");
			fclose(pFile);*/
		}
		break;
	}
	case 'RmvM': {
		if (pControl->isDirty()) {
			removeAPModule(2);
		}
		break;
	}
	}
}

void ReverbNetworkEditor::createAPModule() {

	//CRowColumnView* baseModuleView = new CRowColumnView(CRect(0, 0, 300, 250), CRowColumnView::kRowStyle);
	CRowColumnView* baseModuleView = new CRowColumnView(CRect(CPoint(0 + numberOfAPModules * 20, 0 + numberOfAPModules * 20), CPoint(0, 0)), CRowColumnView::kRowStyle);
	//baseModuleView->setBackgroundColor(CColor(55, 55, 55, 255));

	// Handle view to grab and move the module with the mouse
	GuiModuleHandle* handleView = new GuiModuleHandle(CRect(0, 0, 400, 20));
	handleView->setBackgroundColor(CColor(30, 30, 30, 255));

	// Control view which holds the individual processing modules
	CRowColumnView* controlView = new CRowColumnView(CRect(0, 0, 400, 300), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy);
	//controlView->setBackgroundColor(CColor(150, 150, 150, 255));
	//controlView->addView(knob);

	// Holds the input mixer controls (input gain for each channel)
	CViewContainer* mixerView = new CViewContainer(CRect(0, 0, 100, 300));
	mixerView->setBackgroundColor(CColor(55, 55, 55, 255));

	// Holds the equalizer controls
	CViewContainer* equalizerView = new CViewContainer(CRect(0, 0, 100, 300));
	equalizerView->setBackgroundColor(CColor(55, 55, 55, 255));

	// Holds the allpass controls (delay and decay)
	CRowColumnView* allpassView = new CRowColumnView(CRect(0, 0, 100, 300), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 10.0);
	allpassView->setBackgroundColor(CColor(55, 55, 55, 255));
	// ID-Tag of the main control is the same as the parameter ID

	CViewContainer* delayView = new CViewContainer(CRect(0, 0, 50, 80));
	delayView->setBackgroundColor(CColor(55, 55, 55, 255));
	CTextLabel* delayLabel = new CTextLabel(CRect(CPoint(0, 0), CPoint(delayView->getWidth(), 15)), "Delay");
	delayLabel->setFont(CFontRef(kNormalFontSmall));
	delayLabel->setBackColor(CColor(0, 0, 0, 0));
	delayLabel->setFrameColor(CColor(0, 0, 0, 0));
	CAnimKnob* delayKnob = new CAnimKnob(CRect(CPoint(0 + (delayView->getWidth() - knobBackground->getWidth()) / 2, delayLabel->getViewSize().bottom + 3), CPoint(knobBackground->getWidth(), knobBackground->getWidth())), this, numberOfAPModules + PARAM_ALLPASSDELAY_FIRST, knobBackground->getHeight() / knobBackground->getWidth(), knobBackground->getWidth(), knobBackground);
	CTextEdit* delayEdit = new CTextEdit(CRect(CPoint(0, delayKnob->getViewSize().bottom + 3), CPoint(delayView->getWidth(), 15)), this, 67, "0.0");
	delayEdit->setFont(CFontRef(kNormalFontSmall));
	delayEdit->setBackColor(CColor(0, 0, 0, 0));
	delayEdit->setFrameColor(CColor(0, 0, 0, 0));
	delayView->addView(delayLabel);
	delayView->addView(delayKnob);
	delayView->addView(delayEdit);
	allpassView->addView(delayView);

	CViewContainer* decayView = new CViewContainer(CRect(0, 0, 50, 80));
	decayView->setBackgroundColor(CColor(55, 55, 55, 255));
	CTextLabel* decayLabel = new CTextLabel(CRect(CPoint(0, 0), CPoint(decayView->getWidth(), 15)), "Decay");
	decayLabel->setFont(CFontRef(kNormalFontSmall));
	decayLabel->setBackColor(CColor(0, 0, 0, 0));
	decayLabel->setFrameColor(CColor(0, 0, 0, 0));
	CAnimKnob* decayKnob = new CAnimKnob(CRect(CPoint(0 + (decayView->getWidth() - knobBackground->getWidth()) / 2, decayLabel->getViewSize().bottom + 3), CPoint(knobBackground->getWidth(), knobBackground->getWidth())), this, numberOfAPModules + PARAM_ALLPASSDECAY_FIRST, knobBackground->getHeight() / knobBackground->getWidth(), knobBackground->getWidth(), knobBackground);
	CTextEdit* decayEdit = new CTextEdit(CRect(CPoint(0, decayKnob->getViewSize().bottom + 3), CPoint(decayView->getWidth(), 15)), this, 67, "0.0");
	decayEdit->setFont(CFontRef(kNormalFontSmall));
	decayEdit->setBackColor(CColor(0, 0, 0, 0));
	decayEdit->setFrameColor(CColor(0, 0, 0, 0));
	decayView->addView(decayLabel);
	decayView->addView(decayKnob);
	decayView->addView(decayEdit);
	allpassView->addView(decayView);

	// Holds the output gain control
	CViewContainer* gainView = new CViewContainer(CRect(0, 0, 100, 300));
	gainView->setBackgroundColor(CColor(55, 55, 55, 255));

	// Add process views to the control view
	controlView->addView(mixerView);
	controlView->addView(equalizerView);
	controlView->addView(allpassView);
	controlView->addView(gainView);

	baseModuleView->addView(handleView);
	baseModuleView->addView(controlView);
	baseModuleView->sizeToFit();

	workspaceView->addView(baseModuleView);

	++numberOfAPModules;

}

void ReverbNetworkEditor::removeAPModule(uint16 moduleNumber) {
	// Remove the view and delete it (=> true)
	workspaceView->removeView(frame->getView(moduleNumber));
	workspaceView->setDirty();
	
}

char ReverbNetworkEditor::controlModifierClicked(CControl* pControl, long button) {
	return 0;
}

//------------------------------------------------------------------------
void ReverbNetworkEditor::controlBeginEdit(CControl* pControl) {

}

//------------------------------------------------------------------------
void ReverbNetworkEditor::controlEndEdit(CControl* pControl) {

}

}} // namespaces