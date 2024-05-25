#include "RedMushroom.h"
#include "Textures.h"
#include "Game.h"
#include "Collision.h"

CRedMushroom::CRedMushroom(float x, float y) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = RED_MUSHROOM_GRAVITY;
    vx = RED_MUSHROOM_SPEED;
}

void CRedMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - RED_MUSHROOM_BBOX_WIDTH / 2;
    top = y - RED_MUSHROOM_BBOX_HEIGHT / 2;
    right = left + RED_MUSHROOM_BBOX_WIDTH;
    bottom = top + RED_MUSHROOM_BBOX_HEIGHT;
}

void CRedMushroom::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CRedMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CRedMushroom*>(e->obj)) return;

    if (e->ny != 0)
    {
        vy = 0;
    }
    else if (e->nx != 0)
    {
        vx = -vx;
    }
}

void CRedMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRedMushroom::Render()
{
    LPTEXTURE texture = CTextures::GetInstance()->Get(ID_ANI_REDMUSHROOM);
    CGame::GetInstance()->Draw(x, y, texture);
}
