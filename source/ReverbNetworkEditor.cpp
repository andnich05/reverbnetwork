#include "ReverbNetworkEditor.h"
#include "ReverbNetworkDefines.h"
#include "GuiBaseAPModule.h"

#include "GuiHandleView.h"

namespace Steinberg {
namespace Vst {

	// GUI id mapping
	const int32_t id_addModule = 0;
	const int32_t id_removeModule = 1;
	const int32_t id_apDelayFirst = 2;
	const int32_t id_apDelayLast = id_apDelayFirst + MAXMODULENUMBER - 1;
	const int32_t id_apDecayFirst = id_apDelayLast + 1;
	const int32_t id_apDecayLast = id_apDecayFirst + MAXMODULENUMBER - 1;

ReverbNetworkEditor::ReverbNetworkEditor(void* controller)
: VSTGUIEditor(controller) 
, totalNumberOfCreatedModules(0)
{

	allpassModuleIdPool.resize(MAXMODULENUMBER, false);
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
	//CTextButton* buttonRemoveModule = new CTextButton(CRect(CPoint(750, 230), CPoint(100, 20)), this, 'RmvM', "Remove Module");
	//frame->addView(buttonRemoveModule);
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

	// Delay id range
	if (pControl->getTag() >= id_apDelayFirst && pControl->getTag() <= id_apDelayLast) {
		// Calculate which allpass module is the parent
		uint32 moduleId = pControl->getTag() - id_apDelayFirst;
		// Update parameters
		controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + moduleId, pControl->getValue());
		controller->performEdit(PARAM_ALLPASSDELAY_FIRST + moduleId, pControl->getValue());
		/*FILE* pFile = fopen("E:\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(moduleId).c_str());
		fclose(pFile);*/
	}


	//int32_t tag = pControl->getTag();
	//// Delay tag id
	//if (tag >= id_apDelayFirst && tag <= id_apDelayLast) {
	//	// Set value in the controller
	//	controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + (tag - id_apDelayFirst), pControl->getValue());
	//	controller->performEdit(PARAM_ALLPASSDELAY_FIRST + (tag - id_apDelayFirst), pControl->getValue());
	//	FILE* pFile = fopen("E:\\ogVst.txt", "a");
	//	fprintf(pFile, "y(n): %s\n", std::to_string(tag - id_apDelayFirst).c_str());
	//	fclose(pFile);
	//}

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
	case 'ClsM': {	// Close button of AP module pressed
		if (pControl->isDirty()) {
			// Remove the module view, the parent-child hierarchy is: baseModuleView(handleView(closeViewButton
			for (uint16 i = 0; i < workspaceView->getNbViews(); ++i) {
				if (workspaceView->getView(i) == pControl->getParentView()->getParentView()) {
					allpassModuleIdPool[dynamic_cast<GuiBaseAPModule*>(workspaceView->getView(i))->getModuleId()] = false;
					workspaceView->removeView(pControl->getParentView()->getParentView());
					break;
				}
			}

			//workspaceView->removeView(pControl->getParentView()->getParentView());
			// Update the workspace view
			workspaceView->setDirty();
		}
		break;
	}
	//case 'kDel': {
	//	if (pControl->isDirty()) {
	//		/*for (uint16 i = 0; i < workspaceView->getNbViews(); ++i) {
	//			if (workspaceView->getView(i) == pControl->getParentView()->getParentView()) {
	//				controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + i, pControl->getValue());
	//				controller->performEdit(PARAM_ALLPASSDELAY_FIRST + i, pControl->getValue());
	//				break;
	//			}
	//		}*/


	//		uint32 moduleId = dynamic_cast<GuiBaseAPModule*>(pControl->getParentView()->getParentView()->getParentView()->getParentView())->getModuleId();
	//		//uint32 moduleId = pControl->getTag();
	//		controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + moduleId, pControl->getValue());
	//		controller->performEdit(PARAM_ALLPASSDELAY_FIRST + moduleId, pControl->getValue());
	//		FILE* pFile = fopen("E:\\logVst.txt", "a");
	//		fprintf(pFile, "y(n): %s\n", std::to_string(moduleId).c_str());
	//		fclose(pFile);
	//	}
	//	break;
	//}
	}

}

void ReverbNetworkEditor::createAPModule() {

	if (workspaceView->getNbViews() >= MAXMODULENUMBER || workspaceView->getNbViews() > 999) {
		return;
	}

	// Handle view to grab and move the module with the mouse
	CViewContainer* handleView = new CViewContainer(CRect(0, 0, 400, 20));
	handleView->setBackgroundColor(CColor(0, 0, 0, 0));
	// Button um View "einzuklappen" => Platz sparen

	CTextButton* closeViewButton = new CTextButton(CRect(CPoint(handleView->getWidth() - 20, handleView->getHeight() / 2 - 8), CPoint(16, 16)), this, 'ClsM', "X");
	handleView->addView(closeViewButton);

	CTextEdit* moduleTitle = new CTextEdit(CRect(CPoint(4, handleView->getHeight() / 2 - 8), CPoint(handleView->getWidth() - 30, 16)), this, -1);
	moduleTitle->setBackColor(CColor(0, 0, 0, 0));
	moduleTitle->setFrameColor(CColor(0, 0, 0, 0));
	handleView->addView(moduleTitle);

	//CRowColumnView* baseModuleView = new CRowColumnView(CRect(0, 0, 300, 250), CRowColumnView::kRowStyle);
	CRect handleViewSize = handleView->getViewSize();
	handleViewSize.setWidth(handleViewSize.getWidth() - (closeViewButton->getWidth() + 8));
	// Id in order to identify the module in the valueChanged() function
	uint32 moduleId = 0;
	// Find the lowest id number which is not already taken
	for (uint32 i = 0; i < allpassModuleIdPool.size(); ++i) {
		// False means it's not taken yet
		if (allpassModuleIdPool[i] == false) {
			moduleId = i;
			allpassModuleIdPool[i] = true;
			break;
		}
	}
	GuiBaseAPModule* baseModuleView = new GuiBaseAPModule(CRect(CPoint(0 + (totalNumberOfCreatedModules % 10) * 30, 0 + (totalNumberOfCreatedModules % 10) * 30), CPoint(0, 0)), handleViewSize, moduleId);
	baseModuleView->setBackgroundColor(CColor(55, 55, 55, 255));

	std::string temp = "Allpass Module ";
	temp.append(std::to_string(moduleId));
	moduleTitle->setText(temp.c_str());

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
	
	//uint16 idOffset = 0;
	//bool pushBack = true;
	// Check if a view has been removed, if so the new created module should take the removed one's place and also take his ids for the gui components
	// E.g. module0, module1, module2 => remove(module1) => module0, module2 => createModule() => module0, module1, module2
	//for (uint16 i = 0; i < allpassModules.size(); ++i) {
	//	// false means there was already a module at this vector index position
	//	if (allpassModules[i] == false) {
	//		allpassModules[i] = true;
	//		idOffset = i;
	//		pushBack = false;
	//		break;
	//	}
	//}

	// If the offset is still zero then there were no views removed
	//if (pushBack) {
	//	// In this case add another view to the list
	//	allpassModules.push_back(true);
	//	// and set the id offset
	//	idOffset = allpassModules.size() - 1;
	//}

	allpassView->addView(createKnobGroup("Delay", id_apDelayFirst + moduleId, 'eDel'));
	allpassView->addView(createKnobGroup("Decay", 'KDec', 'eDec'));

	/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(id_apDelayFirst + idOffset).c_str());
	fclose(pFile);*/

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

	//dynamic_cast<GuiBaseAPModule*>(controlView->getParentView())->getModuleId();

	++totalNumberOfCreatedModules;
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
	// the view container fills gaps out by moving the views' index e.g.  workspaceView[0][1][2] => removeView(1) => workspaceView[0][1] (2 is now 1) => like a std::vector
	workspaceView->removeView(workspaceView->getView(moduleNumber), true);
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