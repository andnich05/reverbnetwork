/*
* GuiGraphicsView: Parent view for modules and connections
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

#include "../include/GuiGraphicsView.h"
#include "../include/ValueConversion.h"
#include "../include/GuiGraphicsModule.h"
#include "../include/GuiGraphicsConnections.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"

GuiGraphicsView::GuiGraphicsView(const VSTGUI::CRect& size, const int& numberOfModules, CBaseObject* editor)
: CViewContainer(size), numberOfModules(numberOfModules), editor(editor) {
	modules.resize(numberOfModules);
	connections = new GuiGraphicsConnections(size, numberOfModules);
	connections->setMouseEnabled(false);
	this->addView(connections);
	drawnConnection = Connection();
	moduleClicked = 0;
}


GuiGraphicsView::~GuiGraphicsView() {
	modules.clear();
	vstInputs.clear();
	vstOutputs.clear();
}

void GuiGraphicsView::createModule(const std::string& title, const int& id, const int& numberOfInputs) {
	if (id < (int)(modules.size())) {
		GuiGraphicsModule* module = new GuiGraphicsModule(title, numberOfInputs, true, true);
		this->addView(module);
		modules[id] = module;
		// Connections should be at the top of the view
		this->changeViewZOrder(connections, this->getNbViews() - 1);
	}
}

bool GuiGraphicsView::addModule() {
	for (auto&& m : modules) {
		if (!m->isVisible()) {
			m->setVisible(true);
			// Move to center if it's at 0,0
			if (m->getViewSize().getTopLeft() == VSTGUI::CPoint(0, 0)) {
				m->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(this->getViewSize().getCenter().x, this->getViewSize().getCenter().y / 2), VSTGUI::CPoint(m->getViewSize().getWidth(), m->getViewSize().getHeight())));
			}
			return true;
		}
	}
	return false;
}

void GuiGraphicsView::makeModuleVisible(const int& moduleId, const bool& visible) {
	modules[moduleId]->setVisible(visible);
	if (modules[moduleId]->getViewSize().getTopLeft() == VSTGUI::CPoint(0, 0)) {
		modules[moduleId]->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(this->getViewSize().getCenter().x, this->getViewSize().getCenter().y / 2), VSTGUI::CPoint(modules[moduleId]->getViewSize().getWidth(), modules[moduleId]->getViewSize().getHeight())));
	}
}

bool GuiGraphicsView::isModuleVisible(const int& moduleId) {
	return modules[moduleId]->isVisible();
}

void GuiGraphicsView::createVstInput() {
	std::string title = "VST IN ";
	title.append(std::to_string(vstInputs.size()));
	GuiGraphicsModule* vstInput = new GuiGraphicsModule(title, 0, true, false);
	this->addView(vstInput);
	vstInput->setOutputName("VST IN");
	vstInputs.push_back(vstInput);
	this->changeViewZOrder(connections, this->getNbViews() - 1);
	this->setDirty();
}

void GuiGraphicsView::createVstOutput() {
	std::string title = "VST OUT ";
	title.append(std::to_string(vstOutputs.size()));
	GuiGraphicsModule* vstOutput = new GuiGraphicsModule(title, 1, false, false);
	this->addView(vstOutput);
	std::vector<std::string> name;
	name.push_back("VST OUT");
	vstOutput->setInputNames(name);
	vstOutput->setInputEnabled(0, true);
	vstOutputs.push_back(vstOutput);
	vstOutputConnections.push_back(-1); // -1 means no connection
	this->changeViewZOrder(connections, this->getNbViews() - 1);
	this->setDirty();
}

void GuiGraphicsView::setModuleInputNames(const int& moduleId, const std::vector<std::string> inputNames) {
	if (moduleId < (int)(modules.size())) {
		modules[moduleId]->setInputNames(inputNames);
	}
	this->setDirty();
}

void GuiGraphicsView::updateModule(const int& moduleId, const int& input, const double& gainValue) {
	if (moduleId < (int)(modules.size())) {
		if (gainValue != 0.0) {
			modules[moduleId]->setVisible(true);
		}
		modules[moduleId]->updateInput(input, gainValue);

		this->setDirty();
			
	}
}

void GuiGraphicsView::clearModules() {
	for (auto&& m : modules) {
		m->clearInputs();
		this->setDirty();
	}
}

void GuiGraphicsView::setVstOutputConnection(const int& vstOutput, const int& moduleOrVstInput) {
	if (vstOutput < (int)(vstOutputConnections.size())) {
		vstOutputConnections[vstOutput] = moduleOrVstInput;
		this->setDirty();
	}
}

void GuiGraphicsView::drawBackgroundRect(VSTGUI::CDrawContext* pContext, const VSTGUI::CRect& _updateRect)
{
	CViewContainer::drawBackgroundRect(pContext, _updateRect);

	redraw(pContext);
}

void GuiGraphicsView::redraw(VSTGUI::CDrawContext* pContext) {
	drawModuleConnections(pContext);
}

void GuiGraphicsView::drawModuleConnections(VSTGUI::CDrawContext* pContext) {
	connections->clearConnections();
	// Draw connections from modules/Vst inputs to modules
	for (auto&& m : modules) {
		if (m) {
			if (m->isVisible()) {
				for (int j = 0; j < numberOfModules; ++j) {
					if (m->getGainValue(j) != 0.0) {
						modules[j]->setVisible(true);
						connections->addConnection(m->getInputRectCenter(j), modules[j]->getOutputRectCenter(), std::abs(m->getGainValue(j))); // Gain value determines the transperacy
					}
				}
				for (unsigned int j = 0; j < vstInputs.size(); ++j) {
					if (m->getGainValue(j + numberOfModules) != 0.0) {
						connections->addConnection(m->getInputRectCenter(j + numberOfModules), vstInputs[j]->getOutputRectCenter(), std::abs(m->getGainValue(j + numberOfModules)));
					}
				}
			}
		}
	}
	// Draw connections from modules/Vst inputs to Vst outputs
	for (unsigned int o = 0; o < vstOutputs.size(); ++o) {
		if (vstOutputs[o]->isVisible()) {
			if (vstOutputConnections[o] != -1) {
				if (vstOutputConnections[o] < numberOfModules) {
					connections->addConnection(vstOutputs[o]->getInputRectCenter(0), modules[vstOutputConnections[o]]->getOutputRectCenter(), 1.0);
					modules[vstOutputConnections[o]]->setVisible(true);
				}
				else if (vstOutputConnections[o] < numberOfModules + (int)(vstInputs.size())) {
					connections->addConnection(vstOutputs[o]->getInputRectCenter(0), vstInputs[vstOutputConnections[o] - numberOfModules]->getOutputRectCenter(), 1.0);
				}
			}
		}
	}
}

void GuiGraphicsView::rearrangeModules() {
	// Needs still some tuning...
	int counter = 0;
	for (unsigned int i = 0; i < vstInputs.size(); ++i) {
		vstInputs[i]->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(0, (vstInputs[i]->getHeight() + 5) * counter), VSTGUI::CPoint(vstInputs[i]->getWidth(), vstInputs[i]->getHeight())));
		vstInputs[i]->setMouseableArea(vstInputs[i]->getViewSize());
		++counter;
	}

	counter = 0;
	for (unsigned int i = 0; i < vstOutputs.size(); ++i) {
		vstOutputs[i]->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(this->getVisibleViewSize().getWidth() - vstOutputs[i]->getWidth(), (vstOutputs[i]->getHeight() + 5) * counter), VSTGUI::CPoint(vstOutputs[i]->getWidth(), vstOutputs[i]->getHeight())));
		vstOutputs[i]->setMouseableArea(vstOutputs[i]->getViewSize());
		++counter;
	}

	int moduleCounter = 1;
	int rowCounter = 0;
	int lastUnusedModule = -1;
	double rowMaxYCoordinate = 0.0;
	double lastRowMaxYCoordinate = 0.0;

	for (unsigned int m = 0; m < modules.size(); ++m) {
		if (modules[m]) {
			if (modules[m]->getNumberOfUsedInputs() != 0) {
				if ((modules[m]->getWidth() + 10) * moduleCounter > this->getVisibleViewSize().getWidth() - modules[m]->getWidth() - 8) {
					++rowCounter;
					moduleCounter = 1;
					lastRowMaxYCoordinate = rowMaxYCoordinate;
				}
				modules[m]->setViewSize(VSTGUI::CRect(VSTGUI::CPoint((modules[m]->getWidth() + 8) * moduleCounter, (lastRowMaxYCoordinate + 5)), VSTGUI::CPoint(modules[m]->getWidth(), modules[m]->getHeight())));
				modules[m]->setMouseableArea(modules[m]->getViewSize());
				if (rowMaxYCoordinate < modules[m]->getViewSize().getBottomRight().y) {
					rowMaxYCoordinate = modules[m]->getViewSize().getBottomRight().y;
				}
				++moduleCounter;
			}
			else {
				modules[m]->setVisible(false);
			}
		}
	}
	this->invalid();
}

void GuiGraphicsView::setModulePosition(const int& moduleId, const VSTGUI::CPoint& position) {
	if (moduleId < (int)(modules.size())) {
		modules[moduleId]->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(position), VSTGUI::CPoint(modules[moduleId]->getWidth(), (modules[moduleId]->getHeight()))));
	}
}
	
VSTGUI::CPoint GuiGraphicsView::getModulePosition(const int& moduleId) const {
	if (moduleId < (int)(modules.size())) {
		return modules[moduleId]->getViewSize().getTopLeft();
	}
	return VSTGUI::CPoint(0, 0);
}

void GuiGraphicsView::setVstInputPosition(const int& vstInput, const VSTGUI::CPoint& position) {
	if (vstInput < (int)(vstInputs.size())) {
		vstInputs[vstInput]->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(position), VSTGUI::CPoint(vstInputs[vstInput]->getWidth(), (vstInputs[vstInput]->getHeight()))));
	}
}

VSTGUI::CPoint GuiGraphicsView::getVstInputPosition(const int& vstInput) const {
	if (vstInput < (int)(vstInputs.size())) {
		return vstInputs[vstInput]->getViewSize().getTopLeft();
	}
	return VSTGUI::CPoint(0, 0);
}

void GuiGraphicsView::setVstOutputPosition(const int& vstOutput, const VSTGUI::CPoint& position) {
	if (vstOutput < (int)(vstOutputs.size())) {
		vstOutputs[vstOutput]->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(position), VSTGUI::CPoint(vstOutputs[vstOutput]->getWidth(), (vstOutputs[vstOutput]->getHeight()))));
	}
}

VSTGUI::CPoint GuiGraphicsView::getVstOutputPosition(const int& vstOutput) const {
	if (vstOutput < (int)(vstOutputs.size())) {
		return vstOutputs[vstOutput]->getViewSize().getTopLeft();
	}
	return VSTGUI::CPoint(0, 0);
}

void GuiGraphicsView::setModuleTitle(const int& moduleId, const std::string& title) {
	modules[moduleId]->setTitle(title);
}

VSTGUI::CMessageResult GuiGraphicsView::notify(VSTGUI::CBaseObject* sender, VSTGUI::IdStringPtr message) {
	for (unsigned int i = 0; i < modules.size(); ++i) {
		if (sender == modules[i]) {
			// User has started a new line
			if (message == "StartMouseLine") {
				connections->updateMouseConnectionLine(modules[i]->getOutputRectCenter(), modules[i]->getOutputRectCenter());
				return VSTGUI::kMessageNotified;
			}
			// User has moved the line
			else if (message == "MoveMouseLine") {
				connections->updateMouseConnectionLine(modules[i]->getOutputRectCenter(), modules[i]->getMouseMoveOutputRect());
				return VSTGUI::kMessageNotified;
			}
			// User has finished the line
			else if (message == "EndMouseLine") {
				// Find the module or Vst output to which the connection should go
				for (unsigned int j = 0; j < modules.size(); ++j) {
					if (!modules[j]->isVisible())
					{
						continue; // Invisible modules don't count
					}
					if (modules[j]->getViewSize().pointInside(modules[i]->getMouseUpCoordinates())) {
						if (modules[j]->getGainValue(i) == 0.0) {
							drawnConnection = Connection(i, j, 1.0);
							editor->notify(this, "ConnectionToModule");
							connections->updateMouseConnectionLine(VSTGUI::CPoint(0, 0), VSTGUI::CPoint(0, 0));
							return VSTGUI::kMessageNotified;
						}
					}
				}
				for (unsigned int j = 0; j < vstOutputs.size(); ++j) {
					if (vstOutputs[j]->getViewSize().pointInside(modules[i]->getMouseUpCoordinates())) {
						drawnConnection = Connection(i, j, 1.0);
						editor->notify(this, "ConnectionToVst");
						connections->updateMouseConnectionLine(VSTGUI::CPoint(0, 0), VSTGUI::CPoint(0, 0));
						return VSTGUI::kMessageNotified;
					}
				}
			}
			// User has changed a gain value
			else if (message == "UpdateGainValue") {
				drawnConnection = Connection(modules[i]->getInputToUpdate(), i, modules[i]->getGainValue(modules[i]->getInputToUpdate()));
				editor->notify(this, "ConnectionToModule");
				return VSTGUI::kMessageNotified;
			}
			// User wants to see the detail view of a module
			else if (message == "DoubleClick") {
				moduleClicked = i;
				editor->notify(this, "OpenModuleDetailView");
				return VSTGUI::kMessageNotified;
			}
		}
	}

	// Do the same for every Vst input
	for (unsigned int i = 0; i < vstInputs.size(); ++i) {
		if (sender == vstInputs[i]) {
			if (message == "StartMouseLine") {
				connections->updateMouseConnectionLine(vstInputs[i]->getOutputRectCenter(), vstInputs[i]->getOutputRectCenter());
				return VSTGUI::kMessageNotified;
			}
			else if (message == "MoveMouseLine") {
				connections->updateMouseConnectionLine(vstInputs[i]->getOutputRectCenter(), vstInputs[i]->getMouseMoveOutputRect());
				return VSTGUI::kMessageNotified;
			}
			else if (message == "EndMouseLine") {
				for (unsigned int j = 0; j < modules.size(); ++j) {
					if (!modules[j]->isVisible())
					{
						continue; // Invisible modules don't count
					}
					if (modules[j]->getViewSize().pointInside(vstInputs[i]->getMouseUpCoordinates())) {
						if (modules[j]->getGainValue(i + numberOfModules) == 0.0) {
							drawnConnection = Connection(i + numberOfModules, j, 1.0);
							editor->notify(this, "ConnectionToModule");
							connections->updateMouseConnectionLine(VSTGUI::CPoint(0, 0), VSTGUI::CPoint(0, 0));
							return VSTGUI::kMessageNotified;
						}
					}
				}
				for (unsigned int j = 0; j < vstOutputs.size(); ++j) {
					if (vstOutputs[j]->getViewSize().pointInside(vstInputs[i]->getMouseUpCoordinates())) {
						drawnConnection = Connection(i + numberOfModules, j, 1.0);
						editor->notify(this, "ConnectionToVst");
						connections->updateMouseConnectionLine(VSTGUI::CPoint(0, 0), VSTGUI::CPoint(0, 0));
						return VSTGUI::kMessageNotified;
					}
				}
			}
		}
	}

	// And the Vst outputs
	for (unsigned int i = 0; i < vstOutputs.size(); ++i) {
		if (sender == vstOutputs[i]) {
			// Remove a connection to a Vst output
			if (message == "UpdateGainValue") {
				drawnConnection = Connection(vstOutputs[i]->getInputToUpdate(), i, 0.0);
				editor->notify(this, "ConnectionToVst");
				return VSTGUI::kMessageNotified;
			}
		}
	}

	connections->updateMouseConnectionLine(VSTGUI::CPoint(0, 0), VSTGUI::CPoint(0, 0));
	return VSTGUI::kMessageNotified;
}
