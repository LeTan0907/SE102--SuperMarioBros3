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
void CBullet::CalculateDirection(float targetX, float targetY)
{
    // Calculate direction towards the target
    float dx = targetX - x;
    float dy = targetY - y;
    float length = sqrt(dx * dx + dy * dy);

    // Check if the target is within range
    if (length > 0)
    {
        // Normalize direction vector
        dx /= length;
        dy /= length;

        // Set velocity towards the target
        vx = BULLET_FIRING_SPEED * dx;
        vy = BULLET_FIRING_SPEED * dy;
    }
    else
    {
        // Stop moving if the target is out of range
        vx = 0;
        vy = 0;
    }
}
void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);
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
void FireBullet(float x, float y, float targetX, float targetY)
{
    CBullet* bullet = new CBullet(x, y);
    bullet->CalculateDirection(targetX, targetY); // Calculate direction towards the target
    CPlayScene* currentScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    if (currentScene)
    {
        currentScene->AddObject(bullet);
    }
}
