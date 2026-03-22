#include "level.h"

// 10 verschiedene, leuchtende Farben für die Level-Schrift!
const Color LEVEL_COLORS[] = { 
    WHITE, YELLOW, ORANGE, RED, PINK, MAGENTA, VIOLET, SKYBLUE, LIME, GOLD 
};
const int COLOR_COUNT = 10;

float GetLevelDuration() {
    return 30.0f; 
}

Color GetLevelColor(int currentLevel) {
    // Gibt jetzt die Farbe für den Level-Text zurück
    return LEVEL_COLORS[(currentLevel - 1) % COLOR_COUNT];
}

float CalculateObstacleSpeed(int currentLevel, float timeLeft) {
    float baseSpeed = 350.0f;
    float levelBoost = currentLevel * 70.0f;
    float timeBoost = (GetLevelDuration() - timeLeft) * 2.0f;
    
    return baseSpeed + levelBoost + timeBoost;
}