#include "ReverbNetworkEditor.h"
#include "ReverbNetworkDefines.h"

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
	frame->setBackgroundColor(kGreyCColor);

	workspaceView = new CScrollView(CRect(0, 0, 700, 700), CRect(0, 0, 1500, 1500), CScrollView::kHorizontalScrollbar | CScrollView::kVerticalScrollbar, 16.0);
	workspaceView->setBackgroundColor(kWhiteCColor);

	//CAnimKnob* knob = new CAnimKnob(CRect(0, 0, 42, 42), this, 0, 80, 42, knobBackground);

	//view->setBackgroundColor(kGreenCColor);
	//frame->addView(view);
	//view->addView(knob);
	
	//CBitmap* background = new CBitmap("back.svg");
	//GuiBaseModuleFrame* baseModule = new GuiBaseModuleFrame(CRect(CPoint(50, 0), CPoint(200, 200)));
	//baseModule->setBackgroundColor(CColor(200, 200, 200, 255));

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
			FILE* pFile = fopen("E:\\logVst.txt", "a");
			fprintf(pFile, "y(n): %s\n", "Create");
			fclose(pFile);
		}
		break;
	}
	case 'RmvM': {
		if (pControl->isDirty()) {
			FILE* pFile = fopen("E:\\logVst.txt", "a");
			fprintf(pFile, "y(n): %s\n", "Remove");
			fclose(pFile);
			removeAPModule(2);
		}
		break;
	}
	}
}

void ReverbNetworkEditor::createAPModule() {

	//CRowColumnView* baseModuleView = new CRowColumnView(CRect(0, 0, 300, 250), CRowColumnView::kRowStyle);
	CRowColumnView* baseModuleView = new CRowColumnView(CRect(CPoint(0 + numberOfAPModules * 20, 0 + numberOfAPModules * 20), CPoint(0, 0)), CRowColumnView::kRowStyle);
	baseModuleView->setBackgroundColor(kBlueCColor);

	// Handle view to grab and move the module with the mouse
	CViewContainer* handleView = new CViewContainer(CRect(0, 0, 400, 10));
	handleView->setBackgroundColor(CColor(200, 200, 200, 255));

	// Control view which holds the individual processing modules
	CRowColumnView* controlView = new CRowColumnView(CRect(0, 0, 400, 300), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy);
	controlView->setBackgroundColor(CColor(150, 150, 150, 255));
	//controlView->addView(knob);

	// Holds the input mixer controls (input gain for each channel)
	CViewContainer* mixerView = new CViewContainer(CRect(0, 0, 100, 300));

	// Holds the equalizer controls
	CViewContainer* equalizerView = new CViewContainer(CRect(0, 0, 100, 300));

	// Holds the allpass controls (delay and decay)
	CViewContainer* allpassView = new CViewContainer(CRect(0, 0, 100, 300));
	allpassView->setBackgroundColor(kGreyCColor);
	// ID-Tag of the main control is the same as the parameter ID

	CViewContainer* delayView = new CViewContainer(CRect(0, 0, 50, 100));
	delayView->setBackgroundColor(kGreyCColor);
	CTextLabel* delayLabel = new CTextLabel(CRect(CPoint(0, 0), CPoint(50, 20)), "Delay");
	delayLabel->setFontColor(kRedCColor);
	CAnimKnob* delayKnob = new CAnimKnob(CRect(CPoint(0, 25), CPoint(42, 42)), this, numberOfAPModules + PARAM_ALLPASSDELAY_FIRST, 80, 42, knobBackground);
	CTextEdit* delayEdit = new CTextEdit(CRect(CPoint(0, 70), CPoint(20, 20)), this, 666, "0.0");
	delayView->addView(delayLabel);
	delayView->addView(delayKnob);
	delayView->addView(delayEdit);
	allpassView->addView(delayView);

	CAnimKnob* decayKnob = new CAnimKnob(CRect(CPoint(0, 50), CPoint(42, 42)), this, numberOfAPModules + PARAM_ALLPASSDECAY_FIRST, 80, 42, knobBackground);
	//allpassView->addView(decayKnob);

	// Holds the output gain control
	CViewContainer* gainView = new CViewContainer(CRect(0, 0, 100, 300));

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