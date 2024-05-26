#pragma once
#include "GameObject.h"
#include "Bullet.h"

#define PAKKUN_FLOWER_BBOX_WIDTH 20
#define PAKKUN_FLOWER_BBOX_HEIGHT 40

#define PAKKUN_FLOWER_STATE_INACTIVE 100
#define PAKKUN_FLOWER_STATE_SHOOT 200

#define ID_ANI_PAKKUN_FLOWER_IDLE 12100
#define ID_ANI_PAKKUN_FLOWER_SHOOT 12101

class CPakkunFlower : public CGameObject
{
protected:
    float ax;
    float ay;

    DWORD shoot_start = 0;
    bool isShooting = false;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();

    virtual int IsCollidable() { return 1; };



public:
    CPakkunFlower(float x, float y);
    virtual void SetState(int state);

    void ShootBullet();
};
