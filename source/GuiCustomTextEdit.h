#ifndef GUICUSTOMTEXTEDIT_H
#define GUICUSTOMTEXTEDIT_H

#include "../vstgui4/vstgui/lib/controls/ctextedit.h"

namespace VSTGUI {
	class GuiCustomTextEdit : public CTextEdit {
	public:
		GuiCustomTextEdit(const CRect& size, CControlListener* listener, int32_t tag, UTF8StringPtr txt = 0, CBitmap* background = 0, const int32_t style = 0);

		// From CTextEdit
		virtual	void takeFocus() VSTGUI_OVERRIDE_VMETHOD;
		virtual	void looseFocus() VSTGUI_OVERRIDE_VMETHOD;

		// Set string to remove when user clicks in the view
		void setStringToTruncate(const std::string& str);

	private:
		char stringToTruncate[256];
	};

}

#endif // GUICUSTOMTEXTEDIT_H