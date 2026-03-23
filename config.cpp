#include "config.h"

float GetCurrentSpeed(int score) {
    if (score < MILESTONE_1)     return SPEED_START;
    if (score < MILESTONE_2)     return SPEED_STEP_1;
    if (score < MILESTONE_3)     return SPEED_STEP_2;
    if (score < MILESTONE_LIGHT) return SPEED_SONIC;
    return SPEED_LIGHT;
}

int CalculateStars(int score) {
    if (score < MILESTONE_1) {
        return score / 500;
    } 
    if (score < MILESTONE_2 + 500) { // Ein kleiner Puffer
        return 2 + ((score - MILESTONE_1) / 250);
    } 
    if (score < MILESTONE_LIGHT) {
        return 10 + ((score - MILESTONE_2) / 100);
    }
    // Lichtgeschwindigkeit Bonus
    return 80 + ((score - MILESTONE_LIGHT) / 50);
}

float GetPlayerSpeed(int score) {
    if (score < MILESTONE_2)     return PLAYER_SPEED_NORMAL;
    if (score < MILESTONE_LIGHT) return PLAYER_SPEED_FAST;
    return PLAYER_SPEED_LIGHT; // Bei Lichtgeschwindigkeit muss man extrem flink sein!
}