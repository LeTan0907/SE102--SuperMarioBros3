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
#include "Leaf.h"
#include "RedMushroom.h"
#include "PakkunFlower.h"
#include "Bullet.h"
#include "PlayScene.h"
#include "EnemySpawner.h"
float initial_x = 100.0f;
float initial_y = 80.0f;
#define MARIO_RESPAWN_TIME	1500
#define PICKUP_RANGE	5
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
	else if (dynamic_cast<CESpawner*>(e->obj))
		OnCollisionWithSpawner(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
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
		if (goomba->GetState() == GOOMBA_STATE_WINGED_WALKING|| goomba->GetState() == GOOMBA_STATE_WINGED_FLY)
		{
			goomba->SetState(GOOMBA_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		if (goomba->GetState() == GOOMBA_STATE_WALKING)
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
				if (level == MARIO_LEVEL_TANUKI)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				if (level == MARIO_LEVEL_BIG)
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
void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	SetLevel(MARIO_LEVEL_TANUKI);
	e->obj->Delete();
}
void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

	// Check for valid Koopas object
	if (!koopas) return;

	// Jump on top of Koopas
	if (e->ny < 0)
	{	
		if (koopas->GetState() == KOOPAS_WINGED_WALKING)
		{
			koopas->SetState(KOOPAS_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
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
			koopas->SetState(KOOPAS_STATE_SHELL);
			DebugOut(L">>> Koopas Die >>> \n");
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}

	if ((state==MARIO_STATE_RUNNING_LEFT||state==MARIO_STATE_RUNNING_RIGHT) && (koopas->GetState() == KOOPAS_STATE_SHELL))
	{
		holdKoopas = 1;
		koopas->setisHold(1);
	}
	else // Mario is hit by Koopas
	{
		if (untouchable == 0)
		{
			if (koopas->GetState() != KOOPAS_STATE_DIE && koopas->GetState() != KOOPAS_STATE_SHELL)
			{
				if (level == MARIO_LEVEL_TANUKI)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				if (level == MARIO_LEVEL_BIG)
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
		if (level == MARIO_LEVEL_TANUKI)
		{
			level = MARIO_LEVEL_BIG;
			bullet->Delete(); // Remove the bullet after collision
			StartUntouchable();
		}
		// If Mario is big (or any level higher than small), make him small and start the untouchable timer
		if (level == MARIO_LEVEL_BIG)
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
void CMario::OnCollisionWithSpawner(LPCOLLISIONEVENT e)
{
	CESpawner* spawner= dynamic_cast<CESpawner*>(e->obj);
	spawner->Spawn();
	e->obj->Delete();
}

void CMario::OnCollisionWithQuestionBox(LPCOLLISIONEVENT e) {
	CQuestionBox* questionBox = dynamic_cast<CQuestionBox*>(e->obj);

	if (questionBox) {
		// Check if the collision occurs from below the question box
		if (e->ny > 0) {
			if ((questionBox->GetState() == STATE_ORIGIN) && (questionBox->GetReward() != 0 ))
			{
				if (level >= MARIO_LEVEL_BIG)
				{
					questionBox->setReward(2);
					questionBox->SpawnReward();
					questionBox->SetState(STATE_USED);
				}
				else if (level == MARIO_LEVEL_SMALL)
				{
					questionBox->setReward(1);
					questionBox->SpawnReward();
					questionBox->SetState(STATE_USED);
				}
			}
			else if(questionBox->GetState() == STATE_ORIGIN)
			{
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
int CMario::GetAniIdTanuki()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (isFlying&&state== MARIO_STATE_TANUKI_FLY)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_TANUKI_FLY_RIGHT;
			else
				aniId = ID_ANI_MARIO_TANUKI_FLY_LEFT;
		}
		else if (isFalling)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_TANUKI_FALL_RIGHT;
			else
				aniId = ID_ANI_MARIO_TANUKI_FALL_LEFT;
		}
		else
		{
			if (abs(ax) == MARIO_ACCEL_RUN_X)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_TANUKI_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_MARIO_TANUKI_JUMP_RUN_LEFT;
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_TANUKI_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_TANUKI_JUMP_WALK_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_TANUKI_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_TANUKI_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_TANUKI_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_TANUKI_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_TANUKI_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_TANUKI_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_TANUKI_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_TANUKI_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_TANUKI_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_TANUKI_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_TANUKI_IDLE_RIGHT;

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
	else if (level == MARIO_LEVEL_TANUKI)
		aniId = GetAniIdTanuki();
	/*if (isAttacking)
	{
		aniId = (nx > 0) ? ID_ANI_MARIO_ATTACK_RIGHT : ID_ANI_MARIO_ATTACK_LEFT;
	}
	*/

	// RenderBoundingBox();
	animations->Get(aniId)->Render(x, y);
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
	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
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
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;
	case MARIO_STATE_TANUKI_FLY:
		isFlying = true;
		isFalling = false;
		vy = -MARIO_JUMP_SPEED_Y/3;
		break;
	case MARIO_STATE_TANUKI_FALL:
		isFlying = false;
		isFalling = true;
		vy = MARIO_GRAVITY/10;
		break;
	default:
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
	else if (level == MARIO_LEVEL_TANUKI)
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
	else if (level == MARIO_LEVEL_TANUKI)
	{
		if (isSitting)
		{
			left = x - MARIO_TANUKI_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_TANUKI_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_TANUKI_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_TANUKI_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_TANUKI_BBOX_WIDTH / 2;
			top = y - MARIO_TANUKI_BBOX_HEIGHT / 2;
			right = left + MARIO_TANUKI_BBOX_WIDTH;
			bottom = top + MARIO_TANUKI_BBOX_HEIGHT;
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
bool CMario::CheckKoopasNearby()
{
	vector<LPGAMEOBJECT> objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetObjects();
	for (size_t i = 0; i < objects.size(); i++)
	{
		CKoopas* koopa = dynamic_cast<CKoopas*>(objects[i]);
		if (koopa != nullptr && koopa->GetState() == KOOPAS_STATE_SHELL)
		{
			float koopaX, koopaY;
			koopa->GetPosition(koopaX, koopaY);
			if (abs(x - koopaX) < PICKUP_RANGE && abs(y - koopaY) < PICKUP_RANGE)
			{
				return true;
			}
		}
	}
	return false;
}

void CMario::setTurtle(int i) {
	this->holdKoopas = i;
}
int CMario::getTurtle() {
	return holdKoopas;
}
//void CMario::Attack()
//{
//	if (isAttacking && GetTickCount64() - attack_start > MARIO_ATTACK_TIME)
//	{
//		isAttacking = false;
//	}
//
//	if (isAttacking)
//	{
//		for (auto& obj : *coObjects)
//		{
//			if (dynamic_cast<CGoomba*>(obj))
//			{
//				CGoomba* goomba = dynamic_cast<CGoomba*>(obj);
//				float l, t, r, b;
//				GetBoundingBox(l, t, r, b);
//				float gl, gt, gr, gb;
//				goomba->GetBoundingBox(gl, gt, gr, gb);
//				if (gl < r && gr > l && gt < b && gb > t)
//				{
//					goomba->SetState(GOOMBA_STATE_DIE);
//				}
//			}
//			// Add similar checks for other enemy types
//		}
//	}
//}
