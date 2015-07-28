#ifndef GUISIGNALGENERATOR_H
#define GUISIGNALGENERATOR_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include "GuiCustomTextLabel.h"

namespace VSTGUI {

	class COptionMenu;
	class CTextEdit;
	class CTextButton;

	class GuiSignalGenerator : public CViewContainer, CControlListener {
	public:
		GuiSignalGenerator(const CRect& size);

	private:
		void createGui();

		virtual void valueChanged(VSTGUI::CControl* pControl);

		GuiCustomTextLabel* labelTitle;
		GuiCustomTextLabel* labelSignalType;
		COptionMenu* menuSignalType;
		GuiCustomTextLabel* labelGain;
		CTextEdit* editGain;
		GuiCustomTextLabel* labelWidth;
		CTextEdit* editWidth;
		CTextButton* buttonAutoFire;
		GuiCustomTextLabel* labelFireTime;
		CTextEdit* editFireTime;
		CTextButton* buttonManualFire;
	};
}


#endif GUISIGNALGENERATOR_H // GUISIGNALGENERATOR_H