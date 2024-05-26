#include "Bullet.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include <cmath>

CBullet::CBullet(float x, float y) : CGameObject(x, y)
{
    this->vx = 0.0f;
    this->vy = 0.0f;
    SetState(BULLET_STATE_FIRING);
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);

    // Find Mario
    CMario* mario = nullptr;
    CPlayScene* currentScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    if (currentScene)
    {
        for (auto obj : currentScene->GetObjects())
        {
            mario = dynamic_cast<CMario*>(obj);
            if (mario) break;
        }
    }

    if (mario)
    {
        float marioX, marioY;
        mario->GetPosition(marioX, marioY);
        float dx = marioX - x;
        float dy = marioY - y;
        float length = sqrt(dx * dx + dy * dy);
        vx = BULLET_FIRING_SPEED * (dx / length);
        vy = BULLET_FIRING_SPEED * (dy / length);
    }

    x += vx * dt;
    y += vy * dt;
}

void CBullet::Render()
{
    int aniId = ID_ANI_BULLET_FIRING;
    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    RenderBoundingBox();
}
void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    
}

void CBullet::SetState(int state)
{
    CGameObject::SetState(state);
}

// Method to spawn bullet
void FireBullet(float x, float y)
{
    CBullet* bullet = new CBullet(x, y);
    CPlayScene* currentScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    if (currentScene)
    {
        currentScene->AddObject(bullet);
    }
}
