#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"
#include <string>
#include <vector>

const float SPEED_START  = 400.0f;
const float SPEED_LIGHT  = 1800.0f;
const int PRICE_RED_CAR    = 100;
const int PRICE_PURPLE_CAR = 200;

struct SaveGame {
    int totalStars;
    bool ownsRedCar;
    bool ownsPurpleCar;
    int selectedColorId; 
    bool isEnglish;
    bool isFullscreen; // NEU: Vollbild-Status speichern
    std::string lastPlayerName;
};

SaveGame LoadSaveGame();
void SaveGameData(SaveGame data);
void DeleteSaveData();
float GetCurrentSpeed(int score, float currentFrameSpeed, float deltaTime);
float GetDynamicPlayerSpeed(float currentWorldSpeed);
int CalculateStars(int score);
Color GetCarColor(int colorId); 

#endif