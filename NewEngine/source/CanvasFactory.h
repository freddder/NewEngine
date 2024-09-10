#pragma once

#include "cUIManager.h"

class cOverworldCanvas : public cUICanvas
{
public:
	cOverworldCanvas();
	virtual ~cOverworldCanvas();

	virtual void CancelAction();
};

namespace CanvasFactory
{

}