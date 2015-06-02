#ifndef GUICUSTOMTEXTEDIT_H
#define GUICUSTOMTEXTEDIT_H

#include "../vstgui4/vstgui/lib/controls/ctextedit.h"

namespace VSTGUI {

	class GuiCustomTextEdit : public CTextEdit {

	public:
		GuiCustomTextEdit(const CRect& size, CControlListener* listener, int32_t tag, UTF8StringPtr txt = 0, CBitmap* background = 0, const int32_t style = 0);

		virtual	void takeFocus() VSTGUI_OVERRIDE_VMETHOD;
		virtual	void looseFocus() VSTGUI_OVERRIDE_VMETHOD;

		void setStringToTruncate(UTF8StringPtr string, bool truncateSpaceBeforeString);

	private:
		char stringToTruncate[256];
		char oldString[256];
	};
	
}

#endif // GUICUSTOMTEXTEDIT_H