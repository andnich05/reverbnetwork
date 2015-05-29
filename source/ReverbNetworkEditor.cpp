#include "ReverbNetworkEditor.h"
#include "ReverbNetworkDefines.h"
#include "GuiBaseAPModule.h"

//#include "GuiHandleView.h"
#include "ValueConversion.h"
#include "ReverbNetworkEnums.h"

namespace Steinberg {
namespace Vst {

	// Title bar GUI ids (identification through dynamic_cast)
	const int32_t id_collapseModule = 0;
	const int32_t id_closeModule = id_collapseModule + 1;

	// Mixer GUI ids
	const int32_t id_mixer_optionMenu_inputSelectFirst = id_closeModule + 1;
	const int32_t id_mixer_optionMenu_inputSelectLast = id_mixer_optionMenu_inputSelectFirst + MAXMODULENUMBER*MAXMODULEINPUTS - 1;
	const int32_t id_mixer_knob_gainFirst = id_mixer_optionMenu_inputSelectLast + 1;
	const int32_t id_mixer_knob_gainLast = id_mixer_knob_gainFirst + MAXMODULENUMBER*MAXMODULEINPUTS - 1;
	const int32_t id_mixer_textEdit_gainFirst = id_mixer_knob_gainLast + 1;
	const int32_t id_mixer_textEdit_gainLast = id_mixer_textEdit_gainFirst + MAXMODULENUMBER*MAXMODULEINPUTS - 1;
	const int32_t id_mixer_button_muteFirst = id_mixer_textEdit_gainLast + 1;
	const int32_t id_mixer_button_muteLast = id_mixer_button_muteFirst + MAXMODULENUMBER*MAXMODULEINPUTS - 1;
	const int32_t id_mixer_button_soloFirst = id_mixer_button_muteLast + 1;
	const int32_t id_mixer_button_soloLast = id_mixer_button_soloFirst + MAXMODULENUMBER*MAXMODULEINPUTS - 1;
	const int32_t id_mixer_switch_bypassFirst = id_mixer_button_soloLast + 1;
	const int32_t id_mixer_switch_bypassLast = id_mixer_switch_bypassFirst + MAXMODULENUMBER - 1;

	// Quantizer GUI ids
	const int32_t id_quantizer_knob_quantizationFirst = id_mixer_switch_bypassLast + 1;
	const int32_t id_quantizer_knob_quantizationLast = id_quantizer_knob_quantizationFirst + MAXMODULENUMBER - 1;
	const int32_t id_quantizer_textEdit_quantizationFirst = id_quantizer_knob_quantizationLast + 1;
	const int32_t id_quantizer_textEdit_quantizationLast = id_quantizer_textEdit_quantizationFirst + MAXMODULENUMBER - 1;
	const int32_t id_quantizer_switch_bypassFirst = id_quantizer_textEdit_quantizationLast + 1;
	const int32_t id_quantizer_switch_bypassLast = id_quantizer_switch_bypassFirst + MAXMODULENUMBER - 1;

	// Equalizer GUI ids
	const int32_t id_equalizer_optionMenu_filterTypeFirst = id_quantizer_switch_bypassLast + 1;
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
	const int32_t id_allpass_textEdit_samplesDelayFirst = id_allpass_textEdit_delayLast + 1;
	const int32_t id_allpass_textEdit_samplesDelayLast = id_allpass_textEdit_samplesDelayFirst + MAXMODULENUMBER - 1;
	const int32_t id_allpass_knob_decayFirst = id_allpass_textEdit_samplesDelayLast + 1;
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

	const int32_t id_output_ppmFirst = id_output_switch_bypassLast + 1;
	const int32_t id_output_ppmLast = id_output_ppmFirst + MAXMODULENUMBER - 1;


	const int32_t id_general_checkBox_moduleVisibleFirst = id_output_ppmLast + 1;
	const int32_t id_general_checkBox_moduleVisibleLast = id_general_checkBox_moduleVisibleFirst + MAXMODULENUMBER - 1;

	//------

	// VST output select
	const int32_t id_general_optionMenu_vstOutputFirst = id_general_checkBox_moduleVisibleLast + 1;
	const int32_t id_general_optionMenu_vstOutputLast = id_general_optionMenu_vstOutputFirst + MAXVSTOUTPUTS - 1;

ReverbNetworkEditor::ReverbNetworkEditor(void* controller)
: VSTGUIEditor(controller) 
, totalNumberOfCreatedModules(0)
{
	lastPpmValues.resize(MAXMODULENUMBER, 0.0);
	allpassModuleIdPool.resize(MAXMODULENUMBER, false);
	savedGainValues.resize(MAXINPUTS * MAXMODULENUMBER, DEF_MIXERGAIN);
	//ptrTextEditStringToValueConversion = &textEditStringToValueConversion;
	ViewRect viewRect(0, 0, 1000, 700);
	setRect(viewRect);

	// Set GUI refresh timer to 40 ms (25 Hz)
	setIdleRate(40);

	sampleRate = 0.0;
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

	sampleRate = ValueConversion::getSampleRate();

	knobBackground = new CBitmap("knob.png");
	knobBackgroundSmall = new CBitmap("knob2.png");
	ppmOff = new CBitmap("ppmOff.png");
	ppmOn = new CBitmap("ppmOn.png");

	CRect editorSize(0, 0, 1200, 700);
	frame = new CFrame(editorSize, parent, this);
	frame->setBackgroundColor(CColor(110, 110, 110, 255));

	workspaceView = new CScrollView(CRect(0, 0, 800, 600), CRect(0, 0, 1000, 1000), CScrollView::kHorizontalScrollbar | CScrollView::kVerticalScrollbar | CScrollView::kAutoHideScrollbars, 14.0);
	workspaceView->setBackgroundColor(CColor(80, 80, 80, 255));
	workspaceView->setBackgroundColorDrawStyle(CDrawStyle::kDrawFilledAndStroked);
	workspaceView->getVerticalScrollbar()->setScrollerColor(CColor(50, 50, 50, 255));
	workspaceView->getHorizontalScrollbar()->setScrollerColor(CColor(50, 50, 50, 255));

	// Create Module List
	CRowColumnView* viewModuleListMain = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kRowStyle);
	CScrollView* viewModuleScrollList = new CScrollView(CRect(CPoint(0, 0), CPoint(100, 500)), CRect(CPoint(0, 0), CPoint(0, 0)), CScrollView::kVerticalScrollbar, 10.0);
	viewModuleScrollList->getVerticalScrollbar()->setScrollerColor(CColor(50, 50, 50, 255));
	CRowColumnView* viewModuleList = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 0.0, CRect(5.0, 5.0, 5.0, 5.0));
	for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
		createAPModule();
		CRowColumnView* viewModuleRow = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle);
		std::string title = "APM";
		title.append(std::to_string(i));
		CTextLabel* labelModuleTitle = new CTextLabel(CRect(CPoint(0, 0), CPoint(45, 18)), title.c_str());
		labelModuleTitle->setFont(kNormalFontSmall);
		labelModuleTitle->setBackColor(CColor(0, 0, 0, 0));
		labelModuleTitle->setFrameColor(CColor(0, 0, 0, 0));
		labelModuleTitle->setHoriAlign(CHoriTxtAlign::kLeftText);
		CCheckBox* checkBoxShowHideModule = new CCheckBox(CRect(CPoint(0, 0), CPoint(18, 18)), this, id_general_checkBox_moduleVisibleFirst + i, "");
		addGuiElementPointer(checkBoxShowHideModule, id_general_checkBox_moduleVisibleFirst + i);
		//CTextButton* buttonCopyParameters = new CTextButton(CRect(CPoint(0, 0), CPoint(50, 20)), this, 1234, "Copy");
		//CTextButton* buttonPasteParameters = new CTextButton(CRect(CPoint(0, 0), CPoint(50, 20)), this, 1234, "Paste");
		viewModuleRow->addView(labelModuleTitle);
		viewModuleRow->addView(checkBoxShowHideModule);
		//viewModuleRow->addView(buttonCopyParameters);
		//viewModuleRow->addView(buttonPasteParameters);
		viewModuleRow->sizeToFit();
		viewModuleList->addView(viewModuleRow);
		viewModuleRow->setBackgroundColor(CColor(0, 0, 0, 0));
	}
	viewModuleScrollList->addView(viewModuleList);
	viewModuleScrollList->setContainerSize(viewModuleList->getViewSize());
	viewModuleList->sizeToFit();
	viewModuleListMain->addView(createGroupTitle("Module List:", viewModuleScrollList->getWidth()));
	viewModuleListMain->addView(viewModuleScrollList);
	viewModuleListMain->sizeToFit();
	viewModuleListMain->setBackgroundColor(CColor(0, 0, 0, 0));
	viewModuleScrollList->setBackgroundColor(CColor(50, 50, 50, 255));
	viewModuleList->setBackgroundColor(CColor(0, 0, 0, 0));

	// Create VST output selection 
	CRowColumnView* viewVstOutputSelect = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)));
	viewVstOutputSelect->addView(createGroupTitle("VST Outputs:", 170));
	std::string temp = "";
	for (uint32 i = 0; i < MAXVSTOUTPUTS; ++i) {
		CRowColumnView* viewOutputSelectRow = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy, 0.0);
		std::string title = "VST";
		title.append(std::to_string(i));
		title.append(" OUT:");
		CTextLabel* labelVstOutputTitle = new CTextLabel(CRect(CPoint(0, 0), CPoint(70, 20)), title.c_str());
		labelVstOutputTitle->setFont(kNormalFontSmall);
		labelVstOutputTitle->setBackColor(CColor(0, 0, 0, 0));
		labelVstOutputTitle->setFrameColor(CColor(0, 0, 0, 0));
		//labelVstOutputTitle->setHoriAlign(CHoriTxtAlign::kLeftText);
		viewOutputSelectRow->addView(labelVstOutputTitle);
		COptionMenu* menu = new COptionMenu(CRect(CPoint(0, 0), CPoint(100, 20)), this, id_general_optionMenu_vstOutputFirst + i);
		addGuiElementPointer(menu, id_general_optionMenu_vstOutputFirst + i);
		menu->setFont(kNormalFontSmall);
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
		menu->setFrameColor(CColor(0, 0, 0, 0));
		viewOutputSelectRow->addView(menu);
		viewOutputSelectRow->sizeToFit();
		viewOutputSelectRow->setBackgroundColor(CColor(50, 50, 50, 255));
		viewVstOutputSelect->addView(viewOutputSelectRow);
	}
	viewVstOutputSelect->setBackgroundColor(CColor(0, 0, 0, 0));
	temp = "VST Inputs: ";
	temp.append(std::to_string((int)(MAXVSTINPUTS)));
	CTextLabel* labelNumberOfVstInputs = new CTextLabel(CRect(CPoint(0, 0), CPoint(100, 20)), temp.c_str());
	labelNumberOfVstInputs->setFont(kNormalFontSmall);
	labelNumberOfVstInputs->setBackColor(CColor(0, 0, 0, 0));
	labelNumberOfVstInputs->setFrameColor(CColor(0, 0, 0, 0));
	labelNumberOfVstInputs->setHoriAlign(CHoriTxtAlign::kLeftText);
	temp = "VST Outputs: ";
	temp.append(std::to_string((int)(MAXVSTOUTPUTS)));
	CTextLabel* labelNumberOfVstOutputs = new CTextLabel(CRect(CPoint(0, 0), CPoint(100, 20)), temp.c_str());
	labelNumberOfVstOutputs->setFont(kNormalFontSmall);
	labelNumberOfVstOutputs->setBackColor(CColor(0, 0, 0, 0));
	labelNumberOfVstOutputs->setFrameColor(CColor(0, 0, 0, 0));
	labelNumberOfVstOutputs->setHoriAlign(CHoriTxtAlign::kLeftText);
	temp = "Modules: ";
	temp.append(std::to_string((int)(MAXMODULENUMBER)));
	CTextLabel* labelNumberOfModules = new CTextLabel(CRect(CPoint(0, 0), CPoint(100, 20)), temp.c_str());
	labelNumberOfModules->setFont(kNormalFontSmall);
	labelNumberOfModules->setBackColor(CColor(0, 0, 0, 0));
	labelNumberOfModules->setFrameColor(CColor(0, 0, 0, 0));
	labelNumberOfModules->setHoriAlign(CHoriTxtAlign::kLeftText);
	viewVstOutputSelect->addView(labelNumberOfVstInputs);
	viewVstOutputSelect->addView(labelNumberOfVstOutputs);
	viewVstOutputSelect->addView(labelNumberOfModules);
	viewVstOutputSelect->sizeToFit();

	CRowColumnView* mainView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy, 15.0);
	mainView->addView(workspaceView);
	mainView->addView(viewModuleListMain);
	mainView->addView(viewVstOutputSelect);
	mainView->sizeToFit();
	mainView->setBackgroundColor(CColor(0, 0, 0, 0));
	frame->addView(mainView);
	frame->sizeToFit();
	
	knobBackground->forget();
	knobBackgroundSmall->forget();
	ppmOff->forget();
	ppmOn->forget();

	updateGuiWithControllerParameters();

	return true;
}

void PLUGIN_API ReverbNetworkEditor::close() {
	if (frame)
	{
		guiElements.clear(); // Clear the GUI pointer so that VST can delete the objects (=> refCounter is 0)
		totalNumberOfCreatedModules = 0;
		allpassModuleIdPool.clear();
		allpassModuleIdPool.resize(MAXMODULENUMBER, false);
		frame->forget();	// delete frame
		frame = 0;
	}
}

void ReverbNetworkEditor::valueChanged(CControl* pControl) {
	// Get the GUI id
	int32_t tag = pControl->getTag();
	double value = (double)pControl->getValue();
	
	if (tag == id_collapseModule) {
		// Make sure create function is called only one time (without it it would be two times)
		//if (pControl->isDirty()) {
			for (uint16 i = 0; i < workspaceView->getNbViews(); ++i) {
				if (workspaceView->getView(i) == pControl->getParentView()->getParentView()) {
					//allpassModuleIdPool[dynamic_cast<GuiBaseAPModule*>(workspaceView->getView(i))->getModuleId()] = false;
					if (dynamic_cast<GuiBaseAPModule*>(workspaceView->getView(i))->isCollapsed()) {
						dynamic_cast<GuiBaseAPModule*>(workspaceView->getView(i))->collapseView(false);
					}
					else {
						dynamic_cast<GuiBaseAPModule*>(workspaceView->getView(i))->collapseView(true);
					}
					break;
				}
			}
		//}
	}
	else if (tag == id_closeModule) {	// Close button of AP module pressed
		if (pControl->isDirty()) {
			// Find the right module
			for (uint16 i = 0; i < workspaceView->getNbViews(); ++i) {
				if (workspaceView->getView(i) == pControl->getParentView()->getParentView()) {
					// Update check box in the module list
					guiElements[id_general_checkBox_moduleVisibleFirst + i]->setValue(0.f);
					// Update parameter
					valueChanged(guiElements[id_general_checkBox_moduleVisibleFirst + i]);
					break;
				}
			}

			//workspaceView->removeView(pControl->getParentView()->getParentView());
			// Update the workspace view
			//workspaceView->setDirty();
		}
	}
	// CAnimKnob accepts only normalized values (BUG?)
	// Mixer
	else if (tag >= id_mixer_optionMenu_inputSelectFirst && tag <= id_mixer_optionMenu_inputSelectLast) {
		uint16 moduleNumber = (tag - id_mixer_optionMenu_inputSelectFirst) / MAXMODULEINPUTS;	// Calculate the module number
		// Update the VST parameter variable
		controller->setParamNormalized(PARAM_MIXERINPUTSELECT_FIRST + (tag - id_mixer_optionMenu_inputSelectFirst), ValueConversion::valueToNormMixerInputSelect(value));
		// Update the module member variable (is called in process()), BUT!!!: it also influences the VST parameter (why, Steinberg, why?) so it MUST be normalized!
		controller->performEdit(PARAM_MIXERINPUTSELECT_FIRST + (tag - id_mixer_optionMenu_inputSelectFirst), ValueConversion::valueToNormMixerInputSelect(value));
		if (value == 0.0) {
			guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(ValueConversion::valueToNormInputGain(DEF_MIXERGAIN));
			guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(DEF_MIXERGAIN);
			guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(false);
			guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(false);
		}
		else {
			//uint16 moduleNumber = (pid - PARAM_MIXERGAIN_FIRST) / MAXINPUTS;	// Calculate the module number
			//uint16 moduleInput = (pid - PARAM_MIXERINPUTSELECT_FIRST) % MAXINPUTS;
			uint16 moduleNumber = (tag - id_mixer_optionMenu_inputSelectFirst) / MAXMODULEINPUTS;	// Calculate the module number
			guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(savedGainValues[moduleNumber * MAXINPUTS + (int)(value - 1.0)]);
			guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(ValueConversion::normToValueInputGain(savedGainValues[moduleNumber * MAXINPUTS + (int)(value - 1.0)]));
			guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(true);
			guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(true);
		}
	}
	else if (tag >= id_mixer_knob_gainFirst && tag <= id_mixer_knob_gainLast)  {
		uint16 moduleNumber = (tag - id_mixer_knob_gainFirst) / MAXMODULEINPUTS;	// Calculate the module number
		//uint16 inputSelected = 0;
		if (guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_knob_gainFirst)]->getValue() != 0.0) {
			// Knob value already normalized => no conversion needed
			controller->setParamNormalized(PARAM_MIXERGAIN_FIRST + moduleNumber * MAXINPUTS + (int)(guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_knob_gainFirst)]->getValue() - 1), value);
			controller->performEdit(PARAM_MIXERGAIN_FIRST + moduleNumber * MAXINPUTS + (int)(guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_knob_gainFirst)]->getValue() - 1), value);
			// Update the text edit box under the knob => convert from normalized value to real value
			savedGainValues[moduleNumber * MAXINPUTS + (int)(guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_knob_gainFirst)]->getValue() - 1)] = value;
		}
		guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_knob_gainFirst)]->setValue(ValueConversion::normToValueInputGain(value));
		guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_knob_gainFirst)]->invalid();
	}
	else if (tag >= id_mixer_textEdit_gainFirst && tag <= id_mixer_textEdit_gainLast)  {
		if (guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_knob_gainFirst)]->getValue() != 0.0) {
			uint16 moduleNumber = (tag - id_mixer_textEdit_gainFirst) / MAXMODULEINPUTS;	// Calculate the module number
			controller->setParamNormalized(PARAM_MIXERGAIN_FIRST + moduleNumber * MAXINPUTS + (int)(guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_textEdit_gainFirst)]->getValue() - 1), ValueConversion::valueToNormInputGain(value));
			controller->performEdit(PARAM_MIXERGAIN_FIRST + moduleNumber * MAXINPUTS + (int)(guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_textEdit_gainFirst)]->getValue() - 1), ValueConversion::valueToNormInputGain(value));
			// Update the knob above the textEdit
			savedGainValues[moduleNumber * MAXINPUTS + (int)(guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_textEdit_gainFirst)]->getValue() - 1)] = ValueConversion::valueToNormInputGain(value);
		}
		guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_textEdit_gainFirst)]->setValue(ValueConversion::valueToNormInputGain(value));
		guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_textEdit_gainFirst)]->setDirty();
	}
	//else if (tag >= id_mixer_button_muteFirst && tag <= id_mixer_button_muteLast)  {
	//	controller->setParamNormalized(PARAM_MIXERGAIN_FIRST + (tag - id_mixer_textEdit_gainFirst), ValueConversion::valueToNormGain(value));
	//	controller->performEdit(PARAM_MIXERGAIN_FIRST + (tag - id_mixer_textEdit_gainFirst), ValueConversion::valueToNormGain(value));
	//	// Update the knob above the textEdit
	//	guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_textEdit_gainFirst)]->setValue(ValueConversion::valueToNormGain(value));
	//	guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_textEdit_gainFirst)]->setDirty();
	//}
	else if (tag >= id_mixer_switch_bypassFirst && tag <= id_mixer_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_MIXERBYPASS_FIRST + (tag - id_mixer_switch_bypassFirst), value);
		controller->performEdit(PARAM_MIXERBYPASS_FIRST + (tag - id_mixer_switch_bypassFirst), value);
	}
	// Quantizer
	else if (tag >= id_quantizer_knob_quantizationFirst && tag <= id_quantizer_knob_quantizationLast)  {
		controller->setParamNormalized(PARAM_QUANTIZERBITDEPTH_FIRST + (tag - id_quantizer_knob_quantizationFirst), value);
		controller->performEdit(PARAM_QUANTIZERBITDEPTH_FIRST + (tag - id_quantizer_knob_quantizationFirst), value);
		guiElements[id_quantizer_textEdit_quantizationFirst + (tag - id_quantizer_knob_quantizationFirst)]->setValue(ValueConversion::normToValueQuantization(value));
		guiElements[id_quantizer_textEdit_quantizationFirst + (tag - id_quantizer_knob_quantizationFirst)]->invalid();
	}
	else if (tag >= id_quantizer_textEdit_quantizationFirst && tag <= id_quantizer_textEdit_quantizationLast)  {
		controller->setParamNormalized(PARAM_QUANTIZERBITDEPTH_FIRST + (tag - id_quantizer_textEdit_quantizationFirst), ValueConversion::valueToNormQuantization(value));
		controller->performEdit(PARAM_QUANTIZERBITDEPTH_FIRST + (tag - id_quantizer_textEdit_quantizationFirst), ValueConversion::valueToNormQuantization(value));
		guiElements[id_quantizer_knob_quantizationFirst + (tag - id_quantizer_textEdit_quantizationFirst)]->setValue(ValueConversion::valueToNormQuantization(value));
		guiElements[id_quantizer_knob_quantizationFirst + (tag - id_quantizer_textEdit_quantizationFirst)]->setDirty();
	}
	else if (tag >= id_quantizer_switch_bypassFirst && tag <= id_quantizer_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_QUANTIZERBYPASS_FIRST + (tag - id_quantizer_switch_bypassFirst), value);
		controller->performEdit(PARAM_QUANTIZERBYPASS_FIRST + (tag - id_quantizer_switch_bypassFirst), value);
	}
	// Equalizer
	else if (tag >= id_equalizer_optionMenu_filterTypeFirst && tag <= id_equalizer_optionMenu_filterTypeLast)  {
		controller->setParamNormalized(PARAM_EQFILTERTYPE_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), ValueConversion::valueToNormFilterTypeSelect(value));
		controller->performEdit(PARAM_EQFILTERTYPE_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), ValueConversion::valueToNormFilterTypeSelect(value));
	}
	else if (tag >= id_equalizer_knob_centerFreqFirst && tag <= id_equalizer_knob_centerFreqLast)  {
		controller->setParamNormalized(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_knob_centerFreqFirst), value);
		controller->performEdit(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_knob_centerFreqFirst), value);
		guiElements[id_equalizer_textEdit_centerFreqFirst + (tag - id_equalizer_knob_centerFreqFirst)]->setValue(ValueConversion::normToValueCenterFreq(value));
		guiElements[id_equalizer_textEdit_centerFreqFirst + (tag - id_equalizer_knob_centerFreqFirst)]->invalid();
	}
	else if (tag >= id_equalizer_textEdit_centerFreqFirst && tag <= id_equalizer_textEdit_centerFreqLast)  {
		controller->setParamNormalized(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_textEdit_centerFreqFirst), ValueConversion::valueToNormCenterFreq(value));
		controller->performEdit(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_textEdit_centerFreqFirst), ValueConversion::valueToNormCenterFreq(value));
		guiElements[id_equalizer_knob_centerFreqFirst + (tag - id_equalizer_textEdit_centerFreqFirst)]->setValue(ValueConversion::valueToNormCenterFreq(value));
		guiElements[id_equalizer_knob_centerFreqFirst + (tag - id_equalizer_textEdit_centerFreqFirst)]->setDirty();
	}
	else if (tag >= id_equalizer_knob_qFactorFirst && tag <= id_equalizer_knob_qFactorLast)  {
		controller->setParamNormalized(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_knob_qFactorFirst), value);
		controller->performEdit(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_knob_qFactorFirst), value);
		guiElements[id_equalizer_textEdit_qFactorFirst + (tag - id_equalizer_knob_qFactorFirst)]->setValue(ValueConversion::normToValueQFactor(value));
		guiElements[id_equalizer_textEdit_qFactorFirst + (tag - id_equalizer_knob_qFactorFirst)]->invalid();
	}
	else if (tag >= id_equalizer_textEdit_qFactorFirst && tag <= id_equalizer_textEdit_qFactorLast)  {
		controller->setParamNormalized(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_textEdit_qFactorFirst), ValueConversion::valueToNormQFactor(value));
		controller->performEdit(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_textEdit_qFactorFirst), ValueConversion::valueToNormQFactor(value));
		guiElements[id_equalizer_knob_qFactorFirst + (tag - id_equalizer_textEdit_qFactorFirst)]->setValue(ValueConversion::valueToNormQFactor(value));
		guiElements[id_equalizer_knob_qFactorFirst + (tag - id_equalizer_textEdit_qFactorFirst)]->setDirty();
	}
	else if (tag >= id_equalizer_knob_gainFirst && tag <= id_equalizer_knob_gainLast)  {
		controller->setParamNormalized(PARAM_EQGAIN_FIRST + (tag - id_equalizer_knob_gainFirst), value);
		controller->performEdit(PARAM_EQGAIN_FIRST + (tag - id_equalizer_knob_gainFirst), value);
		guiElements[id_equalizer_textEdit_gainFirst + (tag - id_equalizer_knob_gainFirst)]->setValue(ValueConversion::normToValueEqGain(value));
		guiElements[id_equalizer_textEdit_gainFirst + (tag - id_equalizer_knob_gainFirst)]->invalid();
	}
	else if (tag >= id_equalizer_textEdit_gainFirst && tag <= id_equalizer_textEdit_gainLast)  {
		controller->setParamNormalized(PARAM_EQGAIN_FIRST + (tag - id_equalizer_textEdit_gainFirst), ValueConversion::valueToNormEqGain(value));
		controller->performEdit(PARAM_EQGAIN_FIRST + (tag - id_equalizer_textEdit_gainFirst), ValueConversion::valueToNormEqGain(value));
		guiElements[id_equalizer_knob_gainFirst + (tag - id_equalizer_textEdit_gainFirst)]->setValue(ValueConversion::valueToNormEqGain(value));
		guiElements[id_equalizer_knob_gainFirst + (tag - id_equalizer_textEdit_gainFirst)]->setDirty();
	}
	else if (tag >= id_equalizer_switch_bypassFirst && tag <= id_equalizer_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_EQBYPASS_FIRST + (tag - id_equalizer_switch_bypassFirst), value);
		controller->performEdit(PARAM_EQBYPASS_FIRST + (tag - id_equalizer_switch_bypassFirst), value);
	}
	// Allpass
	else if (tag >= id_allpass_knob_delayFirst && tag <= id_allpass_knob_delayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_knob_delayFirst), value);
		controller->performEdit(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_knob_delayFirst), value);
		guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_knob_delayFirst)]->setValue(ValueConversion::normToValueDelay(value));
		guiElements[id_allpass_textEdit_samplesDelayFirst + (tag - id_allpass_knob_delayFirst)]->setValue(ValueConversion::delayMillisecondsToSamples(ValueConversion::normToValueDelay(value)));
		//guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_knob_delayFirst)]->invalid();
	}
	else if (tag >= id_allpass_textEdit_delayFirst && tag <= id_allpass_textEdit_delayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_textEdit_delayFirst), ValueConversion::valueToNormDelay(value));
		controller->performEdit(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_textEdit_delayFirst), ValueConversion::valueToNormDelay(value));
		guiElements[id_allpass_knob_delayFirst + (tag - id_allpass_textEdit_delayFirst)]->setValue(ValueConversion::valueToNormDelay(value));
		//guiElements[id_allpass_knob_delayFirst + (tag - id_allpass_textEdit_delayFirst)]->setDirty();
		guiElements[id_allpass_textEdit_samplesDelayFirst + (tag - id_allpass_textEdit_delayFirst)]->setValue(value * sampleRate / 1000);
	}
	else if (tag >= id_allpass_textEdit_samplesDelayFirst && tag <= id_allpass_textEdit_samplesDelayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_textEdit_samplesDelayFirst), ValueConversion::valueToNormDelay(ValueConversion::delaySamplesToMilliseconds(value)));
		controller->performEdit(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_textEdit_samplesDelayFirst), ValueConversion::valueToNormDelay(ValueConversion::delaySamplesToMilliseconds(value)));
		guiElements[id_allpass_knob_delayFirst + (tag - id_allpass_textEdit_samplesDelayFirst)]->setValue(ValueConversion::valueToNormDelay(ValueConversion::delaySamplesToMilliseconds(value)));
		//guiElements[id_allpass_knob_delayFirst + (tag - id_allpass_textEdit_samplesDelayFirst)]->setDirty();
		guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_textEdit_samplesDelayFirst)]->setValue(ValueConversion::delaySamplesToMilliseconds(value));
	}
	else if (tag >= id_allpass_knob_decayFirst && tag <= id_allpass_knob_decayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_knob_decayFirst), value);
		controller->performEdit(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_knob_decayFirst), value);
		guiElements[id_allpass_textEdit_decayFirst + (tag - id_allpass_knob_decayFirst)]->setValue(ValueConversion::normToValueDecay(value));
		guiElements[id_allpass_textEdit_decayFirst + (tag - id_allpass_knob_decayFirst)]->invalid();
	}
	else if (tag >= id_allpass_textEdit_decayFirst && tag <= id_allpass_textEdit_decayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_textEdit_decayFirst), ValueConversion::valueToNormDecay(value));
		controller->performEdit(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_textEdit_decayFirst), ValueConversion::valueToNormDecay(value));
		guiElements[id_allpass_knob_decayFirst + (tag - id_allpass_textEdit_decayFirst)]->setValue(ValueConversion::valueToNormDecay(value));
		guiElements[id_allpass_knob_decayFirst + (tag - id_allpass_textEdit_decayFirst)]->setDirty();
	}
	else if (tag >= id_allpass_switch_bypassFirst && tag <= id_allpass_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_ALLPASSBYPASS_FIRST + (tag - id_allpass_switch_bypassFirst), value);
		controller->performEdit(PARAM_ALLPASSBYPASS_FIRST + (tag - id_allpass_switch_bypassFirst), value);
	}
	// Output
	else if (tag >= id_output_knob_gainFirst && tag <= id_output_knob_gainLast)  {
		controller->setParamNormalized(PARAM_OUTGAIN_FIRST + (tag - id_output_knob_gainFirst), value);
		controller->performEdit(PARAM_OUTGAIN_FIRST + (tag - id_output_knob_gainFirst), value);
		guiElements[id_output_textEdit_gainFirst + (tag - id_output_knob_gainFirst)]->setValue(ValueConversion::normToValueOutputGain(value));
		guiElements[id_output_textEdit_gainFirst + (tag - id_output_knob_gainFirst)]->invalid();
	}
	else if (tag >= id_output_textEdit_gainFirst && tag <= id_output_textEdit_gainLast)  {
		controller->setParamNormalized(PARAM_OUTGAIN_FIRST + (tag - id_output_textEdit_gainFirst), ValueConversion::valueToNormOutputGain(value));
		controller->performEdit(PARAM_OUTGAIN_FIRST + (tag - id_output_textEdit_gainFirst), ValueConversion::valueToNormOutputGain(value));
		guiElements[id_output_knob_gainFirst + (tag - id_output_textEdit_gainFirst)]->setValue(ValueConversion::valueToNormOutputGain(value));
		guiElements[id_output_knob_gainFirst + (tag - id_output_textEdit_gainFirst)]->setDirty();
	}
	else if (tag >= id_output_switch_bypassFirst && tag <= id_output_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_OUTBYPASS_FIRST + (tag - id_output_switch_bypassFirst), value);
		controller->performEdit(PARAM_OUTBYPASS_FIRST + (tag - id_output_switch_bypassFirst), value);
	}
	// General
	else if (tag >= id_general_checkBox_moduleVisibleFirst && tag <= id_general_checkBox_moduleVisibleLast) {
		if (pControl->isDirty()) {
			controller->setParamNormalized(PARAM_MODULEVISIBLE_FIRST + (tag - id_general_checkBox_moduleVisibleFirst), value);
			controller->performEdit(PARAM_MODULEVISIBLE_FIRST + (tag - id_general_checkBox_moduleVisibleFirst), value);
			// Find the module with the correct id
			for (uint16 i = 0; i < workspaceView->getNbViews(); ++i) {
				if (dynamic_cast<GuiBaseAPModule*>(workspaceView->getView(i))->getModuleId() == tag - id_general_checkBox_moduleVisibleFirst) {
					workspaceView->getView(i)->setVisible((bool)(value));
					break;
				}
			}
			workspaceView->setDirty();
		}
	}
	else if (tag >= id_general_optionMenu_vstOutputFirst && tag <= id_general_optionMenu_vstOutputLast) {
		controller->setParamNormalized(PARAM_GENERALVSTOUTPUTSELECT_FIRST + (tag - id_general_optionMenu_vstOutputFirst), ValueConversion::valueToNormMixerInputSelect(value));
		controller->performEdit(PARAM_GENERALVSTOUTPUTSELECT_FIRST + (tag - id_general_optionMenu_vstOutputFirst), ValueConversion::valueToNormMixerInputSelect(value));
	}
}

void ReverbNetworkEditor::createAPModule() {

	if (workspaceView->getNbViews() >= MAXMODULENUMBER || workspaceView->getNbViews() > 999) {
		return;
	}

	// Handle view to grab and move the module with the mouse
	CViewContainer* handleView = new CViewContainer(CRect(0, 0, 650, 25));
	handleView->setBackgroundColor(CColor(0, 0, 0, 0));

	CTextButton* closeViewButton = new CTextButton(CRect(CPoint(handleView->getWidth() - 20, handleView->getHeight() / 2 - 8), CPoint(16, 16)), this, id_closeModule, "X");
	addGuiElementPointer(closeViewButton, id_closeModule);
	handleView->addView(closeViewButton);
	CTextButton* hideViewButton = new CTextButton(CRect(CPoint(handleView->getWidth() - 40, handleView->getHeight() / 2 - 8), CPoint(16, 16)), this, id_collapseModule, "^");
	addGuiElementPointer(hideViewButton, id_collapseModule);
	hideViewButton->setStyle(CTextButton::kOnOffStyle);
	handleView->addView(hideViewButton);

	CRect handleViewSize = handleView->getViewSize();
	handleViewSize.setWidth(handleViewSize.getWidth() - (closeViewButton->getWidth() + hideViewButton->getWidth() + 12));

	CTextEdit* moduleTitle = new CTextEdit(handleViewSize, this, -1);
	moduleTitle->setBackColor(CColor(0, 0, 0, 0));
	moduleTitle->setFrameColor(CColor(0, 0, 0, 0));
	handleView->addView(moduleTitle);

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
	
	GuiBaseAPModule* baseModuleView = new GuiBaseAPModule(CRect(CPoint(0 + (totalNumberOfCreatedModules % 10) * 30, 0 + (totalNumberOfCreatedModules % 10) * 30),
		CPoint(0, 0)), handleViewSize, moduleId);
	baseModuleView->setBackgroundColor(CColor(50, 50, 50, 255));

	std::string temp = "Allpass Module ";
	temp.append(std::to_string(moduleId));
	moduleTitle->setText(temp.c_str());

	// Control view which holds the individual processing modules
	CRowColumnView* controlView = new CRowColumnView(CRect(0, handleView->getHeight(), handleView->getWidth(), 300), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	controlView->setBackgroundColor(CColor(0, 0, 0, 0));
	//controlView->addView(knob);
	
	// Holds the input mixer controls (input gain for each channel)
	CRowColumnView* mixerMainView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(230, controlView->getHeight())), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	CRowColumnView* mixerView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 0.0);
	for (uint32 i = 0; i < MAXMODULEINPUTS; ++i) {
		temp = "IN ";
		temp.append(std::to_string(i));
		temp.append(":");
		mixerView->addView(createMixerRow(temp.c_str(), mixerMainView->getWidth(), i + moduleId * MAXMODULEINPUTS));
	}
	mixerView->setBackgroundColor(CColor(50, 50, 50, 255));
	mixerView->sizeToFit();
	CScrollView* mixerScrollView = new CScrollView(CRect(CPoint(0, 0), CPoint(mixerMainView->getWidth(), controlView->getHeight() - 50)), CRect(CPoint(0, 0), CPoint(0, 0)), CScrollView::kVerticalScrollbar, 10.0);
	mixerScrollView->addView(mixerView);
	mixerScrollView->setBackgroundColor(CColor(0, 0, 0, 255));
	mixerScrollView->sizeToFit();
	mixerScrollView->getVerticalScrollbar()->setScrollerColor(CColor(50, 50, 50, 255));


	if (mixerView->getHeight() < mixerScrollView->getHeight()) {
		mixerScrollView->getVerticalScrollbar()->setVisible(false);
	}
	mixerMainView->setBackgroundColor(CColor(0, 0, 0, 0));
	mixerMainView->addView(createGroupTitle("INPUT MIXER", mixerMainView->getWidth()));
	mixerMainView->addView(mixerScrollView);

	// Holds the quantizer controls
	CRowColumnView* quantizerView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(80, controlView->getHeight())), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	quantizerView->setBackgroundColor(CColor(0, 0, 0, 0));
	quantizerView->addView(createGroupTitle("QUANTIZER", quantizerView->getWidth()));
	CCheckBox* checkBoxQuantizerBypass = new CCheckBox(CRect(CPoint(0, 0), CPoint(60, 20)), this, id_quantizer_switch_bypassFirst + moduleId, "Bypass");
	addGuiElementPointer(checkBoxQuantizerBypass, id_quantizer_knob_quantizationFirst + moduleId);
	quantizerView->addView(checkBoxQuantizerBypass);
	quantizerView->addView(createKnobGroup("Quantization", quantizerView->getWidth(), id_quantizer_knob_quantizationFirst + moduleId, id_quantizer_textEdit_quantizationFirst + moduleId,
		MIN_QUANTIZERBITDEPTH, MAX_QUANTIZERBITDEPTH, 0));

	// Holds the equalizer controls
	CRowColumnView* equalizerView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(150, controlView->getHeight())), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	equalizerView->setBackgroundColor(CColor(0, 0, 0, 0));
	CRowColumnView* filterTypeView = new CRowColumnView(CRect(0, 0, 0, 0), CRowColumnView::kColumnStyle);
	filterTypeView->setBackgroundColor(CColor(0, 0, 0, 0));
	CTextLabel* filterTypeTextLabel = new CTextLabel(CRect(0, 0, 64, 20), "Filter Type:");
	filterTypeTextLabel->setFont(CFontRef(kNormalFontSmall));
	filterTypeTextLabel->setBackColor(CColor(0, 0, 0, 0));
	filterTypeTextLabel->setFrameColor(CColor(0, 0, 0, 0));
	COptionMenu* filterTypeMenu = new COptionMenu(CRect(0, 0, 85, 20), this, id_equalizer_optionMenu_filterTypeFirst + moduleId);
	addGuiElementPointer(filterTypeMenu, id_equalizer_optionMenu_filterTypeFirst + moduleId);
	filterTypeMenu->setFont(CFontRef(kNormalFontSmall));
	filterTypeMenu->addEntry("Low Pass");
	filterTypeMenu->addEntry("High Pass");
	filterTypeMenu->addEntry("Band Pass/Stop");
	filterTypeMenu->addEntry("Low Shelf");
	filterTypeMenu->addEntry("High Shelf");
	filterTypeMenu->setCurrent(0);
	filterTypeView->addView(filterTypeTextLabel);
	filterTypeView->addView(filterTypeMenu);
	filterTypeView->sizeToFit();
	CRowColumnView* paramFirstRow = new CRowColumnView(CRect(0, 0, 0, 0), CRowColumnView::kColumnStyle);
	paramFirstRow->setBackgroundColor(CColor(0, 0, 0, 0));
	paramFirstRow->addView(createKnobGroup("Frequency", equalizerView->getWidth() / 2, id_equalizer_knob_centerFreqFirst + moduleId, id_equalizer_textEdit_centerFreqFirst + moduleId, 
		MIN_EQCENTERFREQ, MAX_EQCENTERFREQ, 2));

	paramFirstRow->addView(createKnobGroup("QFactor", equalizerView->getWidth() / 2, id_equalizer_knob_qFactorFirst + moduleId, id_equalizer_textEdit_qFactorFirst + moduleId, 
		MIN_EQQFACTOR, MAX_EQQFACTOR, 2));
	paramFirstRow->sizeToFit();
	CCheckBox* checkBoxEqualizerBypass = new CCheckBox(CRect(CPoint(50, 0), CPoint(60, 20)), this, id_equalizer_switch_bypassFirst + moduleId, "Bypass");
	addGuiElementPointer(checkBoxEqualizerBypass, id_equalizer_switch_bypassFirst + moduleId);
	equalizerView->addView(createGroupTitle("EQUALIZER", equalizerView->getWidth()));
	equalizerView->addView(checkBoxEqualizerBypass);
	equalizerView->addView(filterTypeView);
	equalizerView->addView(paramFirstRow);
	equalizerView->addView(createKnobGroup("Gain", equalizerView->getWidth(), id_equalizer_knob_gainFirst + moduleId, id_equalizer_textEdit_gainFirst + moduleId, 
		MIN_EQGAIN, MAX_EQGAIN, 2));

	// Holds the allpass controls (delay and decay)
	CRowColumnView* allpassView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(80, controlView->getHeight())), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	allpassView->setBackgroundColor(CColor(0, 0, 0, 0));
	CCheckBox *checkBoxAllpassBypass = new CCheckBox(CRect(CPoint(50, 0), CPoint(60, 20)), this, id_allpass_switch_bypassFirst + moduleId, "Bypass");
	addGuiElementPointer(checkBoxAllpassBypass, id_allpass_switch_bypassFirst + moduleId);
	allpassView->addView(createGroupTitle("ALLPASS", allpassView->getWidth()));
	allpassView->addView(checkBoxAllpassBypass);
	allpassView->addView(createKnobGroup("Delay", allpassView->getWidth(), id_allpass_knob_delayFirst + moduleId, id_allpass_textEdit_delayFirst + moduleId, 
		MIN_ALLPASSDELAY, MAX_ALLPASSDELAY, 2));
	CTextEdit* textEditDelayInSample = new CTextEdit(CRect(CPoint(0.0, 0.0), CPoint(allpassView->getWidth(), 15.0)), this, id_allpass_textEdit_samplesDelayFirst + moduleId);
	addGuiElementPointer(textEditDelayInSample, id_allpass_textEdit_samplesDelayFirst + moduleId);
	textEditDelayInSample->setStringToValueProc(&ValueConversion::textEditStringToValueConversion);
	textEditDelayInSample->setValueToStringProc(&ValueConversion::textEditValueToStringConversion, nullptr);
	textEditDelayInSample->setMin(0.0);
	textEditDelayInSample->setMax(sampleRate * MAX_ALLPASSDELAY / 1000);
	textEditDelayInSample->setFont(CFontRef(kNormalFontSmall));
	textEditDelayInSample->setBackColor(CColor(0, 0, 0, 0));
	textEditDelayInSample->setFrameColor(CColor(0, 0, 0, 0));
	allpassView->addView(textEditDelayInSample);
	allpassView->addView(createKnobGroup("Decay", allpassView->getWidth(), id_allpass_knob_decayFirst + moduleId, id_allpass_textEdit_decayFirst + moduleId,
		MIN_ALLPASSDECAY, MAX_ALLPASSDECAY, 2));

	/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(id_apDelayFirst + idOffset).c_str());
	fclose(pFile);*/

	// Holds the output gain control
	CRowColumnView* gainView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(80, controlView->getHeight())), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	CCheckBox *checkBoxGainBypass = new CCheckBox(CRect(CPoint(50, 0), CPoint(60, 20)), this, id_output_switch_bypassFirst + moduleId, "Bypass");
	addGuiElementPointer(checkBoxGainBypass, id_output_switch_bypassFirst + moduleId);
	gainView->addView(createGroupTitle("OUT", gainView->getWidth()));
	gainView->addView(checkBoxGainBypass);

	CRowColumnView* knobPpmView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	knobPpmView->addView(createKnobGroup("Gain", gainView->getWidth()-20, id_output_knob_gainFirst + moduleId, id_output_textEdit_gainFirst + moduleId, 
		MIN_OUTPUTGAIN, MAX_OUTPUTGAIN, 2));
	CVuMeter* ppm = new CVuMeter(CRect(CPoint(0, 0), CPoint(5, 200)), ppmOn, ppmOff, 200);
	addGuiElementPointer(ppm, id_output_ppmFirst + moduleId);
	knobPpmView->addView(ppm);
	knobPpmView->setBackgroundColor(CColor(0, 0, 0, 0));
	knobPpmView->sizeToFit();

	gainView->addView(knobPpmView);
	gainView->setBackgroundColor(CColor(0, 0, 0, 0));

	// Add process views to the control view
	controlView->addView(mixerMainView);
	controlView->addView(quantizerView);
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

CViewContainer* ReverbNetworkEditor::createKnobGroup(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& knobTag, const int32_t& valueEditTag, 
	//const float& valueEditMinValue, const float& valueEditMaxValue, CTextEditStringToValueProc textEditStringToValueFunctionPtr, CParamDisplayValueToStringProc textEditValueToStringFunctionPtr) {
	const float& valueEditMinValue, const float& valueEditMaxValue, const int& valueEditPrecision) {

	CViewContainer* groupView = new CViewContainer(CRect(0, 0, width, 0));
	groupView->setBackgroundColor(CColor(0, 0, 0, 0));
	CTextLabel* groupNameLabel = new CTextLabel(CRect(CPoint(0, 0), CPoint(groupView->getWidth(), 15)), title);
	groupNameLabel->setFont(kNormalFontSmall);
	groupNameLabel->setBackColor(CColor(0, 0, 0, 0));
	groupNameLabel->setFrameColor(CColor(0, 0, 0, 0));
	CAnimKnob* knob = new CAnimKnob(CRect(CPoint(0 + (groupView->getWidth() - knobBackground->getWidth()) / 2, groupNameLabel->getViewSize().bottom + 3), 
		CPoint(knobBackground->getWidth(), knobBackground->getWidth())), this, knobTag, knobBackground->getHeight() / knobBackground->getWidth(), knobBackground->getWidth(), knobBackground);
	addGuiElementPointer(knob, knobTag);
	CTextEdit* groupTextEdit = new CTextEdit(CRect(CPoint(0, knob->getViewSize().bottom + 3), CPoint(groupView->getWidth(), 15)), this, valueEditTag, "0.0");
	addGuiElementPointer(groupTextEdit, valueEditTag);
	groupTextEdit->setStringToValueProc(ValueConversion::textEditStringToValueConversion);
	int* precision = new int(valueEditPrecision);
	groupTextEdit->setValueToStringProc(ValueConversion::textEditValueToStringConversion, precision);
	groupTextEdit->setMin(valueEditMinValue);
	groupTextEdit->setMax(valueEditMaxValue);
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

CRowColumnView* ReverbNetworkEditor::createMixerRow(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& idOffset) {
	CRowColumnView* mixerRow = new CRowColumnView(CRect(CPoint(0, 0), CPoint(width, 20)), CRowColumnView::kColumnStyle);
	mixerRow->setSpacing(5);
	mixerRow->setBackgroundColor(CColor(0, 0, 0, 0));

	CTextLabel* inputTitle = new CTextLabel(CRect(CPoint(0, 0), CPoint(5, 20)), "");
	inputTitle->setFont(CFontRef(kNormalFontSmall));
	inputTitle->setBackColor(CColor(0, 0, 0, 0));
	inputTitle->setFrameColor(CColor(0, 0, 0, 0));

	COptionMenu* inputSelect = new COptionMenu(CRect(CPoint(0, 0), CPoint(90, 20)), this, id_mixer_optionMenu_inputSelectFirst + idOffset);
	addGuiElementPointer(inputSelect, id_mixer_optionMenu_inputSelectFirst + idOffset);
	inputSelect->setFont(CFontRef(kNormalFontSmall));
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
	inputSelect->setFrameColor(CColor(0, 0, 0, 0));

	CAnimKnob* knob = new CAnimKnob(CRect(CPoint(0, 0), CPoint(knobBackgroundSmall->getWidth(), knobBackgroundSmall->getWidth())), 
		this, id_mixer_knob_gainFirst + idOffset, knobBackgroundSmall->getHeight() / knobBackgroundSmall->getWidth(), knobBackgroundSmall->getWidth(), knobBackgroundSmall);
	addGuiElementPointer(knob, id_mixer_knob_gainFirst + idOffset);

	CTextEdit* valueEdit = new CTextEdit(CRect(CPoint(0, 0), CPoint(40, 20)), this, id_mixer_textEdit_gainFirst + idOffset);
	addGuiElementPointer(valueEdit, id_mixer_textEdit_gainFirst + idOffset);
	valueEdit->setStringToValueProc(&ValueConversion::textEditStringToValueConversion);
	valueEdit->setValueToStringProc(&ValueConversion::textEditValueToStringConversion, nullptr);
	valueEdit->setMin(MIN_OUTPUTGAIN);
	valueEdit->setMax(MAX_OUTPUTGAIN);
	valueEdit->setFont(CFontRef(kNormalFontSmall));
	valueEdit->setBackColor(CColor(0, 0, 0, 0));
	valueEdit->setFrameColor(CColor(0, 0, 0, 0));

	CTextButton* buttonMute = new CTextButton(CRect(CPoint(0, 0), CPoint(20, 20)), this, id_mixer_button_muteFirst + idOffset, "M", CTextButton::kOnOffStyle);
	buttonMute->setFont(CFontRef(kNormalFontSmall));
	buttonMute->setTextColor(CColor(180, 0, 0, 255));
	buttonMute->setGradientStartColorHighlighted(CColor(160, 0, 0, 255));
	buttonMute->setGradientEndColorHighlighted(CColor(100, 0, 0, 255));
	
	CTextButton* buttonSolo = new CTextButton(CRect(CPoint(0, 0), CPoint(20, 20)), this, id_mixer_button_soloFirst + idOffset, "S", CTextButton::kOnOffStyle);
	buttonSolo->setFont(CFontRef(kNormalFontSmall));
	buttonSolo->setTextColor(CColor(0, 180, 0, 255));
	buttonSolo->setGradientStartColorHighlighted(CColor(0, 160, 0, 255));
	buttonSolo->setGradientEndColorHighlighted(CColor(0, 100, 0, 255));

	mixerRow->addView(inputTitle);
	mixerRow->addView(inputSelect);
	mixerRow->addView(knob);
	mixerRow->addView(valueEdit);
	mixerRow->addView(buttonMute);
	mixerRow->addView(buttonSolo);

	return mixerRow;
}

void ReverbNetworkEditor::removeAPModule(uint16 moduleNumber) {
	// Remove the view and delete it (=> true)
	// the view container fills gaps out by moving the views' index e.g.  workspaceView[0][1][2] => removeView(1) => workspaceView[0][1] (2 is now 1) => like a std::vector
	workspaceView->removeView(workspaceView->getView(moduleNumber), true);
	workspaceView->setDirty();
}

//bool textEditStringToValueConversion(UTF8StringPtr txt, float& result, void* userData);

void ReverbNetworkEditor::updateGuiWithControllerParameters() {
	//updateGuiParameter(PARAM_MIXERINPUTSELECT_FIRST, PARAM_MIXERINPUTSELECT_LAST, id_mixer_optionMenu_inputSelectFirst, &ValueConversion::normToValueMixerInputSelect);
	//updateGuiParameter(PARAM_MIXERGAIN_FIRST, PARAM_MIXERGAIN_LAST, id_mixer_knob_gainFirst, nullptr);

	for (uint32 i = PARAM_MIXERINPUTSELECT_FIRST; i < PARAM_MIXERINPUTSELECT_LAST + 1; ++i) { // Iterate over all parameters
		double menuIndex = ValueConversion::normToValueMixerInputSelect(getController()->getParamNormalized(i));
		guiElements[id_mixer_optionMenu_inputSelectFirst + (i - PARAM_MIXERINPUTSELECT_FIRST)]->setValue(menuIndex);
		if (menuIndex == 0.0) {
			guiElements[id_mixer_knob_gainFirst + (i - PARAM_MIXERINPUTSELECT_FIRST)]->setValue(ValueConversion::valueToNormInputGain(DEF_MIXERGAIN));
			guiElements[id_mixer_knob_gainFirst + (i - PARAM_MIXERINPUTSELECT_FIRST)]->setMouseEnabled(false);
			guiElements[id_mixer_textEdit_gainFirst + (i - PARAM_MIXERINPUTSELECT_FIRST)]->setMouseEnabled(false);
		}
		else {
			guiElements[id_mixer_knob_gainFirst + (i - PARAM_MIXERINPUTSELECT_FIRST)]->setValue(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + (int)(menuIndex - 1)));
			guiElements[id_mixer_knob_gainFirst + (i - PARAM_MIXERINPUTSELECT_FIRST)]->setMouseEnabled(true);
			guiElements[id_mixer_textEdit_gainFirst + (i - PARAM_MIXERINPUTSELECT_FIRST)]->setMouseEnabled(true);
		}
		valueChanged(guiElements[id_mixer_knob_gainFirst + (i - PARAM_MIXERINPUTSELECT_FIRST)]);
	}
	for (uint32 i = PARAM_MIXERGAIN_FIRST; i < PARAM_MIXERGAIN_LAST + 1; ++i) {
		savedGainValues[i - PARAM_MIXERGAIN_FIRST] = getController()->getParamNormalized(i);
		
	}

	//updateGuiParameter(PARAM_MIXERBYPASS_FIRST, PARAM_MIXERBYPASS_LAST, id_mixer_switch_bypassFirst, nullptr);
	updateGuiParameter(PARAM_QUANTIZERBITDEPTH_FIRST, PARAM_QUANTIZERBITDEPTH_LAST, id_quantizer_knob_quantizationFirst, nullptr);
	updateGuiParameter(PARAM_QUANTIZERBYPASS_FIRST, PARAM_QUANTIZERBYPASS_LAST, id_quantizer_switch_bypassFirst, nullptr);
	updateGuiParameter(PARAM_EQFILTERTYPE_FIRST, PARAM_EQFILTERTYPE_LAST, id_equalizer_optionMenu_filterTypeFirst, &ValueConversion::normToValueFilterTypeSelect);
	updateGuiParameter(PARAM_EQCENTERFREQ_FIRST, PARAM_EQCENTERFREQ_LAST, id_equalizer_knob_centerFreqFirst, nullptr);
	updateGuiParameter(PARAM_EQQFACTOR_FIRST, PARAM_EQQFACTOR_LAST, id_equalizer_knob_qFactorFirst, nullptr);
	updateGuiParameter(PARAM_EQGAIN_FIRST, PARAM_EQGAIN_LAST, id_equalizer_knob_gainFirst, nullptr);
	updateGuiParameter(PARAM_EQBYPASS_FIRST, PARAM_EQBYPASS_LAST, id_equalizer_switch_bypassFirst, nullptr);
	updateGuiParameter(PARAM_ALLPASSDELAY_FIRST, PARAM_ALLPASSDELAY_LAST, id_allpass_knob_delayFirst, nullptr);
	updateGuiParameter(PARAM_ALLPASSDECAY_FIRST, PARAM_ALLPASSDECAY_LAST, id_allpass_knob_decayFirst, nullptr);
	updateGuiParameter(PARAM_ALLPASSBYPASS_FIRST, PARAM_ALLPASSBYPASS_LAST, id_allpass_switch_bypassFirst, nullptr);
	updateGuiParameter(PARAM_OUTGAIN_FIRST, PARAM_OUTGAIN_LAST, id_output_knob_gainFirst, nullptr);
	updateGuiParameter(PARAM_OUTBYPASS_FIRST, PARAM_OUTBYPASS_LAST, id_output_switch_bypassFirst, nullptr);
	updateGuiParameter(PARAM_GENERALVSTOUTPUTSELECT_FIRST, PARAM_GENERALVSTOUTPUTSELECT_LAST, id_general_optionMenu_vstOutputFirst, &ValueConversion::normToValueMixerInputSelect);
	updateGuiParameter(PARAM_MODULEVISIBLE_FIRST, PARAM_MODULEVISIBLE_LAST, id_general_checkBox_moduleVisibleFirst, nullptr);
}

void ReverbNetworkEditor::updateGuiParameter(uint32 firstParamId, uint32 lastParamId, uint32 firstGuiId, ConversionFunction functPtr) {
	for (uint32 i = firstParamId; i < lastParamId + 1; ++i) { // Iterate over all parameters
		if (guiElements[firstGuiId + (i - firstParamId)]) { // Check if the GUI element is valid
			if (functPtr) { // Check if a value conversion is needed
				guiElements[firstGuiId + (i - firstParamId)]->setValue(functPtr(getController()->getParamNormalized(i))); // Set GUI element value
			}
			else {
				guiElements[firstGuiId + (i - firstParamId)]->setValue(getController()->getParamNormalized(i)); // Set GUI element value
			}
			valueChanged(guiElements[firstGuiId + (i - firstParamId)]); // Update the corresponding textEdit (if there exists one)
		}
	}
}

void ReverbNetworkEditor::updateEditorFromController(ParamID tag, ParamValue value)
{
	if (tag >= PARAM_PPMUPDATE_FIRST && tag <= PARAM_PPMUPDATE_LAST) {
		// Update PPM value
		lastPpmValues[tag - PARAM_PPMUPDATE_FIRST] = value;	
	}
}

CMessageResult ReverbNetworkEditor::notify(CBaseObject* sender, const char* message)
{
	if (message == CVSTGUITimer::kMsgTimer)
	{
		// GUI refresh timer, can be set with setIdleRate()
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			if (guiElements[id_output_ppmFirst + i])
			{
				guiElements[id_output_ppmFirst + i]->setValue(1.0 - ((lastPpmValues[i] - 1.0) * (lastPpmValues[i] - 1.0)));
				//lastPpmValues[i] = 0.0;
				/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
				fprintf(pFile, "y(n): %s\n", std::to_string(lastPpmValues[i]).c_str());
				fclose(pFile);*/
			}
		}
	}
	return VSTGUIEditor::notify(sender, message);
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