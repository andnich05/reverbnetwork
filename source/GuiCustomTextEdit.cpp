#include "GuiCustomTextEdit.h"
#include "../vstgui4/vstgui/lib/cframe.h"

namespace VSTGUI {

GuiCustomTextEdit::GuiCustomTextEdit (const CRect& size, CControlListener* listener, int32_t tag, UTF8StringPtr txt, CBitmap* background, const int32_t style)
	: CTextEdit(size, listener, tag, txt, background, style) {
	

}

void GuiCustomTextEdit::takeFocus()
{

	if (platformControl)
		return;
#if TARGET_OS_IPHONE
 	if (getFrame ()->getFocusView () == this)
		return;
#endif
	bWasReturnPressed = false;

	char str[256];
	strcpy(str, getText());
	char* pos = strstr(str, stringToTruncate);
	if (pos) {
		pos[0] = 0;
	}
	
	CTextLabel::setText(str);
	if (platformControl)
		platformControl->setText(getText());

	strcpy(oldString, getText());

	// calculate offset for CViewContainers
	CRect rect (getViewSize ());
	CPoint p (0, 0);
	localToFrame (p);
	rect.offset (p.x, p.y);

	platformControl = getFrame ()->getPlatformFrame ()->createPlatformTextEdit (this);

	

#if TARGET_OS_IPHONE
	getFrame ()->setFocusView (this);
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
	
	updateText (_platformControl);
	
	const char* newText = _platformControl->getText();
	if (strcmp(newText, getText()) == 0) {
		setText(newText);
	}

	_platformControl->forget ();

	// if you want to destroy the text edit do it with the loose focus message
	CView* receiver = pParentView ? pParentView : pParentFrame;
	while (receiver)
	{
		if (receiver->notify (this, kMsgLooseFocus) == kMessageNotified)
			break;
		receiver = receiver->getParentView ();
	}
}
	

void GuiCustomTextEdit::setStringToTruncate(UTF8StringPtr string, bool truncateSpaceBeforeString) {
	if (truncateSpaceBeforeString) {
		stringToTruncate[0] = ' ';
	}
	strncat(stringToTruncate, string, 10);
}

}