#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_QUESTIONBOX_ORIGIN 11001
#define ID_ANI_QUESTIONBOX_USED 11002
#define QSB_WIDTH 16
#define QSB_BBOX_WIDTH 16
#define QSB_BBOX_HEIGHT 16

#define STATE_ORIGIN 100
#define STATE_USED 200
class CQuestionBox : public CGameObject {
	int reward;
public:
	CQuestionBox(float x, float y, int reward) : CGameObject(x, y) {
		this->reward = reward;
		SetState(STATE_ORIGIN);
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	int GetReward()
	{
		return reward;
	}
	void SpawnReward();
};
