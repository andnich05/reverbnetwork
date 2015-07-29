#include "GuiSignalGenerator.h"
#include "../vstgui4/vstgui/lib/crowcolumnview.h"
#include "../vstgui4/vstgui/lib/controls/coptionmenu.h"
#include "../vstgui4/vstgui/lib/controls/ctextedit.h"
#include "../vstgui4/vstgui/lib/controls/cbuttons.h"
#include "ReverbNetworkDefines.h"
#include "ValueConversion.h"


namespace VSTGUI {
	GuiSignalGenerator::GuiSignalGenerator(const CRect& size, Steinberg::Vst::VSTGUIEditor* editor)
		: CViewContainer(size), editor(editor) {

		createGui();
	}

	void GuiSignalGenerator::createGui() {
		labelTitle = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(getViewSize().getWidth(), 25)), "Signal Generator");
		labelSignalType = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(80, 15)), "Signal Type:", kNormalFontSmall, CHoriTxtAlign::kLeftText);
		menuSignalType = new COptionMenu(CRect(CPoint(0, 0), CPoint(80, 15)), this, 0);
		menuSignalType->addEntry("Dirac");
		menuSignalType->setCurrent(0);
		menuSignalType->setFont(kNormalFontSmall);
		menuSignalType->setBackColor(CCOLOR_OPTIONMENU_BACKGROUND);
		menuSignalType->setFontColor(CCOLOR_OPTIONMENU_TEXT);
		menuSignalType->setFrameColor(CCOLOR_OPTIONMENU_FRAME);
		menuSignalType->setMin(MIN_SIGNALGENERATOR_SIGNALTYPE);
		menuSignalType->setMax(MAX_SIGNALGENERATOR_SIGNALTYPE);
		menuSignalType->setDefaultValue(DEF_SIGNALGENERATOR_SIGNALTYPE);
		labelAmplitude = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(80, 15)), "Gain:", kNormalFontSmall, CHoriTxtAlign::kLeftText);
		editAmplitude = new GuiCustomValueEdit(CRect(CPoint(0, 0), CPoint(80, 15)), this, 0, "-6 dB");
		editAmplitude->setFont(kNormalFontSmall);
		editAmplitude->setHoriAlign(CHoriTxtAlign::kLeftText);
		editAmplitude->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
		editAmplitude->setFontColor(CCOLOR_TEXTEDIT_TEXT);
		editAmplitude->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
		valueToStringUserData* userData = new valueToStringUserData;
		userData->precision = 2;
		userData->unit = UNIT_SIGNALGENERATOR_AMPLITUDE;
		editAmplitude->setValueToStringProc(ValueConversion::textEditValueToStringConversion, userData);
		editAmplitude->setStringToValueProc(ValueConversion::textEditStringToValueConversion);
		editAmplitude->setStringToTruncate(UNIT_SIGNALGENERATOR_AMPLITUDE, true);
		editAmplitude->setMin(MIN_SIGNALGENERATOR_AMPLITUDE);
		editAmplitude->setMax(MAX_SIGNALGENERATOR_AMPLITUDE);
		editAmplitude->setDefaultValue(DEF_SIGNALGENERATOR_AMPLITUDE);
		labelWidth = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(80, 15)), "Width:", kNormalFontSmall, CHoriTxtAlign::kLeftText);
		editWidth = new GuiCustomValueEdit(CRect(CPoint(0, 0), CPoint(80, 15)), this, 0, "100 Samples");
		editWidth->setFont(kNormalFontSmall);
		editWidth->setHoriAlign(CHoriTxtAlign::kLeftText);
		editWidth->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
		editWidth->setFontColor(CCOLOR_TEXTEDIT_TEXT);
		editWidth->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
		valueToStringUserData* userData2 = new valueToStringUserData;
		userData2->precision = 0;
		userData2->unit = UNIT_SIGNALGENERATOR_WIDTH;
		editWidth->setValueToStringProc(ValueConversion::textEditValueToStringConversion, userData2);
		editWidth->setStringToValueProc(ValueConversion::textEditStringToValueConversion);
		editWidth->setStringToTruncate(UNIT_SIGNALGENERATOR_WIDTH, true);
		editWidth->setMin(MIN_SIGNALGENERATOR_WIDTH);
		editWidth->setMax(MAX_SIGNALGENERATOR_WIDTH);
		editWidth->setDefaultValue(DEF_SIGNALGENERATOR_WIDTH);
		buttonAutoFire = new CTextButton(CRect(CPoint(0, 0), CPoint(80, 20)), this, 0, "Auto", CTextButton::kOnOffStyle);
		buttonAutoFire->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
		buttonAutoFire->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
		buttonAutoFire->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
		buttonAutoFire->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
		buttonAutoFire->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
		buttonAutoFire->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
		buttonAutoFire->setRoundRadius(1);
		labelFireTime = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(80, 15)), "Auto Time:", kNormalFontSmall, CHoriTxtAlign::kLeftText);
		editFireTime = new GuiCustomValueEdit(CRect(CPoint(0, 0), CPoint(80, 15)), this, 0);
		editFireTime->setFont(kNormalFontSmall);
		editFireTime->setHoriAlign(CHoriTxtAlign::kLeftText);
		editFireTime->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
		editFireTime->setFontColor(CCOLOR_TEXTEDIT_TEXT);
		editFireTime->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
		valueToStringUserData* userData3 = new valueToStringUserData;
		userData3->precision = 2;
		userData3->unit = UNIT_SIGNALGENERATOR_TIME;
		editFireTime->setValueToStringProc(ValueConversion::textEditValueToStringConversion, userData3);
		editFireTime->setStringToValueProc(ValueConversion::textEditStringToValueConversion);
		editFireTime->setStringToTruncate(UNIT_SIGNALGENERATOR_TIME, true);
		editFireTime->setMin(MIN_SIGNALGENERATOR_TIME);
		editFireTime->setMax(MAX_SIGNALGENERATOR_TIME);
		editFireTime->setDefaultValue(DEF_SIGNALGENERATOR_TIME);
		buttonManualFire = new CTextButton(CRect(CPoint(0, 0), CPoint(80, 20)), this, 0, "Fire!");
		buttonManualFire->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
		buttonManualFire->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
		buttonManualFire->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
		buttonManualFire->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
		buttonManualFire->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
		buttonManualFire->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
		buttonManualFire->setRoundRadius(1);

		CRowColumnView* mainView = new CRowColumnView(getViewSize(), CRowColumnView::kRowStyle, CRowColumnView::kLeftTopEqualy, 5.0, 5.0);
		mainView->setBackgroundColor(CCOLOR_NOCOLOR);
		mainView->addView(labelTitle);
		CRowColumnView* signalView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle);
		signalView->setBackgroundColor(CCOLOR_NOCOLOR);
		signalView->addView(labelSignalType);
		signalView->addView(menuSignalType);
		signalView->sizeToFit();
		mainView->addView(signalView);
		CRowColumnView* gainView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle);
		gainView->setBackgroundColor(CCOLOR_NOCOLOR);
		gainView->addView(labelAmplitude);
		gainView->addView(editAmplitude);
		gainView->sizeToFit();
		mainView->addView(gainView);
		CRowColumnView* widthView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle);
		widthView->setBackgroundColor(CCOLOR_NOCOLOR);
		widthView->addView(labelWidth);
		widthView->addView(editWidth);
		widthView->sizeToFit();
		mainView->addView(widthView);
		CRowColumnView* autoView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle);
		autoView->setBackgroundColor(CCOLOR_NOCOLOR);
		autoView->addView(labelFireTime);
		autoView->addView(editFireTime);
		autoView->sizeToFit();
		mainView->addView(autoView);
		CRowColumnView* buttonView = new CRowColumnView(CRect(CPoint(0, 0), CPoint(0, 0)), CRowColumnView::kColumnStyle);
		buttonView->setBackgroundColor(CCOLOR_NOCOLOR);
		buttonView->addView(buttonAutoFire);
		buttonView->addView(buttonManualFire);
		buttonView->sizeToFit();
		mainView->addView(buttonView);
		this->addView(mainView);
	}

	void GuiSignalGenerator::valueChanged(VSTGUI::CControl* pControl) {
		if (pControl == buttonManualFire) {
			if (pControl->getValue() == 0.0) {
				editor->getController()->performEdit(PARAM_SIGNALGENERATOR_FIRE, 1.0);
			}
		}
		else if (pControl == buttonAutoFire) {
			editor->getController()->setParamNormalized(PARAM_SIGNALGENERATOR_AUTOFIREENABLED, pControl->getValue());
			editor->getController()->performEdit(PARAM_SIGNALGENERATOR_AUTOFIREENABLED, pControl->getValue());
		}
		else if (pControl == menuSignalType) {
			editor->getController()->setParamNormalized(PARAM_SIGNALGENERATOR_SIGNALTYPE, pControl->getValueNormalized());
			editor->getController()->performEdit(PARAM_SIGNALGENERATOR_SIGNALTYPE, pControl->getValueNormalized());
		}
		else if (pControl == editAmplitude) {
			editor->getController()->setParamNormalized(PARAM_SIGNALGENERATOR_AMPLITUDE, pControl->getValueNormalized());
			editor->getController()->performEdit(PARAM_SIGNALGENERATOR_AMPLITUDE, pControl->getValueNormalized());
		}
		else if (pControl == editWidth) {
			editor->getController()->setParamNormalized(PARAM_SIGNALGENERATOR_WIDTH, pControl->getValueNormalized());
			editor->getController()->performEdit(PARAM_SIGNALGENERATOR_WIDTH, pControl->getValueNormalized());
		}
		else if (pControl == editFireTime) {
			editor->getController()->setParamNormalized(PARAM_SIGNALGENERATOR_TIME, pControl->getValueNormalized());
			editor->getController()->performEdit(PARAM_SIGNALGENERATOR_TIME, pControl->getValueNormalized());
		}
	}

	void GuiSignalGenerator::updateFromController() {
		menuSignalType->setValueNormalized(editor->getController()->getParamNormalized(PARAM_SIGNALGENERATOR_SIGNALTYPE));
		editAmplitude->setValueNormalized(editor->getController()->getParamNormalized(PARAM_SIGNALGENERATOR_AMPLITUDE));
		editWidth->setValueNormalized(editor->getController()->getParamNormalized(PARAM_SIGNALGENERATOR_WIDTH));
		editFireTime->setValueNormalized(editor->getController()->getParamNormalized(PARAM_SIGNALGENERATOR_TIME));
		buttonAutoFire->setValueNormalized(editor->getController()->getParamNormalized(PARAM_SIGNALGENERATOR_AUTOFIREENABLED));
	}

	void GuiSignalGenerator::updateParameter(const unsigned long int& tag, const double& value) {
		switch (tag) {
		case PARAM_SIGNALGENERATOR_SIGNALTYPE:
			menuSignalType->setValueNormalized(value);
			break;
		case PARAM_SIGNALGENERATOR_AMPLITUDE:
			editAmplitude->setValueNormalized(value);
			break;
		case PARAM_SIGNALGENERATOR_WIDTH:
			editWidth->setValueNormalized(value);
			break;
		case PARAM_SIGNALGENERATOR_TIME:
			editFireTime->setValueNormalized(value);
			break;
		case PARAM_SIGNALGENERATOR_AUTOFIREENABLED:
			buttonAutoFire->setValueNormalized(value);
			break;
		}
	}
}