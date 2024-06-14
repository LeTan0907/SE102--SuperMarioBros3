#pragma once
#include "GameObject.h"
#include "Bullet.h"

#define PAKKUN_FLOWER_BBOX_WIDTH 15
#define PAKKUN_FLOWER_BBOX_HEIGHT 15
#define PAKKUN_FLOWER_INACTIVE_WIDTH 23
#define PAKKUN_FLOWER_INACTIVE_HEIGHT 23

#define PAKKUN_FLOWER_SHOOT_WIDTH 15
#define PAKKUN_FLOWER_SHOOT_HEIGHT 23

#define PAKKUN_FLOWER_STATE_INACTIVE 100
#define PAKKUN_FLOWER_STATE_RISE 150
#define PAKKUN_FLOWER_STATE_SHOOT 200
#define PAKKUN_FLOWER_STATE_RETRACT 250

#define ID_ANI_PAKKUN_FLOWER_IDLE 12100
#define ID_ANI_PAKKUN_FLOWER_SHOOT 12101

#define SHOOT_COOLDOWN 3000 // Cooldown time between shots (in milliseconds)
#define BULLET_FIRING_SPEED 0.05f
#define FLOWER_RISE_SPEED 0.5f
#define FLOWER_HEIGHT 23.0f
#define FLOWER_SHOOT_DELAY 500
#define FLOWER_RETRACT_DELAY 1000

class CPakkunFlower : public CGameObject
{
protected:
    float ax;
    float ay;
    float y_temp; // Initial y position
    DWORD shoot_start = 0;
    DWORD fire_time = 0;
    bool isShooting = false;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();

    virtual int IsCollidable() { return 1; }

public:
    CPakkunFlower(float x, float y);
    virtual void SetState(int state);
    bool IsMarioNearby();
    void ShootBullet();
};