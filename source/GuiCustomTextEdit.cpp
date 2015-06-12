#include "GuiCustomTextEdit.h"
#include "../vstgui4/vstgui/lib/cframe.h"

namespace VSTGUI {

	GuiCustomTextEdit::GuiCustomTextEdit(const CRect& size, CControlListener* listener, int32_t tag, UTF8StringPtr txt, CBitmap* background, const int32_t style)
		: CTextEdit(size, listener, tag, txt, background, style) {


	}

	void GuiCustomTextEdit::setStringToTruncate(const std::string& str) {
		// Additional blank before the string
		/*if (truncateSpaceBeforeString) {
			strncpy(stringToTruncate, " ", 2);
		}*/
		// Save the string
		strncat(stringToTruncate, str.c_str(), 256);
	}

	void GuiCustomTextEdit::takeFocus()
	{

		if (platformControl)
			return;
#if TARGET_OS_IPHONE
		if (getFrame()->getFocusView() == this)
			return;
#endif
		bWasReturnPressed = false;

		//--- CustomGuiCustomValueEdit
		// When user clicks in the text field
		char str[256];
		// Get the text from the TextEdit
		strcpy(str, getText());
		// Search the text for the string to truncate
		char* pos = strstr(str, stringToTruncate);
		// If the string was found (means pos is not a nullpointer) => clear the string by setting it to zero
		if (pos) {
			pos += strlen(stringToTruncate);
			char temp[256];
			strncpy(temp, pos, 256);

			// Set the new text without the string
			CTextLabel::setText(temp);
			if (platformControl)
				platformControl->setText(getText());
		}

		
		//---

		// calculate offset for CViewContainers
		CRect rect(getViewSize());
		CPoint p(0, 0);
		localToFrame(p);
		rect.offset(p.x, p.y);

		platformControl = getFrame()->getPlatformFrame()->createPlatformTextEdit(this);



#if TARGET_OS_IPHONE
		getFrame()->setFocusView(this);
#endif
	}

	//------------------------------------------------------------------------
	void GuiCustomTextEdit::looseFocus()
	{
		/*const char* newString = getText();
		if (strcmp() != 0) {

		}*/

		if (platformControl == 0)
			return;

		IPlatformTextEdit* _platformControl = platformControl;
		platformControl = 0;

		updateText(_platformControl);

		//---
		// When user leaves the text field
		// Get the old text before the user started typing
		const char* newText = _platformControl->getText();
		// This makes sure the conversion is made even if the user didn't type anything (non-standard behaviour of VstGui)
		if (strcmp(newText, getText()) == 0) {
			setText(newText);
		}

		char str[256];
		sprintf(str, stringToTruncate);
		//if (*newText != 0) {
			//sprintf(str + strlen(str), " [");
			sprintf(str + strlen(str), newText);
			//sprintf(str + strlen(str), "]");
			setText(str);
		//}

		//---


		_platformControl->forget();

		// if you want to destroy the text edit do it with the loose focus message
		CView* receiver = pParentView ? pParentView : pParentFrame;
		while (receiver)
		{
			if (receiver->notify(this, kMsgLooseFocus) == kMessageNotified)
				break;
			receiver = receiver->getParentView();
		}
	}

}