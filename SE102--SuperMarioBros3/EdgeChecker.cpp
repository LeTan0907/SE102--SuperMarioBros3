#include "EdgeChecker.h"
void CEdgeChecker::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_EDGECHECKER)->Render(x, y);

	//RenderBoundingBox();
}

void CEdgeChecker::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - EDGE_CHECKER_WIDTH / 2;
	t = y - EDGE_CHECKER_HEIGHT / 2;
	r = l + EDGE_CHECKER_WIDTH;
	b = t + EDGE_CHECKER_HEIGHT;
}
