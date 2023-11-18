#pragma once
#include <vector>

//class cUIElement
//{
//public:
//	bool focusable = false;
//
//	virtual void DrawElement() = 0;
//};

class cUICanvas
{
public:
	enum eAnchors
	{
		TOP_LEFT,
		TOP_MIDDLE,
		TOP_RIGHT,
		MIDLLE_LEFT,
		MIDDLE_MIDDLE,
		MIDDLE_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_MIDDLE,
		BOTTOM_RIGHT
	};
private:
	//std::vector<std::pair<eAnchors, cUIElement>> anchoredElements;

	//void DrawCanvas();

	//cUIElement* currentFocus;
};