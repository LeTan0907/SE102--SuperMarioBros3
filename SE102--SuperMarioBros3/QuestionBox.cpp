#include "QuestionBox.h"
#include "Animations.h"
#include "RedMushroom.h"

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
void CQuestionBox::SpawnMushroom()
{
    // Create a new red mushroom instance and add it to the objects list
    float mushroomX = x;
    float mushroomY = y - RED_MUSHROOM_BBOX_HEIGHT; // Adjust position so mushroom spawns just above the box
    CRedMushroom* mushroom = new CRedMushroom(mushroomX, mushroomY);
    mushroom->Render();
}

