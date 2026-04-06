#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"
#include <string>

constexpr int SCREEN_W = 1000;
constexpr int SCREEN_H = 800;
constexpr int SAVE_VERSION = 1;

constexpr float SPEED_START = 400.0f;
constexpr float SPEED_MAX = 1600.0f;
constexpr float SPEED_ACCELERATION = 2.5f;

constexpr int PRICE_RED_CAR = 30;
constexpr int PRICE_BLUE_CAR = 75;

// Spielerdaten (savegame.dat)
struct SaveGame
{
    int totalStars = 0;
    bool ownsRedCar = false;
    bool ownsBlueCar = false;
    int selectedColorId = 0;
    bool isEnglish = false;
    bool isFullscreen = false;
    std::string lastPlayerName = "Gast";
    float musicVolume = 0.2f;
    bool effectsEnabled = true;
};

SaveGame LoadSaveGame();
void SaveGameData(const SaveGame &data);
void DeleteSaveData();

// Erhöht die Geschwindigkeit pro Frame bis SPEED_MAX.
float GetCurrentSpeed(float currentSpeed, float deltaTime);

// Lenkgeschwindigkeit skaliert mit der Weltgeschwindigkeit (max. 1400).
float GetDynamicPlayerSpeed(float currentWorldSpeed);

// Gibt die Farbe zum Auto-ID zurück (0=weiß, 1=rot, 2=blau).
Color GetCarColor(int colorId);

#endif
