//UNUSED

#include "GuiScrollViewAutoSizeable.h"
#include "cframe.h"

namespace VSTGUI {

	// CScrollContainer is private
	//-----------------------------------------------------------------------------
	class CScrollContainer : public CViewContainer
		//-----------------------------------------------------------------------------
	{
	public:
		CScrollContainer(const CRect &size, const CRect &containerSize);
		CScrollContainer(const CScrollContainer& v);
		~CScrollContainer();

		void setScrollOffset(CPoint offset, bool withRedraw = false);
		void getScrollOffset(CPoint& off) const { off = offset; }
		const CPoint& getScrollOffset() const { return offset; }

		CRect getContainerSize() const { return containerSize; }
		void setContainerSize(const CRect& cs);

		bool isDirty() const;

		void onDragMove(IDataPackage* drag, const CPoint& where);
		void setAutoDragScroll(bool state) { autoDragScroll = state; }

		bool attached(CView* parent);
		CMessageResult notify(CBaseObject* sender, IdStringPtr message);

		CLASS_METHODS(CScrollContainer, CViewContainer)
			//-----------------------------------------------------------------------------
	protected:
		bool getScrollValue(const CPoint& where, float& x, float& y);

		CRect containerSize;
		CPoint offset;
		bool autoDragScroll;
		bool inScrolling;
	};

	GuiScrollViewAutoSizeable::GuiScrollViewAutoSizeable(const CRect& size, const CRect& containerSize, int32_t style, CCoord scrollbarWidth, CBitmap* pBackground)
	: CScrollView(size,containerSize, style, scrollbarWidth, pBackground) {
		subViewDragRegion = CRect(0, 0, 200, 200);
	}


	GuiScrollViewAutoSizeable::~GuiScrollViewAutoSizeable() {

	}

	CMessageResult GuiScrollViewAutoSizeable::notify(CBaseObject* sender, IdStringPtr message)
	{
		/*FILE* pFile = fopen("E:\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", message);
		fclose(pFile);*/

		//if (message == "kMsgViewSizeChanged") {
		//	for (unsigned int i = 0; i < getNbViews(); ++i) {
		//		getView(i)->notify(this, "")
		//	}
		//}

		//if (message == "Move") {

		//	/*for (unsigned int i = 0; i < getNbViews(); ++i) {
		//		if (sender == getView(i)) {
		//			getView(i)->setViewSize(CRect(CPoint(getView(i)->getViewSize().getTopLeft().offset(getHorizontalScrollbar()->getValue() * (getViewSize().getWidth()), getScrollOffset().y)), CPoint(getView(i)->getViewSize().getWidth(), getView(i)->getViewSize().getHeight())));
		//			getView(i)->setMouseableArea(getView(i)->getViewSize());
		//			break;
		//		}
		//	}*/

		//	//CCoord top = 0;
		//	CCoord right = 0;
		//	CCoord bottom = 0;
		//	//CCoord left = 0;
		//	for (unsigned int i = 0; i < getNbViews(); ++i) {
		//		//if (getView(i)->isVisible()) {
		//			CRect viewRect = getView(i)->getViewSize();
		//			//if (viewRect.top > top) top = viewRect.top;
		//			if (viewRect.right > right) right = viewRect.right;
		//			if (viewRect.bottom > bottom) bottom = viewRect.bottom;
		//			//if (viewRect.left > left) left = viewRect.left;

		//			/*CPoint bottomRight = getView(i)->getViewSize().getBottomRight();
		//			bottomRight = bottomRight + getHorizontalScrollbar()->getValue() * (getContainerSize().getWidth() - getViewSize().getWidth());
		//			FILE* pFile = fopen("E:\\logVst.txt", "a");
		//			fprintf(pFile, "y(n): %s\n", std::to_string(getHorizontalScrollbar()->getValue()).c_str());
		//			fprintf(pFile, "y(n): %s\n", std::to_string(getContainerSize().getWidth()).c_str());
		//			fprintf(pFile, "y(n): %s\n", std::to_string(getViewSize().getWidth()).c_str());
		//			fprintf(pFile, "y(n): %s\n", "------------");
		//			fclose(pFile);*/
		//		//}
		//	}
		//	//setContainerSize(CRect(CPoint(getContainerSize().getTopLeft()), CPoint(bottomRight.x, getContainerSize().getHeight())));
		//	setContainerSize(CRect(0, 0, right, bottom));
		//	//FILE* pFile = fopen("E:\\logVst.txt", "a");
		//	////fprintf(pFile, "y(n): %s\n", std::to_string(top).c_str());
		//	//fprintf(pFile, "y(n): %s\n", std::to_string(right).c_str());
		//	//fprintf(pFile, "y(n): %s\n", std::to_string(bottom).c_str());
		//	////fprintf(pFile, "y(n): %s\n", std::to_string(left).c_str());
		//	//fprintf(pFile, "y(n): %s\n", "------------");
		//	//fclose(pFile);
		//}
		if (message == kMsgNewFocusView && getStyle() & kFollowFocusView)
		{
			CView* focusView = (CView*)sender;
			if (sc->isChild(focusView, true))
			{
				CRect r = focusView->getViewSize();
				CPoint p;
				focusView->localToFrame(p);
				frameToLocal(p);
				r.offset(p.x, p.y);
				makeRectVisible(r);
			}
		}
		return CViewContainer::notify(sender, message);
	}

	CMouseEventResult GuiScrollViewAutoSizeable::onMouseDown(CPoint &where, const CButtonState& buttons)
	{
		if (where.isInside(subViewDragRegion)) {
			for (unsigned int i = 0; i < getNbViews(); ++i) {
				CRect viewSize = getView(i)->getViewSize();
				getView(i)->setViewSize(viewSize.offset(40, 0));
				getView(i)->setMouseableArea(getView(i)->getViewSize());
				setDirty();
				FILE* pFile = fopen("E:\\logVst.txt", "a");
				CPoint bla = getView(i)->getViewSize().getTopLeft();
				frameToLocal(bla);
				fprintf(pFile, "y(n): %s\n", std::to_string(bla.x).c_str());
				fclose(pFile);
			}

			CCoord right = 0;
			CCoord bottom = 0;
			for (unsigned int i = 0; i < getNbViews(); ++i) {
						//if (getView(i)->isVisible()) {
							CRect viewRect = getView(i)->getViewSize();
							//if (viewRect.top > top) top = viewRect.top;
							if (viewRect.right > right) right = viewRect.right;
							if (viewRect.bottom > bottom) bottom = viewRect.bottom;
							//if (viewRect.left > left) left = viewRect.left;

							/*CPoint bottomRight = getView(i)->getViewSize().getBottomRight();
							bottomRight = bottomRight + getHorizontalScrollbar()->getValue() * (getContainerSize().getWidth() - getViewSize().getWidth());
							FILE* pFile = fopen("E:\\logVst.txt", "a");
							fprintf(pFile, "y(n): %s\n", std::to_string(getHorizontalScrollbar()->getValue()).c_str());
							fprintf(pFile, "y(n): %s\n", std::to_string(getContainerSize().getWidth()).c_str());
							fprintf(pFile, "y(n): %s\n", std::to_string(getViewSize().getWidth()).c_str());
							fprintf(pFile, "y(n): %s\n", "------------");
							fclose(pFile);*/
						//}
					}
					//setContainerSize(CRect(CPoint(getContainerSize().getTopLeft()), CPoint(bottomRight.x, getContainerSize().getHeight())));
					setContainerSize(CRect(0, 0, 2000, 2000));
			
		}
		// convert to relativ pos
		CPoint where2(where);
		where2.offset(-getViewSize().left, -getViewSize().top);

		FOREACHSUBVIEW_REVERSE(true)
			if (pV && pV->isVisible() && pV->getMouseEnabled() && pV->hitTest(where2, buttons))
			{
			CControl* control = dynamic_cast<CControl*> (pV);
			if (control && control->getListener() && buttons & (kAlt | kShift | kControl | kApple | kRButton))
			{
				if (control->getListener()->controlModifierClicked((CControl*)pV, buttons) != 0)
					return kMouseEventHandled;
			}
			CBaseObjectGuard crg(pV);

			if (pV->wantsFocus())
				getFrame()->setFocusView(pV);

			CMouseEventResult result = pV->onMouseDown(where2, buttons);
			if (result != kMouseEventNotHandled && result != kMouseEventNotImplemented)
			{
				if (pV->getNbReference() > 1 && result == kMouseEventHandled)
					mouseDownView = pV;
				return result;
			}
			if (!pV->getTransparency())
				return result;
			}
		ENDFOREACHSUBVIEW
			return kMouseEventNotHandled;
	}

	//-----------------------------------------------------------------------------
	CMouseEventResult GuiScrollViewAutoSizeable::onMouseUp(CPoint &where, const CButtonState& buttons)
	{
		if (mouseDownView)
		{
			CBaseObjectGuard crg(mouseDownView);

			// convert to relativ pos
			CPoint where2(where);
			where2.offset(-getViewSize().left, -getViewSize().top);
			mouseDownView->onMouseUp(where2, buttons);
			mouseDownView = 0;
			return kMouseEventHandled;
		}
		return kMouseEventNotHandled;
	}

	//-----------------------------------------------------------------------------
	CMouseEventResult GuiScrollViewAutoSizeable::onMouseMoved(CPoint &where, const CButtonState& buttons)
	{
		if (mouseDownView)
		{
			CBaseObjectGuard crg(mouseDownView);

			// convert to relativ pos
			CPoint where2(where);
			where2.offset(-getViewSize().left, -getViewSize().top);
			CMouseEventResult mouseResult = mouseDownView->onMouseMoved(where2, buttons);
			if (mouseResult != kMouseEventHandled && mouseResult != kMouseEventNotImplemented)
			{
				mouseDownView = 0;
				return kMouseEventNotHandled;
			}
			return kMouseEventHandled;
		}
		return kMouseEventNotHandled;
	}

}