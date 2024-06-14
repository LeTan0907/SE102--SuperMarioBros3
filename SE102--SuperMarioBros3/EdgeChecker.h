#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define EDGE_CHECKER_WIDTH 16
#define EDGE_CHECKER_HEIGHT 16
#define ID_ANI_EDGECHECKER 0
#define EDGE_CHECKER_DISTANCE 16

class CEdgeChecker : public CGameObject
{
public:
    CEdgeChecker(float x, float y) : CGameObject(x, y) {}
    void Render();
    virtual int IsBlocking() { return 0; }
    virtual int IsCollidable() { return 1; };
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void OnCollisionWith(LPCOLLISIONEVENT e);
};
