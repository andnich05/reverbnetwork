#ifndef GUICUSTOMTEXTLABEL
#define GUICUSTOMTEXTLABEL

#include "../vstgui4/vstgui/lib/controls/ctextlabel.h"
#include "../vstgui4/vstgui/lib/cfont.h"

namespace VSTGUI {
	class GuiCustomTextLabel : public CTextLabel {
	public:
		// Custom constructor with some handy stuff
		GuiCustomTextLabel(const CRect& size, UTF8StringPtr txt = 0, const CFontRef& font = kNormalFont, const CHoriTxtAlign& textAlign = CHoriTxtAlign::kCenterText, const CColor& backgroundColor = CColor(0, 0, 0, 0), const CColor& frameColor = CColor(0, 0, 0, 0));
	};

}

#endif // GUICUSTOMTEXTLABEL