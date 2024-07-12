#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#define SPAWNER_BBOX_WIDTH 16
#define SPAWNER_BBOX_HEIGHT 200
#define ID_ANI_SPAWNER 0
class CESpawner : public CGameObject
{
    int type;
public:
    CESpawner(float x, float y,int Type) : CGameObject(x, y) {
        type = Type;
    }
    void Render();
    virtual int IsBlocking() { return 0; }
    virtual int IsCollidable() { return 1; };
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void OnCollisionWith(LPCOLLISIONEVENT e);
    void SpawnGoomba();
    void SpawnKoopas();
    void Spawn();
};
