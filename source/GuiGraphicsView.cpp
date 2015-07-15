#include "GuiGraphicsView.h"
#include "ValueConversion.h"
#include "GuiGraphicsModule.h"
#include "GuiGraphicsConnections.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"
#include <iomanip>
#include <sstream>

namespace VSTGUI {

	GuiGraphicsView::GuiGraphicsView(const CRect& size, const int& numberOfModules, CBaseObject* editor)
	: CViewContainer(size), numberOfModules(numberOfModules), editor(editor) {
		modules.resize(numberOfModules);
		connections = new GuiGraphicsConnections(size, numberOfModules);
		connections->setMouseEnabled(false);
		this->addView(connections);
	}


	GuiGraphicsView::~GuiGraphicsView() {
		// Vstgui takes care of that
		/*for (auto&& m : modules) {
			if (m != nullptr) {
				delete m;
				m = nullptr;
			}
		}
		if (connections) {
			delete connections;
			connections = nullptr;
		}*/
	}

	void GuiGraphicsView::addModule(const std::string& title, const int& id, const int& numberOfInputs) {
		if (id < modules.size()) {
			GuiGraphicsModule* module = new GuiGraphicsModule(title, numberOfInputs, true);
			this->addView(module);
			modules[id] = module;
			this->changeViewZOrder(connections, this->getNbViews() - 1);
		}
	}

	void GuiGraphicsView::addVstInput() {
		std::string title = "VST IN ";
		title.append(std::to_string(vstInputs.size()));
		GuiGraphicsModule* vstInput = new GuiGraphicsModule(title, 0, true);
		this->addView(vstInput);
		vstInput->setOutputName("VST IN");
		vstInputs.push_back(vstInput);
	}

	void GuiGraphicsView::addVstOutput() {
		std::string title = "VST OUT ";
		title.append(std::to_string(vstOutputs.size()));
		GuiGraphicsModule* vstOutput = new GuiGraphicsModule(title, 1, false);
		this->addView(vstOutput);
		std::vector<std::string> name;
		name.push_back("VST OUT");
		vstOutput->setInputNames(name);
		vstOutput->setInputEnabled(0, true);
		vstOutputs.push_back(vstOutput);
	}

	void GuiGraphicsView::setModuleInputNames(const int& moduleId, const std::vector<std::string> inputNames) {
		if (moduleId < modules.size()) {
			modules[moduleId]->setInputNames(inputNames);
		}
	}

	void GuiGraphicsView::updateModule(const int& moduleId, const int& input, const double& gainValue) {
		if (moduleId < modules.size()) {
			if (gainValue != 0.0) {
				modules[moduleId]->setEnabled(true);
			}
			modules[moduleId]->updateInput(input, gainValue);
			this->setDirty();
		}
	}

	void GuiGraphicsView::clearModules() {
		for (auto&& m : modules) {
			m->clearInputs();
		}
	}

	void GuiGraphicsView::drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect)
	{
		if (getDrawBackground())
		{
			CRect oldClip;
			pContext->getClipRect(oldClip);
			CRect newClip(_updateRect);
			newClip.bound(oldClip);
			pContext->setClipRect(newClip);
			CRect tr(0, 0, getViewSize().getWidth(), getViewSize().getHeight());
			getDrawBackground()->draw(pContext, tr, backgroundOffset);
			pContext->setClipRect(oldClip);
		}
		else if ((backgroundColor.alpha != 255 && getTransparency()) || !getTransparency())
		{
			pContext->setDrawMode(kAliasing);
			pContext->setLineWidth(1);
			pContext->setFillColor(backgroundColor);
			pContext->setFrameColor(backgroundColor);
			pContext->setLineStyle(kLineSolid);
			CRect r;
			if (backgroundColorDrawStyle == kDrawFilled || (backgroundColorDrawStyle == kDrawFilledAndStroked && backgroundColor.alpha == 255))
			{
				r = _updateRect;
				r.inset(-1, -1);
			}
			else
			{
				r = getViewSize();
				r.offset(-r.left, -r.top);
			}
			pContext->drawRect(r, backgroundColorDrawStyle);
		}

		redraw(pContext);
	}

	void GuiGraphicsView::redraw(CDrawContext* pContext) {
		//drawModuleRects(pContext);
		drawModuleConnections(pContext);
	}

	//void GuiGraphicsView::drawModuleRects(CDrawContext* pContext) {
	//	for (auto&& m : modules) {
	//		if (m) {
	//			if (m->isEnabled()) {
	//				m->setDirty();
	//			}
	//		}
	//	}
	//}

	void GuiGraphicsView::drawModuleConnections(CDrawContext* pContext) {
		connections->clearConnections();
		for (auto&& m : modules) {
			if (m) {
				if (m->isEnabled()) {
					for (unsigned int j = 0; j < numberOfModules; ++j) {
						if (m->getGainValue(j) != 0.0) {
							connections->setConnection(m->getInputRectCenter(j), modules[j]->getOutputRectCenter(), std::abs(m->getGainValue(j)));
						}
					}
				}
			}
		}
		connections->setDirty();
	}

	void GuiGraphicsView::rearrangeModules() {
		int counter = 0;
		for (unsigned int i = 0; i < vstInputs.size(); ++i) {
			vstInputs[i]->setViewSize(CRect(CPoint(0, (vstInputs[i]->getHeight() + 5) * counter), CPoint(vstInputs[i]->getWidth(), vstInputs[i]->getHeight())));
			vstInputs[i]->setMouseableArea(vstInputs[i]->getViewSize());
			++counter;
		}

		counter = 0;
		for (unsigned int i = 0; i < vstOutputs.size(); ++i) {
			vstOutputs[i]->setViewSize(CRect(CPoint(this->getVisibleViewSize().getWidth() - vstOutputs[i]->getWidth(), (vstOutputs[i]->getHeight() + 5) * counter), CPoint(vstOutputs[i]->getWidth(), vstOutputs[i]->getHeight())));
			vstOutputs[i]->setMouseableArea(vstOutputs[i]->getViewSize());
			++counter;
		}

		int moduleCounter = 0;
		int rowCounter = 0;
		int columnCounter = 0;
		int lastUnusedModule = -1;
		for (unsigned int m = 0; m < modules.size(); ++m) {
			if (modules[m]) {
				if (modules[m]->getNumberOfUsedInputs() == 0) {
					/*if (((modules[m]->getHeight()) + 5) * (moduleCounter + 1) + modules[m]->getHeight() > this->getVisibleViewSize().getHeight()) {
						++columnCounter;
						moduleCounter = 0;
					}B
					modules[m]->setViewSize(CRect(CPoint((modules[m]->getWidth() + 5) * columnCounter, ((modules[m]->getHeight()) + 5) * moduleCounter), CPoint(modules[m]->getWidth(), modules[m]->getHeight())));
					modules[m]->setMouseableArea(modules[m]->getViewSize());
					++moduleCounter;
					lastUnusedModule = m;*/
					modules[m]->setEnabled(false);
				}	
			}
		}

		moduleCounter = 1;
		rowCounter = 0;
		columnCounter = 0;
		int offset = 0;
		if (lastUnusedModule != -1) {
			offset = modules[lastUnusedModule]->getViewSize().getTopRight().x + 10;
		}
		for (unsigned int m = 0; m < modules.size(); ++m) {
			if (modules[m]) {
				if (modules[m]->getNumberOfUsedInputs() != 0) {
					if ((modules[m]->getWidth() + 10) * (moduleCounter + 1) > this->getVisibleViewSize().getWidth()) {
						++rowCounter;
						moduleCounter = 0;
					}
					modules[m]->setViewSize(CRect(CPoint((modules[m]->getWidth() + 10) * moduleCounter, (modules[m]->getHeight() + 5) * rowCounter), CPoint(modules[m]->getWidth(), modules[m]->getHeight())).offset(offset, 0));
					modules[m]->setMouseableArea(modules[m]->getViewSize());
					++moduleCounter;
				}
			}
		}
		this->setDirty();
	}

	CMessageResult GuiGraphicsView::notify(CBaseObject* sender, IdStringPtr message) {
		for (unsigned int i = 0; i < modules.size(); ++i) {
			if (sender == modules[i]) {
				if (message == "StartMouseLine") {
					connections->updateMouseConnectionLine(modules[i]->getOutputRectCenter(), modules[i]->getOutputRectCenter());
					return kMessageNotified;
				}
				else if (message == "MoveMouseLine") {
					connections->updateMouseConnectionLine(modules[i]->getOutputRectCenter(), modules[i]->getMouseMoveOutputRect());
		
					return kMessageNotified;
				}
				else if (message == "EndMouseLine") {
					
					for (unsigned int j = 0; j < modules.size(); ++j) {
						/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
						fprintf(pFile, "y(n): %s\n", std::to_string(modules[i]->getMouseUpCoordinates().x).c_str());
						fprintf(pFile, "y(n): %s\n", std::to_string(modules[j]->getViewSize().getTopLeft().x).c_str());
						fclose(pFile);*/
						if (modules[i]->getMouseUpCoordinates().isInside(modules[j]->getViewSize())) {
							//connections->updateMouseConnectionLine(modules[i]->getOutputRectCenter(), modules[j]->getViewSize().getCenter());
							//connections->finishMouseConnectionLine(1.0);
							drawnConnection = Connection(i, j);
							editor->notify(this, "NewConnectionEstablished");
							break;
						}
					}
					connections->updateMouseConnectionLine(0, 0);
					return kMessageNotified;
				}
			}
		}
		return kMessageUnknown;
	}

	//CMouseEventResult GuiGraphicsView::onMouseDown(CPoint &where, const CButtonState& buttons)
	//{

	//	/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	//	fprintf(pFile, "y(n): %s\n", std::to_string(this->getWidth()).c_str());
	//	fclose(pFile);*/

	//	for (auto&& m : modules) {
	//		m->onMouseDown(where, buttons);
	//	}

	//	return kMouseEventHandled;
	//}

	//CMouseEventResult GuiGraphicsView::onMouseMoved(CPoint &where, const CButtonState& buttons)
	//{
	//	for (auto&& m : modules) {
	//		m->onMouseMoved(where, buttons);
	//	}

	//	return kMouseEventHandled;
	//}

	//CMouseEventResult GuiGraphicsView::onMouseUp(CPoint& where, const CButtonState& buttons) {
	//	for (auto&& m : modules) {
	//		m->onMouseUp(where, buttons);
	//	}

	//	return kMouseEventHandled;
	//}
}