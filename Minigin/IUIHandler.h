#pragma once
#include "IMouseHandler.h"

class IUIHandler : public IMouseHandler
{
public:

protected:
	bool IsPointInTex(Vector2 pos, Vector2& pointerPos) override;

};

