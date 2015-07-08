#include "GuiGraphicsModule.h"
#include "ReverbNetworkDefines.h"
#include <sstream>
#include <iomanip>

namespace VSTGUI {

	const int inoutRectWidth = 8;
	const int inoutRectHeight = 10;
	const int spacing = 2;

	GuiGraphicsModule::GuiGraphicsModule(const CPoint& position, const std::string& title, const std::vector<double>& inputGainValues, const std::vector<std::string>& inputNames)
		: CViewContainer(CRect(CPoint(position), CPoint(100, 12))), position(position), title(title), inputGainValues(inputGainValues) {

		inputRects.resize(inputGainValues.size(), CRect(0, 0, 0, 0));
		setEnabled(true);
		updateInputs(inputGainValues, inputNames);
		moveTo(position);
	}

	GuiGraphicsModule::~GuiGraphicsModule() {

	}

	void GuiGraphicsModule::updateInputs(const std::vector<double> inputGainValues, const std::vector<std::string>& inputNames) {
		if (inputGainValues.size() != inputNames.size()) return;
		this->inputGainValues = inputGainValues;
		this->inputNames = inputNames;
		numberOfInputs = inputNames.size();
		moveTo(position); // Update view
	}

	void GuiGraphicsModule::moveTo(const CPoint& position) {
		handleRegion = CRect(CPoint(0, 0), CPoint(100, 12)).offset(position.x, position.y);
		mainRegion = CRect(CPoint(0, handleRegion.getHeight()), CPoint(handleRegion.getWidth(), spacing + numberOfInputs * (spacing + inoutRectHeight) + spacing)).offset(position.x, position.y);
		completeRegion = CRect(CPoint(0, 0), CPoint(handleRegion.getWidth(), handleRegion.getHeight() + mainRegion.getHeight())).offset(position.x, position.y);
		this->setViewSize(CRect(CPoint(position), CPoint(completeRegion.getBottomRight())));
		for (int i = 0; i < numberOfInputs; ++i) {
			inputRects[i] = CRect(CPoint(0, handleRegion.getHeight() + spacing).offset(0, i * (spacing + inoutRectHeight)), CPoint(inoutRectWidth, inoutRectHeight)).offset(position.x, position.y);
		}
		outputRect = CRect(CPoint(handleRegion.getWidth() - inoutRectWidth, completeRegion.getCenter().y - inoutRectHeight / 2), CPoint(inoutRectWidth, inoutRectHeight)).offset(position.x, position.y);
	}

	void GuiGraphicsModule::redraw(CDrawContext* pContext) {
		if (enabled) {
			pContext->setFrameColor(CColor(0, 0, 0));
			pContext->setFillColor(CColor(50, 50, 50));
			pContext->setLineWidth(1);
			pContext->setFont(kNormalFontSmaller);
			pContext->drawRect(handleRegion, CDrawStyle::kDrawFilledAndStroked); // title rect
			pContext->drawString(title.c_str(), CPoint(2, 10).offset(position.x, position.y), false); // title string
			pContext->setFillColor(CColor(50, 50, 50));
			pContext->drawRect(mainRegion, kDrawFilledAndStroked); // main rect
			std::stringstream temp;
			for (int i = 0; i < inputRects.size(); ++i) {
				pContext->setFillColor(CColor(50, 200, 50));
				pContext->drawRect(inputRects[i], CDrawStyle::kDrawFilledAndStroked); // module inputs
				temp.str(std::string());
				temp.clear();
				temp << inputNames[i] << " [" << std::setprecision(2) << inputGainValues[i] << "]";
				pContext->drawString(temp.str().c_str(), CRect(CPoint(inputRects[i].right + spacing, inputRects[i].top), CPoint(100, inoutRectHeight)), CHoriTxtAlign::kLeftText, false);
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