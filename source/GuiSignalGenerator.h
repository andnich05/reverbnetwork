#ifndef GUISIGNALGENERATOR_H
#define GUISIGNALGENERATOR_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include "GuiCustomTextLabel.h"
#include "GuiCustomValueEdit.h"
#include "public.sdk/source/vst/vstguieditor.h"

namespace VSTGUI {

	class COptionMenu;
	class CTextEdit;
	class CTextButton;


	class GuiSignalGenerator : public CViewContainer, CControlListener {
	public:
		GuiSignalGenerator(const CRect& size, Steinberg::Vst::VSTGUIEditor* editor);
		void updateFromController();
		void updateParameter(const unsigned long int& tag, const double& value);

	private:
		void createGui();

		virtual void valueChanged(VSTGUI::CControl* pControl);

		GuiCustomTextLabel* labelTitle;
		GuiCustomTextLabel* labelSignalType;
		COptionMenu* menuSignalType;
		GuiCustomTextLabel* labelAmplitude;
		GuiCustomValueEdit* editAmplitude;
		GuiCustomTextLabel* labelWidth;
		GuiCustomValueEdit* editWidth;
		CTextButton* buttonAutoFire;
		GuiCustomTextLabel* labelFireTime;
		GuiCustomValueEdit* editFireTime;
		CTextButton* buttonManualFire;

		Steinberg::Vst::VSTGUIEditor* editor;
	};
}


#endif GUISIGNALGENERATOR_H // GUISIGNALGENERATOR_H