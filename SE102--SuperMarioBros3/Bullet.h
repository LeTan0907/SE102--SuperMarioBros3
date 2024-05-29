#pragma once
#include "GameObject.h"

#define BULLET_FIRING_SPEED 0.05f


#define BULLET_BBOX_WIDTH 2
#define BULLET_BBOX_HEIGHT 2
#define BULLET_BBOX_HEIGHT_DIE 7


#define BULLET_STATE_FIRING 100

#define ID_ANI_BULLET_FIRING 1110

class CBullet : public CGameObject
{
protected:
	float ax;
	float ay;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
public:
	CBullet(float x, float y);
	virtual void SetState(int state);
};