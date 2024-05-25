#pragma once

#include "GameObject.h"
#define ID_ANI_REDMUSHROOM 11100
#define RED_MUSHROOM_GRAVITY 0.002f
#define RED_MUSHROOM_SPEED 0.05f

#define RED_MUSHROOM_BBOX_WIDTH 16
#define RED_MUSHROOM_BBOX_HEIGHT 16

class CRedMushroom : public CGameObject
{
protected:
    float ax;
    float ay;

public:
    CRedMushroom(float x, float y);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    void OnNoCollision(DWORD dt);
    void OnCollisionWith(LPCOLLISIONEVENT e);
};
