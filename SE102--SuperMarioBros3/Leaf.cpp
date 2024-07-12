#include "Leaf.h"
#include "PlayScene.h"

CLeaf::CLeaf(float x, float y) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = LEAF_GRAVITY;
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
    vy += ay * dt;

    // Change direction at regular intervals
    if (GetTickCount64() - lastDirectionChangeTime > LEAF_DIRECTION_CHANGE_INTERVAL)
    {
        vx = -vx;  // Reverse horizontal direction
        lastDirectionChangeTime = GetTickCount64();
    }

    x += vx * dt;
    y += vy * dt;

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::Render()
{
    CAnimations* animations = CAnimations::GetInstance();
    animations->Get(ID_ANI_LEAF)->Render(x, y);
}