#include "GuiGraphicsModule.h"
#include "ReverbNetworkDefines.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"
#include <sstream>
#include <iomanip>

namespace VSTGUI {

	const int inoutRectWidth = 8;
	const int inoutRectHeight = 10;
	const int spacing = 2;

	GuiGraphicsModule::GuiGraphicsModule(const std::string& title, const std::vector<std::string>& inputNames)
		: CViewContainer(CRect(0, 0, 0, 0)), title(title), inputNames(inputNames) {

		enabled = true;

		numberOfInputs = inputNames.size();
		numberOfUsedInputs = inputNames.size();
		inputGainValues.resize(inputNames.size(), 0.0);

		inputGainValues.resize(inputNames.size());
		inputRects.resize(inputNames.size(), CRect(0, 0, 0, 0));

		mouseDownCoordinates = CPoint(0, 0);
		mouseDownInHandleRegion = false;

		updateShape();
	}

	GuiGraphicsModule::~GuiGraphicsModule() {

	}

	void GuiGraphicsModule::drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect)
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

		updateShape();
		redraw(pContext);
	}

	void GuiGraphicsModule::updateInput(const int& input, const double& gainValue) {
		inputGainValues[input] = gainValue;
	}

	void GuiGraphicsModule::updateShape() {
		handleRegion = CRect(CPoint(0, 0), CPoint(100, 12));
		numberOfUsedInputs = 0;
		for (int i = 0; i < numberOfInputs; ++i) {
			if (inputGainValues[i] != 0.0) {
				inputRects[i] = CRect(CPoint(0, handleRegion.getHeight() + spacing).offset(0, numberOfUsedInputs * (spacing + inoutRectHeight)), CPoint(inoutRectWidth, inoutRectHeight));
				++numberOfUsedInputs;
			}
		}
		mainRegion = CRect(CPoint(0, handleRegion.getHeight()), CPoint(handleRegion.getWidth(), spacing + numberOfUsedInputs * (spacing + inoutRectHeight) + spacing));
		completeRegion = CRect(CPoint(0, 0), CPoint(handleRegion.getWidth(), handleRegion.getHeight() + mainRegion.getHeight()));
		this->setViewSize(CRect(CPoint(this->getViewSize().getTopLeft()), CPoint(completeRegion.getWidth(), completeRegion.getHeight())));
		outputRect = CRect(CPoint(handleRegion.getWidth() - inoutRectWidth, completeRegion.getCenter().y - inoutRectHeight / 2), CPoint(inoutRectWidth, inoutRectHeight));	
	}

	void GuiGraphicsModule::redraw(CDrawContext* pContext) {
		if (enabled) {
			pContext->setFrameColor(CColor(0, 0, 0));
			pContext->setFillColor(CColor(50, 50, 50));
			pContext->setLineWidth(1);
			pContext->setFont(kNormalFontSmaller);
			pContext->setFontColor(CColor(255, 255, 255));
			pContext->drawRect(handleRegion, CDrawStyle::kDrawFilledAndStroked); // title rect
			pContext->drawString(title.c_str(), CPoint(2, 10), false); // title string
			pContext->setFillColor(CColor(50, 50, 50));
			pContext->drawRect(mainRegion, kDrawFilledAndStroked); // main rect
			std::stringstream temp;
			for (int i = 0; i < inputRects.size(); ++i) {
				if (inputGainValues[i] != 0.0) {
					pContext->setFillColor(CColor(50, 200, 50));
					pContext->drawRect(inputRects[i], CDrawStyle::kDrawFilledAndStroked); // module inputs
					temp.str(std::string());
					temp.clear();
					temp << inputNames[i] << " [" << std::setprecision(2) << inputGainValues[i] << "]";
					pContext->drawString(temp.str().c_str(), CRect(CPoint(inputRects[i].right + spacing, inputRects[i].top), CPoint(100, inoutRectHeight)), CHoriTxtAlign::kLeftText, false);
				}
			}
			pContext->setFillColor(CColor(200, 200, 200));
			pContext->drawRect(outputRect, kDrawFilledAndStroked);
			pContext->drawString("OUT", CRect(CPoint(outputRect.left - 50, outputRect.top), CPoint(50, outputRect.getHeight())), CHoriTxtAlign::kRightText, false);
		}
	}

	CMouseEventResult GuiGraphicsModule::onMouseDown(CPoint &where, const CButtonState& buttons)
	{

		/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(this->getWidth()).c_str());
		fclose(pFile);*/

		if (where.isInside(handleRegion)) {
			mouseDownInHandleRegion = true;
			//mouseDownCoordinates = where - modules[i].position;
			return kMouseEventHandled;
		}

		return kMouseEventNotHandled;
	}

	CMouseEventResult GuiGraphicsModule::onMouseMoved(CPoint &where, const CButtonState& buttons)
	{
		/*if (mouseDownInHandleRegion >= 0) {
			if (modules[mouseDownInHandleRegion].enabled) {
				modules[mouseDownInHandleRegion].position = CPoint(where - mouseDownCoordinates);
				this->setDirty();
				return kMouseEventHandled;
			}
		}*/
		return kMouseEventNotHandled;
	}

	CMouseEventResult GuiGraphicsModule::onMouseUp(CPoint& where, const CButtonState& buttons) {
		mouseDownInHandleRegion = -1;
		return kMouseEventHandled;
	}
}