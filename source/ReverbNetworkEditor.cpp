#include "ReverbNetworkEditor.h"
#include "GuiBaseModuleFrame.h"
#include "ReverbNetworkDefines.h"
#include "GuiBaseAPModule.h"

namespace Steinberg {
namespace Vst {

ReverbNetworkEditor::ReverbNetworkEditor(void* controller)
: VSTGUIEditor(controller) {

	
}

ReverbNetworkEditor::~ReverbNetworkEditor() {

}

bool PLUGIN_API ReverbNetworkEditor::open(void* parent, const PlatformType& platformType) {
	
	if (frame) // already attached!
	{
		return false;
	}

	knobBackground = new CBitmap("knob.png");

	CRect editorSize(0, 0, 600, 500);
	frame = new CFrame(editorSize, parent, this);
	frame->setBackgroundColor(kGreyCColor);

	
	//CAnimKnob* knob = new CAnimKnob(CRect(0, 0, 42, 42), this, 0, 80, 42, knobBackground);

	//view->setBackgroundColor(kGreenCColor);
	//frame->addView(view);
	//view->addView(knob);
	
	//CBitmap* background = new CBitmap("back.svg");
	//GuiBaseModuleFrame* baseModule = new GuiBaseModuleFrame(CRect(CPoint(50, 0), CPoint(200, 200)));
	//baseModule->setBackgroundColor(CColor(200, 200, 200, 255));

	CTextButton* buttonAddModule = new CTextButton(CRect(CPoint(400, 200), CPoint(100, 50)), this, 'AddM', "Add Module");
	frame->addView(buttonAddModule);
	CTextButton* buttonRemoveModule = new CTextButton(CRect(CPoint(400, 320), CPoint(100, 50)), this, 'RmvM', "Remove Module");
	frame->addView(buttonRemoveModule);

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
		createAPModule();
		break;
	}
	case 'RmvM': {
		removeAPModule(frame->getNbViews()-1);
		break;
	}
	}
}

void ReverbNetworkEditor::createAPModule() {
	// Base view for an Allpass module which holds all other views
	
	GuiBaseAPModule* baseModuleView = new GuiBaseAPModule(CRect(0, 0, 400, 400), this);
	// Add to the main frame
	frame->addView(baseModuleView);

	// Add it to the list in order to be able to remove it
	//apGuiModules.push_back(baseModuleView);

}

void ReverbNetworkEditor::removeAPModule(uint16 moduleNumber) {
	// Remove the view and delete it (=> true)
	frame->removeView(frame->getView(moduleNumber), true);
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