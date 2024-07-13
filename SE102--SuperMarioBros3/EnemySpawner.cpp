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
   /* RenderBoundingBox();*/
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
        CGameObject* goomba1 = new CGoomba(830, 100 );
        CGameObject* goomba2 = new CGoomba(880 , 100 );
        CGameObject* goomba3 = new CGoomba(930 , 100) ;
        ((CGoomba*)goomba3)->SetState(GOOMBA_STATE_WINGED_WALKING);

        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        scene->AddObject(goomba1);
        scene->AddObject(goomba2);
        scene->AddObject(goomba3);
}

void CESpawner::SpawnKoopas()
{
    CKoopas* koopas1 = new CKoopas(1400, 0);
    CKoopas* koopas2 = new CKoopas(1450, 0);
    CKoopas* koopas3 = new CKoopas(1500, 0);
    koopas1->SetState(KOOPAS_WINGED_WALKING);
    koopas2->SetState(KOOPAS_WINGED_WALKING);
    koopas3->SetState(KOOPAS_WINGED_WALKING);
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    scene->AddObject(koopas1);
    scene->AddObject(koopas2);
    scene->AddObject(koopas3);
}
void CESpawner::Spawn()
{
    if (type == 1)
    {
        SpawnGoomba();
    }
    if (type == 2)
    {
        SpawnKoopas();
    }
}