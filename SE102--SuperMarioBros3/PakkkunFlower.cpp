#include "PakkunFlower.h"
#include "Game.h"
#include "PlayScene.h"
#include "GameObject.h"
#include "Bullet.h"

#define SHOOT_COOLDOWN 3000 // Cooldown time between shots (in milliseconds)
#define BULLET_WALKING_SPEED 0.05f

CPakkunFlower::CPakkunFlower(float x, float y) : CGameObject()
{
    this->x = x;
    this->y = y;
    this->ax = 0;
    this->ay = 0;
    this->shoot_start = GetTickCount();
    this->isShooting = false;
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
        top = y - PAKKUN_FLOWER_SHOOT_HEIGHT/2;
        right = left + PAKKUN_FLOWER_SHOOT_WIDTH;
        bottom = top + PAKKUN_FLOWER_SHOOT_HEIGHT;
    }
}

void CPakkunFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);
    DWORD currentTime = GetTickCount();

    // Handle the cooldown timer for shooting
    if (state == PAKKUN_FLOWER_STATE_INACTIVE && currentTime - shoot_start >= SHOOT_COOLDOWN)
    {
        // Check if Mario is nearby
        if (IsMarioNearby())
        {
            // Shoot a bullet
            ShootBullet();
            SetState(PAKKUN_FLOWER_STATE_SHOOT);
            shoot_start = currentTime;
        }
    }
    else if (state == PAKKUN_FLOWER_STATE_SHOOT && currentTime - shoot_start >= SHOOT_COOLDOWN)
    {
        // Set the Pakkun Flower to inactive state after the cooldown period
        SetState(PAKKUN_FLOWER_STATE_INACTIVE);
    }
}


void CPakkunFlower::ShootBullet()
{
    // Get the player object from the scene
    CGameObject* playerObject = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    // Check if the player object is a CMario instance
    if (dynamic_cast<CMario*>(playerObject) != nullptr)
    {
        // If it's a CMario instance, calculate the direction towards Mario
        CMario* mario = dynamic_cast<CMario*>(playerObject);
        float marioX = mario->x;
        float marioY = mario->y;

        // Calculate the direction towards Mario
        float dirX = marioX - x;
        float dirY = marioY - y;

        // Normalize the direction vector
        float distance = sqrt(dirX * dirX + dirY * dirY);
        dirX /= distance;
        dirY /= distance;

        // Create a bullet directed towards Mario
        float bulletX = x + PAKKUN_FLOWER_INACTIVE_WIDTH / 4 - BULLET_BBOX_WIDTH;
        float bulletY = y - BULLET_BBOX_HEIGHT * 2;
        CBullet* bullet = new CBullet(bulletX, bulletY);
        bullet->SetSpeed(BULLET_WALKING_SPEED * dirX, BULLET_WALKING_SPEED * dirY);
        CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        if (scene) {
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
    float renderX = x;
    float renderY = y;
    if (state == PAKKUN_FLOWER_STATE_INACTIVE)
    {
        renderY -= (PAKKUN_FLOWER_SHOOT_HEIGHT - PAKKUN_FLOWER_INACTIVE_HEIGHT) / 4;
    }
    CAnimations::GetInstance()->Get(ani)->Render(renderX, renderY);
}



void CPakkunFlower::SetState(int state)
{
    CGameObject::SetState(state);
    switch (state)
    {
    case PAKKUN_FLOWER_STATE_INACTIVE:
        // Adjust position if needed
        vx = 0;
        vy = 0;
        isShooting = false;
        break;
    case PAKKUN_FLOWER_STATE_SHOOT:
        // Adjust position if needed
        vx = 0;
        vy = 0;
        isShooting = true;
        break;
    }
}
bool CPakkunFlower::IsMarioNearby()
{
    // Get the player object from the scene
    CGameObject* playerObject = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    // Check if the player object is a CMario instance
    if (dynamic_cast<CMario*>(playerObject) != nullptr)
    {
        // If it's a CMario instance, calculate the distance
        CMario* mario = dynamic_cast<CMario*>(playerObject);
        float marioX = mario->x;
        float marioY = mario->y;

        // Get the distance between Mario and the Pakkun Flower
        float distance = sqrt(pow(x - marioX, 2) + pow(y - marioY, 2));

        // Define the range for Mario to be considered nearby
        float nearbyRange = 120.0f; // Adjust as needed

        // Check if Mario is within the nearby range
        return distance <= nearbyRange;
    }
    else
    {
        // If it's not a CMario instance, Mario is not nearby
        return false;
    }
}
