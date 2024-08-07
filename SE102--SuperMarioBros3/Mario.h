#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "Koopas.h"
#include "debug.h"

#define MARIO_WALKING_SPEED		0.15f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.6f
#define MARIO_JUMP_RUN_SPEED_Y	0.65f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.5f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_HOLD 700
#define MARIO_STATE_THROW 701
#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601


#define MARIO_STATE_ATTACK 800
#define MARIO_ATTACK_TIME 300
#define MARIO_ATTACK_COOLDOWN 500

#define ID_ANI_MARIO_ATTACK_RIGHT 1720
#define ID_ANI_MARIO_ATTACK_LEFT 1721
#define MARIO_STATE_TANUKI_FLY    800
#define MARIO_STATE_TANUKI_FALL   801

#define ID_ANI_MARIO_TANUKI_IDLE_RIGHT       1700
#define ID_ANI_MARIO_TANUKI_IDLE_LEFT        1701
#define ID_ANI_MARIO_TANUKI_WALKING_RIGHT    1702
#define ID_ANI_MARIO_TANUKI_WALKING_LEFT     1703
#define ID_ANI_MARIO_TANUKI_RUNNING_RIGHT    1704
#define ID_ANI_MARIO_TANUKI_RUNNING_LEFT     1705
#define ID_ANI_MARIO_TANUKI_JUMP_WALK_RIGHT  1706
#define ID_ANI_MARIO_TANUKI_JUMP_WALK_LEFT   1707
#define ID_ANI_MARIO_TANUKI_JUMP_RUN_RIGHT   1708
#define ID_ANI_MARIO_TANUKI_JUMP_RUN_LEFT    1709
#define ID_ANI_MARIO_TANUKI_FLY_RIGHT        1711
#define ID_ANI_MARIO_TANUKI_FLY_LEFT         1710
#define ID_ANI_MARIO_TANUKI_FALL_RIGHT       1712
#define ID_ANI_MARIO_TANUKI_FALL_LEFT        1713
#define ID_ANI_MARIO_TANUKI_ATTACK_RIGHT 1714
#define ID_ANI_MARIO_TANUKI_ATTACK_LEFT 1715
#define ID_ANI_MARIO_TANUKI_BRACE_LEFT       2102
#define ID_ANI_MARIO_TANUKI_BRACE_RIGHT      2103
#define ID_ANI_MARIO_TANUKI_SIT_RIGHT        2200
#define ID_ANI_MARIO_TANUKI_SIT_LEFT         2201
#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TANUKI 3
#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16
#define MARIO_TANUKI_BBOX_WIDTH	14
#define MARIO_TANUKI_BBOX_HEIGHT 24
#define MARIO_TANUKI_SITTING_BBOX_WIDTH 14
#define MARIO_TANUKI_SITTING_BBOX_HEIGHT 16
#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12


#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;			// acceleration on y 
	int holdKoopas;
	BOOLEAN isFlying;
	BOOLEAN isFalling;
	ULONGLONG attack_start;
	bool isAttacking;
	int level;
	int untouchable;
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin;
	//CKoopas* heldKoopa;
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBox(LPCOLLISIONEVENT e);
	void OnCollisionWithRedMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithPakkun(LPCOLLISIONEVENT e);
	void OnCollisionWithSpawner(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdTanuki();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	bool CheckKoopasNearby();
	void ThrowKoopas();
	int GetLevel()
	{
		return level;
	}
	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}
	void Attack();
	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	void setTurtle(int i);
	int getTurtle();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};