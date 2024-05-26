#include "PakkunFlower.h"
#include "Game.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "Bullet.h"

#define SHOOT_COOLDOWN 1000 // Cooldown time between shots (in milliseconds)
#define BULLET_WALKING_SPEED 0.1f
CPakkunFlower::CPakkunFlower(float x, float y) : CGameObject()
{
    this->x = x;
    this->y = y;
    this->ax = 0;
    this->ay = 0;
    this->shoot_start = GetTickCount();
    this->isShooting = false;
    this->SetState(PAKKUN_FLOWER_STATE_INACTIVE);
}

void CPakkunFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x;
    top = y;
    right = x + PAKKUN_FLOWER_BBOX_WIDTH;
    bottom = y + PAKKUN_FLOWER_BBOX_HEIGHT;
}

void CPakkunFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);
    DWORD currentTime = GetTickCount();
    if (currentTime - shoot_start >= SHOOT_COOLDOWN)
    {
        ShootBullet();
        shoot_start = currentTime;
    }
}

void CPakkunFlower::Render()
{
    int ani = ID_ANI_PAKKUN_FLOWER_IDLE;
    if (state == PAKKUN_FLOWER_STATE_SHOOT)
    {
        ani = ID_ANI_PAKKUN_FLOWER_SHOOT;
    }

    CAnimations::GetInstance()->Get(ani)->Render(x, y);
}

void CPakkunFlower::SetState(int state)
{
    CGameObject::SetState(state);
    switch (state)
    {
    case PAKKUN_FLOWER_STATE_INACTIVE:
        break;
    case PAKKUN_FLOWER_STATE_SHOOT:
        break;
    }
}

void CPakkunFlower::ShootBullet()
{
    float bulletX = x + PAKKUN_FLOWER_BBOX_WIDTH / 2 - BULLET_BBOX_WIDTH / 2;
    float bulletY = y + PAKKUN_FLOWER_BBOX_HEIGHT / 2 - BULLET_BBOX_HEIGHT / 2;
    CBullet* bullet = new CBullet(bulletX, bulletY);
    bullet->SetSpeed(BULLET_WALKING_SPEED, 0);
    
}
