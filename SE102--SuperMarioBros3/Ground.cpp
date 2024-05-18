#include "Ground.h"

void CGround::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_GROUND)->Render(x, y);
	//RenderBoundingBox();
}
