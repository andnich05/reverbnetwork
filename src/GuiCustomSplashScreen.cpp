/*
* GuiCustomSplashScreen: Added function to splash the view manually
*
* Copyright (C) 2015  Andrej Nichelmann
*                     Klaus Michael Indlekofer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../include/GuiCustomSplashScreen.h"

#include "../vstgui4/vstgui/lib/cframe.h"

GuiCustomSplashScreen::GuiCustomSplashScreen(const VSTGUI::CRect& size, IControlListener* listener, int32_t tag, CView* splashView)
: CSplashScreen(size, listener, tag, splashView)
, savedUserData(nullptr) {

}

GuiCustomSplashScreen::~GuiCustomSplashScreen() {
	if (savedUserData) {
		delete savedUserData;
		savedUserData = nullptr;
	}
}

void GuiCustomSplashScreen::splash() {
	value = 1.0;
	if (modalView && getFrame() && getFrame()->setModalView(modalView))
	{
		CControl::valueChanged();
	}
}
