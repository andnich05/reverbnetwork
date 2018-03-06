/*
* GuiCustomValueEdit: Remove unit from input
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

#include "../include/GuiCustomValueEdit.h"

#include "../vstgui4/vstgui/lib/cframe.h"

GuiCustomValueEdit::GuiCustomValueEdit (const VSTGUI::CRect& size, VSTGUI::IControlListener* listener, int32_t tag, VSTGUI::UTF8StringPtr txt, VSTGUI::CBitmap* background, const int32_t style)
	: CTextEdit(size, listener, tag, txt, background, style) {

}

void GuiCustomValueEdit::takeFocus()
{
	CTextEdit::takeFocus();

	if (!stringToTruncate.empty())
	{
		// When user clicks in the text field
		// Get the text from the TextEdit
		std::string currentString = getText().getString();
		// Search the text for the string to truncate
		size_t pos = currentString.find(stringToTruncate);
		// If the string was found => clear the string
		if (pos != std::string::npos) {
			currentString.erase(pos, stringToTruncate.length());

			// Set the new text without the string
			CTextLabel::setText(VSTGUI::UTF8String(currentString));
			if (platformControl)
				platformControl->setText(getText());
		}
	}
}

void GuiCustomValueEdit::looseFocus()
{
	CTextEdit::looseFocus();

	// When user leaves the text field
	// Get the old text before the user started typing
	std::string currentString = platformControl->getText().getString();
	// This makes sure the conversion is made even if the user didn't type anything (non-standard behaviour of VstGui)
	if (currentString == getText().getString()) {
		setText(VSTGUI::UTF8String(currentString));
	}
}
	
void GuiCustomValueEdit::setStringToTruncate(const std::string& str, bool truncateSpaceBeforeString) {
	if (!str.empty()) {
		stringToTruncate = truncateSpaceBeforeString ? " " + str : str;
	}
}

void GuiCustomValueEdit::setValueToStringUserData(const valueToStringUserData & userData)
{
	this->userData = userData;
}

const valueToStringUserData & GuiCustomValueEdit::getValueToStringUserData() const
{
	return userData;
}
