#pragma once
#include "GameObject.h"
#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.05f
#define KOOPAS_SHELL_SPEED 0.15f

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 24
#define KOOPAS_BBOX_HEIGHT_SHELL 16

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_SHELL 200
#define KOOPAS_STATE_SHELL_MOVING 300
#define KOOPAS_STATE_DIE 400

#define ID_ANI_KOOPAS_WALKING_LEFT 6000
#define ID_ANI_KOOPAS_WALKING_RIGHT 6100
#define ID_ANI_KOOPAS_SHELL 6001
#define ID_ANI_KOOPAS_SHELL_MOVING 6002
#define ID_ANI_KOOPAS_DIE 6003
#define KOOPAS_DIE_TIMEOUT 6004


class CKoopas : public CGameObject
{
protected:
    float ax;
    float ay;

    ULONGLONG die_start;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();

    virtual int IsCollidable() { return 1; }
    virtual int IsBlocking() { return 0; }
    virtual void OnNoCollision(DWORD dt);
    void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
    CKoopas(float x, float y);
    virtual void SetState(int state);
};