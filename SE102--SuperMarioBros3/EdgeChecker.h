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
    bool isOnPlatform = false;

public:
    CEdgeChecker(float x, float y) : CGameObject(x, y) {}
    void Render();
    void SetPosition(float x, float y);
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void UpdateState(bool isOnPlatform);
    bool IsAtEdge();
    void ResetState();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void OnCollisionWith(LPCOLLISIONEVENT e);
};
