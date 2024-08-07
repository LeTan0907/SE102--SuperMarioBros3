#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"
#include "Koopas.h"
void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    switch (KeyCode)
    {
    case DIK_DOWN:
        mario->SetState(MARIO_STATE_SIT);
        break;
    case DIK_S:
        mario->SetState(MARIO_STATE_JUMP);
        break;
    case DIK_1:
        mario->SetLevel(MARIO_LEVEL_SMALL);
        break;
    case DIK_2:
        mario->SetLevel(MARIO_LEVEL_BIG);
        break;
    case DIK_3:
        mario->SetLevel(MARIO_LEVEL_TANUKI);
        break;

    case DIK_0:
        mario->SetState(MARIO_STATE_DIE);
        break;
    case DIK_R: // reset
        //Reload();
        break;
    case DIK_J:
        if (mario->GetLevel() == MARIO_LEVEL_TANUKI)
            mario->SetState(MARIO_STATE_TANUKI_FLY);
        break;
    case DIK_K:
        if (mario->GetLevel() == MARIO_LEVEL_TANUKI)
            mario->SetState(MARIO_STATE_TANUKI_FALL);
        break;
    }
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
    switch (KeyCode)
    {
    case DIK_S:
        mario->SetState(MARIO_STATE_RELEASE_JUMP);
        break;
    case DIK_A:    
        if (mario->getTurtle()) {
            mario->setTurtle(0);
        }
        break;
    case DIK_DOWN:
        mario->SetState(MARIO_STATE_SIT_RELEASE);
        break;
    case DIK_J:
        if (mario->GetLevel() == MARIO_LEVEL_TANUKI)
            mario->SetState(MARIO_STATE_IDLE); // Reset state when J is released
        break;
    case DIK_K:
        if (mario->GetLevel() == MARIO_LEVEL_TANUKI)
            mario->SetState(MARIO_STATE_IDLE); // Reset state when K is released
        break;
    }
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
    LPGAME game = CGame::GetInstance();
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    if (game->IsKeyDown(DIK_RIGHT))
    {
        if (game->IsKeyDown(DIK_A))
            mario->SetState(MARIO_STATE_RUNNING_RIGHT);
        else
            mario->SetState(MARIO_STATE_WALKING_RIGHT);
    }
    else if (game->IsKeyDown(DIK_LEFT))
    {
        if (game->IsKeyDown(DIK_A))
            mario->SetState(MARIO_STATE_RUNNING_LEFT);
        else
            mario->SetState(MARIO_STATE_WALKING_LEFT);
    }
    else
    {
        mario->SetState(MARIO_STATE_IDLE);
    }
    if (mario->GetLevel() == MARIO_LEVEL_TANUKI)
    {
        if (game->IsKeyDown(DIK_J))
        {
            mario->SetState(MARIO_STATE_TANUKI_FLY);
        }
        else if (game->IsKeyDown(DIK_K))
        {
            mario->SetState(MARIO_STATE_TANUKI_FALL);
        }
    }
}
