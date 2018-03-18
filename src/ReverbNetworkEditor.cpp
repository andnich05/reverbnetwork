/*
* ReverbNetworkEditor: Vst Plug-in editor (GUI)
*
* Copyright (C) 2015  Andrej Nichelmann
*                     Klaus Michael Indlekofer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../include/ReverbNetworkEditor.h"

#include "../include/GuiCustomRowColumnView.h"
#include "../include/GuiCustomTextLabel.h"
#include "../include/GuiOptionMenuInputSelector.h"
#include "../include/GuiBaseAPModule.h"
#include "../include/GuiCustomSplashScreen.h"
#include "../include/GuiGraphicsView.h"
#include "../include/GuiSignalGenerator.h"
#include "../include/GuiCustomValueEdit.h"
#include "../include/GuiCustomTextEdit.h"
#include "../include/GuiWorkspaceView.h"

#include "../include/ValueConversion.h"
#include "../include/ReverbNetworkDefines.h"

namespace Steinberg {
namespace Vst {

	const double guiRefreshTime = 40.0; // Refresh time of the GUI in msec
	const double ppmUpdateFactor = 20.0 / 1.7 * (guiRefreshTime / 1000.0); // -20 dB in 1.7 sec

	// Structure for overriding module parameters (when user clicks on "paste")
	struct overrideParametersQuery {
		int moduleId;
		XmlPresetReadWrite::Module *moduleStruct;
	};

	//--- GUI IDs
	// Gui elements in the titlebar of the modules
	const int32_t id_module_textEdit_titleFirst = 0;
	const int32_t id_module_textEdit_titleLast = id_module_textEdit_titleFirst + MAXMODULENUMBER - 1;
	const int32_t id_module_button_collapseFirst = id_module_textEdit_titleLast + 1;
	const int32_t id_module_button_collapseLast = id_module_button_collapseFirst + MAXMODULENUMBER - 1;
	const int32_t id_module_button_hideFirst = id_module_button_collapseLast + 1;
	const int32_t id_module_button_hideLast = id_module_button_hideFirst + MAXMODULENUMBER - 1;
	const int32_t id_module_button_copyParametersFirst = id_module_button_hideLast + 1;
	const int32_t id_module_button_copyParametersLast = id_module_button_copyParametersFirst + MAXMODULENUMBER - 1;
	const int32_t id_module_button_pasteParametersFirst = id_module_button_copyParametersLast + 1;
	const int32_t id_module_button_pasteParametersLast = id_module_button_pasteParametersFirst + MAXMODULENUMBER - 1;
	const int32_t id_module_button_defaultParametersFirst = id_module_button_pasteParametersLast + 1;
	const int32_t id_module_button_defaultParametersLast = id_module_button_defaultParametersFirst + MAXMODULENUMBER - 1;

	// Mixer GUI ids
	const int32_t id_mixer_optionMenu_inputSelectFirst = id_module_button_defaultParametersLast + 1;
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
	const int32_t id_equalizer_button_stabilityFirst = id_equalizer_optionMenu_filterTypeLast + 1;
	const int32_t id_equalizer_button_stabilityLast = id_equalizer_button_stabilityFirst + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_knob_centerFreqFirst = id_equalizer_button_stabilityLast + 1;
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
	const int32_t id_equalizer_textEdit_a0First = id_equalizer_textEdit_gainLast + 1;
	const int32_t id_equalizer_textEdit_a0Last = id_equalizer_textEdit_a0First + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_textEdit_a1First = id_equalizer_textEdit_a0Last + 1;
	const int32_t id_equalizer_textEdit_a1Last = id_equalizer_textEdit_a1First + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_textEdit_a2First = id_equalizer_textEdit_a1Last + 1;
	const int32_t id_equalizer_textEdit_a2Last = id_equalizer_textEdit_a2First + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_textEdit_b1First = id_equalizer_textEdit_a2Last + 1;
	const int32_t id_equalizer_textEdit_b1Last = id_equalizer_textEdit_b1First + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_textEdit_b2First = id_equalizer_textEdit_b1Last + 1;
	const int32_t id_equalizer_textEdit_b2Last = id_equalizer_textEdit_b2First + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_checkBox_limiterFirst = id_equalizer_textEdit_b2Last + 1;
	const int32_t id_equalizer_checkBox_limiterLast = id_equalizer_checkBox_limiterFirst + MAXMODULENUMBER - 1;
	const int32_t id_equalizer_switch_bypassFirst = id_equalizer_checkBox_limiterLast + 1;
	const int32_t id_equalizer_switch_bypassLast = id_equalizer_switch_bypassFirst + MAXMODULENUMBER - 1;

	// Allpass GUI ids
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
	const int32_t id_allpass_textEdit_diffKFirst = id_allpass_textEdit_decayLast + 1;
	const int32_t id_allpass_textEdit_diffKLast = id_allpass_textEdit_diffKFirst + MAXMODULENUMBER - 1;
	const int32_t id_allpass_optionMenu_diffKSignFirst = id_allpass_textEdit_diffKLast + 1;
	const int32_t id_allpass_optionMenu_diffKSignLast = id_allpass_optionMenu_diffKSignFirst + MAXMODULENUMBER - 1;
	const int32_t id_allpass_checkBox_modulationEnabledFirst = id_allpass_optionMenu_diffKSignLast + 1;
	const int32_t id_allpass_checkBox_modulationEnabledLast = id_allpass_checkBox_modulationEnabledFirst + MAXMODULENUMBER - 1;
	const int32_t id_allpass_optionMenu_modulationSignalTypeFirst = id_allpass_checkBox_modulationEnabledLast + 1;
	const int32_t id_allpass_optionMenu_modulationSignalTypeLast = id_allpass_optionMenu_modulationSignalTypeFirst + MAXMODULENUMBER - 1;
	const int32_t id_allpass_textEdit_modulationExcursionFirst = id_allpass_optionMenu_modulationSignalTypeLast + 1;
	const int32_t id_allpass_textEdit_modulationExcursionLast = id_allpass_textEdit_modulationExcursionFirst + MAXMODULENUMBER - 1;
	const int32_t id_allpass_textEdit_modulationRateFirst = id_allpass_textEdit_modulationExcursionLast + 1;
	const int32_t id_allpass_textEdit_modulationRateLast = id_allpass_textEdit_modulationRateFirst + MAXMODULENUMBER - 1;
	const int32_t id_allpass_switch_bypassFirst = id_allpass_textEdit_modulationRateLast + 1;
	const int32_t id_allpass_switch_bypassLast = id_allpass_switch_bypassFirst + MAXMODULENUMBER - 1;

	// Output gain GUI ids
	const int32_t id_output_knob_gainFirst = id_allpass_switch_bypassLast + 1;
	const int32_t id_output_knob_gainLast = id_output_knob_gainFirst + MAXMODULENUMBER - 1;
	const int32_t id_output_textEdit_gainFirst = id_output_knob_gainLast + 1;
	const int32_t id_output_textEdit_gainLast = id_output_textEdit_gainFirst + MAXMODULENUMBER - 1;
	const int32_t id_output_checkbox_limiterFirst = id_output_textEdit_gainLast + 1;
	const int32_t id_output_checkbox_limiterLast = id_output_checkbox_limiterFirst + MAXMODULENUMBER - 1;
	const int32_t id_output_switch_bypassFirst = id_output_checkbox_limiterLast + 1;
	const int32_t id_output_switch_bypassLast = id_output_switch_bypassFirst + MAXMODULENUMBER - 1;

	// PPM GUI ids
	const int32_t id_output_ppmFirst = id_output_switch_bypassLast + 1;
	const int32_t id_output_ppmLast = id_output_ppmFirst + MAXMODULENUMBER - 1;

	// Module visibility
	const int32_t id_general_checkBox_moduleVisibleFirst = id_output_ppmLast + 1;
	const int32_t id_general_checkBox_moduleVisibleLast = id_general_checkBox_moduleVisibleFirst + MAXMODULENUMBER - 1;

	// VST output select
	const int32_t id_general_optionMenu_vstOutputFirst = id_general_checkBox_moduleVisibleLast + 1;
	const int32_t id_general_optionMenu_vstOutputLast = id_general_optionMenu_vstOutputFirst + MAXVSTOUTPUTS - 1;

	// XML Preset
	const int32_t id_general_button_openPreset = id_general_optionMenu_vstOutputLast + 1;
	const int32_t id_general_button_savePreset = id_general_button_openPreset + 1;
	const int32_t id_general_textEdit_presetFilePath = id_general_button_savePreset + 1;

	// Splash screen
	const int32_t id_general_splashScreen_overrideParametersQuery = id_general_textEdit_presetFilePath + 1;
	const int32_t id_general_button_splashViewOk = id_general_splashScreen_overrideParametersQuery + 1;
	const int32_t id_general_button_splashViewCancel = id_general_button_splashViewOk + 1;

	// About Splash Screen
	const int32_t id_general_splashScreen_about = id_general_button_splashViewCancel + 1;
	const int32_t id_general_button_splashViewOkAbout = id_general_splashScreen_about + 1;
	const int32_t id_general_button_about = id_general_button_splashViewOkAbout + 1;

	// Graphics view
	const int32_t id_graphicsView_rearrangeModules = id_general_button_about + 1;
	const int32_t id_graphicsView_addModule = id_graphicsView_rearrangeModules + 1;

// Contructor is called every time the editor is reopened => watch out for memory leaks!
ReverbNetworkEditor::ReverbNetworkEditor(void* controller)
: VSTGUIEditor(controller) 
, totalNumberOfCreatedModules(0)
, fileSelectorStyle(CNewFileSelector::kSelectFile)
{
	pluginVersion = "0";
	tempModuleParameters = {};
	defaultModuleParameters = {};
	lastPpmValues.resize(MAXMODULENUMBER, 0.0);
	ViewRect viewRect(0, 0, 1000, 700);
	setRect(viewRect);
	editorUserData.presetName = "Preset 1";
	editorUserData.moduleNames.resize(MAXMODULENUMBER, "");
	editorUserData.graphicsView = {};

	// Set GUI refresh timer to 40 ms (25 Hz)
	setIdleRate(guiRefreshTime);

	sampleRate = 0.0;
}

ReverbNetworkEditor::~ReverbNetworkEditor() {
	
}

void ReverbNetworkEditor::addGuiElementPointer(CControl* guiElement, const int32_t& guiId) {
	// TODO: Change this to guiId => map to vector index (push_back)
	// Make sure it's a positive id because the id is used as an index number for the vector
	if (guiId >= 0) {
		// Check for out of bounds
		if ((unsigned int)guiId < guiElements.size()) {
			guiElements[guiId] = guiElement;
		}
		// If false: pushBack elements until the guiId is a valid index
		else {
			guiElements.resize(guiId + 1);
			//int size = guiElements.size();
			guiElements[guiId] = guiElement;
		}
	}
}

tresult PLUGIN_API ReverbNetworkEditor::canResize() {
	// Make Editor resizeable (seems only to work in VST3PluginTestHost so disabled for now)
	return kResultFalse;
}

tresult PLUGIN_API ReverbNetworkEditor::onSize(ViewRect* newSize) {
	// Change the views sizes
	/*frame->setSize(newSize->getWidth(), newSize->getHeight());
	frame->setViewSize(CRect(CPoint(0, 0), CPoint(newSize->getWidth(), newSize->getHeight())));
	frame->setMouseableArea(frame->getViewSize());
	mainView->setViewSize(CRect(CPoint(0, 0), CPoint(newSize->getWidth(), newSize->getHeight())));
	mainView->setMouseableArea(mainView->getViewSize());
	splitView->setViewSize(CRect(CPoint(0, 0), CPoint(newSize->getWidth() - viewVstOutputSelect->getWidth() - viewModuleListMain->getWidth() - 40, newSize->getHeight())));
	splitView->setMouseableArea(splitView->getViewSize());
	splashOverrideParametersQuery->setViewSize(mainView->getViewSize());*/
	return kResultTrue;
}

bool PLUGIN_API ReverbNetworkEditor::open(void* parent, const PlatformType& platformType) {
	
	if (frame) // already attached!
	{
		return false;
	}

	xmlPreset = new XmlPresetReadWrite();

	knobBackground = new CBitmap("knob.png");
	knobBackgroundSmall = new CBitmap("knob2.png");
	ppmOff = new CBitmap("ppmOff.png");
	ppmOn = new CBitmap("ppmOn.png");

	allpassModuleIdPool.resize(MAXMODULENUMBER, false);

	CRect editorSize(0, 0, 1120, 700);
	frame = new CFrame(editorSize, this);
	frame->setBackgroundColor(CCOLOR_GRAPHICSVIEW_BACKGROUND);

	splitView = new CSplitView(CRect(CPoint(0, 0), CPoint(940, frame->getHeight())), CSplitView::kVertical, 8);
	splitView->setBackgroundColor(CCOLOR_NOCOLOR);

	workspaceView = new GuiWorkspaceView(CRect(CPoint(0, 0), CPoint(splitView->getViewSize().getWidth() - 90, splitView->getViewSize().getHeight() / 2)), this);
	workspaceView->setBackgroundColor(CCOLOR_WORKSPACE_BACKGROUND);
	workspaceView->setBackgroundColorDrawStyle(CDrawStyle::kDrawFilledAndStroked);

	// Create VST output selection 
	viewVstOutputSelect = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 0.0);

	// Create preset GUI elements
	VSTGUI::CTextEdit* textEditPresetFilePath = new VSTGUI::CTextEdit(CRect(CPoint(0, 0), CPoint(170, 20)), this, id_general_textEdit_presetFilePath, "Preset 1");
	addGuiElementPointer(textEditPresetFilePath, id_general_textEdit_presetFilePath);
	textEditPresetFilePath->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	textEditPresetFilePath->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	textEditPresetFilePath->setFont(kNormalFontSmall);
	VSTGUI::CTextButton* buttonOpenPreset = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(85, 20)), this, id_general_button_openPreset, "Open Preset");
	//buttonOpenPreset->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//buttonOpenPreset->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//buttonOpenPreset->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//buttonOpenPreset->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	buttonOpenPreset->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	buttonOpenPreset->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	addGuiElementPointer(buttonOpenPreset, id_general_button_openPreset);
	buttonOpenPreset->setRoundRadius(1);
	VSTGUI::CTextButton* buttonSavePreset = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(85, 20)), this, id_general_button_savePreset, "Save Preset");
	//buttonSavePreset->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//buttonSavePreset->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//buttonSavePreset->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//buttonSavePreset->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	buttonSavePreset->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	buttonSavePreset->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	addGuiElementPointer(buttonSavePreset, id_general_button_savePreset);
	buttonSavePreset->setRoundRadius(1);
	viewVstOutputSelect->addView(textEditPresetFilePath);
	CRowColumnView* viewPresetButtons = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle);
	viewPresetButtons->addView(buttonOpenPreset);
	viewPresetButtons->addView(buttonSavePreset);
	viewPresetButtons->sizeToFit();
	viewVstOutputSelect->addView(viewPresetButtons);

	CViewContainer* spacing = new CViewContainer(CRect(CPoint(0, 0), CPoint(100, 10)));
	spacing->setBackgroundColor(CCOLOR_NOCOLOR);
	viewVstOutputSelect->addView(spacing);
	
	std::string temp = "";
	for (uint32 i = 0; i < MAXVSTOUTPUTS; ++i) {
		GuiCustomRowColumnView* viewOutputSelectRow = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kColumnStyle, GuiCustomRowColumnView::kLeftTopEqualy, 0.0);
		std::string title = "VST";
		title.append(std::to_string(i));
		title.append(" OUT:");
		GuiCustomTextLabel* labelVstOutputTitle = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(70, 20)), title.c_str(), kNormalFontSmall);
		viewOutputSelectRow->addView(labelVstOutputTitle);
		GuiOptionMenuInputSelector* menu = new GuiOptionMenuInputSelector(CRect(CPoint(0, 0), CPoint(100, 20)), this, id_general_optionMenu_vstOutputFirst + i);
		addGuiElementPointer(menu, id_general_optionMenu_vstOutputFirst + i);
		menu->setBackColor(CCOLOR_OPTIONMENU_BACKGROUND);
		menu->setFrameColor(CCOLOR_OPTIONMENU_FRAME);
		menu->setFont(kNormalFontSmall);
		menu->addEntry("<Not Connected>");
		for (uint32 j = 0; j < MAXMODULENUMBER; ++j) {
			temp = "APM";
			temp.append(std::to_string(j));
			//temp.append(" OUT");
			menu->addEntry((temp).c_str());
		}
		for (uint32 j = 0; j < MAXVSTINPUTS; ++j) {
			temp = "VST";
			temp.append(std::to_string(j));
			temp.append(" IN");
			menu->addEntry((temp).c_str());
		}
		viewOutputSelectRow->addView(menu);
		viewOutputSelectRow->sizeToFit();
		viewOutputSelectRow->setBackgroundColor(CCOLOR_NOCOLOR);
		viewVstOutputSelect->addView(viewOutputSelectRow);
	}
	viewVstOutputSelect->setBackgroundColor(CCOLOR_NOCOLOR);

	// Create signal generator
	signalGenerator = new GuiSignalGenerator(CRect(CPoint(0, 0), CPoint(170, 140)), this);
	signalGenerator->setBackgroundColor(CCOLOR_NOCOLOR);
	viewVstOutputSelect->addView(signalGenerator);

	// Create About Button
	VSTGUI::CTextButton* buttonAbout = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(80, 20)), this, id_general_button_about, "About");
	addGuiElementPointer(buttonAbout, id_graphicsView_addModule);
	//buttonAbout->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//buttonAbout->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//buttonAbout->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//buttonAbout->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	buttonAbout->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	buttonAbout->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	buttonAbout->setRoundRadius(1);
	CRowColumnView* viewAboutDummy = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy, 0.0, CRect(5.0, 5.0, 5.0, 5.0));
	viewAboutDummy->setBackgroundColor(CCOLOR_NOCOLOR);
	viewAboutDummy->addView(buttonAbout);
	viewAboutDummy->sizeToFit();
	viewVstOutputSelect->addView(viewAboutDummy);

	viewVstOutputSelect->sizeToFit();

	// Create graphics view
	graphicsView = new GuiGraphicsView(CRect(CPoint(0, 0), CPoint(splitView->getViewSize().getWidth() - 85, splitView->getViewSize().getHeight())), MAXMODULENUMBER, this);
	graphicsView->setBackgroundColor(CCOLOR_GRAPHICSVIEW_BACKGROUND);
	// Create toolbox for graphics view
	VSTGUI::CTextButton* buttonAddModule = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(85, 20)), this, id_graphicsView_addModule, "Add");
	addGuiElementPointer(buttonAddModule, id_graphicsView_addModule);
	//buttonAddModule->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//buttonAddModule->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//buttonAddModule->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//buttonAddModule->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	buttonAddModule->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	buttonAddModule->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	buttonAddModule->setRoundRadius(1);
	VSTGUI::CTextButton* buttonRearrange = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(85, 20)), this, id_graphicsView_rearrangeModules, "Sort");
	addGuiElementPointer(buttonRearrange, id_graphicsView_rearrangeModules);
	//buttonRearrange->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//buttonRearrange->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//buttonRearrange->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//buttonRearrange->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	buttonRearrange->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	buttonRearrange->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	buttonRearrange->setRoundRadius(1);
	GuiCustomRowColumnView* viewGraphicsViewToolBox = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(100, graphicsView->getHeight())), GuiCustomRowColumnView::kRowStyle);
	//viewGraphicsViewToolBox->setFrameWidth(1);
	//viewGraphicsViewToolBox->setFrameColor(CCOLOR_FRAME);
	viewGraphicsViewToolBox->addView(buttonAddModule);
	viewGraphicsViewToolBox->addView(buttonRearrange);
	viewGraphicsViewToolBox->setBackgroundColor(CCOLOR_GRAPHICSVIEW_TOOLBOXBACKGROUND);
	GuiCustomRowColumnView* graphicsMainView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(splitView->getViewSize().getWidth(), splitView->getViewSize().getHeight() / 2)), GuiCustomRowColumnView::kColumnStyle);
	graphicsMainView->addView(graphicsView);
	graphicsMainView->addView(viewGraphicsViewToolBox);

	// Create Module List
	viewModuleListMain = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kRowStyle);
	viewModuleScrollList = new CScrollView(CRect(CPoint(0, 0), CPoint(85, splitView->getHeight())), CRect(CPoint(0, 0), CPoint(0, 0)), CScrollView::kVerticalScrollbar, 10.0);
	viewModuleScrollList->getVerticalScrollbar()->setScrollerColor(CCOLOR_SCROLLVIEW_SCROLLBAR);
	viewModuleScrollList->setStyle(CScrollView::kVerticalScrollbar | CScrollView::kAutoHideScrollbars);
	GuiCustomRowColumnView* viewModuleList = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kRowStyle, GuiCustomRowColumnView::kLeftTopEqualy, 0.0, CRect(5.0, 5.0, 5.0, 5.0));
	for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
		apGuiModules.push_back(createAPModule());
		std::string title = "APM";
		title.append(std::to_string(i));
		CCheckBox* checkBoxShowHideModule = new CCheckBox(CRect(CPoint(0, 0), CPoint(75, 18)), this, id_general_checkBox_moduleVisibleFirst + i, title.c_str());
		checkBoxShowHideModule->setFont(kNormalFontSmall);
		addGuiElementPointer(checkBoxShowHideModule, id_general_checkBox_moduleVisibleFirst + i);
		viewModuleList->addView(checkBoxShowHideModule);
	}
	viewModuleScrollList->addView(viewModuleList);
	viewModuleScrollList->setContainerSize(viewModuleList->getViewSize());
	viewModuleList->sizeToFit();
	viewModuleListMain->addView(viewModuleScrollList);
	viewModuleListMain->sizeToFit();
	viewModuleListMain->setBackgroundColor(CCOLOR_MODULELIST_BACKGROUND);
	viewModuleScrollList->setBackgroundColor(CCOLOR_NOCOLOR);
	//viewModuleListMain->setFrameWidth(1);
	//viewModuleListMain->setFrameColor(CCOLOR_FRAME);
	viewModuleList->setBackgroundColor(CCOLOR_NOCOLOR);

	// Create main views
	GuiCustomRowColumnView* workspaceMainView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(splitView->getViewSize().getWidth(), splitView->getViewSize().getHeight())), GuiCustomRowColumnView::kColumnStyle);
	workspaceMainView->addView(workspaceView);
	workspaceMainView->addView(viewModuleListMain);
	workspaceMainView->setBackgroundColor(CCOLOR_NOCOLOR);
	splitView->addView(graphicsMainView);
	splitView->addView(workspaceMainView);
	initializeGraphicsView();
	workspaceView->setViewSize(CRect(CPoint(0, 0), CPoint(splitView->getViewSize().getWidth() - 85, splitView->getViewSize().getHeight())));
	workspaceView->setMouseableArea(workspaceView->getViewSize());
	mainView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kColumnStyle, GuiCustomRowColumnView::kLeftTopEqualy, 5.0);
	mainView->addView(splitView);
	//mainView->addView(viewModuleListMain);
	mainView->addView(viewVstOutputSelect);
	mainView->sizeToFit();
	mainView->setBackgroundColor(CCOLOR_SIDEBAR_BACKGROUND);
	frame->addView(mainView);

	// Create splash view
	GuiCustomTextLabel* labelQueryMessage = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(200, 20)), "Override the current parameters?", kNormalFont, CHoriTxtAlign::kLeftText);
	VSTGUI::CTextButton* buttonOk = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(50, 20)), this, id_general_button_splashViewOk, "Yes");
	addGuiElementPointer(buttonOk, id_general_button_splashViewOk);
	//buttonOk->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//buttonOk->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//buttonOk->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//buttonOk->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	buttonOk->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	buttonOk->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	buttonOk->setRoundRadius(1);
	VSTGUI::CTextButton* buttonCancel = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(50, 20)), this, id_general_button_splashViewCancel, "No");
	addGuiElementPointer(buttonCancel, id_general_button_splashViewCancel);
	//buttonCancel->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//buttonCancel->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//buttonCancel->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//buttonCancel->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	buttonCancel->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	buttonCancel->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	buttonCancel->setRoundRadius(1);
	GuiCustomRowColumnView* buttonView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kColumnStyle);
	buttonView->addView(buttonOk);
	buttonView->addView(buttonCancel);
	buttonView->sizeToFit();
	buttonView->setBackgroundColor(CCOLOR_NOCOLOR);
	GuiCustomRowColumnView* querySubView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kRowStyle);
	querySubView->setBackgroundColor(CCOLOR_NOCOLOR);
	querySubView->addView(labelQueryMessage);
	querySubView->addView(buttonView);
	querySubView->sizeToFit();
	querySubView->setViewSize(CRect(CPoint(mainView->getViewSize().getCenter().x - querySubView->getViewSize().getWidth() / 2, mainView->getViewSize().getCenter().y - querySubView->getViewSize().getHeight() / 2), CPoint(querySubView->getViewSize().getSize())));
	querySubView->setMouseableArea(querySubView->getViewSize());
	CViewContainer* queryView = new CViewContainer(mainView->getViewSize());
	queryView->addView(querySubView);
	queryView->setBackgroundColor(CCOLOR_SPLASHVIEW_BACKGROUND);
	splashOverrideParametersQuery = new GuiCustomSplashScreen(CRect(CPoint(0, 0), CPoint(0, 0)), this, id_general_splashScreen_overrideParametersQuery, queryView);
	splashOverrideParametersQuery->sizeToFit();
	addGuiElementPointer(splashOverrideParametersQuery, id_general_splashScreen_overrideParametersQuery);
	mainView->addView(splashOverrideParametersQuery);
	
	// Create About View
	std::string title = "Reverb Network VST Plug-in ";
	title.append(pluginVersion);
	GuiCustomTextLabel* labelVersion = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(500, 15)), title.c_str(), kNormalFont, CHoriTxtAlign::kLeftText);
	GuiCustomTextLabel* labelCopyright = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(550, 15)), "Copyright (c) 2014-2015 by Andrej Nichelmann and Klaus Michael Indlekofer. All rights reserved.", kNormalFont, CHoriTxtAlign::kLeftText);
	VSTGUI::CTextButton* buttonOkAbout = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(80, 20)), this, id_general_button_splashViewOkAbout, "Okay");
	addGuiElementPointer(buttonOkAbout, id_general_button_splashViewOkAbout);
	//buttonOkAbout->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//buttonOkAbout->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//buttonOkAbout->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//buttonOkAbout->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	buttonOkAbout->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	buttonOkAbout->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	buttonOkAbout->setRoundRadius(1);
	CRowColumnView* subViewAbout = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	subViewAbout->setBackgroundColor(CCOLOR_NOCOLOR);
	subViewAbout->addView(labelVersion);
	subViewAbout->addView(labelCopyright);
	subViewAbout->addView(buttonOkAbout);
	subViewAbout->sizeToFit();
	subViewAbout->setViewSize(CRect(CPoint(mainView->getViewSize().getCenter().x - subViewAbout->getViewSize().getWidth() / 2, mainView->getViewSize().getCenter().y - subViewAbout->getViewSize().getHeight() / 2), CPoint(subViewAbout->getViewSize().getSize())));
	subViewAbout->setMouseableArea(subViewAbout->getViewSize());
	CViewContainer* viewAbout = new CViewContainer(mainView->getViewSize());
	viewAbout->addView(subViewAbout);
	viewAbout->setBackgroundColor(CCOLOR_SPLASHVIEW_BACKGROUND);
	GuiCustomSplashScreen* splashScreenAbout = new GuiCustomSplashScreen(mainView->getViewSize(), this, id_general_splashScreen_about, viewAbout);
	addGuiElementPointer(splashScreenAbout, id_general_splashScreen_about);
	mainView->addView(splashScreenAbout);

	// Delete the bitmap obejcts
	knobBackground->forget();
	knobBackgroundSmall->forget();
	ppmOff->forget();
	ppmOn->forget();

	// Initialize EQ stability buttons
	updateGuiWithControllerParameters();
	for (int i = 0; i < MAXMODULENUMBER; ++i) {
		updateEditorFromController(PARAM_EQSTABILITY_FIRST + i, 1.0);
	}

	// Sort the graphics view
	graphicsView->rearrangeModules();

	workspaceView->parentSizeChanged();

	// Save the default module parameters for later (when the user clicks on "Default")
	if (apGuiModules.size() > 0) {
		copyModuleParameters(0, defaultModuleParameters);
	}

	IPlatformFrameConfig* config = nullptr;

	// Open the UI
	getFrame()->open(parent, platformType, config);

	return true;
}


void PLUGIN_API ReverbNetworkEditor::close() {
	if (frame)
	{
		guiElements.clear();
		apGuiModules.clear();
		if (xmlPreset) {
			delete xmlPreset;
		}
		totalNumberOfCreatedModules = 0;
		allpassModuleIdPool.clear();
		frame->forget();	// delete frame
		frame = 0;
	}
}

void ReverbNetworkEditor::valueChanged(CControl* pControl) {
	// This function is called when the user changes a GU element, pControl is a pointer to the object which has been changed
	int32_t tag = pControl->getTag(); // Get the GUI ID
	double value = (double)pControl->getValue(); // Get the value
	
	if (tag >= id_module_textEdit_titleFirst && tag <= id_module_textEdit_titleLast) {
		for (int i = id_mixer_optionMenu_inputSelectFirst; i <= id_mixer_optionMenu_inputSelectLast; ++i) {
			// Build up the name ("APMXXX" should always be there at the beginning of the title)
			std::string temp = "APM";
			temp.append(std::to_string(tag - id_module_textEdit_titleFirst));
			temp.append(" ");
			temp.append(dynamic_cast<VSTGUI::CTextEdit*>(pControl)->getText()); // Get the text the user has entered
			// Update all option menus
			double gainValue = ValueConversion::normToPlainInputGain(controller->getParamNormalized(PARAM_MIXERGAIN_FIRST + tag - id_module_textEdit_titleFirst + ((i - id_mixer_optionMenu_inputSelectFirst) / MAXMODULEINPUTS) * MAXINPUTS));
			if (gainValue != 0.0) {
				temp.append(" [");
				if (gainValue >= 0.0) {
					temp.append(std::to_string(gainValue).c_str(), 4);
				}
				else {
					temp.append(std::to_string(gainValue).c_str(), 5);
				}
				temp.append("]");
			}
			dynamic_cast<VSTGUI::COptionMenu*>(guiElements[i])->getEntry(tag - id_module_textEdit_titleFirst + 1)->setTitle(temp.c_str());
			guiElements[i]->setDirty();
		}
		// Update all VST outputs
		for (int i = id_general_optionMenu_vstOutputFirst; i <= id_general_optionMenu_vstOutputLast; ++i) {
			std::string temp = "APM";
			temp.append(std::to_string(tag - id_module_textEdit_titleFirst));
			temp.append(" ");
			temp.append(dynamic_cast<VSTGUI::CTextEdit*>(pControl)->getText());
			dynamic_cast<VSTGUI::COptionMenu*>(guiElements[i])->getEntry(tag - id_module_textEdit_titleFirst + 1)->setTitle(temp.c_str());
			guiElements[i]->setDirty();
		} 
		editorUserData.moduleNames[tag - id_module_textEdit_titleFirst] = dynamic_cast<VSTGUI::CTextEdit*>(pControl)->getText(); // Save the names for later
		std::string temp = "APM";
		temp.append(std::to_string(tag - id_module_textEdit_titleFirst));
		temp.append(" ");
		temp.append(dynamic_cast<VSTGUI::CTextEdit*>(pControl)->getText());
		graphicsView->setModuleTitle(tag - id_module_textEdit_titleFirst, temp); // Update the titles in the graphics view
		graphicsView->invalid();
	}
	else if (tag >= id_module_button_collapseFirst && tag <= id_module_button_collapseLast) {
		if (apGuiModules[tag - id_module_button_collapseFirst]->isCollapsed()) {
			apGuiModules[tag - id_module_button_collapseFirst]->collapseView(false);
		}
		else {
			apGuiModules[tag - id_module_button_collapseFirst]->collapseView(true);
		}
	}
	else if (tag >= id_module_button_hideFirst && tag <= id_module_button_hideLast) {	// Close button of AP module pressed
		if (pControl->getValue() == 1.0) {
			openModuleDetailView(tag - id_module_button_hideFirst, false);
		}
	}
	else if (tag >= id_module_button_copyParametersFirst && tag <= id_module_button_copyParametersLast) {
		if (pControl->getValue() == 1.0) {
			copyModuleParameters(tag - id_module_button_copyParametersFirst, tempModuleParameters);
		}
	}
	else if (tag >= id_module_button_pasteParametersFirst && tag <= id_module_button_pasteParametersLast) {
		if (pControl->getValue() == 1.0) {
			overrideParametersQuery* userData = new overrideParametersQuery;
			userData->moduleId = tag - id_module_button_pasteParametersFirst;
			userData->moduleStruct = &tempModuleParameters; // Get the copied parameters
			dynamic_cast<GuiCustomSplashScreen*>(guiElements[id_general_splashScreen_overrideParametersQuery])->setUserData(userData);
			dynamic_cast<GuiCustomSplashScreen*>(guiElements[id_general_splashScreen_overrideParametersQuery])->splash(); // Show the splash view
			frame->setMouseableArea(guiElements[id_general_splashScreen_overrideParametersQuery]->getViewSize()); // Disable the mouse for everything else
		}
	}
	else if (tag >= id_module_button_defaultParametersFirst && tag <= id_module_button_defaultParametersLast) {
		if (pControl->getValue() == 1.0) {
			overrideParametersQuery* userData = new overrideParametersQuery;
			userData->moduleId = tag - id_module_button_defaultParametersFirst;
			userData->moduleStruct = &defaultModuleParameters; // Get the default parameters
			dynamic_cast<GuiCustomSplashScreen*>(guiElements[id_general_splashScreen_overrideParametersQuery])->setUserData(userData);
			dynamic_cast<GuiCustomSplashScreen*>(guiElements[id_general_splashScreen_overrideParametersQuery])->splash();
			frame->setMouseableArea(guiElements[id_general_splashScreen_overrideParametersQuery]->getViewSize());
		}
	}
	else if (tag == id_general_button_splashViewOk) {
		if (pControl->getValue() == 1.0) {
			// Get the userData
			overrideParametersQuery* userData = (overrideParametersQuery*)(dynamic_cast<GuiCustomSplashScreen*>(guiElements[id_general_splashScreen_overrideParametersQuery])->getUserData());
			// Apply the copied parameters
			pasteModuleParameters(userData->moduleId, XmlPresetReadWrite::Module(*userData->moduleStruct));
			dynamic_cast<GuiCustomSplashScreen*>(guiElements[id_general_splashScreen_overrideParametersQuery])->unSplash(); // Hide the splash view
			frame->setMouseableArea(frame->getViewSize());
		}
	}
	else if (tag == id_general_button_splashViewCancel) {
		if (pControl->getValue() == 1.0) {
			dynamic_cast<GuiCustomSplashScreen*>(guiElements[id_general_splashScreen_overrideParametersQuery])->unSplash();
			frame->setMouseableArea(frame->getViewSize());
		}
	}
	// Mixer
	else if (tag >= id_mixer_optionMenu_inputSelectFirst && tag <= id_mixer_optionMenu_inputSelectLast) {
		uint16 moduleNumber = (tag - id_mixer_optionMenu_inputSelectFirst) / MAXMODULEINPUTS;	// Calculate the module number
		// Update the VST parameter variable
		controller->setParamNormalized(PARAM_MIXERINPUTSELECT_FIRST + (tag - id_mixer_optionMenu_inputSelectFirst), ValueConversion::plainToNormMixerInputSelect(value));
		// Update the module member variable (is called in process() in processor)
		controller->performEdit(PARAM_MIXERINPUTSELECT_FIRST + (tag - id_mixer_optionMenu_inputSelectFirst), ValueConversion::plainToNormMixerInputSelect(value));

		// Get the menu entry that was selected before the current one and enable it in all OptionMenus
		int lastIndex = dynamic_cast<GuiOptionMenuInputSelector*>(guiElements[tag])->getLastIndex();
		for (int i = 0; i < MAXMODULEINPUTS; ++i) {
			dynamic_cast<GuiOptionMenuInputSelector*>(guiElements[id_mixer_optionMenu_inputSelectFirst + moduleNumber * MAXMODULEINPUTS + i])->getEntry(lastIndex)->setEnabled(true);
		}

		if (value == 0.0) {
			// 0.0 means <Select> is selected in the dropdown menu
			// Set default values for the GUI elements and disable them
			guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(ValueConversion::plainToNormInputGain(DEF_MIXERGAIN));
			guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(DEF_MIXERGAIN);
			guiElements[id_mixer_button_muteFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(0.0);
			guiElements[id_mixer_button_soloFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(0.0);
			guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(false);
			guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(false);
			guiElements[id_mixer_button_muteFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(false);
			guiElements[id_mixer_button_soloFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(false);
		}
		else {
			uint16 moduleNumber = (tag - id_mixer_optionMenu_inputSelectFirst) / MAXMODULEINPUTS;	// Calculate the module number
			// Restore the saved gain value and update the GUI
			guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + moduleNumber * MAXINPUTS + value - 1));
			guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + moduleNumber * MAXINPUTS + value - 1)));
			guiElements[id_mixer_button_muteFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(getController()->getParamNormalized(PARAM_MIXERINPUTMUTED_FIRST + moduleNumber * MAXINPUTS + value - 1));
			guiElements[id_mixer_button_soloFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setValue(getController()->getParamNormalized(PARAM_MIXERINPUTSOLOED_FIRST + moduleNumber * MAXINPUTS + value - 1));
			// Enable the GUI elements in case they were disabled
			guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(true);
			guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(true);
			guiElements[id_mixer_button_muteFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(true);
			guiElements[id_mixer_button_soloFirst + (tag - id_mixer_optionMenu_inputSelectFirst)]->setMouseEnabled(true);
			// Get the selected menu entry and disable it in all OptionMenus
			int currentIndex = dynamic_cast<GuiOptionMenuInputSelector*>(guiElements[tag])->getCurrentIndex();
			for (int i = 0; i < MAXMODULEINPUTS; ++i) {
				dynamic_cast<GuiOptionMenuInputSelector*>(guiElements[id_mixer_optionMenu_inputSelectFirst + moduleNumber * MAXMODULEINPUTS + i])->getEntry(currentIndex)->setEnabled(false);
			}
		}
	}
	else if (tag >= id_mixer_knob_gainFirst && tag <= id_mixer_knob_gainLast)  {
		uint16 moduleNumber = (tag - id_mixer_knob_gainFirst) / MAXMODULEINPUTS;	// Calculate the module number
		int inputIndex = (int)(guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_knob_gainFirst)]->getValue()); // Calculate the input number
		if (inputIndex != 0.0) {
			// 0.0 means <Select> is selected so no update needed
			// Knob value already normalized => no conversion needed
			controller->setParamNormalized(PARAM_MIXERGAIN_FIRST + moduleNumber * MAXINPUTS + inputIndex - 1, value);
			controller->performEdit(PARAM_MIXERGAIN_FIRST + moduleNumber * MAXINPUTS + inputIndex - 1, value);
			// Update the textEdit
			guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_knob_gainFirst)]->setValue(ValueConversion::normToPlainInputGain(value));
			guiElements[id_mixer_textEdit_gainFirst + (tag - id_mixer_knob_gainFirst)]->invalid(); // setDirty() doesn't work here (why?)
			// Update the gain values in the option menus
			updateGainValuesInOptionMenus(moduleNumber, inputIndex - 1, ValueConversion::normToPlainInputGain(value));
			// Update the graphics view
			updateGraphicsViewModule(moduleNumber, inputIndex - 1, ValueConversion::normToPlainInputGain(value));
			splitView->invalid();
		}
	}
	else if (tag >= id_mixer_textEdit_gainFirst && tag <= id_mixer_textEdit_gainLast)  {
		int inputIndex = (int)(guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_textEdit_gainFirst)]->getValue());
		if (inputIndex != 0.0) {
			uint16 moduleNumber = (tag - id_mixer_textEdit_gainFirst) / MAXMODULEINPUTS;	// Calculate the module number
			controller->setParamNormalized(PARAM_MIXERGAIN_FIRST + moduleNumber * MAXINPUTS + inputIndex - 1, ValueConversion::plainToNormInputGain(value));
			controller->performEdit(PARAM_MIXERGAIN_FIRST + moduleNumber * MAXINPUTS + inputIndex - 1, ValueConversion::plainToNormInputGain(value));
			// Update the knob
			guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_textEdit_gainFirst)]->setValue(ValueConversion::plainToNormInputGain(value));
			guiElements[id_mixer_knob_gainFirst + (tag - id_mixer_textEdit_gainFirst)]->setDirty();
			updateGainValuesInOptionMenus(moduleNumber, inputIndex - 1, value);
			updateGraphicsViewModule(moduleNumber, inputIndex - 1, value);
			splitView->invalid();
		}
	}
	else if (tag >= id_mixer_button_muteFirst && tag <= id_mixer_button_muteLast)  {
		uint16 moduleNumber = (tag - id_mixer_button_muteFirst) / MAXMODULEINPUTS;
		int inputIndex = (int)(std::round(guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_button_muteFirst)]->getValue()));
		if (guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_button_muteFirst)]->getValue() != 0.0) {
			controller->setParamNormalized(PARAM_MIXERINPUTMUTED_FIRST + moduleNumber * MAXINPUTS + inputIndex - 1, value);
			controller->performEdit(PARAM_MIXERINPUTMUTED_FIRST + moduleNumber * MAXINPUTS + inputIndex - 1, value);
		}
	}
	else if (tag >= id_mixer_button_soloFirst && tag <= id_mixer_button_soloLast)  {
		uint16 moduleNumber = (tag - id_mixer_button_soloFirst) / MAXMODULEINPUTS;
		int inputIndex = (int)(std::round(guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_button_soloFirst)]->getValue()));
		if (guiElements[id_mixer_optionMenu_inputSelectFirst + (tag - id_mixer_button_soloFirst)]->getValue() != 0.0) {
			controller->setParamNormalized(PARAM_MIXERINPUTSOLOED_FIRST + moduleNumber * MAXINPUTS + inputIndex - 1, value);
			controller->performEdit(PARAM_MIXERINPUTSOLOED_FIRST + moduleNumber * MAXINPUTS + inputIndex - 1, value);
		}
	}
	// Quantizer
	else if (tag >= id_quantizer_knob_quantizationFirst && tag <= id_quantizer_knob_quantizationLast)  {
		controller->setParamNormalized(PARAM_QUANTIZERBITDEPTH_FIRST + (tag - id_quantizer_knob_quantizationFirst), value);
		controller->performEdit(PARAM_QUANTIZERBITDEPTH_FIRST + (tag - id_quantizer_knob_quantizationFirst), value);
		guiElements[id_quantizer_textEdit_quantizationFirst + (tag - id_quantizer_knob_quantizationFirst)]->setValue(ValueConversion::normToPlainQuantization(value));
		guiElements[id_quantizer_textEdit_quantizationFirst + (tag - id_quantizer_knob_quantizationFirst)]->invalid();
	}
	else if (tag >= id_quantizer_textEdit_quantizationFirst && tag <= id_quantizer_textEdit_quantizationLast)  {
		controller->setParamNormalized(PARAM_QUANTIZERBITDEPTH_FIRST + (tag - id_quantizer_textEdit_quantizationFirst), ValueConversion::plainToNormQuantization(value));
		controller->performEdit(PARAM_QUANTIZERBITDEPTH_FIRST + (tag - id_quantizer_textEdit_quantizationFirst), ValueConversion::plainToNormQuantization(value));
		guiElements[id_quantizer_knob_quantizationFirst + (tag - id_quantizer_textEdit_quantizationFirst)]->setValue(ValueConversion::plainToNormQuantization(value));
		guiElements[id_quantizer_knob_quantizationFirst + (tag - id_quantizer_textEdit_quantizationFirst)]->setDirty();
	}
	else if (tag >= id_quantizer_switch_bypassFirst && tag <= id_quantizer_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_QUANTIZERBYPASS_FIRST + (tag - id_quantizer_switch_bypassFirst), value);
		controller->performEdit(PARAM_QUANTIZERBYPASS_FIRST + (tag - id_quantizer_switch_bypassFirst), value);
	}
	// Equalizer
	else if (tag >= id_equalizer_optionMenu_filterTypeFirst && tag <= id_equalizer_optionMenu_filterTypeLast)  {
		controller->setParamNormalized(PARAM_EQFILTERTYPE_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), ValueConversion::plainToNormFilterTypeSelect(value));
		controller->performEdit(PARAM_EQFILTERTYPE_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), ValueConversion::plainToNormFilterTypeSelect(value));
		// Get the equalizer view (could be buggy...)
		if (pControl->getParentView() && pControl->getParentView()->getParentView()) {
			CViewContainer* equalizerView = dynamic_cast<CViewContainer*>(pControl->getParentView()->getParentView());
			if (equalizerView) {
				// Get the view with the knobs and stuff
				CViewContainer* layeredView = dynamic_cast<CViewContainer*>(equalizerView->getView(equalizerView->getNbViews() - 1));
				if (layeredView) {
					if (layeredView->getNbViews() >= 2) {
						// Set the right view visible (either the normal one with the knobs or the other one with the coefficients)
						layeredView->getView(0)->setVisible((FilterType)(int)value != FilterType::rawBiquad);
						layeredView->getView(1)->setVisible((FilterType)(int)value == FilterType::rawBiquad);
					}
				}
			}
		}
		if ((FilterType)(int)value == FilterType::rawBiquad) {
			// Update the coefficients when Raw Biquad
			controller->performEdit(PARAM_EQCOEFFICIENTA0_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), controller->getParamNormalized(PARAM_EQCOEFFICIENTA0_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst)));
			controller->performEdit(PARAM_EQCOEFFICIENTA1_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), controller->getParamNormalized(PARAM_EQCOEFFICIENTA1_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst)));
			controller->performEdit(PARAM_EQCOEFFICIENTA2_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), controller->getParamNormalized(PARAM_EQCOEFFICIENTA2_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst)));
			controller->performEdit(PARAM_EQCOEFFICIENTB1_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), controller->getParamNormalized(PARAM_EQCOEFFICIENTB1_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst)));
			controller->performEdit(PARAM_EQCOEFFICIENTB2_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), controller->getParamNormalized(PARAM_EQCOEFFICIENTB2_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst)));
			// And the stability indicator
			updateEqStability(tag - id_equalizer_optionMenu_filterTypeFirst, 
				ValueConversion::checkEqStability(guiElements[id_equalizer_textEdit_b1First + (tag - id_equalizer_optionMenu_filterTypeFirst)]->getValue(), 
				guiElements[id_equalizer_textEdit_b2First + (tag - id_equalizer_optionMenu_filterTypeFirst)]->getValue()));
		}
		else {
			// Update the other parameters when not Raw Biquad
			updateEqStability(tag - id_equalizer_optionMenu_filterTypeFirst, true);
			controller->performEdit(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), controller->getParamNormalized(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst)));
			controller->performEdit(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), controller->getParamNormalized(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst)));
			controller->performEdit(PARAM_EQGAIN_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst), controller->getParamNormalized(PARAM_EQGAIN_FIRST + (tag - id_equalizer_optionMenu_filterTypeFirst)));
		}
	}
	else if (tag >= id_equalizer_knob_centerFreqFirst && tag <= id_equalizer_knob_centerFreqLast)  {	
		controller->setParamNormalized(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_knob_centerFreqFirst), value);
		controller->performEdit(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_knob_centerFreqFirst), value);
		guiElements[id_equalizer_textEdit_centerFreqFirst + (tag - id_equalizer_knob_centerFreqFirst)]->setValue(ValueConversion::normToPlainProcCenterFreq(value));
		guiElements[id_equalizer_textEdit_centerFreqFirst + (tag - id_equalizer_knob_centerFreqFirst)]->invalid();
	}
	else if (tag >= id_equalizer_textEdit_centerFreqFirst && tag <= id_equalizer_textEdit_centerFreqLast)  {
		controller->setParamNormalized(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_textEdit_centerFreqFirst), ValueConversion::plainToNormProcCenterFreq(value));
		controller->performEdit(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_textEdit_centerFreqFirst), ValueConversion::plainToNormProcCenterFreq(value));
		guiElements[id_equalizer_knob_centerFreqFirst + (tag - id_equalizer_textEdit_centerFreqFirst)]->setValue(ValueConversion::plainToNormProcCenterFreq(value));
		guiElements[id_equalizer_knob_centerFreqFirst + (tag - id_equalizer_textEdit_centerFreqFirst)]->setDirty();
	}
	else if (tag >= id_equalizer_knob_qFactorFirst && tag <= id_equalizer_knob_qFactorLast)  {
		controller->setParamNormalized(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_knob_qFactorFirst), value);
		controller->performEdit(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_knob_qFactorFirst), value);
		guiElements[id_equalizer_textEdit_qFactorFirst + (tag - id_equalizer_knob_qFactorFirst)]->setValue(ValueConversion::normToPlainQFactor(value));
		guiElements[id_equalizer_textEdit_qFactorFirst + (tag - id_equalizer_knob_qFactorFirst)]->invalid();
	}
	else if (tag >= id_equalizer_textEdit_qFactorFirst && tag <= id_equalizer_textEdit_qFactorLast)  {
		controller->setParamNormalized(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_textEdit_qFactorFirst), ValueConversion::plainToNormQFactor(value));
		controller->performEdit(PARAM_EQQFACTOR_FIRST + (tag - id_equalizer_textEdit_qFactorFirst), ValueConversion::plainToNormQFactor(value));
		guiElements[id_equalizer_knob_qFactorFirst + (tag - id_equalizer_textEdit_qFactorFirst)]->setValue(ValueConversion::plainToNormQFactor(value));
		guiElements[id_equalizer_knob_qFactorFirst + (tag - id_equalizer_textEdit_qFactorFirst)]->setDirty();
	}
	else if (tag >= id_equalizer_knob_gainFirst && tag <= id_equalizer_knob_gainLast)  {
		controller->setParamNormalized(PARAM_EQGAIN_FIRST + (tag - id_equalizer_knob_gainFirst), value);
		controller->performEdit(PARAM_EQGAIN_FIRST + (tag - id_equalizer_knob_gainFirst), value);
		guiElements[id_equalizer_textEdit_gainFirst + (tag - id_equalizer_knob_gainFirst)]->setValue(ValueConversion::normToPlainEqGain(value));
		guiElements[id_equalizer_textEdit_gainFirst + (tag - id_equalizer_knob_gainFirst)]->invalid();
	}
	else if (tag >= id_equalizer_textEdit_gainFirst && tag <= id_equalizer_textEdit_gainLast)  {
		controller->setParamNormalized(PARAM_EQGAIN_FIRST + (tag - id_equalizer_textEdit_gainFirst), ValueConversion::plainToNormEqGain(value));
		controller->performEdit(PARAM_EQGAIN_FIRST + (tag - id_equalizer_textEdit_gainFirst), ValueConversion::plainToNormEqGain(value));
		guiElements[id_equalizer_knob_gainFirst + (tag - id_equalizer_textEdit_gainFirst)]->setValue(ValueConversion::plainToNormEqGain(value));
		guiElements[id_equalizer_knob_gainFirst + (tag - id_equalizer_textEdit_gainFirst)]->setDirty();
	}
	else if (tag >= id_equalizer_textEdit_a0First && tag <= id_equalizer_textEdit_a0Last) {
		controller->setParamNormalized(PARAM_EQCOEFFICIENTA0_FIRST + (tag - id_equalizer_textEdit_a0First), ValueConversion::plainToNormEqCoefficients(value));
		controller->performEdit(PARAM_EQCOEFFICIENTA0_FIRST + (tag - id_equalizer_textEdit_a0First), ValueConversion::plainToNormEqCoefficients(value));
	}
	else if (tag >= id_equalizer_textEdit_a1First && tag <= id_equalizer_textEdit_a1Last) {
		controller->setParamNormalized(PARAM_EQCOEFFICIENTA1_FIRST + (tag - id_equalizer_textEdit_a1First), ValueConversion::plainToNormEqCoefficients(value));
		controller->performEdit(PARAM_EQCOEFFICIENTA1_FIRST + (tag - id_equalizer_textEdit_a1First), ValueConversion::plainToNormEqCoefficients(value));
	}
	else if (tag >= id_equalizer_textEdit_a2First && tag <= id_equalizer_textEdit_a2Last) {
		controller->setParamNormalized(PARAM_EQCOEFFICIENTA2_FIRST + (tag - id_equalizer_textEdit_a2First), ValueConversion::plainToNormEqCoefficients(value));
		controller->performEdit(PARAM_EQCOEFFICIENTA2_FIRST + (tag - id_equalizer_textEdit_a2First), ValueConversion::plainToNormEqCoefficients(value));
	}
	else if (tag >= id_equalizer_textEdit_b1First && tag <= id_equalizer_textEdit_b1Last) {
		controller->setParamNormalized(PARAM_EQCOEFFICIENTB1_FIRST + (tag - id_equalizer_textEdit_b1First), ValueConversion::plainToNormEqCoefficients(value));
		controller->performEdit(PARAM_EQCOEFFICIENTB1_FIRST + (tag - id_equalizer_textEdit_b1First), ValueConversion::plainToNormEqCoefficients(value));
		// Simulate the transmission to the processing components by convert the plain values to norm and back to plain values (=> rounding errors!)
		updateEqStability(tag - id_equalizer_textEdit_b1First, ValueConversion::checkEqStability(ValueConversion::normToPlainEqCoefficients(ValueConversion::plainToNormEqCoefficients(value)), ValueConversion::normToPlainEqCoefficients(ValueConversion::plainToNormEqCoefficients(guiElements[id_equalizer_textEdit_b2First + (tag - id_equalizer_textEdit_b1First)]->getValue()))));
	}
	else if (tag >= id_equalizer_textEdit_b2First && tag <= id_equalizer_textEdit_b2Last) {
		controller->setParamNormalized(PARAM_EQCOEFFICIENTB2_FIRST + (tag - id_equalizer_textEdit_b2First), ValueConversion::plainToNormEqCoefficients(value));
		controller->performEdit(PARAM_EQCOEFFICIENTB2_FIRST + (tag - id_equalizer_textEdit_b2First), ValueConversion::plainToNormEqCoefficients(value));
		updateEqStability(tag - id_equalizer_textEdit_b2First, ValueConversion::checkEqStability(ValueConversion::normToPlainEqCoefficients(ValueConversion::plainToNormEqCoefficients(guiElements[id_equalizer_textEdit_b1First + (tag - id_equalizer_textEdit_b2First)]->getValue())), ValueConversion::normToPlainEqCoefficients(ValueConversion::plainToNormEqCoefficients(value))));
	}
	else if (tag >= id_equalizer_checkBox_limiterFirst && tag <= id_equalizer_checkBox_limiterLast)  {
		controller->setParamNormalized(PARAM_EQLIMITER_FIRST + (tag - id_equalizer_checkBox_limiterFirst), value);
		controller->performEdit(PARAM_EQLIMITER_FIRST + (tag - id_equalizer_checkBox_limiterFirst), value);
	}
	else if (tag >= id_equalizer_switch_bypassFirst && tag <= id_equalizer_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_EQBYPASS_FIRST + (tag - id_equalizer_switch_bypassFirst), value);
		controller->performEdit(PARAM_EQBYPASS_FIRST + (tag - id_equalizer_switch_bypassFirst), value);
	}
	// Allpass
	else if (tag >= id_allpass_knob_delayFirst && tag <= id_allpass_knob_delayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_knob_delayFirst), value);
		controller->performEdit(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_knob_delayFirst), value);
		guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_knob_delayFirst)]->setValue(ValueConversion::normToPlainDelay(value));
		guiElements[id_allpass_textEdit_samplesDelayFirst + (tag - id_allpass_knob_delayFirst)]->setValue(ValueConversion::delayMillisecondsToSamples(ValueConversion::normToPlainDelay(value)));
		guiElements[id_allpass_textEdit_diffKFirst + (tag - id_allpass_knob_delayFirst)]
			->setValue(ValueConversion::calculateDiffK(ValueConversion::normToPlainDelay(value), guiElements[id_allpass_textEdit_decayFirst + (tag - id_allpass_knob_delayFirst)]->getValue()));
	}
	else if (tag >= id_allpass_textEdit_delayFirst && tag <= id_allpass_textEdit_delayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_textEdit_delayFirst), ValueConversion::plainToNormDelay(value));
		controller->performEdit(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_textEdit_delayFirst), ValueConversion::plainToNormDelay(value));
		guiElements[id_allpass_knob_delayFirst + (tag - id_allpass_textEdit_delayFirst)]->setValue(ValueConversion::plainToNormDelay(value));
		guiElements[id_allpass_textEdit_samplesDelayFirst + (tag - id_allpass_textEdit_delayFirst)]->setValue(value * sampleRate / 1000);
		guiElements[id_allpass_textEdit_diffKFirst + (tag - id_allpass_textEdit_delayFirst)]
			->setValue(ValueConversion::calculateDiffK(value, guiElements[id_allpass_textEdit_decayFirst + (tag - id_allpass_textEdit_delayFirst)]->getValue()));
	}
	else if (tag >= id_allpass_textEdit_samplesDelayFirst && tag <= id_allpass_textEdit_samplesDelayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_textEdit_samplesDelayFirst), ValueConversion::plainToNormDelay(ValueConversion::delaySamplesToMilliseconds(value)));
		controller->performEdit(PARAM_ALLPASSDELAY_FIRST + (tag - id_allpass_textEdit_samplesDelayFirst), ValueConversion::plainToNormDelay(ValueConversion::delaySamplesToMilliseconds(value)));
		guiElements[id_allpass_knob_delayFirst + (tag - id_allpass_textEdit_samplesDelayFirst)]->setValue(ValueConversion::plainToNormDelay(ValueConversion::delaySamplesToMilliseconds(value)));
		guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_textEdit_samplesDelayFirst)]->setValue(ValueConversion::delaySamplesToMilliseconds(value));
		guiElements[id_allpass_textEdit_diffKFirst + (tag - id_allpass_textEdit_samplesDelayFirst)]
			->setValue(ValueConversion::calculateDiffK(guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_textEdit_samplesDelayFirst)]->getValue(), 
			guiElements[id_allpass_textEdit_decayFirst + (tag - id_allpass_textEdit_samplesDelayFirst)]->getValue()));
	}
	else if (tag >= id_allpass_knob_decayFirst && tag <= id_allpass_knob_decayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_knob_decayFirst), value);
		controller->performEdit(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_knob_decayFirst), value);
		guiElements[id_allpass_textEdit_decayFirst + (tag - id_allpass_knob_decayFirst)]->setValue(ValueConversion::normToPlainDecay(value));
		guiElements[id_allpass_textEdit_decayFirst + (tag - id_allpass_knob_decayFirst)]->invalid();
		guiElements[id_allpass_textEdit_diffKFirst + (tag - id_allpass_knob_decayFirst)]
			->setValue(ValueConversion::calculateDiffK(guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_knob_decayFirst)]->getValue(), ValueConversion::normToPlainDecay(value)));
	}
	else if (tag >= id_allpass_textEdit_decayFirst && tag <= id_allpass_textEdit_decayLast)  {
		controller->setParamNormalized(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_textEdit_decayFirst), ValueConversion::plainToNormDecay(value));
		controller->performEdit(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_textEdit_decayFirst), ValueConversion::plainToNormDecay(value));
		guiElements[id_allpass_knob_decayFirst + (tag - id_allpass_textEdit_decayFirst)]->setValue(ValueConversion::plainToNormDecay(value));
		guiElements[id_allpass_knob_decayFirst + (tag - id_allpass_textEdit_decayFirst)]->setDirty();
		guiElements[id_allpass_textEdit_diffKFirst + (tag - id_allpass_textEdit_decayFirst)]
			->setValue(ValueConversion::calculateDiffK(guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_textEdit_decayFirst)]->getValue(), value));
	}
	else if (tag >= id_allpass_textEdit_diffKFirst && tag <= id_allpass_textEdit_diffKLast) {
		controller->setParamNormalized(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_textEdit_diffKFirst), ValueConversion::plainToNormDecay(ValueConversion::diffKToDecay(value, guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_textEdit_diffKFirst)]->getValue())));
		controller->performEdit(PARAM_ALLPASSDECAY_FIRST + (tag - id_allpass_textEdit_diffKFirst), ValueConversion::plainToNormDecay(ValueConversion::diffKToDecay(value, guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_textEdit_diffKFirst)]->getValue())));
		guiElements[id_allpass_knob_decayFirst + (tag - id_allpass_textEdit_diffKFirst)]->setValue(ValueConversion::plainToNormDecay(ValueConversion::diffKToDecay(value, guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_textEdit_diffKFirst)]->getValue())));
		guiElements[id_allpass_textEdit_decayFirst + (tag - id_allpass_textEdit_diffKFirst)]->setValue(ValueConversion::diffKToDecay(value, guiElements[id_allpass_textEdit_delayFirst + (tag - id_allpass_textEdit_diffKFirst)]->getValue()));
	}
	else if (tag >= id_allpass_optionMenu_diffKSignFirst && tag <= id_allpass_optionMenu_diffKSignLast) {
		controller->setParamNormalized(PARAM_ALLPASSDIFFKSIGN_FIRST + (tag - id_allpass_optionMenu_diffKSignFirst), value);
		controller->performEdit(PARAM_ALLPASSDIFFKSIGN_FIRST + (tag - id_allpass_optionMenu_diffKSignFirst), value);
	}
	else if (tag >= id_allpass_checkBox_modulationEnabledFirst && tag <= id_allpass_checkBox_modulationEnabledLast) {
		controller->setParamNormalized(PARAM_ALLPASSMODENABLED_FIRST + (tag - id_allpass_checkBox_modulationEnabledFirst), value);
		controller->performEdit(PARAM_ALLPASSMODENABLED_FIRST + (tag - id_allpass_checkBox_modulationEnabledFirst), value);
	}
	else if (tag >= id_allpass_optionMenu_modulationSignalTypeFirst && tag <= id_allpass_optionMenu_modulationSignalTypeLast) {
		controller->setParamNormalized(PARAM_ALLPASSMODSIGNALTYPE_FIRST + (tag - id_allpass_optionMenu_modulationSignalTypeFirst), ValueConversion::plainToNormModSignalType(value));
		controller->performEdit(PARAM_ALLPASSMODSIGNALTYPE_FIRST + (tag - id_allpass_optionMenu_modulationSignalTypeFirst), ValueConversion::plainToNormModSignalType(value));
	}
	else if (tag >= id_allpass_textEdit_modulationExcursionFirst && tag <= id_allpass_textEdit_modulationExcursionLast) {
		controller->setParamNormalized(PARAM_ALLPASSMODEXCURSION_FIRST + (tag - id_allpass_textEdit_modulationExcursionFirst), pControl->getValueNormalized());
		controller->performEdit(PARAM_ALLPASSMODEXCURSION_FIRST + (tag - id_allpass_textEdit_modulationExcursionFirst), pControl->getValueNormalized());
	}
	else if (tag >= id_allpass_textEdit_modulationRateFirst && tag <= id_allpass_textEdit_modulationRateLast) {
		controller->setParamNormalized(PARAM_ALLPASSMODRATE_FIRST + (tag - id_allpass_textEdit_modulationRateFirst), pControl->getValueNormalized());
		controller->performEdit(PARAM_ALLPASSMODRATE_FIRST + (tag - id_allpass_textEdit_modulationRateFirst), pControl->getValueNormalized());
	}
	else if (tag >= id_allpass_switch_bypassFirst && tag <= id_allpass_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_ALLPASSBYPASS_FIRST + (tag - id_allpass_switch_bypassFirst), value);
		controller->performEdit(PARAM_ALLPASSBYPASS_FIRST + (tag - id_allpass_switch_bypassFirst), value);
	}
	// Output
	else if (tag >= id_output_knob_gainFirst && tag <= id_output_knob_gainLast)  {
		controller->setParamNormalized(PARAM_OUTGAIN_FIRST + (tag - id_output_knob_gainFirst), value);
		controller->performEdit(PARAM_OUTGAIN_FIRST + (tag - id_output_knob_gainFirst), value);
		guiElements[id_output_textEdit_gainFirst + (tag - id_output_knob_gainFirst)]->setValue(ValueConversion::normToPlainOutputGain(value));
		guiElements[id_output_textEdit_gainFirst + (tag - id_output_knob_gainFirst)]->invalid();
	}
	else if (tag >= id_output_textEdit_gainFirst && tag <= id_output_textEdit_gainLast)  {
		controller->setParamNormalized(PARAM_OUTGAIN_FIRST + (tag - id_output_textEdit_gainFirst), ValueConversion::plainToNormOutputGain(value));
		controller->performEdit(PARAM_OUTGAIN_FIRST + (tag - id_output_textEdit_gainFirst), ValueConversion::plainToNormOutputGain(value));
		guiElements[id_output_knob_gainFirst + (tag - id_output_textEdit_gainFirst)]->setValue(ValueConversion::plainToNormOutputGain(value));
		guiElements[id_output_knob_gainFirst + (tag - id_output_textEdit_gainFirst)]->setDirty();
	}
	else if (tag >= id_output_checkbox_limiterFirst && tag <= id_output_checkbox_limiterLast)  {
		controller->setParamNormalized(PARAM_OUTLIMITER_FIRST + (tag - id_output_checkbox_limiterFirst), value);
		controller->performEdit(PARAM_OUTLIMITER_FIRST + (tag - id_output_checkbox_limiterFirst), value);
	}
	else if (tag >= id_output_switch_bypassFirst && tag <= id_output_switch_bypassLast)  {
		controller->setParamNormalized(PARAM_OUTBYPASS_FIRST + (tag - id_output_switch_bypassFirst), value);
		controller->performEdit(PARAM_OUTBYPASS_FIRST + (tag - id_output_switch_bypassFirst), value);
	}
	// General
	else if (tag >= id_general_checkBox_moduleVisibleFirst && tag <= id_general_checkBox_moduleVisibleLast) {
		controller->setParamNormalized(PARAM_MODULEVISIBLE_FIRST + (tag - id_general_checkBox_moduleVisibleFirst), value);
		controller->performEdit(PARAM_MODULEVISIBLE_FIRST + (tag - id_general_checkBox_moduleVisibleFirst), value);
		apGuiModules[tag - id_general_checkBox_moduleVisibleFirst]->setVisible(value != 0.0);
		if (value == 1.0) {
			// Bring the current visible view to the top
			workspaceView->changeViewZOrder(apGuiModules[tag - id_general_checkBox_moduleVisibleFirst], workspaceView->getNbViews() - 1);
			// Update the graphics view
			graphicsView->makeModuleVisible(tag - id_general_checkBox_moduleVisibleFirst, true);
		}
		workspaceView->invalid();
	}
	else if (tag >= id_general_optionMenu_vstOutputFirst && tag <= id_general_optionMenu_vstOutputLast) {
		controller->setParamNormalized(PARAM_GENERALVSTOUTPUTSELECT_FIRST + (tag - id_general_optionMenu_vstOutputFirst), ValueConversion::plainToNormMixerInputSelect(value));
		controller->performEdit(PARAM_GENERALVSTOUTPUTSELECT_FIRST + (tag - id_general_optionMenu_vstOutputFirst), ValueConversion::plainToNormMixerInputSelect(value));
		if (value != 0.0) {
			graphicsView->setVstOutputConnection(tag - id_general_optionMenu_vstOutputFirst, value - 1.0);
		}
		else {
			graphicsView->setVstOutputConnection(tag - id_general_optionMenu_vstOutputFirst, -1);
		}
	}
	else if (tag == id_general_textEdit_presetFilePath) {
		editorUserData.presetName = dynamic_cast<VSTGUI::CTextEdit*>(pControl)->getText();
	}
	else if (tag == id_general_button_openPreset) {
		if (pControl->getValue() == 1.0) {
			CNewFileSelector* fileSelector = CNewFileSelector::create(this->getFrame(), CNewFileSelector::kSelectFile);
			if (fileSelector) {
				fileSelector->addFileExtension(CFileExtension("*.xml", "xml"));
				fileSelector->setDefaultExtension(CFileExtension("*.xml", "xml"));
				fileSelector->setTitle("Choose Preset XML file");
				fileSelector->setAllowMultiFileSelection(false);
				fileSelectorStyle = CNewFileSelector::kSelectFile;
				fileSelector->run(this);
				fileSelector->forget();
				pControl->setDirty();
			}
		}
	}
	else if (tag == id_general_button_savePreset) {
		if (pControl->getValue() == 1.0) {
			CNewFileSelector* fileSelector = CNewFileSelector::create(this->getFrame(), CNewFileSelector::kSelectSaveFile);
			if (fileSelector) {
				fileSelector->addFileExtension(CFileExtension("*.xml", "xml"));
				fileSelector->setDefaultExtension(CFileExtension("*.xml", "xml"));
				fileSelector->setTitle("Save Preset XML file");
				fileSelector->setAllowMultiFileSelection(false);
				fileSelectorStyle = CNewFileSelector::kSelectSaveFile;
				fileSelector->setDefaultSaveName(dynamic_cast<VSTGUI::CTextEdit*>(guiElements[id_general_textEdit_presetFilePath])->getText());
				fileSelector->run(this);
				fileSelector->forget();
				pControl->setDirty();
			}
		}
	}
	else if (tag == id_general_button_about) {
		if (pControl->getValue() == 1.0) {
			dynamic_cast<GuiCustomSplashScreen*>(guiElements[id_general_splashScreen_about])->splash();
			frame->setMouseableArea(guiElements[id_general_splashScreen_about]->getViewSize());
		}
	}
	else if (tag == id_general_button_splashViewOkAbout) {
		if (pControl->getValue() == 1.0) {
			dynamic_cast<GuiCustomSplashScreen*>(guiElements[id_general_splashScreen_about])->unSplash();
			frame->setMouseableArea(frame->getViewSize());
		}
	}
	else if (tag == id_graphicsView_rearrangeModules) {
		if (value == 0.0) {
			graphicsView->rearrangeModules();
		}
	}
	else if (tag == id_graphicsView_addModule) {
		if (value == 0.0) {
			graphicsView->addModule();
		}
	}
}

void ReverbNetworkEditor::controlBeginEdit(CControl* pControl)
{
	/*int32_t tag = pControl->getTag();
	if (tag >= PARAM_EQCENTERFREQ_FIRST && tag <= PARAM_EQCENTERFREQ_LAST) {
		controller->beginEdit(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_knob_centerFreqFirst));
	}*/
}

void ReverbNetworkEditor::controlEndEdit(CControl* pControl)
{
	/*int32_t tag = pControl->getTag();
	if (tag >= PARAM_EQCENTERFREQ_FIRST && tag <= PARAM_EQCENTERFREQ_LAST) {
		controller->endEdit(PARAM_EQCENTERFREQ_FIRST + (tag - id_equalizer_knob_centerFreqFirst));
	}*/
}

GuiBaseAPModule* ReverbNetworkEditor::createAPModule() {
	// Id in order to identify the module
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

	// Handle view to grab and move the module with the mouse
	GuiCustomRowColumnView* handleView = new GuiCustomRowColumnView(CRect(0, 0, 710, 25), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy, 5.0, CRect(1.0, 1.0, 1.0, 1.0));
	handleView->setFrameWidth(1);
	handleView->setFrameColor(CCOLOR_MODULE_HANDLEFRAME);
	handleView->setBackgroundColor(CCOLOR_MODULE_HANDLEBACKGROUND);

	VSTGUI::CTextButton* closeViewButton = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(16, 16)), this, id_module_button_hideFirst + moduleId, "X");
	addGuiElementPointer(closeViewButton, id_module_button_hideFirst + moduleId);
	//closeViewButton->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//closeViewButton->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//closeViewButton->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//closeViewButton->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	closeViewButton->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	closeViewButton->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	closeViewButton->setRoundRadius(1);
	VSTGUI::CTextButton* hideViewButton = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(16, 16)), this, id_module_button_collapseFirst + moduleId, "^");
	addGuiElementPointer(hideViewButton, id_module_button_collapseFirst + moduleId);
	//hideViewButton->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//hideViewButton->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//hideViewButton->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//hideViewButton->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	hideViewButton->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	hideViewButton->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	hideViewButton->setRoundRadius(1);
	hideViewButton->setStyle(VSTGUI::CTextButton::kOnOffStyle);
	VSTGUI::CTextButton* defaultParametersButton = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(56, 16)), this, id_module_button_defaultParametersFirst + moduleId, "Default");
	addGuiElementPointer(defaultParametersButton, id_module_button_defaultParametersFirst + moduleId);
	//defaultParametersButton->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//defaultParametersButton->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//defaultParametersButton->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//defaultParametersButton->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	defaultParametersButton->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	defaultParametersButton->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	defaultParametersButton->setRoundRadius(1);
	defaultParametersButton->setFont(kNormalFontSmall);
	VSTGUI::CTextButton* copyParametersButton = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(56, 16)), this, id_module_button_copyParametersFirst + moduleId, "Copy");
	addGuiElementPointer(copyParametersButton, id_module_button_copyParametersFirst + moduleId);
	//copyParametersButton->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//copyParametersButton->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//copyParametersButton->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//copyParametersButton->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	copyParametersButton->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	copyParametersButton->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	copyParametersButton->setRoundRadius(1);
	copyParametersButton->setFont(kNormalFontSmall);
	VSTGUI::CTextButton* pasteParametersButton = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(56, 16)), this, id_module_button_pasteParametersFirst + moduleId, "Paste");
	addGuiElementPointer(pasteParametersButton, id_module_button_pasteParametersFirst + moduleId);
	//pasteParametersButton->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//pasteParametersButton->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//pasteParametersButton->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//pasteParametersButton->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	pasteParametersButton->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	pasteParametersButton->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	pasteParametersButton->setRoundRadius(1);
	pasteParametersButton->setFont(kNormalFontSmall);

	CRect handleViewSize = handleView->getViewSize();
	handleViewSize.setWidth(handleViewSize.getWidth() - (closeViewButton->getWidth() + hideViewButton->getWidth() + defaultParametersButton->getWidth() + 
		copyParametersButton->getWidth() + pasteParametersButton->getWidth() + 26));

	/*GuiBaseAPModule* baseModuleView = new GuiBaseAPModule(CRect(CPoint(0 + (totalNumberOfCreatedModules % 10) * 30, 0 + (totalNumberOfCreatedModules % 10) * 30),
		CPoint(0, 0)), handleViewSize, moduleId, this);
	baseModuleView->setBackgroundColor(CCOLOR_MODULE_MAINBACKGROUND);*/
	GuiBaseAPModule* baseModuleView = new GuiBaseAPModule(CRect(CPoint(0, 0), CPoint(0, 0)), handleViewSize, moduleId, this);
	baseModuleView->setBackgroundColor(CCOLOR_MODULE_MAINBACKGROUND);

	std::string temp = "APM";
	temp.append(std::to_string(moduleId));
	temp.append(" ");
	GuiCustomTextEdit* moduleTitle = new GuiCustomTextEdit(handleViewSize, this, id_module_textEdit_titleFirst + moduleId, temp.c_str());
	moduleTitle->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	moduleTitle->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	addGuiElementPointer(moduleTitle, id_module_textEdit_titleFirst + moduleId);
	moduleTitle->setStringToTruncate(temp);
	moduleTitle->setBackColor(CCOLOR_TEXTLABEL_BACKGROUND);
	moduleTitle->setFrameColor(CCOLOR_TEXTLABEL_FRAME);

	handleView->addView(moduleTitle);
	handleView->addView(defaultParametersButton);
	handleView->addView(copyParametersButton);
	handleView->addView(pasteParametersButton);
	handleView->addView(hideViewButton);
	handleView->addView(closeViewButton);
	
	// Control view which holds the individual processing modules
	GuiCustomRowColumnView* controlView = new GuiCustomRowColumnView(CRect(0, handleView->getHeight(), handleView->getWidth(), 290), GuiCustomRowColumnView::kColumnStyle, GuiCustomRowColumnView::kLeftTopEqualy, 3.0, CRect(1.0, 1.0, 1.0, 1.0));
	controlView->setBackgroundColor(CCOLOR_NOCOLOR);
	controlView->setFrameWidth(1);
	controlView->setFrameColor(CCOLOR_MODULE_MAINFRAME);

	// Add process views to the control view
	controlView->addView(createMixer(controlView->getViewSize(), moduleId));
	controlView->addView(createQuantizer(controlView->getViewSize(), moduleId));
	controlView->addView(createEqualizer(controlView->getViewSize(), moduleId));
	controlView->addView(createAllpass(controlView->getViewSize(), moduleId));
	controlView->addView(createOutput(controlView->getViewSize(), moduleId));

	baseModuleView->addView(handleView);
	baseModuleView->addView(controlView);
	baseModuleView->sizeToFit();

	workspaceView->addView(baseModuleView);

	++totalNumberOfCreatedModules;

	return baseModuleView;
}

GuiCustomRowColumnView* ReverbNetworkEditor::createMixer(const CRect& parentViewSize, const int& moduleId) {
	// Holds the input mixer controls (input gain for each channel)
	GuiCustomRowColumnView* mixerMainView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(220, parentViewSize.getHeight())), GuiCustomRowColumnView::kRowStyle, GuiCustomRowColumnView::kLeftTopEqualy, 5.0, CRect(0.0, 0.0, 0.0, 0.0));
	GuiCustomRowColumnView* mixerView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kRowStyle, GuiCustomRowColumnView::kLeftTopEqualy, 0.0);
	for (uint32 i = 0; i < MAXMODULEINPUTS; ++i) {
		/*temp = "IN ";
		temp.append(std::to_string(i));
		temp.append(":");*/
		mixerView->addView(createMixerRow("", mixerMainView->getWidth(), i + moduleId * MAXMODULEINPUTS));
	}
	mixerView->setBackgroundColor(CCOLOR_NOCOLOR);
	mixerView->sizeToFit();
	CScrollView* mixerScrollView = new CScrollView(CRect(CPoint(0, 0), CPoint(mixerMainView->getWidth(), parentViewSize.getHeight() - 50)), CRect(CPoint(0, 0), CPoint(0, 0)), CScrollView::kVerticalScrollbar, 10.0);
	mixerScrollView->addView(mixerView);
	mixerScrollView->setBackgroundColor(CCOLOR_NOCOLOR);
	mixerScrollView->sizeToFit();
	mixerScrollView->getVerticalScrollbar()->setScrollerColor(CCOLOR_NOCOLOR);
	if (mixerView->getHeight() < mixerScrollView->getHeight()) {
		mixerScrollView->getVerticalScrollbar()->setVisible(false);
	}
	mixerMainView->setBackgroundColor(CCOLOR_NOCOLOR);
	mixerMainView->addView(createGroupTitle("INPUT MIXER", mixerMainView->getWidth()));
	mixerMainView->addView(mixerScrollView);
	return mixerMainView;
}

GuiCustomRowColumnView* ReverbNetworkEditor::createQuantizer(const CRect& parentViewSize, const int& moduleId) {
	// Holds the quantizer controls
	GuiCustomRowColumnView* quantizerView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(90, parentViewSize.getHeight())), GuiCustomRowColumnView::kRowStyle, GuiCustomRowColumnView::kLeftTopEqualy, 5.0, CRect(0.0, 0.0, 0.0, 0.0));
	quantizerView->setBackgroundColor(CCOLOR_NOCOLOR);
	quantizerView->addView(createGroupTitle("QUANTIZER", quantizerView->getWidth()));
	CCheckBox* checkBoxQuantizerBypass = new CCheckBox(CRect(CPoint(0, 0), CPoint(60, 15)), this, id_quantizer_switch_bypassFirst + moduleId, "Bypass");
	addGuiElementPointer(checkBoxQuantizerBypass, id_quantizer_switch_bypassFirst + moduleId);
	quantizerView->addView(checkBoxQuantizerBypass);
	quantizerView->addView(createKnobGroup("Quantization", quantizerView->getWidth(), id_quantizer_knob_quantizationFirst + moduleId, id_quantizer_textEdit_quantizationFirst + moduleId,
		MIN_QUANTIZERBITDEPTH, MAX_QUANTIZERBITDEPTH, 0, UNIT_QUANTIZERBITDEPTH));
	return quantizerView;
}

GuiCustomRowColumnView* ReverbNetworkEditor::createEqualizer(const CRect& parentViewSize, const int& moduleId) {
	// Holds the equalizer controls
	GuiCustomRowColumnView* equalizerView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(150, parentViewSize.getHeight())), GuiCustomRowColumnView::kRowStyle, GuiCustomRowColumnView::kLeftTopEqualy, 5.0, CRect(0.0, 0.0, 0.0, 0.0));
	equalizerView->setBackgroundColor(CCOLOR_NOCOLOR);
	GuiCustomRowColumnView* filterTypeView = new GuiCustomRowColumnView(CRect(0, 0, 0, 0), GuiCustomRowColumnView::kColumnStyle);
	filterTypeView->setBackgroundColor(CCOLOR_NOCOLOR);
	GuiCustomTextLabel* filterTypeTextLabel = new GuiCustomTextLabel(CRect(0, 0, 64, 20), "Filter Type:", kNormalFontSmall);
	GuiOptionMenuInputSelector* filterTypeMenu = new GuiOptionMenuInputSelector(CRect(0, 0, 85, 20), this, id_equalizer_optionMenu_filterTypeFirst + moduleId);
	addGuiElementPointer(filterTypeMenu, id_equalizer_optionMenu_filterTypeFirst + moduleId);
	filterTypeMenu->setBackColor(CCOLOR_OPTIONMENU_BACKGROUND);
	filterTypeMenu->setFrameColor(CCOLOR_OPTIONMENU_FRAME);
	filterTypeMenu->setFont(CFontRef(kNormalFontSmall));
	filterTypeMenu->addEntry("Low Pass");
	filterTypeMenu->addEntry("High Pass");
	filterTypeMenu->addEntry("Band Pass/Stop");
	filterTypeMenu->addEntry("Low Shelf");
	filterTypeMenu->addEntry("High Shelf");
	filterTypeMenu->addEntry("Raw Biquad");
	filterTypeMenu->setCurrent(0);
	filterTypeView->addView(filterTypeTextLabel);
	filterTypeView->addView(filterTypeMenu);
	filterTypeView->sizeToFit();
	GuiCustomRowColumnView* paramFirstRow = new GuiCustomRowColumnView(CRect(0, 0, 0, 0), GuiCustomRowColumnView::kColumnStyle);
	paramFirstRow->setBackgroundColor(CCOLOR_NOCOLOR);
	paramFirstRow->addView(createKnobGroup("Frequency", equalizerView->getWidth() / 2, id_equalizer_knob_centerFreqFirst + moduleId, id_equalizer_textEdit_centerFreqFirst + moduleId,
		MIN_EQCENTERFREQ, ValueConversion::getMaxEqFrequency(), 2, UNIT_EQCENTERFREQ));
	paramFirstRow->addView(createKnobGroup("QFactor", equalizerView->getWidth() / 2, id_equalizer_knob_qFactorFirst + moduleId, id_equalizer_textEdit_qFactorFirst + moduleId,
		MIN_EQQFACTOR, MAX_EQQFACTOR, 2, UNIT_EQQFACTOR));
	paramFirstRow->sizeToFit();
	CCheckBox* checkBoxEqualizerBypass = new CCheckBox(CRect(CPoint(50, 0), CPoint(60, 15)), this, id_equalizer_switch_bypassFirst + moduleId, "Bypass");
	addGuiElementPointer(checkBoxEqualizerBypass, id_equalizer_switch_bypassFirst + moduleId);
	VSTGUI::CTextButton* buttonStability = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(equalizerView->getWidth() / 2 - 10, 15)), this, id_equalizer_button_stabilityFirst + moduleId, "", VSTGUI::CTextButton::kOnOffStyle);
	addGuiElementPointer(buttonStability, id_equalizer_button_stabilityFirst + moduleId);
	//buttonStability->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//buttonStability->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	//buttonStability->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
	//buttonStability->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
	buttonStability->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
	buttonStability->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
	buttonStability->setRoundRadius(1);
	buttonStability->setMouseEnabled(false);
	buttonStability->setFont(CFontRef(kNormalFontSmall));
	CCheckBox* checkBoxLimiter = new CCheckBox(CRect(CPoint(0, 0), CPoint(equalizerView->getWidth() / 2, 15)), this, id_equalizer_checkBox_limiterFirst + moduleId, "Limiter");
	addGuiElementPointer(checkBoxLimiter, id_equalizer_checkBox_limiterFirst + moduleId);
	checkBoxLimiter->setFont(CFontRef(kNormalFontSmall));
	CRowColumnView* viewStabilityLimiter = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle, CRowColumnView::kLeftTopEqualy, 10.0);
	viewStabilityLimiter->setBackgroundColor(CCOLOR_NOCOLOR);
	viewStabilityLimiter->addView(buttonStability);
	viewStabilityLimiter->addView(checkBoxLimiter);
	viewStabilityLimiter->sizeToFit();
	equalizerView->addView(createGroupTitle("EQUALIZER", equalizerView->getWidth()));
	equalizerView->addView(checkBoxEqualizerBypass);
	equalizerView->addView(viewStabilityLimiter);
	equalizerView->addView(filterTypeView);
	GuiCustomRowColumnView* equalizerNormalView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kRowStyle);
	equalizerNormalView->setBackgroundColor(CCOLOR_NOCOLOR);
	equalizerNormalView->addView(paramFirstRow);
	equalizerNormalView->addView(createKnobGroup("Gain", equalizerView->getWidth(), id_equalizer_knob_gainFirst + moduleId, id_equalizer_textEdit_gainFirst + moduleId,
		MIN_EQGAIN, MAX_EQGAIN, 2, UNIT_EQGAIN));
	equalizerNormalView->sizeToFit();
	GuiCustomRowColumnView* equalizerRawView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kRowStyle);
	equalizerRawView->setBackgroundColor(CCOLOR_NOCOLOR);
	GuiCustomTextLabel* labelTitle = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(100, 20)), "Biquad Cofficients:");
	GuiCustomRowColumnView* a0View = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(equalizerView->getWidth(), 20)), GuiCustomRowColumnView::kColumnStyle);
	a0View->setBackgroundColor(CCOLOR_NOCOLOR);
	GuiCustomTextLabel* labelA0 = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(20, 20)), "a0:", kNormalFontSmall);
	GuiCustomValueEdit* editA0 = new GuiCustomValueEdit(CRect(CPoint(0, 0), CPoint(100, 20)), this, id_equalizer_textEdit_a0First + moduleId);
	addGuiElementPointer(editA0, id_equalizer_textEdit_a0First + moduleId);
	editA0->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	editA0->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	editA0->setFont(CFontRef(kNormalFontSmall));
	valueToStringUserData userData;
	userData.precision = 5;
	userData.unit = "";
	editA0->setValueToStringUserData(userData);
	editA0->setStringToValueFunction(&ValueConversion::textEditStringToValueConversion);
	editA0->setValueToStringFunction(&ValueConversion::textEditValueToStringConversion);
	editA0->setMin(MIN_EQCOEFFICIENTS);
	editA0->setMax(MAX_EQCOEFFICIENTS);
	editA0->setPrecision(5);
	a0View->addView(labelA0);
	a0View->addView(editA0);
	GuiCustomRowColumnView* a1View = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(equalizerView->getWidth(), 20)), GuiCustomRowColumnView::kColumnStyle);
	a1View->setBackgroundColor(CCOLOR_NOCOLOR);
	GuiCustomTextLabel* labelA1 = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(20, 20)), "a1:", kNormalFontSmall);
	GuiCustomValueEdit* editA1 = new GuiCustomValueEdit(CRect(CPoint(0, 0), CPoint(100, 20)), this, id_equalizer_textEdit_a1First + moduleId);
	addGuiElementPointer(editA1, id_equalizer_textEdit_a1First + moduleId);
	editA1->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	editA1->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	editA1->setFont(CFontRef(kNormalFontSmall));
	valueToStringUserData userData2;
	userData2.precision = 5;
	userData2.unit = "";
	editA1->setValueToStringUserData(userData2);
	editA1->setStringToValueFunction(&ValueConversion::textEditStringToValueConversion);
	editA1->setValueToStringFunction(&ValueConversion::textEditValueToStringConversion);
	editA1->setMin(MIN_EQCOEFFICIENTS);
	editA1->setMax(MAX_EQCOEFFICIENTS);
	editA1->setPrecision(5);
	a1View->addView(labelA1);
	a1View->addView(editA1);
	GuiCustomRowColumnView* a2View = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(equalizerView->getWidth(), 20)), GuiCustomRowColumnView::kColumnStyle);
	a2View->setBackgroundColor(CCOLOR_NOCOLOR);
	GuiCustomTextLabel* labelA2 = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(20, 20)), "a2:", kNormalFontSmall);
	GuiCustomValueEdit* editA2 = new GuiCustomValueEdit(CRect(CPoint(0, 0), CPoint(100, 20)), this, id_equalizer_textEdit_a2First + moduleId);
	addGuiElementPointer(editA2, id_equalizer_textEdit_a2First + moduleId);
	editA2->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	editA2->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	editA2->setFont(CFontRef(kNormalFontSmall));
	valueToStringUserData userData3;
	userData3.precision = 5;
	userData3.unit = "";
	editA2->setValueToStringUserData(userData3);
	editA2->setStringToValueFunction(&ValueConversion::textEditStringToValueConversion);
	editA2->setValueToStringFunction(&ValueConversion::textEditValueToStringConversion);
	editA2->setMin(MIN_EQCOEFFICIENTS);
	editA2->setMax(MAX_EQCOEFFICIENTS);
	editA2->setPrecision(5);
	a2View->addView(labelA2);
	a2View->addView(editA2);
	GuiCustomRowColumnView* b1View = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(equalizerView->getWidth(), 20)), GuiCustomRowColumnView::kColumnStyle);
	b1View->setBackgroundColor(CCOLOR_NOCOLOR);
	GuiCustomTextLabel* labelB1 = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(20, 20)), "b1:", kNormalFontSmall);
	GuiCustomValueEdit* editB1 = new GuiCustomValueEdit(CRect(CPoint(0, 0), CPoint(100, 20)), this, id_equalizer_textEdit_b1First + moduleId);
	addGuiElementPointer(editB1, id_equalizer_textEdit_b1First + moduleId);
	editB1->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	editB1->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	editB1->setFont(CFontRef(kNormalFontSmall));
	valueToStringUserData userData4;
	userData4.precision = 5;
	userData4.unit = "";
	editB1->setValueToStringUserData(userData4);
	editB1->setStringToValueFunction(&ValueConversion::textEditStringToValueConversion);
	editB1->setValueToStringFunction(&ValueConversion::textEditValueToStringConversion);
	editB1->setMin(MIN_EQCOEFFICIENTS);
	editB1->setMax(MAX_EQCOEFFICIENTS);
	editB1->setPrecision(5);
	b1View->addView(labelB1);
	b1View->addView(editB1);
	GuiCustomRowColumnView* b2View = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(equalizerView->getWidth(), 20)), GuiCustomRowColumnView::kColumnStyle);
	b2View->setBackgroundColor(CCOLOR_NOCOLOR);
	GuiCustomTextLabel* labelB2 = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(20, 20)), "b2:", kNormalFontSmall);
	GuiCustomValueEdit* editB2 = new GuiCustomValueEdit(CRect(CPoint(0, 0), CPoint(100, 20)), this, id_equalizer_textEdit_b2First + moduleId);
	addGuiElementPointer(editB2, id_equalizer_textEdit_b2First + moduleId);
	editB2->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	editB2->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	editB2->setFont(CFontRef(kNormalFontSmall));
	valueToStringUserData userData5;
	userData5.precision = 5;
	userData5.unit = "";
	editB2->setValueToStringUserData(userData5);
	editB2->setStringToValueFunction(&ValueConversion::textEditStringToValueConversion);
	editB2->setValueToStringFunction(&ValueConversion::textEditValueToStringConversion);
	editB2->setMin(MIN_EQCOEFFICIENTS);
	editB2->setMax(MAX_EQCOEFFICIENTS);
	editB2->setPrecision(5);
	b2View->addView(labelB2);
	b2View->addView(editB2);
	equalizerRawView->addView(labelTitle);
	equalizerRawView->addView(a0View);
	equalizerRawView->addView(a1View);
	equalizerRawView->addView(a2View);
	equalizerRawView->addView(b1View);
	equalizerRawView->addView(b2View);
	equalizerRawView->sizeToFit();
	CViewContainer* layeredView = new CViewContainer(CRect(CPoint(0, 0), CPoint(0, 0)));
	layeredView->setBackgroundColor(CCOLOR_NOCOLOR);
	layeredView->addView(equalizerNormalView);
	layeredView->addView(equalizerRawView);
	layeredView->sizeToFit();
	equalizerView->addView(layeredView);
	equalizerRawView->setVisible(false);
	return equalizerView;
}

GuiCustomRowColumnView* ReverbNetworkEditor::createAllpass(const CRect& parentViewSize, const int& moduleId) {
	// Holds the allpass controls (delay and decay)
	GuiCustomRowColumnView* allpassView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(160, parentViewSize.getHeight())), GuiCustomRowColumnView::kRowStyle, GuiCustomRowColumnView::kLeftTopEqualy, 5.0, CRect(0.0, 0.0, 0.0, 0.0));
	allpassView->setBackgroundColor(CCOLOR_NOCOLOR);
	CCheckBox *checkBoxAllpassBypass = new CCheckBox(CRect(CPoint(50, 0), CPoint(60, 15)), this, id_allpass_switch_bypassFirst + moduleId, "Bypass");
	addGuiElementPointer(checkBoxAllpassBypass, id_allpass_switch_bypassFirst + moduleId);
	allpassView->addView(createGroupTitle("ALLPASS", allpassView->getWidth()));
	allpassView->addView(checkBoxAllpassBypass);
	GuiCustomRowColumnView* diffKView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kColumnStyle);
	diffKView->setBackgroundColor(CCOLOR_NOCOLOR);
	VSTGUI::COptionMenu* labelDiffK = new VSTGUI::COptionMenu(CRect(CPoint(0, 0), CPoint(20, 15)), this, id_allpass_optionMenu_diffKSignFirst + moduleId);
	addGuiElementPointer(labelDiffK, id_allpass_optionMenu_diffKSignFirst + moduleId);
	labelDiffK->setBackColor(CCOLOR_OPTIONMENU_BACKGROUND);
	labelDiffK->setFrameColor(CCOLOR_OPTIONMENU_FRAME);
	labelDiffK->addEntry("+k:");
	labelDiffK->addEntry("-k:");
	labelDiffK->setCurrent(0);
	labelDiffK->setFont(CFontRef(kNormalFontSmall));
	GuiCustomValueEdit* textEditDiffK = new GuiCustomValueEdit(CRect(CPoint(0.0, 0.0), CPoint(allpassView->getWidth() / 2 - labelDiffK->getWidth(), 15)), this, id_allpass_textEdit_diffKFirst + moduleId);
	addGuiElementPointer(textEditDiffK, id_allpass_textEdit_diffKFirst + moduleId);
	textEditDiffK->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	textEditDiffK->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	valueToStringUserData userData2;
	userData2.precision = 5;
	userData2.unit = "";
	textEditDiffK->setValueToStringUserData(userData2);
	textEditDiffK->setStringToValueFunction(&ValueConversion::textEditStringToValueConversion);
	textEditDiffK->setValueToStringFunction(&ValueConversion::textEditValueToStringConversion);
	textEditDiffK->setMin(0.0);
	textEditDiffK->setMax(1.0);
	textEditDiffK->setFont(CFontRef(kNormalFontSmall));
	diffKView->addView(labelDiffK);
	diffKView->addView(textEditDiffK);
	diffKView->sizeToFit();
	GuiCustomValueEdit* textEditDelayInSamples = new GuiCustomValueEdit(CRect(CPoint(0.0, 0.0), CPoint(allpassView->getWidth() / 2, 15.0)), this, id_allpass_textEdit_samplesDelayFirst + moduleId);
	addGuiElementPointer(textEditDelayInSamples, id_allpass_textEdit_samplesDelayFirst + moduleId);
	textEditDelayInSamples->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	textEditDelayInSamples->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	valueToStringUserData userData;
	userData.precision = 0;
	userData.unit = "samples";
	textEditDelayInSamples->setValueToStringUserData(userData);
	textEditDelayInSamples->setStringToValueFunction(&ValueConversion::textEditStringToValueConversion);
	textEditDelayInSamples->setValueToStringFunction(&ValueConversion::textEditValueToStringConversion);
	textEditDelayInSamples->setMin(0.0);
	textEditDelayInSamples->setStringToTruncate("samples", true);
	textEditDelayInSamples->setMax(sampleRate * MAX_ALLPASSDELAY / 1000);
	textEditDelayInSamples->setFont(CFontRef(kNormalFontSmall));

	CRowColumnView* viewDelayDecay = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle);
	viewDelayDecay->setBackgroundColor(CCOLOR_NOCOLOR);
	CRowColumnView* viewDelay = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	viewDelay->setBackgroundColor(CCOLOR_NOCOLOR);
	CRowColumnView* viewDecay = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	viewDecay->setBackgroundColor(CCOLOR_NOCOLOR);
	viewDelay->addView(createKnobGroup("Delay", allpassView->getWidth() / 2, id_allpass_knob_delayFirst + moduleId, id_allpass_textEdit_delayFirst + moduleId,
		MIN_ALLPASSDELAY, MAX_ALLPASSDELAY, 2, UNIT_ALLPASSDELAY));
	viewDelay->addView(textEditDelayInSamples);
	viewDecay->addView(createKnobGroup("Decay", allpassView->getWidth() / 2, id_allpass_knob_decayFirst + moduleId, id_allpass_textEdit_decayFirst + moduleId,
		MIN_ALLPASSDECAY, MAX_ALLPASSDECAY, 2, UNIT_ALLPASSDECAY));
	viewDecay->addView(diffKView);
	viewDelay->sizeToFit();
	viewDecay->sizeToFit();
	viewDelayDecay->addView(viewDelay);
	viewDelayDecay->addView(viewDecay);
	viewDelayDecay->sizeToFit();

	
	CCheckBox* checkBoxModulationEnabled = new CCheckBox(CRect(CPoint(0, 0), CPoint(allpassView->getWidth(), 15)), this, id_allpass_checkBox_modulationEnabledFirst + moduleId, "Delay Modulation");
	addGuiElementPointer(checkBoxModulationEnabled, id_allpass_checkBox_modulationEnabledFirst + moduleId);
	checkBoxModulationEnabled->setFont(kNormalFontSmall);
	GuiCustomTextLabel* labelSignalType = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(allpassView->getWidth() / 2, 15)), "Waveform:", kNormalFontSmall, CHoriTxtAlign::kLeftText);
	VSTGUI::COptionMenu* optionMenuSignalType = new VSTGUI::COptionMenu(CRect(CPoint(0, 0), CPoint(allpassView->getWidth() / 2, 15)), this, id_allpass_optionMenu_modulationSignalTypeFirst + moduleId);
	addGuiElementPointer(optionMenuSignalType, id_allpass_optionMenu_modulationSignalTypeFirst + moduleId);
	optionMenuSignalType->setBackColor(CCOLOR_OPTIONMENU_BACKGROUND);
	optionMenuSignalType->setFrameColor(CCOLOR_OPTIONMENU_FRAME);
	optionMenuSignalType->addEntry("Sine");
	optionMenuSignalType->addEntry("Triangle");
	optionMenuSignalType->setCurrent(0);
	optionMenuSignalType->setFont(CFontRef(kNormalFontSmall));
	GuiCustomTextLabel* labelExcursion = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(allpassView->getWidth() / 2, 15)), "Excursion:", kNormalFontSmall, CHoriTxtAlign::kLeftText);
	GuiCustomValueEdit* editExcursion = new GuiCustomValueEdit(CRect(CPoint(0, 0), CPoint(allpassView->getWidth() / 2, 15)), this, id_allpass_textEdit_modulationExcursionFirst + moduleId);
	addGuiElementPointer(editExcursion, id_allpass_textEdit_modulationExcursionFirst + moduleId);
	editExcursion->setStringToTruncate(UNIT_ALLPASSMODEXCURSION, true);
	valueToStringUserData userData4;
	userData4.precision = 2;
	userData4.unit = UNIT_ALLPASSMODEXCURSION;
	editExcursion->setValueToStringUserData(userData4);
	editExcursion->setStringToValueFunction(&ValueConversion::textEditStringToValueConversion);
	editExcursion->setValueToStringFunction(&ValueConversion::textEditValueToStringConversion);
	editExcursion->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	editExcursion->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	editExcursion->setFontColor(CCOLOR_TEXTEDIT_TEXT);
	editExcursion->setFont(kNormalFontSmall);
	editExcursion->setMin((float)MIN_ALLPASSMODEXCURSION);
	editExcursion->setMax(MAX_ALLPASSMODEXCURSION);
	editExcursion->setDefaultValue(DEF_ALLPASSMODEXCURSION);
	GuiCustomTextLabel* labelRate = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(allpassView->getWidth() / 2, 15)), "Rate:", kNormalFontSmall, CHoriTxtAlign::kLeftText);
	GuiCustomValueEdit* editRate = new GuiCustomValueEdit(CRect(CPoint(0, 0), CPoint(allpassView->getWidth() / 2, 15)), this, id_allpass_textEdit_modulationRateFirst + moduleId);
	addGuiElementPointer(editRate, id_allpass_textEdit_modulationRateFirst + moduleId);
	editRate->setStringToTruncate(UNIT_ALLPASSMODRATE, true);
	valueToStringUserData userData3;
	userData3.precision = 2;
	userData3.unit = UNIT_ALLPASSMODRATE;
	editRate->setValueToStringUserData(userData3);
	editRate->setStringToValueFunction(&ValueConversion::textEditStringToValueConversion);
	editRate->setValueToStringFunction(&ValueConversion::textEditValueToStringConversion);
	editRate->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	editRate->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	editRate->setFontColor(CCOLOR_TEXTEDIT_TEXT);
	editRate->setFont(kNormalFontSmall);
	editRate->setMin((float)MIN_ALLPASSMODRATE);
	editRate->setMax(MAX_ALLPASSMODRATE);
	editRate->setDefaultValue(DEF_ALLPASSMODRATE);
	CRowColumnView* viewSignalType = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle);
	viewSignalType->setBackgroundColor(CCOLOR_NOCOLOR);
	viewSignalType->addView(labelSignalType);
	viewSignalType->addView(optionMenuSignalType);
	viewSignalType->sizeToFit();
	CRowColumnView* viewExcursion = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle);
	viewExcursion->setBackgroundColor(CCOLOR_NOCOLOR);
	viewExcursion->addView(labelExcursion);
	viewExcursion->addView(editExcursion);
	viewExcursion->sizeToFit();
	CRowColumnView* viewRate = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle);
	viewRate->setBackgroundColor(CCOLOR_NOCOLOR);
	viewRate->addView(labelRate);
	viewRate->addView(editRate);
	viewRate->sizeToFit();
	allpassView->addView(viewDelayDecay);
	allpassView->addView(checkBoxModulationEnabled);
	allpassView->addView(viewSignalType);
	allpassView->addView(viewExcursion);
	allpassView->addView(viewRate);

	return allpassView;
}

GuiCustomRowColumnView* ReverbNetworkEditor::createOutput(const CRect& parentViewSize, const int& moduleId) {
	// Holds the output gain control
	GuiCustomRowColumnView* gainView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(80, parentViewSize.getHeight())), GuiCustomRowColumnView::kRowStyle, GuiCustomRowColumnView::kLeftTopEqualy, 5.0, CRect(0.0, 0.0, 0.0, 0.0));
	CCheckBox *checkBoxGainBypass = new CCheckBox(CRect(CPoint(50, 0), CPoint(60, 15)), this, id_output_switch_bypassFirst + moduleId, "Bypass");
	addGuiElementPointer(checkBoxGainBypass, id_output_switch_bypassFirst + moduleId);
	gainView->addView(createGroupTitle("OUT", gainView->getWidth()));
	gainView->addView(checkBoxGainBypass);
	CCheckBox* checkBoxLimiter = new CCheckBox(CRect(CPoint(0, 0), CPoint(60, 15)), this, id_output_checkbox_limiterFirst + moduleId, "Limiter");
	addGuiElementPointer(checkBoxLimiter, id_output_checkbox_limiterFirst + moduleId);
	checkBoxLimiter->setFont(kNormalFontSmall);
	GuiCustomRowColumnView* knobPpmView = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), GuiCustomRowColumnView::kColumnStyle, GuiCustomRowColumnView::kLeftTopEqualy, 5.0);
	CRowColumnView* viewKnobLimiter = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0);
	viewKnobLimiter->setBackgroundColor(CCOLOR_NOCOLOR);
	viewKnobLimiter->addView(createKnobGroup("Gain", gainView->getWidth() - 20, id_output_knob_gainFirst + moduleId, id_output_textEdit_gainFirst + moduleId,
		MIN_OUTPUTGAIN, MAX_OUTPUTGAIN, 2, UNIT_OUTPUTGAIN));
	viewKnobLimiter->addView(checkBoxLimiter);
	viewKnobLimiter->sizeToFit();
	knobPpmView->addView(viewKnobLimiter);
	CVuMeter* ppm = new CVuMeter(CRect(CPoint(0, 0), CPoint(5, 200)), ppmOn, ppmOff, 200);
	addGuiElementPointer(ppm, id_output_ppmFirst + moduleId);
	knobPpmView->addView(ppm);
	knobPpmView->setBackgroundColor(CCOLOR_NOCOLOR);
	knobPpmView->sizeToFit();
	gainView->addView(knobPpmView);
	gainView->setBackgroundColor(CCOLOR_NOCOLOR);
	
	return gainView;
}

CViewContainer* ReverbNetworkEditor::createKnobGroup(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& knobTag, const int32_t& valueEditTag, 
	const double& valueEditMinValue, const double& valueEditMaxValue, const int& valueEditPrecision, const std::string& unit) {

	CViewContainer* groupView = new CViewContainer(CRect(0, 0, width, 0));
	groupView->setBackgroundColor(CCOLOR_NOCOLOR);
	GuiCustomTextLabel* groupNameLabel = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(groupView->getWidth(), 15)), title, kNormalFontSmall);
	CAnimKnob* knob = new CAnimKnob(CRect(CPoint(0 + (groupView->getWidth() - knobBackground->getWidth()) / 2, groupNameLabel->getViewSize().bottom + 3), 
		CPoint(knobBackground->getWidth(), knobBackground->getWidth())), this, knobTag, knobBackground->getHeight() / knobBackground->getWidth(), knobBackground->getWidth(), knobBackground);
	addGuiElementPointer(knob, knobTag);
	GuiCustomValueEdit* groupTextEdit = new GuiCustomValueEdit(CRect(CPoint(0, knob->getViewSize().bottom + 3), CPoint(groupView->getWidth(), 15)), this, valueEditTag, "0.0");
	addGuiElementPointer(groupTextEdit, valueEditTag);
	groupTextEdit->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	groupTextEdit->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	groupTextEdit->setStringToValueFunction(ValueConversion::textEditStringToValueConversion);
	valueToStringUserData userData;
	userData.precision = valueEditPrecision;
	userData.unit = unit;
	groupTextEdit->setValueToStringUserData(userData);
	groupTextEdit->setValueToStringFunction(ValueConversion::textEditValueToStringConversion);
	groupTextEdit->setMin(valueEditMinValue);
	groupTextEdit->setMax(valueEditMaxValue);
	groupTextEdit->setFont(CFontRef(kNormalFontSmall));
	groupTextEdit->setStringToTruncate(unit, true);
	groupView->addView(groupNameLabel);
	groupView->addView(knob);
	groupView->addView(groupTextEdit);
	groupView->sizeToFit();
	getController()->getParamNormalized(2);
	return groupView;
}

CTextLabel* ReverbNetworkEditor::createGroupTitle(const VSTGUI::UTF8StringPtr title, const CCoord& width) const {
	CTextLabel* label = new CTextLabel(CRect(CPoint(0, 0), CPoint(width, 30)), title);
	label->setFont(kNormalFontBig);
	label->setBackColor(CCOLOR_NOCOLOR);
	label->setFrameColor(CCOLOR_NOCOLOR);
	return label;
}

GuiCustomRowColumnView* ReverbNetworkEditor::createMixerRow(const VSTGUI::UTF8StringPtr title, const CCoord& width, const int32_t& idOffset) {
	GuiCustomRowColumnView* mixerRow = new GuiCustomRowColumnView(CRect(CPoint(0, 0), CPoint(width, 20)), GuiCustomRowColumnView::kColumnStyle);
	mixerRow->setSpacing(5);
	mixerRow->setBackgroundColor(CCOLOR_NOCOLOR);

	GuiCustomTextLabel* inputTitle = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(5, 20)), "", kNormalFontSmall);

	GuiOptionMenuInputSelector* inputSelect = new GuiOptionMenuInputSelector(CRect(CPoint(0, 0), CPoint(90, 20)), this, id_mixer_optionMenu_inputSelectFirst + idOffset);
	addGuiElementPointer(inputSelect, id_mixer_optionMenu_inputSelectFirst + idOffset);
	inputSelect->setBackColor(CCOLOR_OPTIONMENU_BACKGROUND);
	inputSelect->setFrameColor(CCOLOR_OPTIONMENU_FRAME);
	inputSelect->setFont(CFontRef(kNormalFontSmall));
	inputSelect->addEntry("<Select>");
	inputSelect->setCurrent(0);
	std::string temp = "";
	for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
		temp = "APM";
		temp.append(std::to_string(i));
		inputSelect->addEntry((temp).c_str());
	}
	for (uint32 i = 0; i < MAXVSTINPUTS; ++i) {
		temp = "VST";
		temp.append(std::to_string(i));
		temp.append(" IN");
		inputSelect->addEntry((temp).c_str());
	}
	inputSelect->addEntry("SIGNALGEN");
	CAnimKnob* knob = new CAnimKnob(CRect(CPoint(0, 0), CPoint(knobBackgroundSmall->getWidth(), knobBackgroundSmall->getWidth())), 
		this, id_mixer_knob_gainFirst + idOffset, knobBackgroundSmall->getHeight() / knobBackgroundSmall->getWidth(), knobBackgroundSmall->getWidth(), knobBackgroundSmall);
	addGuiElementPointer(knob, id_mixer_knob_gainFirst + idOffset);

	GuiCustomValueEdit* valueEdit = new GuiCustomValueEdit(CRect(CPoint(0, 0), CPoint(30, 20)), this, id_mixer_textEdit_gainFirst + idOffset);
	addGuiElementPointer(valueEdit, id_mixer_textEdit_gainFirst + idOffset);
	valueEdit->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
	valueEdit->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
	valueEdit->setStringToValueFunction(ValueConversion::textEditStringToValueConversion);
	valueToStringUserData userData;
	userData.precision = 2;
	userData.unit = "";
	valueEdit->setValueToStringUserData(userData);
	valueEdit->setValueToStringFunction(ValueConversion::textEditValueToStringConversion);
	valueEdit->setMin(MIN_MIXERGAIN);
	valueEdit->setMax(MAX_MIXERGAIN);
	valueEdit->setFont(CFontRef(kNormalFontSmall));

	VSTGUI::CTextButton* buttonMute = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(20, 20)), this, id_mixer_button_muteFirst + idOffset, "M", VSTGUI::CTextButton::kOnOffStyle);
	addGuiElementPointer(buttonMute, id_mixer_button_muteFirst + idOffset);
	//buttonMute->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//buttonMute->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	buttonMute->setFont(CFontRef(kNormalFontSmall));
	buttonMute->setTextColor(CColor(200, 0, 0, 255));
	buttonMute->setTextColorHighlighted(CColor(255, 255, 255));
	//buttonMute->setGradientStartColorHighlighted(CColor(200, 0, 0, 255));
	//buttonMute->setGradientEndColorHighlighted(CColor(200, 0, 0, 255));
	buttonMute->setRoundRadius(1);
	
	VSTGUI::CTextButton* buttonSolo = new VSTGUI::CTextButton(CRect(CPoint(0, 0), CPoint(20, 20)), this, id_mixer_button_soloFirst + idOffset, "S", VSTGUI::CTextButton::kOnOffStyle);
	addGuiElementPointer(buttonSolo, id_mixer_button_soloFirst + idOffset);
	//buttonSolo->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
	//buttonSolo->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
	buttonSolo->setFont(CFontRef(kNormalFontSmall));
	buttonSolo->setTextColor(CColor(0, 150, 0, 255));
	//buttonSolo->setGradientStartColorHighlighted(CColor(0, 150, 0, 255));
	//buttonSolo->setGradientEndColorHighlighted(CColor(0, 150, 0, 255));
	buttonSolo->setRoundRadius(1);

	mixerRow->addView(inputTitle);
	mixerRow->addView(inputSelect);
	mixerRow->addView(knob);
	mixerRow->addView(valueEdit);
	mixerRow->addView(buttonMute);
	mixerRow->addView(buttonSolo);

	return mixerRow;
}

void ReverbNetworkEditor::updateGuiWithControllerParameters() {
	sampleRate = ValueConversion::getSampleRate(); // Update the sample rate (difficult to say when exactly this function returns the right value
	// Update the maximum value of all delay sample textEdits with the new sample rate
	for (int i = id_allpass_textEdit_samplesDelayFirst; i <= id_allpass_textEdit_samplesDelayLast; ++i) {
		guiElements[i]->setMax(sampleRate * MAX_ALLPASSDELAY / 1000);
	}
	// Update the optionMenus of the mixer
	for (uint32 i = id_mixer_optionMenu_inputSelectFirst; i <= id_mixer_optionMenu_inputSelectLast; ++i) {
		int menuIndex = ValueConversion::normToPlainMixerInputSelect(getController()->getParamNormalized(PARAM_MIXERINPUTSELECT_FIRST + (i - id_mixer_optionMenu_inputSelectFirst)));
		guiElements[i]->setValue(menuIndex);
		valueChanged(guiElements[i]);
	}
	// Update the gain knobs of the mixer
	for (int i = id_mixer_knob_gainFirst; i <= id_mixer_knob_gainLast; ++i) {
		valueChanged(guiElements[i]);
	}
	for (int i = 0; i < MAXMODULENUMBER; ++i) {
		for (int j = 0; j < MAXMODULENUMBER; ++j) {
			updateGainValuesInOptionMenus(i, j, ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + j)));
			updateGraphicsViewModule(i, j, ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + j)));
		}
		for (int j = 0; j < MAXVSTINPUTS; ++j) {
			updateGainValuesInOptionMenus(i, MAXMODULENUMBER + j, ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + MAXMODULENUMBER + j)));
			updateGraphicsViewModule(i, MAXMODULENUMBER + j, ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + MAXMODULENUMBER + j)));
		}
		updateGainValuesInOptionMenus(i, MAXMODULENUMBER + MAXVSTINPUTS, ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + MAXMODULENUMBER + MAXVSTINPUTS)));
		updateGraphicsViewModule(i, MAXMODULENUMBER + MAXVSTINPUTS, ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + MAXMODULENUMBER + MAXVSTINPUTS)));
	}

	// Update all other parameters
	//updateGuiParameter(PARAM_MIXERBYPASS_FIRST, PARAM_MIXERBYPASS_LAST, id_mixer_switch_bypassFirst, nullptr);
	updateGuiParameter(PARAM_QUANTIZERBITDEPTH_FIRST, PARAM_QUANTIZERBITDEPTH_LAST, id_quantizer_knob_quantizationFirst, nullptr);
	updateGuiParameter(PARAM_QUANTIZERBYPASS_FIRST, PARAM_QUANTIZERBYPASS_LAST, id_quantizer_switch_bypassFirst, nullptr);
	updateGuiParameter(PARAM_EQFILTERTYPE_FIRST, PARAM_EQFILTERTYPE_LAST, id_equalizer_optionMenu_filterTypeFirst, &ValueConversion::normToPlainFilterTypeSelect);
	updateGuiParameter(PARAM_EQCENTERFREQ_FIRST, PARAM_EQCENTERFREQ_LAST, id_equalizer_knob_centerFreqFirst, nullptr);
	updateGuiParameter(PARAM_EQQFACTOR_FIRST, PARAM_EQQFACTOR_LAST, id_equalizer_knob_qFactorFirst, nullptr);
	updateGuiParameter(PARAM_EQGAIN_FIRST, PARAM_EQGAIN_LAST, id_equalizer_knob_gainFirst, nullptr);
	updateGuiParameter(PARAM_EQCOEFFICIENTA0_FIRST, PARAM_EQCOEFFICIENTA0_LAST, id_equalizer_textEdit_a0First, &ValueConversion::normToPlainEqCoefficients);
	updateGuiParameter(PARAM_EQCOEFFICIENTA1_FIRST, PARAM_EQCOEFFICIENTA1_LAST, id_equalizer_textEdit_a1First, &ValueConversion::normToPlainEqCoefficients);
	updateGuiParameter(PARAM_EQCOEFFICIENTA2_FIRST, PARAM_EQCOEFFICIENTA2_LAST, id_equalizer_textEdit_a2First, &ValueConversion::normToPlainEqCoefficients);
	updateGuiParameter(PARAM_EQCOEFFICIENTB1_FIRST, PARAM_EQCOEFFICIENTB1_LAST, id_equalizer_textEdit_b1First, &ValueConversion::normToPlainEqCoefficients);
	updateGuiParameter(PARAM_EQCOEFFICIENTB2_FIRST, PARAM_EQCOEFFICIENTB2_LAST, id_equalizer_textEdit_b2First, &ValueConversion::normToPlainEqCoefficients);
	updateGuiParameter(PARAM_EQLIMITER_FIRST, PARAM_EQLIMITER_LAST, id_equalizer_checkBox_limiterFirst, nullptr);
	updateGuiParameter(PARAM_EQBYPASS_FIRST, PARAM_EQBYPASS_LAST, id_equalizer_switch_bypassFirst, nullptr);
	updateGuiParameter(PARAM_ALLPASSDELAY_FIRST, PARAM_ALLPASSDELAY_LAST, id_allpass_knob_delayFirst, nullptr);
	updateGuiParameter(PARAM_ALLPASSDECAY_FIRST, PARAM_ALLPASSDECAY_LAST, id_allpass_knob_decayFirst, nullptr);
	updateGuiParameter(PARAM_ALLPASSDIFFKSIGN_FIRST, PARAM_ALLPASSDIFFKSIGN_LAST, id_allpass_optionMenu_diffKSignFirst, nullptr);
	updateGuiParameter(PARAM_ALLPASSMODENABLED_FIRST, PARAM_ALLPASSMODENABLED_LAST, id_allpass_checkBox_modulationEnabledFirst, nullptr);
	updateGuiParameter(PARAM_ALLPASSMODSIGNALTYPE_FIRST, PARAM_ALLPASSMODSIGNALTYPE_LAST, id_allpass_optionMenu_modulationSignalTypeFirst, &ValueConversion::normToPlainModSignalType);
	updateGuiParameter(PARAM_ALLPASSMODEXCURSION_FIRST, PARAM_ALLPASSMODEXCURSION_LAST, id_allpass_textEdit_modulationExcursionFirst, &ValueConversion::normToPlainModExcursion);
	updateGuiParameter(PARAM_ALLPASSMODRATE_FIRST, PARAM_ALLPASSMODRATE_LAST, id_allpass_textEdit_modulationRateFirst, &ValueConversion::normToPlainModRate);
	updateGuiParameter(PARAM_ALLPASSBYPASS_FIRST, PARAM_ALLPASSBYPASS_LAST, id_allpass_switch_bypassFirst, nullptr);
	updateGuiParameter(PARAM_OUTGAIN_FIRST, PARAM_OUTGAIN_LAST, id_output_knob_gainFirst, nullptr);
	updateGuiParameter(PARAM_OUTLIMITER_FIRST, PARAM_OUTLIMITER_LAST, id_output_checkbox_limiterFirst, nullptr);
	updateGuiParameter(PARAM_OUTBYPASS_FIRST, PARAM_OUTBYPASS_LAST, id_output_switch_bypassFirst, nullptr);
	updateGuiParameter(PARAM_GENERALVSTOUTPUTSELECT_FIRST, PARAM_GENERALVSTOUTPUTSELECT_LAST, id_general_optionMenu_vstOutputFirst, &ValueConversion::normToPlainMixerInputSelect);
	updateGuiParameter(PARAM_MODULEVISIBLE_FIRST, PARAM_MODULEVISIBLE_LAST, id_general_checkBox_moduleVisibleFirst, nullptr);
	
	// Update the signal generator
	signalGenerator->updateFromController();

	// Apply the saved user data (module names...)
	applyUserData();
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

void ReverbNetworkEditor::updateEditorFromController(ParamID tag, ParamValue value) {
	if (tag >= PARAM_PPMUPDATE_FIRST && tag <= PARAM_PPMUPDATE_LAST) {
		// Update the PPM with values from Processor
		if (value > lastPpmValues[tag - PARAM_PPMUPDATE_FIRST]) {
			if (value != 0.0) {
				lastPpmValues[tag - PARAM_PPMUPDATE_FIRST] = 1.0 - ValueConversion::linearToLog(std::abs(value)) / -60.0; // 60 dB range (ToDo: show the exact value or add labels to the PPM)
			}
			else {
				lastPpmValues[tag - PARAM_PPMUPDATE_FIRST] = 0.0;
			}
		}
	}
	else if (tag >= PARAM_EQSTABILITY_FIRST && tag <= PARAM_EQSTABILITY_LAST) {
		// Update EQ stability
		EqualizerStability stability;
		stability.moduleNumber = tag - PARAM_EQSTABILITY_FIRST;
		stability.isStable = (value != 0.0);
		// Add the change to the ToDo-queue (see notify)
		eqStabilityValues.push_back(stability);
	}

	#ifdef GUIUPDATEWHENAUTOMATING
	// The following updates are called when the parameters are beeing automated so that the GUI elements move accordingly
	else if (tag >= PARAM_MIXERGAIN_FIRST && tag <= PARAM_MIXERGAIN_LAST) {
		int moduleNumber = (int)((tag - PARAM_MIXERGAIN_FIRST) / MAXINPUTS);
		for (unsigned int i = 0; i < MAXMODULEINPUTS; ++i) {
			if (guiElements[id_mixer_optionMenu_inputSelectFirst + MAXMODULEINPUTS * moduleNumber + i]->getValue() == (tag - PARAM_MIXERGAIN_FIRST - moduleNumber * MAXINPUTS + 1)) {
				guiElements[id_mixer_knob_gainFirst + MAXMODULEINPUTS * moduleNumber + i]->setValueNormalized(value);
				guiElements[id_mixer_textEdit_gainFirst + MAXMODULEINPUTS * moduleNumber + i]->setValueNormalized(value);
			}
		}
	}
	else if (tag >= PARAM_QUANTIZERBITDEPTH_FIRST && tag <= PARAM_QUANTIZERBITDEPTH_LAST) {
		guiElements[id_quantizer_knob_quantizationFirst + (tag - PARAM_QUANTIZERBITDEPTH_FIRST)]->setValueNormalized(value);
		guiElements[id_quantizer_textEdit_quantizationFirst + (tag - PARAM_QUANTIZERBITDEPTH_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_QUANTIZERBYPASS_FIRST && tag <= PARAM_QUANTIZERBYPASS_LAST) {
		guiElements[id_quantizer_switch_bypassFirst + (tag - PARAM_QUANTIZERBYPASS_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_EQCENTERFREQ_FIRST && tag <= PARAM_EQCENTERFREQ_LAST) {
		guiElements[id_equalizer_knob_centerFreqFirst + (tag - PARAM_EQCENTERFREQ_FIRST)]->setValueNormalized(value);
		guiElements[id_equalizer_textEdit_centerFreqFirst + (tag - PARAM_EQCENTERFREQ_FIRST)]->setValue(ValueConversion::normToPlainProcCenterFreq(value));
	}
	else if (tag >= PARAM_EQQFACTOR_FIRST && tag <= PARAM_EQQFACTOR_LAST) {
		guiElements[id_equalizer_knob_qFactorFirst + (tag - PARAM_EQQFACTOR_FIRST)]->setValueNormalized(value);
		guiElements[id_equalizer_textEdit_qFactorFirst + (tag - PARAM_EQQFACTOR_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_EQGAIN_FIRST && tag <= PARAM_EQGAIN_LAST) {
		guiElements[id_equalizer_knob_gainFirst + (tag - PARAM_EQGAIN_FIRST)]->setValueNormalized(value);
		guiElements[id_equalizer_textEdit_gainFirst + (tag - PARAM_EQGAIN_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_EQCOEFFICIENTA0_FIRST && tag <= PARAM_EQCOEFFICIENTA0_LAST) {
		guiElements[id_equalizer_textEdit_a0First + (tag - PARAM_EQCOEFFICIENTA0_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_EQCOEFFICIENTA1_FIRST && tag <= PARAM_EQCOEFFICIENTA1_LAST) {
		guiElements[id_equalizer_textEdit_a1First + (tag - PARAM_EQCOEFFICIENTA1_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_EQCOEFFICIENTA2_FIRST && tag <= PARAM_EQCOEFFICIENTA2_LAST) {
		guiElements[id_equalizer_textEdit_a2First + (tag - PARAM_EQCOEFFICIENTA2_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_EQCOEFFICIENTB1_FIRST && tag <= PARAM_EQCOEFFICIENTB1_LAST) {
		guiElements[id_equalizer_textEdit_b1First + (tag - PARAM_EQCOEFFICIENTB1_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_EQCOEFFICIENTB2_FIRST && tag <= PARAM_EQCOEFFICIENTB2_LAST) {
		guiElements[id_equalizer_textEdit_b2First + (tag - PARAM_EQCOEFFICIENTB2_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_EQBYPASS_FIRST && tag <= PARAM_EQBYPASS_LAST) {
		guiElements[id_equalizer_switch_bypassFirst + (tag - PARAM_EQBYPASS_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_ALLPASSDELAY_FIRST && tag <= PARAM_ALLPASSDELAY_LAST) {
		guiElements[id_allpass_knob_delayFirst + (tag - PARAM_ALLPASSDELAY_FIRST)]->setValueNormalized(value);
		guiElements[id_allpass_textEdit_delayFirst + (tag - PARAM_ALLPASSDELAY_FIRST)]->setValueNormalized(value);
		guiElements[id_allpass_textEdit_samplesDelayFirst + (tag - PARAM_ALLPASSDELAY_FIRST)]->setValue(ValueConversion::delayMillisecondsToSamples(ValueConversion::normToPlainDelay(value)));
		guiElements[id_allpass_textEdit_diffKFirst + (tag - PARAM_ALLPASSDELAY_FIRST)]
			->setValue(ValueConversion::calculateDiffK(ValueConversion::normToPlainDelay(value), guiElements[id_allpass_textEdit_decayFirst + (tag - PARAM_ALLPASSDELAY_FIRST)]->getValue()));
	}
	else if (tag >= PARAM_ALLPASSDECAY_FIRST && tag <= PARAM_ALLPASSDECAY_LAST) {
		guiElements[id_allpass_knob_decayFirst + (tag - PARAM_ALLPASSDECAY_FIRST)]->setValueNormalized(value);
		guiElements[id_allpass_textEdit_decayFirst + (tag - PARAM_ALLPASSDECAY_FIRST)]->setValueNormalized(value);
		guiElements[id_allpass_textEdit_diffKFirst + (tag - PARAM_ALLPASSDECAY_FIRST)]
			->setValue(ValueConversion::calculateDiffK(guiElements[id_allpass_textEdit_delayFirst + (tag - PARAM_ALLPASSDECAY_FIRST)]->getValue(), ValueConversion::normToPlainDecay(value)));
	}
	else if (tag >= PARAM_ALLPASSDIFFKSIGN_FIRST && tag <= PARAM_ALLPASSDIFFKSIGN_LAST) {
		guiElements[id_allpass_optionMenu_diffKSignFirst + (tag - PARAM_ALLPASSDIFFKSIGN_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_ALLPASSBYPASS_FIRST && tag <= PARAM_ALLPASSBYPASS_LAST) {
		guiElements[id_allpass_switch_bypassFirst + (tag - PARAM_ALLPASSBYPASS_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_OUTGAIN_FIRST && tag <= PARAM_OUTGAIN_LAST) {
		guiElements[id_output_knob_gainFirst + (tag - PARAM_OUTGAIN_FIRST)]->setValueNormalized(value);
		guiElements[id_output_textEdit_gainFirst + (tag - PARAM_OUTGAIN_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_OUTBYPASS_FIRST && tag <= PARAM_OUTBYPASS_LAST) {
		guiElements[id_output_switch_bypassFirst + (tag - PARAM_OUTBYPASS_FIRST)]->setValueNormalized(value);
	}
	else if (tag >= PARAM_SIGNALGENERATOR_SIGNALTYPE && tag <= PARAM_SIGNALGENERATOR_FIRE) {
		signalGenerator->updateParameter(tag, value);
	}
	#endif // GUIUPDATEWHENAUTOMATING
}

CMessageResult ReverbNetworkEditor::notify(CBaseObject* sender, const char* message) {
	if (!message) return kMessageUnknown;
	// GUI refresh timer, can be set with setIdleRate()
	// Any GUI changes coming from controller/processor should be made here after the editor's variables have been updated (thread-safe!)
	if (message == CVSTGUITimer::kMsgTimer)
	{
		// Update PPMs of the modules
		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			if (guiElements[id_output_ppmFirst + i]) {
				if (lastPpmValues[i] > guiElements[id_output_ppmFirst + i]->getValue()) {
					guiElements[id_output_ppmFirst + i]->setValue(lastPpmValues[i]);
				}
				else {
					guiElements[id_output_ppmFirst + i]->setValue(guiElements[id_output_ppmFirst + i]->getValue() - (ppmUpdateFactor / 60.0));
				}
				lastPpmValues[i] = 0.0;
			}
		}
		// If there is something in the ToDo-queue
		if (eqStabilityValues.size() > 0) {
			for (auto&& i : eqStabilityValues) {
				if (guiElements[id_equalizer_button_stabilityFirst + i.moduleNumber]) {
					updateEqStability(i.moduleNumber, i.isStable);
				}
			}
			// Clear the queue
			eqStabilityValues.clear();
		}

		// ToDo: Save the position of the graphics view modules when closing the view (and also the gain values / connections...)
		/*editorUserData = {};
		for (unsigned int i = 0; i < MAXMODULENUMBER; ++i) {
		XmlPresetReadWrite::graphicsModule module;
		module.isVisible = graphicsView->isModuleVisible(i);
		module.positionX = graphicsView->getModulePosition(i).x;
		module.positionY = graphicsView->getModulePosition(i).y;
		editorUserData.graphicsView.modules.push_back(module);
		}
		for (unsigned int i = 0; i < MAXVSTINPUTS; ++i) {
		XmlPresetReadWrite::graphicsVstInput vstInput;
		vstInput.positionX = graphicsView->getVstInputPosition(i).x;
		vstInput.positionY = graphicsView->getVstInputPosition(i).y;
		editorUserData.graphicsView.vstInputs.push_back(vstInput);
		}
		for (unsigned int i = 0; i < MAXVSTOUTPUTS; ++i) {
		XmlPresetReadWrite::graphicsVstOutput vstOutput;
		vstOutput.positionX = graphicsView->getVstOutputPosition(i).x;
		vstOutput.positionY = graphicsView->getVstOutputPosition(i).y;
		editorUserData.graphicsView.vstOutputs.push_back(vstOutput);
		}*/


	}
	else if (message == GuiBaseAPModule::kModuleWantsFocus) {
		// User has clicked on a module => bring the module to the top of the view
		for (auto&& module : apGuiModules) {
			if (module == dynamic_cast<GuiBaseAPModule*>(sender)) {
				workspaceView->changeViewZOrder(dynamic_cast<GuiBaseAPModule*>(sender), workspaceView->getNbViews() - 1);
				workspaceView->setDirty();
				break;
			}
		}
	}
	else if (message == CNewFileSelector::kSelectEndMessage) {
		// User has selected a file in the file selector
		CNewFileSelector* selector = dynamic_cast<CNewFileSelector*>(sender);
		if (selector && selector->getSelectedFile(0)) {
			// Open or save?
			if (fileSelectorStyle == CNewFileSelector::kSelectFile) {
				setXmlPreset(xmlPreset->loadPreset(selector->getSelectedFile(0)));
				workspaceView->setDirty();
				return kMessageNotified;
			}
			else if (fileSelectorStyle == CNewFileSelector::kSelectSaveFile) {
				xmlPreset->savePreset(selector->getSelectedFile(0), getXmlPreset());
				return kMessageNotified;
			}
		}
	}
	else if (sender == graphicsView) {
		if (message == "ConnectionToModule") {
			// User has drawn a new connection
			Connection connection = graphicsView->getDrawnConnection();
			controller->setParamNormalized(PARAM_MIXERGAIN_FIRST + connection.destination * MAXINPUTS + connection.source, ValueConversion::plainToNormInputGain(connection.gainValue));
			controller->performEdit(PARAM_MIXERGAIN_FIRST + connection.destination * MAXINPUTS + connection.source, ValueConversion::plainToNormInputGain(connection.gainValue));
			updateGainValuesInOptionMenus(connection.destination, connection.source, connection.gainValue);
			updateGraphicsViewModule(connection.destination, connection.source, connection.gainValue);
			// Check if the input is already selected in an input menu
			for (int i = 0; i < MAXMODULEINPUTS; ++i) {
				if (guiElements[id_mixer_optionMenu_inputSelectFirst + connection.destination * MAXMODULEINPUTS + i]->getValue() == connection.source + 1) {
					// If so => set the gain knob value to 1.0 (happens only if knob is at 0.0 => see Graphics View)
					guiElements[id_mixer_knob_gainFirst + connection.destination * MAXMODULEINPUTS + i]->setValue(ValueConversion::plainToNormInputGain(connection.gainValue));
					valueChanged(guiElements[id_mixer_knob_gainFirst + connection.destination * MAXMODULEINPUTS + i]);
					
				}
			}
			splitView->invalid();
		}
		else if (message == "ConnectionToVst") {
			// User has drawn a new connection
			Connection connection = graphicsView->getDrawnConnection();
			if (connection.gainValue == 1.0) {
				guiElements[id_general_optionMenu_vstOutputFirst + connection.destination]->setValue(connection.source + 1);
			}
			else {
				guiElements[id_general_optionMenu_vstOutputFirst + connection.destination]->setValue(0);
			}
			valueChanged(guiElements[id_general_optionMenu_vstOutputFirst + connection.destination]);
			splitView->invalid();
		}
		else if (message == "OpenModuleDetailView") {
			// User has double-clicked on a module
			openModuleDetailView(graphicsView->getModuleClicked(), true);
			this->notify(apGuiModules[graphicsView->getModuleClicked()], GuiBaseAPModule::kModuleWantsFocus);
		}
	}
	return VSTGUIEditor::notify(sender, message);
} 

void ReverbNetworkEditor::setXmlPreset(const XmlPresetReadWrite::Preset& presetStruct) {
	if (presetStruct.maxModuleNumber != MAXMODULENUMBER || presetStruct.maxVstInputs != MAXVSTINPUTS || presetStruct.maxVstOutputs != MAXVSTOUTPUTS) {
		// If those defines in the XML differ from the defines in the actual build then the preset will be probably incompatible
		// To make it work anyway change the values in the XML file to match the current build
		return;
	}
	graphicsView->clearModules();
	editorUserData.presetName = presetStruct.name; // save the preset name for later
	
	// Read and apply all loaded parameter values
	for (unsigned int i = 0; i < presetStruct.modules.size(); ++i) {
		if (i >= MAXMODULENUMBER) break;
		editorUserData.moduleNames[i] = presetStruct.modules[i].name;
		apGuiModules[i]->setViewSize(CRect(CPoint(presetStruct.modules[i].positionX, presetStruct.modules[i].positionY), CPoint(apGuiModules[i]->getWidth(), apGuiModules[i]->getHeight())));
		apGuiModules[i]->setMouseableArea(apGuiModules[i]->getViewSize());
		getController()->setParamNormalized(PARAM_MODULEVISIBLE_FIRST + i, presetStruct.modules[i].isVisible);
		getController()->performEdit(PARAM_MODULEVISIBLE_FIRST + i, presetStruct.modules[i].isVisible);
		apGuiModules[i]->collapseView(presetStruct.modules[i].isCollapsed);

		// Set mixer parameters	
		// Module outputs first
		for (unsigned int k = 0; k < presetStruct.modules[i].mixerParameters.moduleOutputs.size(); ++k) {
			if (k >= MAXMODULENUMBER) break;
			getController()->setParamNormalized(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + k, ValueConversion::plainToNormInputGain(presetStruct.modules[i].mixerParameters.moduleOutputs[k].gainFactor));
			getController()->performEdit(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + k, ValueConversion::plainToNormInputGain(presetStruct.modules[i].mixerParameters.moduleOutputs[k].gainFactor));
			getController()->setParamNormalized(PARAM_MIXERINPUTMUTED_FIRST + i * MAXINPUTS + k, presetStruct.modules[i].mixerParameters.moduleOutputs[k].muted);
			getController()->performEdit(PARAM_MIXERINPUTMUTED_FIRST + i * MAXINPUTS + k, presetStruct.modules[i].mixerParameters.moduleOutputs[k].muted);
			getController()->setParamNormalized(PARAM_MIXERINPUTSOLOED_FIRST + i * MAXINPUTS + k, presetStruct.modules[i].mixerParameters.moduleOutputs[k].soloed);
			getController()->performEdit(PARAM_MIXERINPUTSOLOED_FIRST + i * MAXINPUTS + k, presetStruct.modules[i].mixerParameters.moduleOutputs[k].soloed);
		}
		// Then the Vst inputs
		for (unsigned int k = 0; k < presetStruct.modules[i].mixerParameters.vstInputs.size(); ++k) {
			if (k >= MAXVSTINPUTS) break;
			getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + k + presetStruct.modules[i].mixerParameters.moduleOutputs.size()), ValueConversion::plainToNormInputGain(presetStruct.modules[i].mixerParameters.vstInputs[k].gainFactor));
			getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + k + presetStruct.modules[i].mixerParameters.moduleOutputs.size()), ValueConversion::plainToNormInputGain(presetStruct.modules[i].mixerParameters.vstInputs[k].gainFactor));
			getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTMUTED_FIRST + i * MAXINPUTS + k + presetStruct.modules[i].mixerParameters.moduleOutputs.size()), presetStruct.modules[i].mixerParameters.vstInputs[k].muted);
			getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTMUTED_FIRST + i * MAXINPUTS + k + presetStruct.modules[i].mixerParameters.moduleOutputs.size()), presetStruct.modules[i].mixerParameters.vstInputs[k].muted);
			getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTSOLOED_FIRST + i * MAXINPUTS + k + presetStruct.modules[i].mixerParameters.moduleOutputs.size()), presetStruct.modules[i].mixerParameters.vstInputs[k].soloed);
			getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTSOLOED_FIRST + i * MAXINPUTS + k + presetStruct.modules[i].mixerParameters.moduleOutputs.size()), presetStruct.modules[i].mixerParameters.vstInputs[k].soloed);
		}
		getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + presetStruct.modules[i].mixerParameters.moduleOutputs.size() + presetStruct.modules[i].mixerParameters.vstInputs.size()), ValueConversion::plainToNormInputGain(presetStruct.modules[i].mixerParameters.signalGeneratorInput.gainFactor));
		getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + presetStruct.modules[i].mixerParameters.moduleOutputs.size() + presetStruct.modules[i].mixerParameters.vstInputs.size()), ValueConversion::plainToNormInputGain(presetStruct.modules[i].mixerParameters.signalGeneratorInput.gainFactor));
		getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTMUTED_FIRST + i * MAXINPUTS + presetStruct.modules[i].mixerParameters.moduleOutputs.size() + presetStruct.modules[i].mixerParameters.vstInputs.size()), presetStruct.modules[i].mixerParameters.signalGeneratorInput.muted);
		getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTMUTED_FIRST + i * MAXINPUTS + presetStruct.modules[i].mixerParameters.moduleOutputs.size() + presetStruct.modules[i].mixerParameters.vstInputs.size()), presetStruct.modules[i].mixerParameters.signalGeneratorInput.muted);
		getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTSOLOED_FIRST + i * MAXINPUTS + presetStruct.modules[i].mixerParameters.moduleOutputs.size() + presetStruct.modules[i].mixerParameters.vstInputs.size()), presetStruct.modules[i].mixerParameters.signalGeneratorInput.soloed);
		getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTSOLOED_FIRST + i * MAXINPUTS + presetStruct.modules[i].mixerParameters.moduleOutputs.size() + presetStruct.modules[i].mixerParameters.vstInputs.size()), presetStruct.modules[i].mixerParameters.signalGeneratorInput.soloed);
		// Finally the input slots
		for (unsigned int k = 0; k < presetStruct.modules[i].mixerParameters.inputSlots.size(); ++k) {
			if (k >= MAXMODULEINPUTS) break;
			getController()->setParamNormalized(PARAM_MIXERINPUTSELECT_FIRST + i * MAXMODULEINPUTS + k, ValueConversion::plainToNormMixerInputSelect(presetStruct.modules[i].mixerParameters.inputSlots[k]));
			getController()->performEdit(PARAM_MIXERINPUTSELECT_FIRST + i * MAXMODULEINPUTS + k, ValueConversion::plainToNormMixerInputSelect(presetStruct.modules[i].mixerParameters.inputSlots[k]));
		}

		// Set quantizer parameters
		getController()->setParamNormalized(PARAM_QUANTIZERBITDEPTH_FIRST + i, ValueConversion::plainToNormQuantization(presetStruct.modules[i].quantizerParameters.quantization));
		getController()->performEdit(PARAM_QUANTIZERBITDEPTH_FIRST + i, ValueConversion::plainToNormQuantization(presetStruct.modules[i].quantizerParameters.quantization));
		getController()->setParamNormalized(PARAM_QUANTIZERBYPASS_FIRST + i, presetStruct.modules[i].quantizerParameters.bypass);
		getController()->performEdit(PARAM_QUANTIZERBYPASS_FIRST + i, presetStruct.modules[i].quantizerParameters.bypass);

		// Set equalizer parameters
		getController()->setParamNormalized(PARAM_EQFILTERTYPE_FIRST + i, ValueConversion::plainToNormFilterTypeSelect(presetStruct.modules[i].equalizerParameters.filterTypeIndex));
		getController()->performEdit(PARAM_EQFILTERTYPE_FIRST + i, ValueConversion::plainToNormFilterTypeSelect(presetStruct.modules[i].equalizerParameters.filterTypeIndex));
		getController()->setParamNormalized(PARAM_EQCENTERFREQ_FIRST + i, ValueConversion::plainToNormProcCenterFreq(presetStruct.modules[i].equalizerParameters.frequency));
		getController()->performEdit(PARAM_EQCENTERFREQ_FIRST + i, ValueConversion::plainToNormProcCenterFreq(presetStruct.modules[i].equalizerParameters.frequency));
		getController()->setParamNormalized(PARAM_EQQFACTOR_FIRST + i, ValueConversion::plainToNormQFactor(presetStruct.modules[i].equalizerParameters.qFactor));
		getController()->performEdit(PARAM_EQQFACTOR_FIRST + i, ValueConversion::plainToNormQFactor(presetStruct.modules[i].equalizerParameters.qFactor));
		getController()->setParamNormalized(PARAM_EQGAIN_FIRST + i, ValueConversion::plainToNormEqGain(presetStruct.modules[i].equalizerParameters.gain));
		getController()->performEdit(PARAM_EQGAIN_FIRST + i, ValueConversion::plainToNormEqGain(presetStruct.modules[i].equalizerParameters.gain));
		getController()->setParamNormalized(PARAM_EQBYPASS_FIRST + i, presetStruct.modules[i].equalizerParameters.bypass);
		getController()->performEdit(PARAM_EQBYPASS_FIRST + i, presetStruct.modules[i].equalizerParameters.bypass);
		getController()->setParamNormalized(PARAM_EQCOEFFICIENTA0_FIRST + i, ValueConversion::plainToNormEqCoefficients(presetStruct.modules[i].equalizerParameters.a0));
		getController()->performEdit(PARAM_EQCOEFFICIENTA0_FIRST + i, ValueConversion::plainToNormEqCoefficients(presetStruct.modules[i].equalizerParameters.a0));
		getController()->setParamNormalized(PARAM_EQCOEFFICIENTA1_FIRST + i, ValueConversion::plainToNormEqCoefficients(presetStruct.modules[i].equalizerParameters.a1));
		getController()->performEdit(PARAM_EQCOEFFICIENTA1_FIRST + i, ValueConversion::plainToNormEqCoefficients(presetStruct.modules[i].equalizerParameters.a1));
		getController()->setParamNormalized(PARAM_EQCOEFFICIENTA2_FIRST + i, ValueConversion::plainToNormEqCoefficients(presetStruct.modules[i].equalizerParameters.a2));
		getController()->performEdit(PARAM_EQCOEFFICIENTA2_FIRST + i, ValueConversion::plainToNormEqCoefficients(presetStruct.modules[i].equalizerParameters.a2));
		getController()->setParamNormalized(PARAM_EQCOEFFICIENTB1_FIRST + i, ValueConversion::plainToNormEqCoefficients(presetStruct.modules[i].equalizerParameters.b1));
		getController()->performEdit(PARAM_EQCOEFFICIENTB1_FIRST + i, ValueConversion::plainToNormEqCoefficients(presetStruct.modules[i].equalizerParameters.b1));
		getController()->setParamNormalized(PARAM_EQCOEFFICIENTB2_FIRST + i, ValueConversion::plainToNormEqCoefficients(presetStruct.modules[i].equalizerParameters.b2));
		getController()->performEdit(PARAM_EQCOEFFICIENTB2_FIRST + i, ValueConversion::plainToNormEqCoefficients(presetStruct.modules[i].equalizerParameters.b2));
		getController()->setParamNormalized(PARAM_EQLIMITER_FIRST + i, presetStruct.modules[i].equalizerParameters.limiter);
		getController()->performEdit(PARAM_EQLIMITER_FIRST + i, presetStruct.modules[i].equalizerParameters.limiter);

		// Set allpass parameters
		getController()->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + i, ValueConversion::plainToNormDelay(presetStruct.modules[i].allpassParameters.delay));
		getController()->performEdit(PARAM_ALLPASSDELAY_FIRST + i, ValueConversion::plainToNormDelay(presetStruct.modules[i].allpassParameters.delay));
		getController()->setParamNormalized(PARAM_ALLPASSDECAY_FIRST + i, ValueConversion::plainToNormDecay(presetStruct.modules[i].allpassParameters.decay));
		getController()->performEdit(PARAM_ALLPASSDECAY_FIRST + i, ValueConversion::plainToNormDecay(presetStruct.modules[i].allpassParameters.decay));
		getController()->setParamNormalized(PARAM_ALLPASSDIFFKSIGN_FIRST + i, presetStruct.modules[i].allpassParameters.diffKSignNegative);
		getController()->performEdit(PARAM_ALLPASSDIFFKSIGN_FIRST + i, presetStruct.modules[i].allpassParameters.diffKSignNegative);
		getController()->setParamNormalized(PARAM_ALLPASSMODENABLED_FIRST + i, presetStruct.modules[i].allpassParameters.modulationEnabled);
		getController()->performEdit(PARAM_ALLPASSMODENABLED_FIRST + i, presetStruct.modules[i].allpassParameters.modulationEnabled);
		getController()->setParamNormalized(PARAM_ALLPASSMODSIGNALTYPE_FIRST + i, ValueConversion::plainToNormModSignalType(presetStruct.modules[i].allpassParameters.modulationSignalType));
		getController()->performEdit(PARAM_ALLPASSMODSIGNALTYPE_FIRST + i, ValueConversion::plainToNormModSignalType(presetStruct.modules[i].allpassParameters.modulationSignalType));
		getController()->setParamNormalized(PARAM_ALLPASSMODEXCURSION_FIRST + i, ValueConversion::plainToNormModExcursion(presetStruct.modules[i].allpassParameters.modulationExcursion));
		getController()->performEdit(PARAM_ALLPASSMODEXCURSION_FIRST + i, ValueConversion::plainToNormModExcursion(presetStruct.modules[i].allpassParameters.modulationExcursion));
		getController()->setParamNormalized(PARAM_ALLPASSMODRATE_FIRST + i, ValueConversion::plainToNormModRate(presetStruct.modules[i].allpassParameters.modulationRate));
		getController()->performEdit(PARAM_ALLPASSMODRATE_FIRST + i, ValueConversion::plainToNormModRate(presetStruct.modules[i].allpassParameters.modulationRate));
		getController()->setParamNormalized(PARAM_ALLPASSBYPASS_FIRST + i, presetStruct.modules[i].allpassParameters.bypass);
		getController()->performEdit(PARAM_ALLPASSBYPASS_FIRST + i, presetStruct.modules[i].allpassParameters.bypass);

		// Set output parameters
		getController()->setParamNormalized(PARAM_OUTGAIN_FIRST + i, ValueConversion::plainToNormOutputGain(presetStruct.modules[i].outputParameters.gain));
		getController()->performEdit(PARAM_OUTGAIN_FIRST + i, ValueConversion::plainToNormOutputGain(presetStruct.modules[i].outputParameters.gain));
		getController()->setParamNormalized(PARAM_OUTLIMITER_FIRST + i, presetStruct.modules[i].outputParameters.limiter);
		getController()->performEdit(PARAM_OUTLIMITER_FIRST + i, presetStruct.modules[i].outputParameters.limiter);
		getController()->setParamNormalized(PARAM_OUTBYPASS_FIRST + i, presetStruct.modules[i].outputParameters.bypass);
		getController()->performEdit(PARAM_OUTBYPASS_FIRST + i, presetStruct.modules[i].outputParameters.bypass);
	
		apGuiModules[i]->setDirty();
	}

	// Signal generator 
	getController()->setParamNormalized(PARAM_SIGNALGENERATOR_SIGNALTYPE, ValueConversion::plainToNormSignalType(presetStruct.signalGenerator.signalType));
	getController()->performEdit(PARAM_SIGNALGENERATOR_SIGNALTYPE, ValueConversion::plainToNormSignalType(presetStruct.signalGenerator.signalType));
	getController()->setParamNormalized(PARAM_SIGNALGENERATOR_AMPLITUDE, ValueConversion::plainToNormSignalAmplitude(presetStruct.signalGenerator.gain));
	getController()->performEdit(PARAM_SIGNALGENERATOR_AMPLITUDE, ValueConversion::plainToNormSignalAmplitude(presetStruct.signalGenerator.gain));
	getController()->setParamNormalized(PARAM_SIGNALGENERATOR_WIDTH, ValueConversion::plainToNormSignalWidth(presetStruct.signalGenerator.width));
	getController()->performEdit(PARAM_SIGNALGENERATOR_WIDTH, ValueConversion::plainToNormSignalWidth(presetStruct.signalGenerator.width));
	getController()->setParamNormalized(PARAM_SIGNALGENERATOR_TIME, ValueConversion::plainToNormSignalTime(presetStruct.signalGenerator.time));
	getController()->performEdit(PARAM_SIGNALGENERATOR_TIME, ValueConversion::plainToNormSignalTime(presetStruct.signalGenerator.time));
	getController()->setParamNormalized(PARAM_SIGNALGENERATOR_AUTOFIREENABLED, presetStruct.signalGenerator.autoFireEnabled);
	getController()->performEdit(PARAM_SIGNALGENERATOR_AUTOFIREENABLED, presetStruct.signalGenerator.autoFireEnabled);

	// Graphics view
	for (unsigned int i = 0; i < presetStruct.graphicsView.modules.size(); ++i) {
		if (i >= MAXMODULENUMBER) break;
		graphicsView->setModulePosition(i, CPoint(presetStruct.graphicsView.modules[i].positionX, presetStruct.graphicsView.modules[i].positionY));
		graphicsView->makeModuleVisible(i, presetStruct.graphicsView.modules[i].isVisible);
	}
	for (unsigned int i = 0; i < presetStruct.graphicsView.vstInputs.size(); ++i) {
		if (i >= MAXVSTINPUTS) break;
		graphicsView->setVstInputPosition(i, CPoint(presetStruct.graphicsView.vstInputs[i].positionX, presetStruct.graphicsView.vstInputs[i].positionY));
	}
	for (unsigned int i = 0; i < presetStruct.graphicsView.vstOutputs.size(); ++i) {
		if (i >= MAXVSTOUTPUTS) break;
		graphicsView->setVstOutputPosition(i, CPoint(presetStruct.graphicsView.vstOutputs[i].positionX, presetStruct.graphicsView.vstOutputs[i].positionY));
	}

	for (unsigned int i = 0; i < presetStruct.generalParamters.vstOutputMenuIndexes.size(); ++i) {
		if (i >= MAXVSTOUTPUTS) break;
		getController()->setParamNormalized(PARAM_GENERALVSTOUTPUTSELECT_FIRST + i, ValueConversion::plainToNormMixerInputSelect(presetStruct.generalParamters.vstOutputMenuIndexes[i]));
		getController()->performEdit(PARAM_GENERALVSTOUTPUTSELECT_FIRST + i, ValueConversion::plainToNormMixerInputSelect(presetStruct.generalParamters.vstOutputMenuIndexes[i]));
	}

	// Update the GUI with the new parameter values
	updateGuiWithControllerParameters();

	/*workspaceView->setDirty();
	graphicsView->setDirty();*/
	workspaceView->invalid();
	graphicsView->invalid();
}

const XmlPresetReadWrite::Preset ReverbNetworkEditor::getXmlPreset() {
	XmlPresetReadWrite::Preset p = {}; // Initialize
	
	// build info...
	p.name = dynamic_cast<VSTGUI::CTextEdit*>(guiElements[id_general_textEdit_presetFilePath])->getText();

	p.buildVersion = pluginVersion;
	p.maxModuleNumber = MAXMODULENUMBER;
	p.maxVstInputs = MAXVSTINPUTS;
	p.maxVstOutputs = MAXVSTOUTPUTS;

	// Build up the preset structure
	for (unsigned int i = 0; i < apGuiModules.size(); ++i) {
		XmlPresetReadWrite::Module m = {};
		std::string temp = dynamic_cast<VSTGUI::CTextEdit*>(guiElements[id_module_textEdit_titleFirst + i])->getText();
		std::string stringToErase = "APM";
		stringToErase.append(std::to_string(i));
		stringToErase.append(" ");
		std::string::size_type w = temp.find(stringToErase);
		if (w != std::string::npos) {
			temp.erase(w, stringToErase.length());
		}
		m.name = temp;
		m.id = apGuiModules[i]->getModuleId();
		m.positionX = apGuiModules[i]->getViewSize().getTopLeft().x;
		m.positionY = apGuiModules[i]->getViewSize().getTopLeft().y;
		m.isVisible = apGuiModules[i]->isVisible();
		m.isCollapsed = apGuiModules[i]->isCollapsed();

		XmlPresetReadWrite::Mixer mixer = {};
		for (unsigned int j = 0; j < MAXMODULENUMBER; ++j) {
			XmlPresetReadWrite::ModuleOutput mo = {};
			mo.gainFactor = ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + j));
			mo.muted = (getController()->getParamNormalized(PARAM_MIXERINPUTMUTED_FIRST + i * MAXINPUTS + j) != 0.0);
			mo.soloed = (getController()->getParamNormalized(PARAM_MIXERINPUTSOLOED_FIRST + i * MAXINPUTS + j) != 0.0);
			mixer.moduleOutputs.push_back(mo);
		}
		for (unsigned int j = 0; j < MAXVSTINPUTS; ++j) {
			XmlPresetReadWrite::VstInput vi = {};
			vi.gainFactor = ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + j + MAXMODULENUMBER));
			vi.muted = (getController()->getParamNormalized(PARAM_MIXERINPUTMUTED_FIRST + i * MAXINPUTS + j + MAXMODULENUMBER) != 0.0);
			vi.soloed = (getController()->getParamNormalized(PARAM_MIXERINPUTSOLOED_FIRST + i * MAXINPUTS + j + MAXMODULENUMBER) != 0.0);
			mixer.vstInputs.push_back(vi);
		}
		XmlPresetReadWrite::SignalGeneratorInput sgI = {};
		sgI.gainFactor = ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + i * MAXINPUTS + MAXMODULENUMBER + MAXVSTINPUTS));
		sgI.muted = (getController()->getParamNormalized(PARAM_MIXERINPUTMUTED_FIRST + i * MAXINPUTS + MAXMODULENUMBER + MAXVSTINPUTS) != 0.0);
		sgI.soloed = (getController()->getParamNormalized(PARAM_MIXERINPUTSOLOED_FIRST + i * MAXINPUTS + MAXMODULENUMBER + MAXVSTINPUTS) != 0.0);
		mixer.signalGeneratorInput = sgI;
		for (unsigned int j = 0; j < MAXMODULEINPUTS; ++j) {
			mixer.inputSlots.push_back(ValueConversion::normToPlainMixerInputSelect(getController()->getParamNormalized(PARAM_MIXERINPUTSELECT_FIRST + i * MAXMODULEINPUTS + j)));
		}
		m.mixerParameters = mixer;

		XmlPresetReadWrite::Quantizer q = {};
		q.quantization = ValueConversion::normToPlainQuantization(getController()->getParamNormalized(PARAM_QUANTIZERBITDEPTH_FIRST + i));
		q.bypass = (getController()->getParamNormalized(PARAM_QUANTIZERBYPASS_FIRST + i) != 0.0);
		m.quantizerParameters = q;

		XmlPresetReadWrite::Equalizer e = {};
		e.filterTypeIndex = ValueConversion::normToPlainFilterTypeSelect(getController()->getParamNormalized(PARAM_EQFILTERTYPE_FIRST + i));
		e.frequency = ValueConversion::normToPlainProcCenterFreq(getController()->getParamNormalized(PARAM_EQCENTERFREQ_FIRST + i));
		e.qFactor = ValueConversion::normToPlainQFactor(getController()->getParamNormalized(PARAM_EQQFACTOR_FIRST + i));
		e.gain = ValueConversion::normToPlainEqGain(getController()->getParamNormalized(PARAM_EQGAIN_FIRST + i));
		e.a0 = ValueConversion::normToPlainEqCoefficients(getController()->getParamNormalized(PARAM_EQCOEFFICIENTA0_FIRST + i));
		e.a1 = ValueConversion::normToPlainEqCoefficients(getController()->getParamNormalized(PARAM_EQCOEFFICIENTA1_FIRST + i));
		e.a2 = ValueConversion::normToPlainEqCoefficients(getController()->getParamNormalized(PARAM_EQCOEFFICIENTA2_FIRST + i));
		e.b1 = ValueConversion::normToPlainEqCoefficients(getController()->getParamNormalized(PARAM_EQCOEFFICIENTB1_FIRST + i));
		e.b2 = ValueConversion::normToPlainEqCoefficients(getController()->getParamNormalized(PARAM_EQCOEFFICIENTB2_FIRST + i));
		e.limiter = (getController()->getParamNormalized(PARAM_EQLIMITER_FIRST + i) != 0.0);
		e.bypass = (getController()->getParamNormalized(PARAM_EQBYPASS_FIRST + i) != 0.0);
		m.equalizerParameters = e;

		XmlPresetReadWrite::Allpass a = {};
		a.delay = ValueConversion::normToPlainDelay(getController()->getParamNormalized(PARAM_ALLPASSDELAY_FIRST + i));
		a.decay = ValueConversion::normToPlainDecay(getController()->getParamNormalized(PARAM_ALLPASSDECAY_FIRST + i));
		a.diffKSignNegative = (getController()->getParamNormalized(PARAM_ALLPASSDIFFKSIGN_FIRST + i) != 0.0);
		a.modulationEnabled = (getController()->getParamNormalized(PARAM_ALLPASSMODENABLED_FIRST + i) != 0.0);
		a.modulationSignalType = ValueConversion::normToPlainModSignalType(getController()->getParamNormalized(PARAM_ALLPASSMODSIGNALTYPE_FIRST + i));
		a.modulationExcursion = ValueConversion::normToPlainModExcursion(getController()->getParamNormalized(PARAM_ALLPASSMODEXCURSION_FIRST + i));
		a.modulationRate = ValueConversion::normToPlainModRate(getController()->getParamNormalized(PARAM_ALLPASSMODRATE_FIRST + i));
		a.bypass = (getController()->getParamNormalized(PARAM_ALLPASSBYPASS_FIRST + i) != 0.0);
		m.allpassParameters = a;

		XmlPresetReadWrite::Output o = {};
		o.gain = ValueConversion::normToPlainOutputGain(getController()->getParamNormalized(PARAM_OUTGAIN_FIRST + i));
		o.limiter = (getController()->getParamNormalized(PARAM_OUTLIMITER_FIRST + i) != 0.0);
		o.bypass = (getController()->getParamNormalized(PARAM_OUTBYPASS_FIRST + i) != 0.0);
		m.outputParameters = o;

		p.modules.push_back(m);
	}

	XmlPresetReadWrite::SignalGenerator sG = {};
	sG.signalType = ValueConversion::normToPlainSignalType(getController()->getParamNormalized(PARAM_SIGNALGENERATOR_SIGNALTYPE));
	sG.gain = ValueConversion::normToPlainSignalAmplitude(getController()->getParamNormalized(PARAM_SIGNALGENERATOR_AMPLITUDE));
	sG.width = ValueConversion::normToPlainSignalWidth(getController()->getParamNormalized(PARAM_SIGNALGENERATOR_WIDTH));
	sG.time = ValueConversion::normToPlainSignalTime(getController()->getParamNormalized(PARAM_SIGNALGENERATOR_TIME));
	sG.autoFireEnabled = getController()->getParamNormalized(PARAM_SIGNALGENERATOR_AUTOFIREENABLED) != 0.0;
	p.signalGenerator = sG;

	XmlPresetReadWrite::GraphicsView gV = {};
	for (unsigned int i = 0; i < MAXMODULENUMBER; ++i) {
		XmlPresetReadWrite::GraphicsModule gM = {};
		gM.isVisible = graphicsView->isModuleVisible(i);
		gM.positionX = graphicsView->getModulePosition(i).x;
		gM.positionY = graphicsView->getModulePosition(i).y;
		gV.modules.push_back(gM);
	}
	for (unsigned int i = 0; i < MAXVSTINPUTS; ++i) {
		XmlPresetReadWrite::GraphicsVstInput gI = {};
		gI.positionX = graphicsView->getVstInputPosition(i).x;
		gI.positionY = graphicsView->getVstInputPosition(i).y;
		gV.vstInputs.push_back(gI);
	}
	for (unsigned int i = 0; i < MAXVSTOUTPUTS; ++i) {
		XmlPresetReadWrite::GraphicsVstOutput gO = {};
		gO.positionX = graphicsView->getVstOutputPosition(i).x;
		gO.positionY = graphicsView->getVstOutputPosition(i).y;
		gV.vstOutputs.push_back(gO);
	}
	p.graphicsView = gV;

	XmlPresetReadWrite::General g = {};
	for (unsigned int i = 0; i < MAXVSTOUTPUTS; ++i) {
		g.vstOutputMenuIndexes.push_back(ValueConversion::normToPlainMixerInputSelect(getController()->getParamNormalized(PARAM_GENERALVSTOUTPUTSELECT_FIRST + i)));
	}
	p.generalParamters = g;

	return p;
}

void ReverbNetworkEditor::copyModuleParameters(const unsigned int& sourceModuleId, XmlPresetReadWrite::Module& m) {
	XmlPresetReadWrite::Mixer mixer = {};
	for (unsigned int j = 0; j < MAXMODULENUMBER; ++j) {
		XmlPresetReadWrite::ModuleOutput mo = {};
		mo.gainFactor = ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + sourceModuleId * MAXINPUTS + j));
		mo.muted = (getController()->getParamNormalized(PARAM_MIXERINPUTMUTED_FIRST + sourceModuleId * MAXINPUTS + j) != 0.0);
		mo.soloed = (getController()->getParamNormalized(PARAM_MIXERINPUTSOLOED_FIRST + sourceModuleId * MAXINPUTS + j) != 0.0);
		mixer.moduleOutputs.push_back(mo);
	}
	for (unsigned int j = 0; j < MAXVSTINPUTS; ++j) {
		XmlPresetReadWrite::VstInput vi = {};
		vi.gainFactor = ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + sourceModuleId * MAXINPUTS + j + MAXMODULENUMBER));
		vi.muted = (getController()->getParamNormalized(PARAM_MIXERINPUTMUTED_FIRST + sourceModuleId * MAXINPUTS + j + MAXMODULENUMBER) != 0.0);
		vi.soloed = (getController()->getParamNormalized(PARAM_MIXERINPUTSOLOED_FIRST + sourceModuleId * MAXINPUTS + j + MAXMODULENUMBER) != 0.0);
		mixer.vstInputs.push_back(vi);
	}
	XmlPresetReadWrite::SignalGeneratorInput sgI = {};
	sgI.gainFactor = ValueConversion::normToPlainInputGain(getController()->getParamNormalized(PARAM_MIXERGAIN_FIRST + sourceModuleId * MAXINPUTS + MAXMODULENUMBER + MAXVSTINPUTS));
	sgI.muted = (getController()->getParamNormalized(PARAM_MIXERINPUTMUTED_FIRST + sourceModuleId * MAXINPUTS + MAXMODULENUMBER + MAXVSTINPUTS) != 0.0);
	sgI.soloed = (getController()->getParamNormalized(PARAM_MIXERINPUTSOLOED_FIRST + sourceModuleId * MAXINPUTS + MAXMODULENUMBER + MAXVSTINPUTS) != 0.0);
	mixer.signalGeneratorInput = sgI;
	for (unsigned int j = 0; j < MAXMODULEINPUTS; ++j) {
		mixer.inputSlots.push_back(ValueConversion::normToPlainMixerInputSelect(getController()->getParamNormalized(PARAM_MIXERINPUTSELECT_FIRST + sourceModuleId * MAXMODULEINPUTS + j)));
	}
	m.mixerParameters = mixer;

	XmlPresetReadWrite::Quantizer q = {};
	q.quantization = ValueConversion::normToPlainQuantization(getController()->getParamNormalized(PARAM_QUANTIZERBITDEPTH_FIRST + sourceModuleId));
	q.bypass = (getController()->getParamNormalized(PARAM_QUANTIZERBYPASS_FIRST + sourceModuleId) != 0.0);
	m.quantizerParameters = q;

	XmlPresetReadWrite::Equalizer e = {};
	e.filterTypeIndex = ValueConversion::normToPlainFilterTypeSelect(getController()->getParamNormalized(PARAM_EQFILTERTYPE_FIRST + sourceModuleId));
	e.frequency = ValueConversion::normToPlainProcCenterFreq(getController()->getParamNormalized(PARAM_EQCENTERFREQ_FIRST + sourceModuleId));
	e.qFactor = ValueConversion::normToPlainQFactor(getController()->getParamNormalized(PARAM_EQQFACTOR_FIRST + sourceModuleId));
	e.gain = ValueConversion::normToPlainEqGain(getController()->getParamNormalized(PARAM_EQGAIN_FIRST + sourceModuleId));
	e.a0 = ValueConversion::normToPlainEqCoefficients(getController()->getParamNormalized(PARAM_EQCOEFFICIENTA0_FIRST + sourceModuleId));
	e.a1 = ValueConversion::normToPlainEqCoefficients(getController()->getParamNormalized(PARAM_EQCOEFFICIENTA1_FIRST + sourceModuleId));
	e.a2 = ValueConversion::normToPlainEqCoefficients(getController()->getParamNormalized(PARAM_EQCOEFFICIENTA2_FIRST + sourceModuleId));
	e.b1 = ValueConversion::normToPlainEqCoefficients(getController()->getParamNormalized(PARAM_EQCOEFFICIENTB1_FIRST + sourceModuleId));
	e.b2 = ValueConversion::normToPlainEqCoefficients(getController()->getParamNormalized(PARAM_EQCOEFFICIENTB2_FIRST + sourceModuleId));
	e.limiter = (getController()->getParamNormalized(PARAM_EQLIMITER_FIRST + sourceModuleId) != 0.0);
	e.bypass = (getController()->getParamNormalized(PARAM_EQBYPASS_FIRST + sourceModuleId) != 0.0);
	m.equalizerParameters = e;

	XmlPresetReadWrite::Allpass a = {};
	a.delay = ValueConversion::normToPlainDelay(getController()->getParamNormalized(PARAM_ALLPASSDELAY_FIRST + sourceModuleId));
	a.decay = ValueConversion::normToPlainDecay(getController()->getParamNormalized(PARAM_ALLPASSDECAY_FIRST + sourceModuleId));
	a.diffKSignNegative = (getController()->getParamNormalized(PARAM_ALLPASSDIFFKSIGN_FIRST + sourceModuleId) != 0.0);
	a.modulationEnabled = (getController()->getParamNormalized(PARAM_ALLPASSMODENABLED_FIRST + sourceModuleId) != 0.0);
	a.modulationSignalType = ValueConversion::normToPlainModSignalType(getController()->getParamNormalized(PARAM_ALLPASSMODSIGNALTYPE_FIRST + sourceModuleId));
	a.modulationExcursion = ValueConversion::normToPlainModExcursion(getController()->getParamNormalized(PARAM_ALLPASSMODEXCURSION_FIRST + sourceModuleId));
	a.modulationRate = ValueConversion::normToPlainModRate(getController()->getParamNormalized(PARAM_ALLPASSMODRATE_FIRST + sourceModuleId));
	a.bypass = (getController()->getParamNormalized(PARAM_ALLPASSBYPASS_FIRST + sourceModuleId) != 0.0);
	m.allpassParameters = a;

	XmlPresetReadWrite::Output o = {};
	o.gain = ValueConversion::normToPlainOutputGain(getController()->getParamNormalized(PARAM_OUTGAIN_FIRST + sourceModuleId));
	o.limiter = (getController()->getParamNormalized(PARAM_OUTLIMITER_FIRST + sourceModuleId) != 0.0);
	o.bypass = (getController()->getParamNormalized(PARAM_OUTBYPASS_FIRST + sourceModuleId) != 0.0);
	m.outputParameters = o;
}

void ReverbNetworkEditor::pasteModuleParameters(const unsigned int& destModuleId, const XmlPresetReadWrite::Module& m) {
	// Set mixer parameters	
	// Module outputs first
	for (unsigned int k = 0; k < m.mixerParameters.moduleOutputs.size(); ++k) {
		getController()->setParamNormalized(PARAM_MIXERGAIN_FIRST + destModuleId * MAXINPUTS + k, ValueConversion::plainToNormInputGain(m.mixerParameters.moduleOutputs[k].gainFactor));
		getController()->performEdit(PARAM_MIXERGAIN_FIRST + destModuleId * MAXINPUTS + k, ValueConversion::plainToNormInputGain(m.mixerParameters.moduleOutputs[k].gainFactor));
		getController()->setParamNormalized(PARAM_MIXERINPUTMUTED_FIRST + destModuleId * MAXINPUTS + k, m.mixerParameters.moduleOutputs[k].muted);
		getController()->performEdit(PARAM_MIXERINPUTMUTED_FIRST + destModuleId * MAXINPUTS + k, m.mixerParameters.moduleOutputs[k].muted);
		getController()->setParamNormalized(PARAM_MIXERINPUTSOLOED_FIRST + destModuleId * MAXINPUTS + k, m.mixerParameters.moduleOutputs[k].soloed);
		getController()->performEdit(PARAM_MIXERINPUTSOLOED_FIRST + destModuleId * MAXINPUTS + k, m.mixerParameters.moduleOutputs[k].soloed);
	}
	// Then the Vst inputs
	for (unsigned int k = 0; k < m.mixerParameters.vstInputs.size(); ++k) {
		getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERGAIN_FIRST + destModuleId * MAXINPUTS + k + m.mixerParameters.moduleOutputs.size()), ValueConversion::plainToNormInputGain(m.mixerParameters.vstInputs[k].gainFactor));
		getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERGAIN_FIRST + destModuleId * MAXINPUTS + k + m.mixerParameters.moduleOutputs.size()), ValueConversion::plainToNormInputGain(m.mixerParameters.vstInputs[k].gainFactor));
		getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTMUTED_FIRST + destModuleId * MAXINPUTS + k + m.mixerParameters.moduleOutputs.size()), m.mixerParameters.vstInputs[k].muted);
		getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTMUTED_FIRST + destModuleId * MAXINPUTS + k + m.mixerParameters.moduleOutputs.size()), m.mixerParameters.vstInputs[k].muted);
		getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTSOLOED_FIRST + destModuleId * MAXINPUTS + k + m.mixerParameters.moduleOutputs.size()), m.mixerParameters.vstInputs[k].soloed);
		getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTSOLOED_FIRST + destModuleId * MAXINPUTS + k + m.mixerParameters.moduleOutputs.size()), m.mixerParameters.vstInputs[k].soloed);
	}
	getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERGAIN_FIRST + destModuleId * MAXINPUTS + m.mixerParameters.moduleOutputs.size() + m.mixerParameters.vstInputs.size()), ValueConversion::plainToNormInputGain(m.mixerParameters.signalGeneratorInput.gainFactor));
	getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERGAIN_FIRST + destModuleId * MAXINPUTS + m.mixerParameters.moduleOutputs.size() + m.mixerParameters.vstInputs.size()), ValueConversion::plainToNormInputGain(m.mixerParameters.signalGeneratorInput.gainFactor));
	getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTMUTED_FIRST + destModuleId * MAXINPUTS + m.mixerParameters.moduleOutputs.size() + m.mixerParameters.vstInputs.size()), m.mixerParameters.signalGeneratorInput.muted);
	getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTMUTED_FIRST + destModuleId * MAXINPUTS + m.mixerParameters.moduleOutputs.size() + m.mixerParameters.vstInputs.size()), m.mixerParameters.signalGeneratorInput.muted);
	getController()->setParamNormalized(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTSOLOED_FIRST + destModuleId * MAXINPUTS + m.mixerParameters.moduleOutputs.size() + m.mixerParameters.vstInputs.size()), m.mixerParameters.signalGeneratorInput.soloed);
	getController()->performEdit(static_cast<Steinberg::Vst::ParamID>(PARAM_MIXERINPUTSOLOED_FIRST + destModuleId * MAXINPUTS + m.mixerParameters.moduleOutputs.size() + m.mixerParameters.vstInputs.size()), m.mixerParameters.signalGeneratorInput.soloed);
	// Finally the input slots
	for (unsigned int k = 0; k < m.mixerParameters.inputSlots.size(); ++k) {
		getController()->setParamNormalized(PARAM_MIXERINPUTSELECT_FIRST + destModuleId * MAXMODULEINPUTS + k, ValueConversion::plainToNormMixerInputSelect(m.mixerParameters.inputSlots[k]));
		getController()->performEdit(PARAM_MIXERINPUTSELECT_FIRST + destModuleId * MAXMODULEINPUTS + k, ValueConversion::plainToNormMixerInputSelect(m.mixerParameters.inputSlots[k]));
	}

	// Set quantizer parameters
	getController()->setParamNormalized(PARAM_QUANTIZERBITDEPTH_FIRST + destModuleId, ValueConversion::plainToNormQuantization(m.quantizerParameters.quantization));
	getController()->performEdit(PARAM_QUANTIZERBITDEPTH_FIRST + destModuleId, ValueConversion::plainToNormQuantization(m.quantizerParameters.quantization));
	getController()->setParamNormalized(PARAM_QUANTIZERBYPASS_FIRST + destModuleId, m.quantizerParameters.bypass);
	getController()->performEdit(PARAM_QUANTIZERBYPASS_FIRST + destModuleId, m.quantizerParameters.bypass);

	// Set equalizer parameters
	getController()->setParamNormalized(PARAM_EQFILTERTYPE_FIRST + destModuleId, ValueConversion::plainToNormFilterTypeSelect(m.equalizerParameters.filterTypeIndex));
	getController()->performEdit(PARAM_EQFILTERTYPE_FIRST + destModuleId, ValueConversion::plainToNormFilterTypeSelect(m.equalizerParameters.filterTypeIndex));
	getController()->setParamNormalized(PARAM_EQCENTERFREQ_FIRST + destModuleId, ValueConversion::plainToNormProcCenterFreq(m.equalizerParameters.frequency));
	getController()->performEdit(PARAM_EQCENTERFREQ_FIRST + destModuleId, ValueConversion::plainToNormProcCenterFreq(m.equalizerParameters.frequency));
	getController()->setParamNormalized(PARAM_EQQFACTOR_FIRST + destModuleId, ValueConversion::plainToNormQFactor(m.equalizerParameters.qFactor));
	getController()->performEdit(PARAM_EQQFACTOR_FIRST + destModuleId, ValueConversion::plainToNormQFactor(m.equalizerParameters.qFactor));
	getController()->setParamNormalized(PARAM_EQGAIN_FIRST + destModuleId, ValueConversion::plainToNormEqGain(m.equalizerParameters.gain));
	getController()->performEdit(PARAM_EQGAIN_FIRST + destModuleId, ValueConversion::plainToNormEqGain(m.equalizerParameters.gain));
	getController()->setParamNormalized(PARAM_EQCOEFFICIENTA0_FIRST + destModuleId, ValueConversion::plainToNormEqCoefficients(m.equalizerParameters.a0));
	getController()->performEdit(PARAM_EQCOEFFICIENTA0_FIRST + destModuleId, ValueConversion::plainToNormEqCoefficients(m.equalizerParameters.a0));
	getController()->setParamNormalized(PARAM_EQCOEFFICIENTA1_FIRST + destModuleId, ValueConversion::plainToNormEqCoefficients(m.equalizerParameters.a1));
	getController()->performEdit(PARAM_EQCOEFFICIENTA1_FIRST + destModuleId, ValueConversion::plainToNormEqCoefficients(m.equalizerParameters.a1));
	getController()->setParamNormalized(PARAM_EQCOEFFICIENTA2_FIRST + destModuleId, ValueConversion::plainToNormEqCoefficients(m.equalizerParameters.a2));
	getController()->performEdit(PARAM_EQCOEFFICIENTA2_FIRST + destModuleId, ValueConversion::plainToNormEqCoefficients(m.equalizerParameters.a2));
	getController()->setParamNormalized(PARAM_EQCOEFFICIENTB1_FIRST + destModuleId, ValueConversion::plainToNormEqCoefficients(m.equalizerParameters.b1));
	getController()->performEdit(PARAM_EQCOEFFICIENTB1_FIRST + destModuleId, ValueConversion::plainToNormEqCoefficients(m.equalizerParameters.b1));
	getController()->setParamNormalized(PARAM_EQCOEFFICIENTB2_FIRST + destModuleId, ValueConversion::plainToNormEqCoefficients(m.equalizerParameters.b2));
	getController()->performEdit(PARAM_EQCOEFFICIENTB2_FIRST + destModuleId, ValueConversion::plainToNormEqCoefficients(m.equalizerParameters.b2));
	getController()->setParamNormalized(PARAM_EQLIMITER_FIRST + destModuleId, m.equalizerParameters.limiter);
	getController()->performEdit(PARAM_EQLIMITER_FIRST + destModuleId, m.equalizerParameters.limiter);
	getController()->setParamNormalized(PARAM_EQBYPASS_FIRST + destModuleId, m.equalizerParameters.bypass);
	getController()->performEdit(PARAM_EQBYPASS_FIRST + destModuleId, m.equalizerParameters.bypass);

	// Set allpass parameters
	getController()->setParamNormalized(PARAM_ALLPASSDELAY_FIRST + destModuleId, ValueConversion::plainToNormDelay(m.allpassParameters.delay));
	getController()->performEdit(PARAM_ALLPASSDELAY_FIRST + destModuleId, ValueConversion::plainToNormDelay(m.allpassParameters.delay));
	getController()->setParamNormalized(PARAM_ALLPASSDECAY_FIRST + destModuleId, ValueConversion::plainToNormDecay(m.allpassParameters.decay));
	getController()->performEdit(PARAM_ALLPASSDECAY_FIRST + destModuleId, ValueConversion::plainToNormDecay(m.allpassParameters.decay));
	getController()->setParamNormalized(PARAM_ALLPASSDIFFKSIGN_FIRST + destModuleId, m.allpassParameters.diffKSignNegative);
	getController()->performEdit(PARAM_ALLPASSDIFFKSIGN_FIRST + destModuleId, m.allpassParameters.diffKSignNegative);
	getController()->setParamNormalized(PARAM_ALLPASSMODENABLED_FIRST + destModuleId, m.allpassParameters.modulationEnabled);
	getController()->performEdit(PARAM_ALLPASSMODENABLED_FIRST + destModuleId, m.allpassParameters.modulationEnabled);
	getController()->setParamNormalized(PARAM_ALLPASSMODSIGNALTYPE_FIRST + destModuleId, ValueConversion::plainToNormModSignalType(m.allpassParameters.modulationSignalType));
	getController()->performEdit(PARAM_ALLPASSMODSIGNALTYPE_FIRST + destModuleId, ValueConversion::plainToNormModSignalType(m.allpassParameters.modulationSignalType));
	getController()->setParamNormalized(PARAM_ALLPASSMODEXCURSION_FIRST + destModuleId, ValueConversion::plainToNormModExcursion(m.allpassParameters.modulationExcursion));
	getController()->performEdit(PARAM_ALLPASSMODEXCURSION_FIRST + destModuleId, ValueConversion::plainToNormModExcursion(m.allpassParameters.modulationExcursion));
	getController()->setParamNormalized(PARAM_ALLPASSMODRATE_FIRST + destModuleId, ValueConversion::plainToNormModRate(m.allpassParameters.modulationRate));
	getController()->performEdit(PARAM_ALLPASSMODRATE_FIRST + destModuleId, ValueConversion::plainToNormModRate(m.allpassParameters.modulationRate));
	getController()->setParamNormalized(PARAM_ALLPASSBYPASS_FIRST + destModuleId, m.allpassParameters.bypass);
	getController()->performEdit(PARAM_ALLPASSBYPASS_FIRST + destModuleId, m.allpassParameters.bypass);

	// Set output parameters
	getController()->setParamNormalized(PARAM_OUTGAIN_FIRST + destModuleId, ValueConversion::plainToNormOutputGain(m.outputParameters.gain));
	getController()->performEdit(PARAM_OUTGAIN_FIRST + destModuleId, ValueConversion::plainToNormOutputGain(m.outputParameters.gain));
	getController()->setParamNormalized(PARAM_OUTLIMITER_FIRST + destModuleId, m.outputParameters.limiter);
	getController()->performEdit(PARAM_OUTLIMITER_FIRST + destModuleId, m.outputParameters.limiter);
	getController()->setParamNormalized(PARAM_OUTBYPASS_FIRST + destModuleId, m.outputParameters.bypass);
	getController()->performEdit(PARAM_OUTBYPASS_FIRST + destModuleId, m.outputParameters.bypass);

	apGuiModules[destModuleId]->setDirty();
	workspaceView->setDirty();
	updateGuiWithControllerParameters();
}

void ReverbNetworkEditor::applyUserData() {
	if (!editorUserData.presetName.empty()) {
		dynamic_cast<VSTGUI::CTextEdit*>(guiElements[id_general_textEdit_presetFilePath])->setText(editorUserData.presetName.c_str());
	}
	for (unsigned int i = 0; i < editorUserData.moduleNames.size(); ++i) {
		if (i >= MAXMODULENUMBER) break;
		dynamic_cast<VSTGUI::CTextEdit*>(guiElements[id_module_textEdit_titleFirst + i])->setText(editorUserData.moduleNames[i].c_str());
		valueChanged(guiElements[id_module_textEdit_titleFirst + i]);
		std::string temp = "APM";
		temp.append(std::to_string(i));
		temp.append(" ");
		temp.append(editorUserData.moduleNames[i]);
		dynamic_cast<VSTGUI::CTextEdit*>(guiElements[id_module_textEdit_titleFirst + i])->setText(temp.c_str());
	}
	for (unsigned int i = 0; i < editorUserData.graphicsView.modules.size(); ++i) {
		if (i >= MAXMODULENUMBER) break;
		graphicsView->makeModuleVisible(i, editorUserData.graphicsView.modules[i].isVisible);
		graphicsView->setModulePosition(i, CPoint(editorUserData.graphicsView.modules[i].positionX, editorUserData.graphicsView.modules[i].positionY));
	}
	for (unsigned int i = 0; i < editorUserData.graphicsView.vstInputs.size(); ++i) {
		if (i >= MAXVSTINPUTS) break;
		graphicsView->setVstInputPosition(i, CPoint(editorUserData.graphicsView.vstInputs[i].positionX, editorUserData.graphicsView.vstInputs[i].positionY));
	}
	for (unsigned int i = 0; i < editorUserData.graphicsView.vstOutputs.size(); ++i) {
		if (i >= MAXVSTOUTPUTS) break;
		graphicsView->setVstOutputPosition(i, CPoint(editorUserData.graphicsView.vstOutputs[i].positionX, editorUserData.graphicsView.vstOutputs[i].positionY));
	}
}

void ReverbNetworkEditor::initializeGraphicsView() {
	std::vector<std::string> inputNames;
	for (int i = 0; i < MAXMODULENUMBER; ++i) {
		inputNames.push_back("APM" + std::to_string(i));
	}
	for (int i = 0; i < MAXVSTINPUTS; ++i) {
		inputNames.push_back("VST" + std::to_string(i));
	}
	inputNames.push_back("SIGNALGEN");
	for (int i = 0; i < MAXVSTINPUTS; ++i) {
		graphicsView->createVstInput();
	}
	for (int i = 0; i < MAXVSTOUTPUTS; ++i) {
		graphicsView->createVstOutput();
	}
	for (int i = 0; i < MAXMODULENUMBER; ++i) {
		graphicsView->createModule(dynamic_cast<VSTGUI::CTextEdit*>(guiElements[id_module_textEdit_titleFirst + i])->getText().getString(), i, MAXINPUTS);
		graphicsView->setModuleInputNames(i, inputNames);
	}
	graphicsView->rearrangeModules();
}

void ReverbNetworkEditor::updateGraphicsViewModule(const int& moduleId, const int& input, const double& gainValue) {
	graphicsView->updateModule(moduleId, input, gainValue);
	graphicsView->invalid();
}

void ReverbNetworkEditor::updateGainValuesInOptionMenus(const int& moduleNumber, const int& input, const double& gainValue) {
	std::string temp = "";
	if (input < MAXMODULENUMBER) {
		temp = dynamic_cast<VSTGUI::CTextEdit*>(guiElements[id_module_textEdit_titleFirst + input])->getText();
	}
	else if (input - MAXMODULENUMBER < MAXVSTINPUTS) {
		temp = "VST";
		temp.append(std::to_string(input - MAXMODULENUMBER));
		temp.append(" IN");
	}
	else if (input - MAXMODULENUMBER - MAXVSTINPUTS < 1) {
		temp = "SIGNALGEN";
	}
	if (gainValue != 0.0) {
		temp.append(" [");
		if (gainValue >= 0.0) {
			temp.append(std::to_string(gainValue).c_str(), 4);
		}
		else {
			temp.append(std::to_string(gainValue).c_str(), 5);
		}
		temp.append("] ");
	}
	for (int i = 0; i < MAXMODULEINPUTS; ++i) {
		dynamic_cast<VSTGUI::COptionMenu*>(guiElements[id_mixer_optionMenu_inputSelectFirst + moduleNumber * MAXMODULEINPUTS + i])->getEntry(input + 1)->setTitle(temp.c_str());
		dynamic_cast<VSTGUI::COptionMenu*>(guiElements[id_mixer_optionMenu_inputSelectFirst + moduleNumber * MAXMODULEINPUTS + i])->setDirty();
	}
}

void ReverbNetworkEditor::updateEqStability(const int& moduleNumber, const bool& stable) {
	if (stable) {
		dynamic_cast<VSTGUI::CTextButton*>(guiElements[id_equalizer_button_stabilityFirst + moduleNumber])->setTitle("Stable");
		//dynamic_cast<VSTGUI::CTextButton*>(guiElements[id_equalizer_button_stabilityFirst + moduleNumber])->setGradientStartColor(CColor(0, 200, 0, 255));
	}
	else {
		dynamic_cast<VSTGUI::CTextButton*>(guiElements[id_equalizer_button_stabilityFirst + moduleNumber])->setTitle("Unstable");
		//dynamic_cast<VSTGUI::CTextButton*>(guiElements[id_equalizer_button_stabilityFirst + moduleNumber])->setGradientStartColor(CColor(200, 0, 0, 255));
		guiElements[id_equalizer_switch_bypassFirst + moduleNumber]->setValue(1.0);
		valueChanged(guiElements[id_equalizer_switch_bypassFirst + moduleNumber]);
	}
	dynamic_cast<VSTGUI::CTextButton*>(guiElements[id_equalizer_button_stabilityFirst + moduleNumber])->setDirty();
}

void ReverbNetworkEditor::openModuleDetailView(const int& moduleNumber, const bool& open) {
	// Update check box in the module list
	guiElements[id_general_checkBox_moduleVisibleFirst + moduleNumber]->setValue(open);
	// Update parameter
	valueChanged(guiElements[id_general_checkBox_moduleVisibleFirst + moduleNumber]);
}

tresult PLUGIN_API ReverbNetworkEditor::onKeyDown(char16 key, int16 keyMsg, int16 modifiers) {
	// This will most likely not work because the VST Host will catch all keyboard events
	if (key == 'f') {
		getController()->performEdit(PARAM_SIGNALGENERATOR_FIRE, 1.0);
		return kResultTrue;
	}
	return kResultFalse;
}

char ReverbNetworkEditor::controlModifierClicked(CControl* pControl, long button) {
	return 0;
}

}} // namespaces
