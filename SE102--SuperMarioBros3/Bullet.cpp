#include "Bullet.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include <cmath>

CBullet::CBullet(float x, float y) : CGameObject(x, y)
{
    this->vx = 0.0f;
    this->vy = 0.0f;
    SetState(BULLET_STATE_FIRING);
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);
    x += vx * dt;
    y += vy * dt;
}


void CBullet::Render()
{
    int aniId = ID_ANI_BULLET_FIRING;
    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    RenderBoundingBox();
}
void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    
}

void CBullet::SetState(int state)
{
    CGameObject::SetState(state);
}
