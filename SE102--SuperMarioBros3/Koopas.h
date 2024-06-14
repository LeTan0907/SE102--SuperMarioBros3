#pragma once
#include "GameObject.h"
#include "EdgeChecker.h"
#include "Mario.h"
#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.04f
#define KOOPAS_SHELL_SPEED 0.15f

#define KOOPAS_BBOX_WIDTH 15
#define KOOPAS_BBOX_HEIGHT 24
#define KOOPAS_BBOX_HEIGHT_SHELL 16

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_SHELL 200
#define KOOPAS_STATE_SHELL_MOVING 300
#define KOOPAS_STATE_DIE 400
#define KOOPAS_REVIVE_TIMEOUT 5000
#define KOOPAS_DIE_TIMEOUT 3004

#define ID_ANI_KOOPAS_WALKING_LEFT 6000
#define ID_ANI_KOOPAS_WALKING_RIGHT 6100
#define ID_ANI_KOOPAS_SHELL 6001
#define ID_ANI_KOOPAS_SHELL_MOVING 6002
#define ID_ANI_KOOPAS_DIE 6003
#define ID_ANI_KOOPAS_DIE_RETURN 6004


class CKoopas : public CGameObject
{
protected:
    float ax;
    float ay;
    bool isHold;
    ULONGLONG die_start;
    ULONGLONG shell_start;
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual int IsCollidable() { return state!=KOOPAS_STATE_DIE; }
    virtual int IsBlocking() { return 0; }
    void OnNoCollision(DWORD dt);
    
    void OnCollisionWith(LPCOLLISIONEVENT e);
    CEdgeChecker* edgeChecker;
public:
    CKoopas(float x, float y);
    virtual void SetState(int state);
    void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
    //CKoopas* CheckNearbyKoopas(CMario* mario);
};