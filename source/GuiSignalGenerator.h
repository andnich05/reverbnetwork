#ifndef GUISIGNALGENERATOR_H
#define GUISIGNALGENERATOR_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"

// For CControlListener
#include "public.sdk/source/vst/vstguieditor.h"

namespace VSTGUI {

	class COptionMenu;
	class CTextEdit;
	class CTextButton;
	class GuiCustomTextLabel;
	class GuiCustomValueEdit;

	class GuiSignalGenerator : public CViewContainer, CControlListener {
	public:
		GuiSignalGenerator(const CRect& size, Steinberg::Vst::VSTGUIEditor* editor);
		// Update the GUI elements with Controller values
		void updateFromController();
		// Update a parameter by parameter tag
		void updateParameter(const unsigned long int& tag, const double& value);

	private:
		// Inititalize
		void createGui();
		// Called when the user changes something
		virtual void valueChanged(VSTGUI::CControl* pControl);

		// Gui elements
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

		Steinberg::Vst::VSTGUIEditor* editor; // Pointer to ReverbNetworkEditor
	};
}


#endif GUISIGNALGENERATOR_H // GUISIGNALGENERATOR_H