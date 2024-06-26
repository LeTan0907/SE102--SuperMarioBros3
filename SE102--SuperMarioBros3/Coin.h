#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16
#define COIN_SPAWN 1000
class CCoin : public CGameObject {
protected:
	float temp_y;
public:
	CCoin(float x, float y) : CGameObject(x, y) {
		temp_y = y;
	}
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void CoinFly();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};