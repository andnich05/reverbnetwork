#include "ReverbNetworkEditor.h"
#include "ReverbNetworkDefines.h"
#include "GuiBaseAPModule.h"

#include "GuiHandleView.h"

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

	workspaceView = new CScrollView(CRect(0, 0, 700, 700), CRect(0, 0, 700, 700), CScrollView::kHorizontalScrollbar | CScrollView::kVerticalScrollbar, 16.0);
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

	// Handle view to grab and move the module with the mouse
	CViewContainer* handleView = new CViewContainer(CRect(0, 0, 400, 20));
	handleView->setBackgroundColor(CColor(0, 0, 0, 0));

	//CRowColumnView* baseModuleView = new CRowColumnView(CRect(0, 0, 300, 250), CRowColumnView::kRowStyle);
	GuiBaseAPModule* baseModuleView = new GuiBaseAPModule(CRect(CPoint(0 + numberOfAPModules * 20, 0 + numberOfAPModules * 20), CPoint(0, 0)), handleView->getViewSize());
	baseModuleView->setBackgroundColor(CColor(55, 55, 55, 255));

	// Control view which holds the individual processing modules
	CRowColumnView* controlView = new CRowColumnView(CRect(0, handleView->getHeight(), 400, 300), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy);
	controlView->setBackgroundColor(CColor(0, 0, 0, 0));
	//controlView->addView(knob);

	// Holds the input mixer controls (input gain for each channel)
	CViewContainer* mixerView = new CViewContainer(CRect(0, 0, 100, 300));
	mixerView->setBackgroundColor(CColor(0, 0, 0, 0));

	// Holds the equalizer controls
	CViewContainer* equalizerView = new CViewContainer(CRect(0, 0, 100, 300));
	equalizerView->setBackgroundColor(CColor(0, 0, 0, 0));

	// Holds the allpass controls (delay and decay)
	CRowColumnView* allpassView = new CRowColumnView(CRect(0, 0, 100, 300), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 10.0);
	allpassView->setBackgroundColor(CColor(0, 0, 0, 0));
	// ID-Tag of the main control is the same as the parameter ID

	allpassView->addView(createKnobGroup("Delay", 0, 1));
	allpassView->addView(createKnobGroup("Decay", 2, 3));

	// Holds the output gain control
	CViewContainer* gainView = new CViewContainer(CRect(0, 0, 100, 300));
	gainView->setBackgroundColor(CColor(0, 0, 0, 0));

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

CViewContainer* ReverbNetworkEditor::createKnobGroup(const VSTGUI::UTF8StringPtr groupName, const int32_t& knobTag, const int32_t& valueEditTag) {
	CViewContainer* groupView = new CViewContainer(CRect(0, 0, 50, 0));
	// 55,55,55
	groupView->setBackgroundColor(CColor(0, 0, 0, 0));
	CTextLabel* groupNameLabel = new CTextLabel(CRect(CPoint(0, 0), CPoint(groupView->getWidth(), 15)), groupName);
	groupNameLabel->setFont(CFontRef(kNormalFontSmall));
	groupNameLabel->setBackColor(CColor(0, 0, 0, 0));
	groupNameLabel->setFrameColor(CColor(0, 0, 0, 0));
	CAnimKnob* knob = new CAnimKnob(CRect(CPoint(0 + (groupView->getWidth() - knobBackground->getWidth()) / 2, groupNameLabel->getViewSize().bottom + 3), CPoint(knobBackground->getWidth(), knobBackground->getWidth())), this, knobTag, knobBackground->getHeight() / knobBackground->getWidth(), knobBackground->getWidth(), knobBackground);
	CTextEdit* groupTextEdit = new CTextEdit(CRect(CPoint(0, knob->getViewSize().bottom + 3), CPoint(groupView->getWidth(), 15)), this, valueEditTag, "0.0");
	groupTextEdit->setFont(CFontRef(kNormalFontSmall));
	groupTextEdit->setBackColor(CColor(0, 0, 0, 0));
	groupTextEdit->setFrameColor(CColor(0, 0, 0, 0));
	groupView->addView(groupNameLabel);
	groupView->addView(knob);
	groupView->addView(groupTextEdit);
	groupView->sizeToFit();
	return groupView;
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