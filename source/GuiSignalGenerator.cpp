#include "GuiSignalGenerator.h"
#include "../vstgui4/vstgui/lib/crowcolumnview.h"
#include "../vstgui4/vstgui/lib/controls/coptionmenu.h"
#include "../vstgui4/vstgui/lib/controls/ctextedit.h"
#include "../vstgui4/vstgui/lib/controls/cbuttons.h"
#include "ReverbNetworkDefines.h"
#include "ValueConversion.h"

namespace VSTGUI {
	GuiSignalGenerator::GuiSignalGenerator(const CRect& size)
		: CViewContainer(size) {

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
		labelGain = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(80, 15)), "Gain:", kNormalFontSmall, CHoriTxtAlign::kLeftText);
		editGain = new CTextEdit(CRect(CPoint(0, 0), CPoint(80, 15)), this, 0, "-6 dB");
		editGain->setFont(kNormalFontSmall);
		editGain->setHoriAlign(CHoriTxtAlign::kLeftText);
		editGain->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
		editGain->setFontColor(CCOLOR_TEXTEDIT_TEXT);
		editGain->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
		labelWidth = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(80, 15)), "Width:", kNormalFontSmall, CHoriTxtAlign::kLeftText);
		editWidth = new CTextEdit(CRect(CPoint(0, 0), CPoint(80, 15)), this, 0, "100 Samples");
		editWidth->setFont(kNormalFontSmall);
		editWidth->setHoriAlign(CHoriTxtAlign::kLeftText);
		editWidth->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
		editWidth->setFontColor(CCOLOR_TEXTEDIT_TEXT);
		editWidth->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
		buttonAutoFire = new CTextButton(CRect(CPoint(0, 0), CPoint(80, 20)), this, 0, "Auto", CTextButton::kOnOffStyle);
		buttonAutoFire->setGradientStartColor(CCOLOR_BUTTON_STARTNORMALBACKGROUND);
		buttonAutoFire->setGradientEndColor(CCOLOR_BUTTON_ENDNORMALBACKGROUND);
		buttonAutoFire->setGradientStartColorHighlighted(CCOLOR_BUTTON_STARTPRESSEDBACKGROUND);
		buttonAutoFire->setGradientEndColorHighlighted(CCOLOR_BUTTON_ENDPRESSEDBACKGROUND);
		buttonAutoFire->setTextColor(CCOLOR_BUTTON_TEXTNORMAL);
		buttonAutoFire->setTextColorHighlighted(CCOLOR_BUTTON_TEXTPRESSED);
		buttonAutoFire->setRoundRadius(1);
		labelFireTime = new GuiCustomTextLabel(CRect(CPoint(0, 0), CPoint(80, 15)), "Auto Time:", kNormalFontSmall, CHoriTxtAlign::kLeftText);
		editFireTime = new CTextEdit(CRect(CPoint(0, 0), CPoint(80, 15)), this, 0);
		editFireTime->setFont(kNormalFontSmall);
		editFireTime->setHoriAlign(CHoriTxtAlign::kLeftText);
		editFireTime->setBackColor(CCOLOR_TEXTEDIT_BACKGROUND);
		editFireTime->setFontColor(CCOLOR_TEXTEDIT_TEXT);
		editFireTime->setFrameColor(CCOLOR_TEXTEDIT_FRAME);
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
		gainView->addView(labelGain);
		gainView->addView(editGain);
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
		//if (pControl == )
	}
}