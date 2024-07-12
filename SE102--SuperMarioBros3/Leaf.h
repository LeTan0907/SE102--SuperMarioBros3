#pragma once
#include "GameObject.h"
#include "Animations.h"
#include "Collision.h"

#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16

#define LEAF_GRAVITY 0.0002f
#define LEAF_FALL_SPEED 0.002f
#define LEAF_HORIZONTAL_SPEED 0.002f
#define LEAF_DIRECTION_CHANGE_INTERVAL 500
#define ID_ANI_LEAF 112
class CLeaf : public CGameObject
{
protected:
    float ax;
    float ay;
    ULONGLONG lastDirectionChangeTime;
public:
    CLeaf(float x, float y);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    virtual int IsCollidable() { return 1; };
    virtual int IsBlocking() { return 0; }
    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    void OnNoCollision(DWORD dt);
    void OnCollisionWith(LPCOLLISIONEVENT e);

};