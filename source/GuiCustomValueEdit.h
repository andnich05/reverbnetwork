#ifndef GUICUSTOMVALUEEDIT_H
#define GUICUSTOMVALUEEDIT_H

#include "../vstgui4/vstgui/lib/controls/ctextedit.h"

namespace VSTGUI {

	class GuiCustomValueEdit : public CTextEdit {

	public:
		GuiCustomValueEdit(const CRect& size, CControlListener* listener, int32_t tag, UTF8StringPtr txt = 0, CBitmap* background = 0, const int32_t style = 0);

		virtual	void takeFocus() VSTGUI_OVERRIDE_VMETHOD;
		virtual	void looseFocus() VSTGUI_OVERRIDE_VMETHOD;

		void setStringToTruncate(const std::string& str, bool truncateSpaceBeforeString);

	private:
		char stringToTruncate[256];
	};
	
}

#endif // GUICUSTOMVALUEEDIT_H