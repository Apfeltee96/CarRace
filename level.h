#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"

// Gibt die Dauer eines Levels in Sekunden zurück
float GetLevelDuration();

// Gibt die Hintergrundfarbe für das aktuelle Level zurück
Color GetLevelColor(int currentLevel);

// Berechnet, wie schnell die Hindernisse aktuell sein sollen
float CalculateObstacleSpeed(int currentLevel, float timeLeft);

#endif