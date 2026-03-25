#include "config.h"

float GetCurrentSpeed(int score, float currentFrameSpeed, float deltaTime) {
    float targetSpeed = SPEED_START;

    if (score < MILESTONE_1)          targetSpeed = SPEED_START;
    else if (score < MILESTONE_2)     targetSpeed = SPEED_STEP_1;
    else if (score < MILESTONE_3)     targetSpeed = SPEED_STEP_2;
    else if (score < MILESTONE_LIGHT) targetSpeed = SPEED_SONIC;
    else                              targetSpeed = SPEED_LIGHT;

    if (currentFrameSpeed < targetSpeed) {
        currentFrameSpeed += 150.0f * deltaTime; 
        if (currentFrameSpeed > targetSpeed) currentFrameSpeed = targetSpeed;
    }
    else if (currentFrameSpeed > targetSpeed) {
        currentFrameSpeed -= 150.0f * deltaTime; 
        if (currentFrameSpeed < targetSpeed) currentFrameSpeed = targetSpeed;
    }

    return currentFrameSpeed;
}

// Da wir jetzt Sammel-Sterne haben, gibt diese Funktion 
// in der config.cpp nur noch 0 zurück oder wir löschen sie später.
int CalculateStars(int score) {
    return 0; 
}

float GetPlayerSpeed(int score) {
    if (score < MILESTONE_2)     return PLAYER_SPEED_NORMAL;
    if (score < MILESTONE_LIGHT) return PLAYER_SPEED_FAST;
    return PLAYER_SPEED_LIGHT;
}