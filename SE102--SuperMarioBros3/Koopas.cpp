#include "Koopas.h"
#include "Goomba.h"
#include "debug.h"
#include "EdgeChecker.h"
#include "PlayScene.h"
#define PICKUP_RANGE 100
CKoopas::CKoopas(float x, float y) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = KOOPAS_GRAVITY;
    die_start = -1;
    shell_start = -1; // Initialize shell start time
    SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SHELL_MOVING)
    {
        left = x - KOOPAS_BBOX_WIDTH / 2;
        top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
        right = left + KOOPAS_BBOX_WIDTH;
        bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
    }
    else
    {
        left = x - KOOPAS_BBOX_WIDTH / 2;
        top = y - KOOPAS_BBOX_HEIGHT / 2;
        right = left + KOOPAS_BBOX_WIDTH;
        bottom = top + KOOPAS_BBOX_HEIGHT;
    }
}

void CKoopas::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CGoomba*>(e->obj))
    {
        OnCollisionWithGoomba(e);
    }
    if (dynamic_cast<CEdgeChecker*>(e->obj))
    {
        OnCollisionWithEdgeChecker(e);
    }

    if (!e->obj->IsBlocking()) return;

    if (e->ny != 0)
    {
        vy = 0;
    }
    else if (e->nx != 0)
    {
        vx = -vx;
    }
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    if (state == KOOPAS_STATE_DIE && GetTickCount64() - die_start > KOOPAS_DIE_TIMEOUT)
    {
        isDeleted = true;
        return;
    }

    if (state == KOOPAS_STATE_SHELL && GetTickCount64() - shell_start > KOOPAS_REVIVE_TIMEOUT)
    {
        SetState(KOOPAS_STATE_WALKING);
        y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2; // Reset position to avoid falling through the ground
    }
    //CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    //// Spawn edge checker if not already spawned
    //if (!edgeChecker) {

    //    if (scene) {
    //        float edgeCheckerX = x + (vx > 0 ? 1 : -1) * EDGE_CHECKER_DISTANCE;
    //        float edgeCheckerY = y-2;
    //        edgeChecker = new CEdgeChecker(edgeCheckerX, edgeCheckerY);
    //        scene->AddObject(edgeChecker);
    //    }
    //}
    //if (edgeChecker) {
    //    edgeChecker->SetPosition(x + (vx > 0 ? 1 : -1) * EDGE_CHECKER_DISTANCE, y);
    //}
    //if (edgeChecker && edgeChecker->IsAtEdge())
    //{
    //    vx = -vx;
    //}

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

//CKoopas* CKoopas::CheckNearbyKoopas(CMario* mario) {
//    // Logic to check if a Koopa shell is near Mario
//    vector<LPGAMEOBJECT> objects = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetObjects();
//    for (size_t i = 0; i < objects.size(); i++) {
//        CKoopas* koopa = dynamic_cast<CKoopas*>(objects[i]);
//        if (koopa != nullptr && koopa->GetState() == KOOPAS_STATE_SHELL) {
//            // Check if the Koopa is close enough to Mario
//            float koopaX, koopaY;
//            koopa->GetPosition(koopaX, koopaY);
//            if (abs(mario->GetX() - koopaX) < PICKUP_RANGE && abs(mario->GetY() - koopaY) < PICKUP_RANGE) {
//                return koopa;
//            }
//        }
//    }
//    return nullptr;
//}

void CKoopas::Render()
{
    int aniId = ID_ANI_KOOPAS_WALKING_LEFT;

    if (vx > 0)
    {
        aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
    }
    if (state == KOOPAS_STATE_DIE)
    {
        aniId = ID_ANI_KOOPAS_DIE;
    }
    else if (state == KOOPAS_STATE_SHELL)
    {
        aniId = ID_ANI_KOOPAS_SHELL;
    }
    else if (state == KOOPAS_STATE_SHELL_MOVING)
    {
        aniId = ID_ANI_KOOPAS_SHELL_MOVING;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}
void CKoopas::OnCollisionWithEdgeChecker(LPCOLLISIONEVENT e)
{
    CEdgeChecker* edgechecker = dynamic_cast<CEdgeChecker*>(e->obj);
    if (edgechecker != nullptr)
    {
        vx = -vx;
    }
}

void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
    CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
    if (goomba == nullptr)
    {
        return;
    }

    if (state == KOOPAS_STATE_SHELL_MOVING)
    {
        if (goomba->GetState() != GOOMBA_STATE_DIE)
        {
            goomba->SetState(GOOMBA_STATE_DIE);
        }
    }
 
}


void CKoopas::SetState(int state)
{
    CGameObject::SetState(state);
    switch (state)
    {
    case KOOPAS_STATE_DIE:
        die_start = GetTickCount64();
        y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
        vx = 0;
        vy = -0.3f;
        ay = 0;
        break;
    case KOOPAS_STATE_WALKING:
        vx = -KOOPAS_WALKING_SPEED;
        ax = 0;
        ay = KOOPAS_GRAVITY;
        break;
    case KOOPAS_STATE_SHELL:
        shell_start = GetTickCount64(); // Start the shell timer
        vx = 0;
        ax = 0;
        ay = KOOPAS_GRAVITY;
        break;
    case KOOPAS_STATE_SHELL_MOVING:
        vx = KOOPAS_SHELL_SPEED;
        ax = 0;
        ay = KOOPAS_GRAVITY;
        break;
    }
}
