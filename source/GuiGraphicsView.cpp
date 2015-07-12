#include "GuiGraphicsView.h"
#include "ValueConversion.h"
#include "GuiGraphicsModule.h"
#include "GuiGraphicsConnections.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"
#include <iomanip>
#include <sstream>

namespace VSTGUI {

	GuiGraphicsView::GuiGraphicsView(const CRect& size, const int& numberOfModules)
	: CViewContainer(size), numberOfModules(numberOfModules) {
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

	void GuiGraphicsView::addModule(const std::string& title, const int& id, const std::vector<std::string>& inputNames) {
		GuiGraphicsModule* module = new GuiGraphicsModule(title, inputNames);
		this->addView(module);
		modules[id] = module;
		this->changeViewZOrder(connections, this->getNbViews() - 1);
	}

	void GuiGraphicsView::updateModule(const int& moduleId, const int& input, const double& gainValue) {
		modules[moduleId]->updateInput(input, gainValue);
		this->setDirty();
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

	void GuiGraphicsView::drawModuleRects(CDrawContext* pContext) {
		for (auto&& m : modules) {
			if (m) {
				if (m->isEnabled()) {
					m->setDirty();
				}
			}
		}
	}

	void GuiGraphicsView::drawModuleConnections(CDrawContext* pContext) {
		for (auto&& m : modules) {
			if (m) {
				if (m->isEnabled()) {
					for (unsigned int j = 0; j < numberOfModules; ++j) {
						if (m->getGainValue(j) != 0.0) {
							connections->setConnection(m->getInputRectCenter(j), modules[j]->getOutputRectCenter(), std::abs(m->getGainValue(j)), j);
						}
					}
				}
			}
		}
		connections->setDirty();
	}

	void GuiGraphicsView::rearrangeModules() {
		int notUsedCounter = 0;
		int usedCounter = 0;
		for (unsigned int m = 0; m < modules.size(); ++m) {
			if (modules[m]) {
				if (modules[m]->getNumberOfUsedInputs() == 0) {
					modules[m]->setViewSize(CRect(CPoint(this->getWidth() - modules[m]->getWidth(), ((modules[m]->getHeight()) + 5)* notUsedCounter), CPoint(modules[m]->getWidth(), modules[m]->getHeight())));
					modules[m]->setMouseableArea(modules[m]->getViewSize());
					++notUsedCounter;
				}
				else {
					/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
					fprintf(pFile, "y(n): %s\n", std::to_string(modules[m]->getWidth()).c_str());
					fclose(pFile);*/
					modules[m]->setViewSize(CRect(CPoint((modules[m]->getWidth() + 10) * usedCounter, 10), CPoint(modules[m]->getWidth(), modules[m]->getHeight())));
					modules[m]->setMouseableArea(modules[m]->getViewSize());
					++usedCounter;
				}
				
			}
		}
		this->setDirty();
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