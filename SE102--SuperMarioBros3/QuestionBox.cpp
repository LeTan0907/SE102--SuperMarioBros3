#include "QuestionBox.h"
#include "Animations.h"
#include "RedMushroom.h"
#include "PlayScene.h"
#include "Leaf.h"
#include "Coin.h"
void CQuestionBox::Render() {
    int aniId = ID_ANI_QUESTIONBOX_ORIGIN;
    if (state == STATE_USED) {
        aniId = ID_ANI_QUESTIONBOX_USED;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CQuestionBox::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - QSB_BBOX_WIDTH / 2;
    t = y - QSB_BBOX_HEIGHT / 2;
    r = l + QSB_BBOX_WIDTH;
    b = t + QSB_BBOX_HEIGHT;
}

void CQuestionBox::SetState(int state) {
    this->state = state;
    switch (state) {
    case STATE_ORIGIN:
        // Logic for the origin state
        break;
    case STATE_USED:
        // Logic for the used state
        break;
    }
}
void CQuestionBox::SpawnReward() {
    CGameObject* rewardObject = nullptr;
    if (reward == 0) {
        rewardObject = new CCoin(x, y - QSB_BBOX_HEIGHT);
        if (GetTickCount64() - COIN_SPAWN == 0) {
            rewardObject->Delete();
        }// Spawn a coin
    }
    else if (reward == 1) {
        rewardObject = new CRedMushroom(x, y - QSB_BBOX_HEIGHT);
    }
    else
    {
        rewardObject = new CLeaf(x, y - QSB_BBOX_HEIGHT);
    }
    if (rewardObject) {
        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        scene->AddObject(rewardObject);
    }
}