#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#define EDGE_CHECKER_WIDTH 16 // Define width here
#define EDGE_CHECKER_HEIGHT 16 // Define height here
#define ID_ANI_EDGECHECKER 0
class CEdgeChecker: public CGameObject {
public:
	CEdgeChecker(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	virtual int IsCollidable() { return 1; };
};