#include "PakkunFlower.h"
#include "Game.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "Bullet.h"

#define SHOOT_COOLDOWN 3000 // Cooldown time between shots (in milliseconds)
#define BULLET_FIRING_SPEED 0.05f
#define FLOWER_RISE_SPEED 0.5f
#define FLOWER_HEIGHT 23.0f
#define FLOWER_SHOOT_DELAY 500
#define FLOWER_RETRACT_DELAY 1000

CPakkunFlower::CPakkunFlower(float x, float y) : CGameObject()
{
    this->x = x;
    this->y = y;
    this->ax = 0;
    this->ay = 0;
    this->shoot_start = GetTickCount64();
    this->isShooting = false;
    this->y_temp = y; // Initialize the original y position
    this->SetState(PAKKUN_FLOWER_STATE_INACTIVE);
}

void CPakkunFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == PAKKUN_FLOWER_STATE_INACTIVE)
    {
        left = x - PAKKUN_FLOWER_INACTIVE_WIDTH / 2;
        top = y - PAKKUN_FLOWER_INACTIVE_HEIGHT / 2;
        right = left + PAKKUN_FLOWER_INACTIVE_WIDTH;
        bottom = top + PAKKUN_FLOWER_INACTIVE_HEIGHT;
    }
    else if (state == PAKKUN_FLOWER_STATE_SHOOT)
    {
        left = x - PAKKUN_FLOWER_SHOOT_WIDTH / 2;
        top = y - PAKKUN_FLOWER_SHOOT_HEIGHT / 2;
        right = left + PAKKUN_FLOWER_SHOOT_WIDTH;
        bottom = top + PAKKUN_FLOWER_SHOOT_HEIGHT;
    }
}

void CPakkunFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);
    DWORD currentTime = GetTickCount64();

    if (state == PAKKUN_FLOWER_STATE_INACTIVE && currentTime - shoot_start >= SHOOT_COOLDOWN)
    {
        if (IsMarioNearby())
        {
            SetState(PAKKUN_FLOWER_STATE_RISE);
        }
    }
    else if (state == PAKKUN_FLOWER_STATE_RISE)
    {
        if (y > y_temp - FLOWER_HEIGHT)
        {
            y -= FLOWER_RISE_SPEED;
        }
        else
        {
            y = y_temp - FLOWER_HEIGHT;
            fire_time = currentTime;
            SetState(PAKKUN_FLOWER_STATE_SHOOT);
        }
    }
    else if (state == PAKKUN_FLOWER_STATE_SHOOT && currentTime - fire_time >= FLOWER_SHOOT_DELAY)
    {
        ShootBullet();
        fire_time = currentTime;
        SetState(PAKKUN_FLOWER_STATE_RETRACT);
    }
    else if (state == PAKKUN_FLOWER_STATE_RETRACT && currentTime - fire_time >= FLOWER_RETRACT_DELAY)
    {
        if (y < y_temp)
        {
            y += FLOWER_RISE_SPEED;
        }
        else
        {
            y = y_temp;
            shoot_start = currentTime;
            SetState(PAKKUN_FLOWER_STATE_INACTIVE);
        }
    }

    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPakkunFlower::ShootBullet()
{
    CGameObject* playerObject = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    if (dynamic_cast<CMario*>(playerObject) != nullptr)
    {
        CMario* mario = dynamic_cast<CMario*>(playerObject);
        float marioX = mario->x;
        float marioY = mario->y;
        float dirX = marioX - x;
        float dirY = marioY - y;
        float distance = sqrt(dirX * dirX + dirY * dirY);
        dirX /= distance;
        dirY /= distance;
        float bulletX = x + PAKKUN_FLOWER_INACTIVE_WIDTH / 4 - BULLET_BBOX_WIDTH;
        float bulletY = y - BULLET_BBOX_HEIGHT * 2;
        CBullet* bullet = new CBullet(bulletX, bulletY);
        bullet->SetSpeed(BULLET_FIRING_SPEED * dirX, BULLET_FIRING_SPEED * dirY);
        CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        if (scene)
        {
            scene->AddObject(bullet);
        }
    }
}

void CPakkunFlower::Render()
{
    int ani = ID_ANI_PAKKUN_FLOWER_IDLE;
    if (state == PAKKUN_FLOWER_STATE_SHOOT)
    {
        ani = ID_ANI_PAKKUN_FLOWER_SHOOT;
    }
    CAnimations::GetInstance()->Get(ani)->Render(x, y);
}

void CPakkunFlower::SetState(int state)
{
    CGameObject::SetState(state);
    switch (state)
    {
    case PAKKUN_FLOWER_STATE_INACTIVE:
        vx = 0;
        vy = 0;
        isShooting = false;
        break;
    case PAKKUN_FLOWER_STATE_RISE:
        vx = 0;
        vy = 0;
        isShooting = false;
        break;
    case PAKKUN_FLOWER_STATE_SHOOT:
        vx = 0;
        vy = 0;
        isShooting = true;
        break;
    case PAKKUN_FLOWER_STATE_RETRACT:
        vx = 0;
        vy = 0;
        isShooting = false;
        break;
    }
}

bool CPakkunFlower::IsMarioNearby()
{
    CGameObject* playerObject = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
    if (dynamic_cast<CMario*>(playerObject) != nullptr)
    {
        CMario* mario = dynamic_cast<CMario*>(playerObject);
        float marioX = mario->x;
        float marioY = mario->y;
        float distance = sqrt(pow(x - marioX, 2) + pow(y - marioY, 2));
        float nearbyRange = 120.0f; // Adjust as needed

        return distance <= nearbyRange;
    }
    else
    {
        return false;
    }
}
