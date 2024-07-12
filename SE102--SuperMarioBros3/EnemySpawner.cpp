#include "EnemySpawner.h"
#include "Goomba.h"
#include "Koopas.h"
#include "PlayScene.h"
void CESpawner::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x;
    top = y;
    right = left + SPAWNER_BBOX_WIDTH;
    bottom = top + SPAWNER_BBOX_HEIGHT;
}
void CESpawner::Render()
{
    CAnimations* animations = CAnimations::GetInstance();
    CAnimations::GetInstance()->Get(ID_ANI_SPAWNER)->Render(x, y);
}
void CESpawner::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CESpawner::OnCollisionWith(LPCOLLISIONEVENT e)
{

}
void CESpawner::SpawnGoomba()
{
    CGameObject* enemy = nullptr;
        CGameObject* goomba1 = new CGoomba(x, y );
        CGameObject* goomba2 = new CGoomba(x , y );
        CGameObject* goomba3 = new CGoomba(x , y) ;
        ((CGoomba*)goomba3)->SetState(GOOMBA_STATE_WINGED_WALKING);

        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        scene->AddObject(goomba1);
        scene->AddObject(goomba2);
        scene->AddObject(goomba3);
}

void CESpawner::SpawnKoopas()
{

}