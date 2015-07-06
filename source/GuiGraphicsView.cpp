#include "GuiGraphicsView.h"
#include "ReverbNetworkDefines.h"
#include "ValueConversion.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"
#include <iomanip>
#include <sstream>

namespace VSTGUI {

	GuiGraphicsView::GuiGraphicsView(const CRect& size)
	: CViewContainer(size),
	mouseDownCoordinates(CPoint(0, 0)),
	mouseDownInHandleRegion(-1) {
		for (int i = 0; i < MAXMODULENUMBER; ++i) {
			Module m;
			m.enabled = false;
			m.inputGainValues.resize(MAXINPUTS);
			m.inputRects.resize(MAXINPUTS);
			m.outputRect = CRect(0, 0, 0, 0);
			m.position = CPoint(0.0);
			m.title = "";
			m.handleRegion = CRect(0, 0, 0, 0);
			m.completeRegion = CRect(0, 0, 0, 0);
			modules.push_back(m);
		}
	}


	GuiGraphicsView::~GuiGraphicsView() {

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
		drawModuleRects(pContext);
		drawModuleConnections(pContext);
	}

	void GuiGraphicsView::drawModuleRects(CDrawContext* pContext) {
		for (auto&& m : modules) {
			if (m.enabled) {
				pContext->setFrameColor(CColor(0, 0, 0));
				pContext->setFillColor(CColor(50, 50, 50));
				pContext->setLineWidth(1);
				pContext->setFont(kNormalFontSmaller);
				m.handleRegion = CRect(CPoint(0, 0), CPoint(100, 12)).offset(m.position.x, m.position.y);
				pContext->drawRect(m.handleRegion, CDrawStyle::kDrawFilledAndStroked); // title rect
				pContext->drawString(m.title.c_str(), CPoint(2, 10).offset(m.position.x, m.position.y), false); // title string
				pContext->setFillColor(CColor(50, 50, 50));
				CRect mainRegion = CRect(CPoint(0, 12), CPoint(100, 12 * m.numberOfUsedInputs + 2)).offset(m.position.x, m.position.y);
				pContext->drawRect(mainRegion, kDrawFilledAndStroked); // main rect
				m.completeRegion = m.handleRegion.offset(mainRegion.getBottomRight().x, mainRegion.getBottomRight().y);
				std::stringstream temp;
				int rectNumber = 0;
				for (int i = 0; i < MAXMODULENUMBER; ++i) {
					if (m.inputGainValues[i] != 0.0) {
						pContext->setFillColor(CColor(50, 200, 50));
						m.inputRects[i] = CRect(CPoint(0, 14).offset(0, rectNumber * 12), CPoint(8, 10)).offset(m.position.x, m.position.y);
						pContext->drawRect(m.inputRects[i], CDrawStyle::kDrawFilledAndStroked); // module inputs
						temp.str(std::string());
						temp.clear();
						temp << "APM" << i << " [" << std::setprecision(2) << m.inputGainValues[i] << "]";
						pContext->drawString(temp.str().c_str(), CRect(CPoint(10, 14).offset(0, rectNumber * 12), CPoint(100, 10)).offset(m.position.x, m.position.y), CHoriTxtAlign::kLeftText, false);
						++rectNumber;
					}
				}
				for (int i = MAXMODULENUMBER; i < MAXMODULENUMBER + MAXVSTINPUTS; ++i) {
					if (m.inputGainValues[i] != 0.0) {
						if (m.inputGainValues[i] == 0.0) pContext->setFillColor(CColor(200, 200, 200));
						else pContext->setFillColor(CColor(100, 200, 100));
						m.inputRects[i] = CRect(CPoint(0, 14).offset(0, rectNumber * 12), CPoint(8, 10)).offset(m.position.x, m.position.y);
						pContext->drawRect(m.inputRects[i], CDrawStyle::kDrawFilledAndStroked); // VST inputs
						temp.str(std::string());
						temp.clear();
						temp << "VST" << i - MAXMODULENUMBER << " [" << std::setprecision(2) << m.inputGainValues[i] << "]";
						pContext->drawString(temp.str().c_str() , CRect(CPoint(10, 14).offset(0, rectNumber * 12), CPoint(100, 10)).offset(m.position.x, m.position.y), CHoriTxtAlign::kLeftText, false);
						++rectNumber;
					}
				}
				pContext->setFillColor(CColor(200, 200, 200));
				m.outputRect = (CRect(CPoint(92, (rectNumber * 12 + 2 + 12) / 2 - 5), CPoint(8, 10)).offset(m.position.x, m.position.y));
				pContext->drawRect(m.outputRect, kDrawFilledAndStroked);
				pContext->drawString("OUT", CRect(CPoint(m.outputRect.left - 50, m.outputRect.top), CPoint(50, m.outputRect.getHeight())), CHoriTxtAlign::kRightText, false);
			}
		}


		//!!!!!!!!!!!!!!
		//rearrangeModules();
	}

	void GuiGraphicsView::drawModuleConnections(CDrawContext* pContext) {
		for (auto&& m : modules) {
			if (m.enabled) {
				for (unsigned int j = 0; j < m.inputGainValues.size(); ++j) {
					if (m.inputGainValues[j] != 0.0) {
						pContext->setFrameColor(CColor(255, 255, 255, (uint8_t)(255 * abs(m.inputGainValues[j]))));
						CPoint line[2];
						line[0] = m.inputRects[j].getCenter();
						line[1] = modules[j].outputRect.getCenter();
						pContext->drawLines(line, 2);
					}
				}
			}
		}
	}

	void GuiGraphicsView::rearrangeModules() {
		int notUsedCounter = 0;
		for (unsigned int m = 0; m < modules.size(); ++m) {
			if (modules[m].numberOfUsedInputs == 0) {
				modules[m].position.x = this->getWidth() - modules[m].completeRegion.getWidth();
				modules[m].position.y = 0 + notUsedCounter * modules[m].completeRegion.getHeight();
				++notUsedCounter;
				/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
				fprintf(pFile, "y(n): %s\n", std::to_string(m.completeRegion.getWidth()).c_str());
				fclose(pFile);*/
			}
			else {
				modules[m].position.x = (modules[m].completeRegion.getWidth() + 20) * m;
			}

			modules[m].completeRegion = CRect(CPoint(modules[m].position.x, (modules[m].position.y)), CPoint(modules[m].completeRegion.getWidth(), modules[m].completeRegion.getHeight()));
			modules[m].handleRegion = CRect(CPoint(modules[m].position.x, (modules[m].position.y)), CPoint(modules[m].handleRegion.getWidth(), modules[m].handleRegion.getHeight()));
		}
		this->setDirty();
	}

	void GuiGraphicsView::addModule(const std::string& title, const int& id, const CPoint& position, const std::vector<double>& inputGainValues) {
		modules[id].enabled = true;
		modules[id].title = title;
		//modules[id].position = position;
		modules[id].inputGainValues = inputGainValues;
		int counter = 0;
		for (auto&& i : inputGainValues) {
			if (i != 0.0) ++counter;
		}
		modules[id].numberOfUsedInputs = counter;
	}

	void GuiGraphicsView::removeModule(const int& id) {
		modules[id].enabled = false;
	}

	CMouseEventResult GuiGraphicsView::onMouseDown(CPoint &where, const CButtonState& buttons)
	{

		/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(this->getWidth()).c_str());
		fclose(pFile);*/

		for (unsigned int i = modules.size() - 1; i >= 0; --i) {
			if (where.isInside(modules[i].handleRegion)) {
				mouseDownInHandleRegion = i;
				mouseDownCoordinates = where - modules[i].position;
				return kMouseEventHandled;
			}
		}

		return kMouseEventNotHandled;
	}

	CMouseEventResult GuiGraphicsView::onMouseMoved(CPoint &where, const CButtonState& buttons)
	{
		if (mouseDownInHandleRegion >= 0) {
			if (modules[mouseDownInHandleRegion].enabled) {
				modules[mouseDownInHandleRegion].position = CPoint(where - mouseDownCoordinates);
				this->setDirty();
				return kMouseEventHandled;
			}
		}
		return kMouseEventNotHandled;
	}

	CMouseEventResult GuiGraphicsView::onMouseUp(CPoint& where, const CButtonState& buttons) {
		mouseDownInHandleRegion = -1;
		return kMouseEventHandled;
	}
}