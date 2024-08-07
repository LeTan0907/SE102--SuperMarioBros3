#include "Goomba.h"
#include "Koopas.h"
CGoomba::CGoomba(float x, float y):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == GOOMBA_STATE_DIE)
    {
        left = x - GOOMBA_BBOX_WIDTH / 2;
        top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
        right = left + GOOMBA_BBOX_WIDTH;
        bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
    }
    else if (state == GOOMBA_STATE_WINGED_WALKING || state == GOOMBA_STATE_WINGED_FLY)
    {
        left = x - GOOMBA_WINGED_BBOX_WIDTH / 2;
        top = y - GOOMBA_WINGED_BBOX_HEIGHT / 2;
        right = left + GOOMBA_WINGED_BBOX_WIDTH;
        bottom = top + GOOMBA_WINGED_BBOX_HEIGHT;
    }
    else
    {
        left = x - GOOMBA_BBOX_WIDTH / 2;
        top = y - GOOMBA_BBOX_HEIGHT / 2;
        right = left + GOOMBA_BBOX_WIDTH;
        bottom = top + GOOMBA_BBOX_HEIGHT;
    }
}


void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	// Update Goomba's position and check collisions
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

	// Handle winged Goomba flying logic
	if (state == GOOMBA_STATE_WINGED_WALKING || state == GOOMBA_STATE_WINGED_FLY)
	{
		if (state == GOOMBA_STATE_WINGED_WALKING && GetTickCount64() - fly_start > 1000)
		{
			SetState(GOOMBA_STATE_WINGED_FLY);
		}
		else if (state == GOOMBA_STATE_WINGED_FLY && GetTickCount64() - fly_start > 1000)
		{
			SetState(GOOMBA_STATE_WINGED_WALKING);
		}
	}

	// Handle death state timeout
	if (state == GOOMBA_STATE_DIE && GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT)
	{
		// Mark as deleted if die timeout has passed
		isDeleted = true;
		return;
	}
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE) 
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}
	if (state == GOOMBA_STATE_WINGED_WALKING||state == GOOMBA_STATE_WINGED_FLY)
	{
		aniId = ID_ANI_GOOMBA_WINGED;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
			break;
		case GOOMBA_STATE_WINGED_WALKING:
			vx = -GOOMBA_WALKING_SPEED;
			fly_start = GetTickCount64();
			ay = GOOMBA_GRAVITY;
			break;
		case GOOMBA_STATE_WINGED_FLY:
			fly_start = GetTickCount64();
			vy = -GOOMBA_WALKING_SPEED;
			ay = 0;
			
			break;
	}
}
