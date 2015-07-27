#include "GuiCustomTextLabel.h"

namespace VSTGUI {

	GuiCustomTextLabel::GuiCustomTextLabel(const CRect& size, UTF8StringPtr txt, const CFontRef& font, const CHoriTxtAlign& textAlign, const CColor& backgroundColor, const CColor& frameColor)
	: CTextLabel(size, txt, 0, 0) {

		setBackColor(backgroundColor);
		setFrameColor(frameColor);
		setFont(font);
		setHoriAlign(textAlign);
	}
}