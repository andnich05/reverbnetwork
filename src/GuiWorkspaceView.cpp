/*
* GuiWorkspaceView: Detail view with all modules
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

#include "../include/GuiWorkspaceView.h"
#include <string>

GuiWorkspaceView::GuiWorkspaceView(const CRect& size, Steinberg::Vst::VSTGUIEditor* editor)
	: CViewContainer(size), editor(editor) {

}

void GuiWorkspaceView::parentSizeChanged() {
	editor->notify(this, "ViewSizeChanged");
}
