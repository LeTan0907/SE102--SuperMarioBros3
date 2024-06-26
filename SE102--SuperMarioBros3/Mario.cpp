#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "QuestionBox.h"
#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "Koopas.h"
#include "Collision.h"
#include "RedMushroom.h"
#include "PakkunFlower.h"
#include "Bullet.h"
float initial_x = 100.0f;
float initial_y = 80.0f;
#define MARIO_RESPAWN_TIME	1500
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<CRedMushroom*>(e->obj))
		OnCollisionWithRedMushroom(e);
	else if (dynamic_cast<CQuestionBox*>(e->obj))
		OnCollisionWithQuestionBox(e);
	else if (dynamic_cast<CBullet*>(e->obj))
		OnCollisionWithBullet(e);
	else if (dynamic_cast<CPakkunFlower*>(e->obj))
		OnCollisionWithPakkun(e);
}
void CMario::OnCollisionWithPakkun(LPCOLLISIONEVENT e)
{
	CPakkunFlower* pakkun = dynamic_cast<CPakkunFlower*>(e->obj);
	if (untouchable == 0)
	{
		if (pakkun->GetState() != PAKKUN_FLOWER_STATE_INACTIVE)
		{
			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}
void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

	// Check for valid Koopas object
	if (!koopas) return;

	// Jump on top of Koopas
	if (e->ny < 0)
	{
		if (koopas->GetState() == KOOPAS_STATE_WALKING)
		{
			koopas->SetState(KOOPAS_STATE_SHELL);
			DebugOut(L">> Koopas Shell >>> \n");
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopas->GetState() == KOOPAS_STATE_SHELL)
		{
			koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
			DebugOut(L">>> Koopas Shell Moving >>> \n");
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopas->GetState() == KOOPAS_STATE_SHELL_MOVING)
		{
			koopas->SetState(KOOPAS_STATE_DIE);
			DebugOut(L">>> Koopas Die >>> \n");
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}

	else if (e->nx != 0 && koopas->GetState() == KOOPAS_STATE_SHELL)
	{
	koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
	koopas->SetSpeed(nx * KOOPAS_SHELL_SPEED, 0);
	}
	else // Mario is hit by Koopas
	{
		if (untouchable == 0)
		{
			if (koopas->GetState() != KOOPAS_STATE_DIE && koopas->GetState() != KOOPAS_STATE_SHELL)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithBullet(LPCOLLISIONEVENT e)
{
	// Ensure the collision object is a bullet
	CBullet* bullet = dynamic_cast<CBullet*>(e->obj);
	if (!bullet) return;

	// Check if Mario is not in the untouchable state
	if (untouchable == 0)
	{
		// If Mario is big (or any level higher than small), make him small and start the untouchable timer
		if (level > MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_SMALL;
			bullet->Delete(); // Remove the bullet after collision
			StartUntouchable();
		}
		else
		{
			// If Mario is already small, he dies
			DebugOut(L">>> Mario DIE >>> \n");
			bullet->Delete(); // Remove the bullet after collision
			SetState(MARIO_STATE_DIE);
		}
	}
}


void CMario::OnCollisionWithQuestionBox(LPCOLLISIONEVENT e) {
	CQuestionBox* questionBox = dynamic_cast<CQuestionBox*>(e->obj);

	if (questionBox) {
		// Check if the collision occurs from below the question box
		if (e->ny > 0) {
			if (questionBox->GetState() == STATE_ORIGIN) {
				questionBox->SpawnReward();
				questionBox->SetState(STATE_USED);
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}
void CMario::OnCollisionWithRedMushroom(LPCOLLISIONEVENT e)
{
	CRedMushroom* mushroom = dynamic_cast<CRedMushroom*>(e->obj);

	if (mushroom)
	{
		mushroom->Delete();  // Remove the mushroom from the game

		if (level == MARIO_LEVEL_SMALL)
		{
			SetLevel(MARIO_LEVEL_BIG);
			StartUntouchable();
			DebugOut(L">>> Mario levels up to Big Mario >>> \n");
		}
	}
}
//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();

	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;
	//case MARIO_STATE_HOLD:
	//	if (heldKoopa != nullptr) {
	//		// Logic for holding a Koopa shell
	//		heldKoopa->SetPosition(x, y); // Keep Koopa at Mario's position
	//	}
	//	break;
	//case MARIO_STATE_THROW:
	//	if (heldKoopa != nullptr) {
	//		// Logic for throwing a Koopa shell
	//		heldKoopa->SetState(KOOPAS_STATE_SHELL_MOVING);
	//		heldKoopa->SetSpeed(0.2f * nx, 0); // Adjust the speed and direction
	//		heldKoopa = nullptr; // Release the Koopa shell
	//	}
	//	break;
	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}
//void CMario::HoldKoopas(CKoopas* koopas) {
//	heldKoopa = koopas;
//	SetState(MARIO_STATE_HOLD);
//}

void CMario::ThrowKoopas() {
	SetState(MARIO_STATE_THROW);
}

