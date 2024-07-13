#include "Leaf.h"
#include "PlayScene.h"

CLeaf::CLeaf(float x, float y) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = LEAF_GRAVITY;
    x_temp = x;
    this->vx = LEAF_HORIZONTAL_SPEED;
    this->lastDirectionChangeTime = GetTickCount64();
}

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - LEAF_BBOX_WIDTH / 2;
    top = y - LEAF_BBOX_HEIGHT / 2;
    right = left + LEAF_BBOX_WIDTH;
    bottom = top + LEAF_BBOX_HEIGHT;
}

void CLeaf::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
};

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    ay = LEAF_GRAVITY*10;
    if (x < x_temp)
    {
        vx = LEAF_HORIZONTAL_SPEED;
    }
    if (x - x_temp > 90)
    {
        vx = -LEAF_HORIZONTAL_SPEED;
    }
    x += vx*1000;
    y += ay * dt;
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::Render()
{
    CAnimations* animations = CAnimations::GetInstance();
    animations->Get(ID_ANI_LEAF)->Render(x, y);
}