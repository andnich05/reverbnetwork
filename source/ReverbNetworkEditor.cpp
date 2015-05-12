#include "ReverbNetworkEditor.h"
#include "ReverbNetworkDefines.h"
#include "GuiBaseAPModule.h"

#include "GuiHandleView.h"
#include "ValueConversion.h"

namespace Steinberg {
namespace Vst {

	// Title bar GUI ids (identification through dynamic_cast)
	const int32_t id_addModule = 0;
	const int32_t id_hideModule = id_addModule + 1;
	const int32_t id_removeModule = id_hideModule + 1;

	// Mixer GUI ids
	const int32_t id_mixer_optionMenu_inputSelectFirst = id_removeModule + 1;
	const int32_t id_mixer_optionMenu_inputSelectLast = id_mixer_optionMenu_inputSelectFirst + MAXMODULENUMBER*MAXMODULEINPUTS - 1;
	const int32_t id_mixer_knob_gainFirst = id_mixer_optionMenu_inputSelectLast + 1;
	const int32_t id_mixer_knob_gainLast = id_mixer_knob_gainFirst + MAXMODULENUMBER*MAXMODULEINPUTS - 1;
	const int32_t id_mixer_textEdit_gainFirst = id_mixer_knob_gainLast + 1;
	const int32_t id_mixer_textEdit_gainLast = id_mixer_textEdit_gainFirst + MAXMODULENUMBER*MAXMODULEINPUTS - 1;
	const int32_t id_mixer_switch_bypassFirst = id_mixer_textEdit_gainLast + 1;
	const int32_t id_mixer_switch_bypassLast = id_mixer_switch_bypassFirst + MAXMODULENUMBER - 1;

	// Equalizer GUI ids
	const int32_t id_equalizer_optionMenu_filterTypeFirst = id_mixer_switch_bypassLast + 1;
	const int32_t id_equalizer_optionMenu_filterTypeLast = id_equalizer_optionMenu_filterTypeFirst + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_knob_centerFreqFirst = id_equalizer_optionMenu_filterTypeLast + 1;
	const int32_t id_equalizer_knob_centerFreqLast = id_equalizer_knob_centerFreqFirst + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_textEdit_centerFreqFirst = id_equalizer_knob_centerFreqLast + 1;
	const int32_t id_equalizer_textEdit_centerFreqLast = id_equalizer_textEdit_centerFreqFirst + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_knob_qFactorFirst = id_equalizer_textEdit_centerFreqLast + 1;
	const int32_t id_equalizer_knob_qFactorLast = id_equalizer_knob_qFactorFirst + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_textEdit_qFactorFirst = id_equalizer_knob_qFactorLast + 1;
	const int32_t id_equalizer_textEdit_qFactorLast = id_equalizer_textEdit_qFactorFirst + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_knob_gainFirst = id_equalizer_textEdit_qFactorLast + 1;
	const int32_t id_equalizer_knob_gainLast = id_equalizer_knob_gainFirst + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_textEdit_gainFirst = id_equalizer_knob_gainLast + 1;
	const int32_t id_equalizer_textEdit_gainLast = id_equalizer_textEdit_gainFirst + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_switch_bypassFirst = id_equalizer_textEdit_gainLast + 1;
	const int32_t id_equalizer_switch_bypassLast = id_equalizer_switch_bypassFirst + MAXMODULENUMBER - 1;

	// Schroeder allpass GUI ids
	const int32_t id_allpass_knob_delayFirst = id_equalizer_switch_bypassLast + 1;
	const int32_t id_allpass_knob_delayLast = id_allpass_knob_delayFirst + MAXMODULENUMBER - 1;
	const int32_t id_allpass_textEdit_delayFirst = id_allpass_knob_delayLast + 1;
	const int32_t id_allpass_textEdit_delayLast = id_allpass_textEdit_delayFirst + MAXMODULENUMBER - 1;
	const int32_t id_allpass_knob_decayFirst = id_allpass_textEdit_delayLast + 1;
	const int32_t id_allpass_knob_decayLast = id_allpass_knob_decayFirst + MAXMODULENUMBER - 1;
	const int32_t id_allpass_textEdit_decayFirst = id_allpass_knob_decayLast + 1;
	const int32_t id_allpass_textEdit_decayLast = id_allpass_textEdit_decayFirst + MAXMODULENUMBER - 1;
	const int32_t id_allpass_switch_bypassFirst = id_allpass_textEdit_decayLast + 1;
	const int32_t id_allpass_switch_bypassLast = id_allpass_switch_bypassFirst + MAXMODULENUMBER - 1;

	// Output gain GUI ids
	const int32_t id_output_knob_gainFirst = id_allpass_switch_bypassLast + 1;
	const int32_t id_output_knob_gainLast = id_output_knob_gainFirst + MAXMODULENUMBER - 1;
	const int32_t id_output_textEdit_gainFirst = id_output_knob_gainLast + 1;
	const int32_t id_output_textEdit_gainLast = id_output_textEdit_gainFirst + MAXMODULENUMBER - 1;
	const int32_t id_output_switch_bypassFirst = id_output_textEdit_gainLast + 1;
	const int32_t id_output_switch_bypassLast = id_output_switch_bypassFirst + MAXMODULENUMBER - 1;

	//------

	// VST output select
	const int32_t id_general_optionMenu_vstOutputFirst = id_output_switch_bypassLast + 1;
	const int32_t id_general_optionMenu_vstOutputLast = id_general_optionMenu_vstOutputFirst + MAXVSTOUTPUTS - 1;



	// Last used id of a GUI element!
	const int32_t id_last = id_general_optionMenu_vstOutputLast;

//bool(ReverbNetworkEditor::*ptrTextEditStringToValueConversion) (UTF8StringPtr text, float& result, void* data);

ReverbNetworkEditor::ReverbNetworkEditor(void* controller)
: VSTGUIEditor(controller) 
, totalNumberOfCreatedModules(0)
{

	allpassModuleIdPool.resize(MAXMODULENUMBER, false);
	//ptrTextEditStringToValueConversion = &textEditStringToValueConversion;
}

ReverbNetworkEditor::~ReverbNetworkEditor() {

}

void ReverbNetworkEditor::addGuiElementPointer(CControl* guiElement, const int32_t& guiId) {
	uint32 vectorSize = guiElements.size();
	// Check for out of bounds
	if (guiId < vectorSize) {
		guiElements[guiId] = guiElement;
	}
	// If false: pushBack elements until the guiId is a valid index
	else {
		for (uint32 i = 0; i < (guiId - vectorSize + 1); ++i) {
			guiElements.push_back(nullptr);
		}
		guiElements[guiId] = guiElement;
	}
}

bool PLUGIN_API ReverbNetworkEditor::open(void* parent, const PlatformType& platformType) {
	
	if (frame) // already attached!
	{
		return false;
	}

	// Initialize the vector
	//guiElements.resize(id_last, nullptr);

	knobBackground = new CBitmap("knob.png");
	knobBackgroundSmall = new CBitmap("knob2.png");
	groupFrame = new CBitmap("groupframe.png");

	CRect editorSize(0, 0, 1000, 700);
	frame = new CFrame(editorSize, parent, this);
	frame->setBackgroundColor(CColor(110, 110, 110, 255));

	workspaceView = new CScrollView(CRect(0, 0, 700, 700), CRect(0, 0, 700, 700), CScrollView::kHorizontalScrollbar | CScrollView::kVerticalScrollbar, 16.0);
	workspaceView->setBackgroundColor(CColor(80, 80, 80, 255));

	CTextButton* buttonAddModule = new CTextButton(CRect(CPoint(750, 200), CPoint(100, 20)), this, 'AddM', "Add Module");
	frame->addView(buttonAddModule);
	//CTextButton* buttonRemoveModule = new CTextButton(CRect(CPoint(750, 230), CPoint(100, 20)), this, 'RmvM', "Remove Module");
	//frame->addView(buttonRemoveModule);

	CRowColumnView* vstOuputSelect = new CRowColumnView(CRect(CPoint(800, 0), CPoint(0, 0)));
	std::string temp = "";
	for (uint32 i = 0; i < MAXVSTOUTPUTS; ++i) {
		COptionMenu* menu = new COptionMenu(CRect(CPoint(0, 0), CPoint(100, 20)), this, id_general_optionMenu_vstOutputFirst + i);
		addGuiElementPointer(menu, id_general_optionMenu_vstOutputFirst + i);
		menu->addEntry("<Not Connected>");
		for (uint32 j = 0; j < MAXMODULENUMBER; ++j) {
			temp = "APM";
			temp.append(std::to_string(j));
			temp.append(" OUT");
			menu->addEntry((temp).c_str());
		}
		for (uint32 j = 0; j < MAXVSTINPUTS; ++j) {
			temp = "VST";
			temp.append(std::to_string(j));
			temp.append(" IN");
			menu->addEntry((temp).c_str());
		}
		vstOuputSelect->addView(menu);
	}
	vstOuputSelect->sizeToFit();

	frame->addView(workspaceView);
	frame->addView(vstOuputSelect);
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
	int32_t tag = pControl->getTag();
	
	if(tag == 'AddM') {
		// Make sure create function is called only one time (without it would be two times)
		if (pControl->isDirty()) {
			createAPModule();

			/*FILE* pFile = fopen("E:\\logVst.txt", "a");
			fprintf(pFile, "y(n): %s\n", "Create");
			fclose(pFile);*/
		}
	}
	else if (tag == 'HidM') {
		// Make sure create function is called only one time (without it it would be two times)
		if (pControl->isDirty()) {
			// Hide...
		}
	}
	else if (tag == 'ClsM') {	// Close button of AP module pressed
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
	}
	// Mixer
	else if (tag >= id_mixer_optionMenu_inputSelectFirst && tag <= id_mixer_optionMenu_inputSelectLast) {
		// Update the VST parameter variable
		controller->setParamNormalized(PARAM_MIXERINPUTSELECT_FIRST + (tag - id_mixer_optionMenu_inputSelectFirst), pControl->getValue());
		// Update the module member variable (is called in process())
		controller->performEdit(PARAM_MIXERINPUTSELECT_FIRST + (tag - id_mixer_optionMenu_inputSelectFirst), pControl->getValue());
	}
	else if (tag >= id_mixer_knob_gainFirst && tag <= id_mixer_knob_gainLast)  {
		controller->setParamNormalized(PARAM_MIXERGAIN_FIRST + (tag - id_mixer_knob_gainFirst), pControl->getValue());
		controller->performEdit(PARAM_MIXERGAIN_FIRST + (tag - id_mixer_knob_gainFirst), pControl->getValue());
		// Update the text edit box under the knob
		guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_knob_gainFirst)]->setValue(pControl->getValue());
		guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_knob_gainFirst)]->invalid();
	}
	else if (tag >= id_mixer_textEdit_gainFirst && tag <= id_mixer_textEdit_gainLast)  {
		controller->setParamNormalized(PARAM_MIXERGAIN_FIRST + (tag - id_mixer_textEdit_gainFirst), pControl->getValue());
		controller->performEdit(PARAM_MIXERGAIN_FIRST + (tag - id_mixer_textEdit_gainFirst), pControl->getValue());
		// Update the knob above the textEdit
		guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_textEdit_gainFirst)]->setValue(pControl->getValue());
		guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_textEdit_gainFirst)]->setDirty();
	}
	else if (tag >= id_mixer_switch_bypassFirst && tag <= id_mixer_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_MIXERBYPASS_FIRST + (tag - id_mixer_switch_bypassFirst), pControl->getValue());
		controller->performEdit(PARAM_MIXERBYPASS_FIRST + (tag - id_mixer_switch_bypassFirst), pControl->getValue());
	}
	// Equalizer
	else if (tag >= id_equalizer_optionMenu_filterTypeFirst && tag <= id_equalizer_optionMenu_filterTypeLast)  {
		controller->setParamNormalized(PARAM_EQFILTERTYPE_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), pControl->getValue());
		controller->performEdit(PARAM_EQFILTERTYPE_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), pControl->getValue());
	}
	else if (tag >= id_equalizer_knob_centerFreqFirst && tag <= id_equalizer_knob_centerFreqLast)  {
		controller->setParamNormalized(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_knob_centerFreqFirst), pControl->getValue());
		controller->performEdit(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_knob_centerFreqFirst), pControl->getValue());
		guiElements[id_equalizer_textEdit_centerFreqFirst + (tag - id_equalizer_knob_centerFreqFirst)]->setValue(pControl->getValue());
		guiElements[id_equalizer_textEdit_centerFreqFirst + (tag - id_equalizer_knob_centerFreqFirst)]->invalid();
	}
	else if (tag >= id_equalizer_textEdit_centerFreqFirst && tag <= id_equalizer_textEdit_centerFreqLast)  {
		controller->setParamNormalized(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_textEdit_centerFreqFirst), pControl->getValue());
		controller->performEdit(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_textEdit_centerFreqFirst), pControl->getValue());
		guiElements[id_equalizer_knob_centerFreqFirst + (tag - id_equalizer_textEdit_centerFreqFirst)]->setValue(pControl->getValue());
		guiElements[id_equalizer_knob_centerFreqFirst + (tag - id_equalizer_textEdit_centerFreqFirst)]->setDirty();
	}
	else if (tag >= id_equalizer_knob_qFactorFirst && tag <= id_equalizer_knob_qFactorLast)  {
		controller->setParamNormalized(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_knob_qFactorFirst), pControl->getValue());
		controller->performEdit(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_knob_qFactorFirst), pControl->getValue());
		guiElements[id_equalizer_textEdit_qFactorFirst + (tag - id_equalizer_knob_qFactorFirst)]->setValue(pControl->getValue());
		guiElements[id_equalizer_textEdit_qFactorFirst + (tag - id_equalizer_knob_qFactorFirst)]->invalid();
	}
	else if (tag >= id_equalizer_textEdit_qFactorFirst && tag <= id_equalizer_textEdit_qFactorLast)  {
		controller->setParamNormalized(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_textEdit_qFactorFirst), pControl->getValue());
		controller->performEdit(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_textEdit_qFactorFirst), pControl->getValue());
		guiElements[id_equalizer_knob_qFactorFirst + (tag - id_equalizer_textEdit_qFactorFirst)]->setValue(pControl->getValue());
		guiElements[id_equalizer_knob_qFactorFirst + (tag - id_equalizer_textEdit_qFactorFirst)]->setDirty();
	}
	else if (tag >= id_equalizer_knob_gainFirst && tag <= id_equalizer_knob_gainLast)  {
		controller->setParamNormalized(PARAM_EQGAIN_FIRST + (tag - id_equalizer_knob_gainFirst), pControl->getValue());
		controller->performEdit(PARAM_EQGAIN_FIRST + (tag - id_equalizer_knob_gainFirst), pControl->getValue());
		guiElements[id_equalizer_textEdit_gainFirst + (tag - id_equalizer_knob_gainFirst)]->setValue(pControl->getValue());
		guiElements[id_equalizer_textEdit_gainFirst + (tag - id_equalizer_knob_gainFirst)]->invalid();
	}
	else if (tag >= id_equalizer_textEdit_gainFirst && tag <= id_equalizer_textEdit_gainLast)  {
		controller->setParamNormalized(PARAM_EQGAIN_FIRST + (tag - id_equalizer_textEdit_gainFirst), pControl->getValue());
		controller->performEdit(PARAM_EQGAIN_FIRST + (tag - id_equalizer_textEdit_gainFirst), pControl->getValue());
		guiElements[id_equalizer_knob_gainFirst + (tag - id_equalizer_textEdit_gainFirst)]->setValue(pControl->getValue());
		guiElements[id_equalizer_knob_gainFirst + (tag - id_equalizer_textEdit_gainFirst)]->setDirty();
	}
	else if (tag >= id_equalizer_switch_bypassFirst && tag <= id_equalizer_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_EQBYPASS_FIRST + (tag - id_equalizer_switch_bypassFirst), pControl->getValue());
		controller->performEdit(PARAM_EQBYPASS_FIRST + (tag - id_equalizer_switch_bypassFirst), pControl->getValue());
	}
	// Allpass
	else if (tag >= id_allpass_knob_delayFirst && tag <= id_allpass_knob_delayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_knob_delayFirst), pControl->getValue());
		controller->performEdit(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_knob_delayFirst), pControl->getValue());
		guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_knob_delayFirst)]->setValue(pControl->getValue());
		guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_knob_delayFirst)]->invalid();
	}
	else if (tag >= id_allpass_textEdit_delayFirst && tag <= id_allpass_textEdit_delayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_textEdit_delayFirst), pControl->getValue());
		controller->performEdit(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_textEdit_delayFirst), pControl->getValue());
		guiElements[id_allpass_knob_delayFirst + (tag - id_allpass_textEdit_delayFirst)]->setValue(pControl->getValue());
		guiElements[id_allpass_knob_delayFirst + (tag - id_allpass_textEdit_delayFirst)]->setDirty();
	}
	else if (tag >= id_allpass_knob_decayFirst && tag <= id_allpass_knob_decayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_knob_decayFirst), pControl->getValue());
		controller->performEdit(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_knob_decayFirst), pControl->getValue());
		guiElements[id_allpass_textEdit_decayFirst + (tag - id_allpass_knob_decayFirst)]->setValue(pControl->getValue());
		guiElements[id_allpass_textEdit_decayFirst + (tag - id_allpass_knob_decayFirst)]->invalid();
	}
	else if (tag >= id_allpass_textEdit_decayFirst && tag <= id_allpass_textEdit_decayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_textEdit_decayFirst), pControl->getValue());
		controller->performEdit(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_textEdit_decayFirst), pControl->getValue());
		guiElements[id_allpass_knob_decayFirst + (tag - id_allpass_textEdit_decayFirst)]->setValue(pControl->getValue());
		guiElements[id_allpass_knob_decayFirst + (tag - id_allpass_textEdit_decayFirst)]->setDirty();
	}
	else if (tag >= id_allpass_switch_bypassFirst && tag <= id_allpass_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_ALLPASSBYPASS_FIRST + (tag - id_allpass_switch_bypassFirst), pControl->getValue());
		controller->performEdit(PARAM_ALLPASSBYPASS_FIRST + (tag - id_allpass_switch_bypassFirst), pControl->getValue());
	}
	// Output
	else if (tag >= id_output_knob_gainFirst && tag <= id_output_knob_gainLast)  {
		controller->setParamNormalized(PARAM_OUTGAIN_FIRST + (tag - id_output_knob_gainFirst), pControl->getValue());
		controller->performEdit(PARAM_OUTGAIN_FIRST + (tag - id_output_knob_gainFirst), pControl->getValue());
		guiElements[id_output_textEdit_gainFirst + (tag - id_output_knob_gainFirst)]->setValue(pControl->getValue());
		guiElements[id_output_textEdit_gainFirst + (tag - id_output_knob_gainFirst)]->invalid();
	}
	else if (tag >= id_output_textEdit_gainFirst && tag <= id_output_textEdit_gainLast)  {
		controller->setParamNormalized(PARAM_OUTGAIN_FIRST + (tag - id_output_textEdit_gainFirst), pControl->getValue());
		controller->performEdit(PARAM_OUTGAIN_FIRST + (tag - id_output_textEdit_gainFirst), pControl->getValue());
		guiElements[id_output_knob_gainFirst + (tag - id_output_textEdit_gainFirst)]->setValue(pControl->getValue());
		guiElements[id_output_knob_gainFirst + (tag - id_output_textEdit_gainFirst)]->setDirty();
	}
	else if (tag >= id_output_switch_bypassFirst && tag <= id_output_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_OUTBYPASS_FIRST + (tag - id_output_switch_bypassFirst), pControl->getValue());
		controller->performEdit(PARAM_OUTBYPASS_FIRST + (tag - id_output_switch_bypassFirst), pControl->getValue());
	}
	// General
	else if (tag >= id_general_optionMenu_vstOutputFirst && tag <= id_general_optionMenu_vstOutputLast) {
		controller->setParamNormalized(PARAM_GENERALVSTOUTPUTSELECT_FIRST + (tag - id_general_optionMenu_vstOutputFirst), pControl->getValue());
		controller->performEdit(PARAM_GENERALVSTOUTPUTSELECT_FIRST + (tag - id_general_optionMenu_vstOutputFirst), pControl->getValue());
	}
}

void ReverbNetworkEditor::createAPModule() {

	if (workspaceView->getNbViews() >= MAXMODULENUMBER || workspaceView->getNbViews() > 999) {
		return;
	}

	// Handle view to grab and move the module with the mouse
	CViewContainer* handleView = new CViewContainer(CRect(0, 0, 500, 20));
	handleView->setBackgroundColor(CColor(0, 0, 0, 0));
	// Button um View "einzuklappen" => Platz sparen

	CTextButton* closeViewButton = new CTextButton(CRect(CPoint(handleView->getWidth() - 20, handleView->getHeight() / 2 - 8), CPoint(16, 16)), this, 'ClsM', "X");
	handleView->addView(closeViewButton);

	CTextEdit* moduleTitle = new CTextEdit(CRect(CPoint(4, handleView->getHeight() / 2 - 8), CPoint(handleView->getWidth() - 30, 16)), this, -1);
	moduleTitle->setBackColor(CColor(0, 0, 0, 0));
	moduleTitle->setFrameColor(CColor(0, 0, 0, 0));
	handleView->addView(moduleTitle);

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
	CRowColumnView* controlView = new CRowColumnView(CRect(0, handleView->getHeight(), handleView->getWidth(), 250), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy);
	controlView->setBackgroundColor(CColor(0, 0, 0, 0));
	//controlView->addView(knob);

	// Holds the input mixer controls (input gain for each channel)
	CRowColumnView* mixerView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(200, controlView->getHeight())), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	mixerView->setBackgroundColor(CColor(0, 0, 0, 0));
	mixerView->addView(createGroupTitle("MIXER", mixerView->getWidth()));
	for (uint32 i = 0; i < MAXMODULEINPUTS; ++i) {
		temp = "IN ";
		temp.append(std::to_string(i));
		temp.append(":");
		mixerView->addView(createMixerRow(temp.c_str(), mixerView->getWidth(), id_mixer_optionMenu_inputSelectFirst + i + moduleId * MAXMODULEINPUTS, id_mixer_knob_gainFirst + i + moduleId * MAXMODULEINPUTS, id_mixer_textEdit_gainFirst + i + moduleId * MAXMODULEINPUTS));
	}


	// Holds the equalizer controls
	CRowColumnView* equalizerView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(140, controlView->getHeight())), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	equalizerView->setBackgroundColor(CColor(0, 0, 0, 0));
	CRowColumnView* filterTypeView = new CRowColumnView(CRect(0, 0, 0, 0), CRowColumnView::kColumnStyle);
	filterTypeView->setBackgroundColor(CColor(0, 0, 0, 0));
	CTextLabel* filterTypeTextLabel = new CTextLabel(CRect(0, 0, 70, 20), "Filter Type: ");
	filterTypeTextLabel->setFont(CFontRef(kNormalFontSmall));
	filterTypeTextLabel->setBackColor(CColor(0, 0, 0, 0));
	filterTypeTextLabel->setFrameColor(CColor(0, 0, 0, 0));
	COptionMenu* filterTypeMenu = new COptionMenu(CRect(0, 0, 70, 20), this, id_equalizer_optionMenu_filterTypeFirst + moduleId);
	addGuiElementPointer(filterTypeMenu, id_equalizer_optionMenu_filterTypeFirst + moduleId);
	filterTypeMenu->setFont(CFontRef(kNormalFontSmall));
	filterTypeMenu->addEntry("Low Pass");
	filterTypeMenu->addEntry("High Pass");
	filterTypeMenu->addEntry("Band Pass");
	filterTypeMenu->addEntry("Band Stop");
	filterTypeMenu->addEntry("Low Shelf");
	filterTypeMenu->addEntry("High Shelf");
	filterTypeMenu->setCurrent(0);
	filterTypeView->addView(filterTypeTextLabel);
	filterTypeView->addView(filterTypeMenu);
	filterTypeView->sizeToFit();
	CRowColumnView* paramFirstRow = new CRowColumnView(CRect(0, 0, 0, 0), CRowColumnView::kColumnStyle);
	paramFirstRow->setBackgroundColor(CColor(0, 0, 0, 0));
	paramFirstRow->addView(createKnobGroup("CFreq", equalizerView->getWidth() / 2, id_equalizer_knob_centerFreqFirst + moduleId, id_equalizer_textEdit_centerFreqFirst + moduleId, &ValueConversion::textEditStringToValueConversionCenterFreq, &ValueConversion::textEditValueToStringConversionCenterFreq));
	paramFirstRow->addView(createKnobGroup("QFactor", equalizerView->getWidth() / 2, id_equalizer_knob_qFactorFirst + moduleId, id_equalizer_textEdit_qFactorFirst + moduleId, &ValueConversion::textEditStringToValueConversionQFactor, &ValueConversion::textEditValueToStringConversionQFactor));
	paramFirstRow->sizeToFit();
	equalizerView->addView(createGroupTitle("EQUALIZER", equalizerView->getWidth()));
	equalizerView->addView(filterTypeView);
	equalizerView->addView(paramFirstRow);
	equalizerView->addView(createKnobGroup("Gain", equalizerView->getWidth(), id_equalizer_knob_gainFirst + moduleId, id_equalizer_textEdit_gainFirst + moduleId, &ValueConversion::textEditStringToValueConversionEqGain, &ValueConversion::textEditValueToStringConversionEqGain));


	// Holds the allpass controls (delay and decay)
	CRowColumnView* allpassView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(80, controlView->getHeight())), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	allpassView->setBackgroundColor(CColor(0, 0, 0, 0));
	allpassView->addView(createGroupTitle("ALLPASS", allpassView->getWidth()));
	allpassView->addView(createKnobGroup("Delay", allpassView->getWidth(), id_allpass_knob_delayFirst + moduleId, id_allpass_textEdit_delayFirst + moduleId, &ValueConversion::textEditStringToValueConversionDelay, &ValueConversion::textEditValueToStringConversionDelay));
	allpassView->addView(createKnobGroup("Decay", allpassView->getWidth(), id_allpass_knob_decayFirst + moduleId, id_allpass_textEdit_decayFirst + moduleId, &ValueConversion::textEditStringToValueConversionDecay, &ValueConversion::textEditValueToStringConversionDecay));

	/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(id_apDelayFirst + idOffset).c_str());
	fclose(pFile);*/

	// Holds the output gain control
	CRowColumnView* gainView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(80, controlView->getHeight())), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	gainView->addView(createGroupTitle("OUT", gainView->getWidth()));
	gainView->addView(createKnobGroup("Gain", gainView->getWidth(), id_output_knob_gainFirst + moduleId, id_output_textEdit_gainFirst + moduleId, &ValueConversion::textEditStringToValueConversionGain, &ValueConversion::textEditValueToStringConversionGain));
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

	/*mixerView->setBackground(groupFrame);
	equalizerView->setBackground(groupFrame);
	allpassView->setBackground(groupFrame);
	gainView->setBackground(groupFrame);*/

	++totalNumberOfCreatedModules;
}

CViewContainer* ReverbNetworkEditor::createKnobGroup(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& knobTag, const int32_t& valueEditTag, CTextEditStringToValueProc textEditStringToValueFunctionPtr, CParamDisplayValueToStringProc textEditValueToStringFunctionPtr) {
	CViewContainer* groupView = new CViewContainer(CRect(0, 0, width, 0));
	groupView->setBackgroundColor(CColor(0, 0, 0, 0));
	CTextLabel* groupNameLabel = new CTextLabel(CRect(CPoint(0, 0), CPoint(groupView->getWidth(), 15)), title);
	groupNameLabel->setFont(kNormalFontSmall);
	groupNameLabel->setBackColor(CColor(0, 0, 0, 0));
	groupNameLabel->setFrameColor(CColor(0, 0, 0, 0));
	CAnimKnob* knob = new CAnimKnob(CRect(CPoint(0 + (groupView->getWidth() - knobBackground->getWidth()) / 2, groupNameLabel->getViewSize().bottom + 3), CPoint(knobBackground->getWidth(), knobBackground->getWidth())), this, knobTag, knobBackground->getHeight() / knobBackground->getWidth(), knobBackground->getWidth(), knobBackground);
	addGuiElementPointer(knob, knobTag);
	CTextEdit* groupTextEdit = new CTextEdit(CRect(CPoint(0, knob->getViewSize().bottom + 3), CPoint(groupView->getWidth(), 15)), this, valueEditTag, "0.0");
	addGuiElementPointer(groupTextEdit, valueEditTag);
	groupTextEdit->setStringToValueProc(textEditStringToValueFunctionPtr);
	groupTextEdit->setValueToStringProc(textEditValueToStringFunctionPtr);
	groupTextEdit->setFont(CFontRef(kNormalFontSmall));
	groupTextEdit->setBackColor(CColor(0, 0, 0, 0));
	groupTextEdit->setFrameColor(CColor(0, 0, 0, 0));
	groupView->addView(groupNameLabel);
	groupView->addView(knob);
	groupView->addView(groupTextEdit);
	groupView->sizeToFit();
	return groupView;
}

CTextLabel* ReverbNetworkEditor::createGroupTitle(const VSTGUI::UTF8StringPtr title, const CCoord& width) {
	CTextLabel* label = new CTextLabel(CRect(CPoint(0, 0), CPoint(width, 30)), title);
	label->setFont(kNormalFontBig);
	label->setBackColor(CColor(0, 0, 0, 0));
	label->setFrameColor(CColor(0, 0, 0, 0));
	return label;
}

CRowColumnView* ReverbNetworkEditor::createMixerRow(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& optionMenuTag, const int32_t& knobTag, const int32_t& valueEditTag) {
	CRowColumnView* mixerRow = new CRowColumnView(CRect(CPoint(0, 0), CPoint(width, 20)), CRowColumnView::kColumnStyle);
	mixerRow->setSpacing(2);
	mixerRow->setBackgroundColor(CColor(0, 0, 0, 0));

	CTextLabel* inputTitle = new CTextLabel(CRect(CPoint(0, 0), CPoint(40, 20)), title);
	inputTitle->setFont(CFontRef(kNormalFontSmall));
	inputTitle->setBackColor(CColor(0, 0, 0, 0));
	inputTitle->setFrameColor(CColor(0, 0, 0, 0));

	COptionMenu* inputSelect = new COptionMenu(CRect(CPoint(0, 0), CPoint(0, 0)), this, optionMenuTag);
	addGuiElementPointer(inputSelect, optionMenuTag);
	inputSelect->setFont(CFontRef(kNormalFontSmall));
	inputSelect->setViewSize(CRect(CPoint(0, 0), CPoint(90, 20)));
	inputSelect->addEntry("<Not Connected>");
	inputSelect->setCurrent(0);
	std::string temp = "";
	for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
		temp = "APM";
		temp.append(std::to_string(i));
		temp.append(" OUT");
		inputSelect->addEntry((temp).c_str());
	}
	for (uint32 i = 0; i < MAXVSTINPUTS; ++i) {
		temp = "VST";
		temp.append(std::to_string(i));
		temp.append(" IN");
		inputSelect->addEntry((temp).c_str());
	}

	CAnimKnob* knob = new CAnimKnob(CRect(CPoint(0, 0), CPoint(knobBackgroundSmall->getWidth(), knobBackgroundSmall->getWidth())), 
		this, knobTag, knobBackgroundSmall->getHeight() / knobBackgroundSmall->getWidth(), knobBackgroundSmall->getWidth(), knobBackgroundSmall);
	addGuiElementPointer(knob, knobTag);

	CTextEdit* valueEdit = new CTextEdit(CRect(CPoint(0, 0), CPoint(40, 20)), this, valueEditTag, "0.0");
	addGuiElementPointer(valueEdit, valueEditTag);
	valueEdit->setStringToValueProc(&ValueConversion::textEditStringToValueConversionGain);
	valueEdit->setValueToStringProc(&ValueConversion::textEditValueToStringConversionGain);
	valueEdit->setFont(CFontRef(kNormalFontSmall));
	valueEdit->setBackColor(CColor(0, 0, 0, 0));
	valueEdit->setFrameColor(CColor(0, 0, 0, 0));

	mixerRow->addView(inputTitle);
	mixerRow->addView(inputSelect);
	mixerRow->addView(knob);
	mixerRow->addView(valueEdit);

	return mixerRow;
}

void ReverbNetworkEditor::removeAPModule(uint16 moduleNumber) {
	// Remove the view and delete it (=> true)
	// the view container fills gaps out by moving the views' index e.g.  workspaceView[0][1][2] => removeView(1) => workspaceView[0][1] (2 is now 1) => like a std::vector
	workspaceView->removeView(workspaceView->getView(moduleNumber), true);
	workspaceView->setDirty();
}

//bool textEditStringToValueConversion(UTF8StringPtr txt, float& result, void* userData);



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