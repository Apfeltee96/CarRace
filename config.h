#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"
#include <string>

constexpr float SPEED_START        = 400.0f;
constexpr float SPEED_MAX          = 1600.0f;
constexpr float SPEED_ACCELERATION = 2.5f;

constexpr int PRICE_RED_CAR  = 30;
constexpr int PRICE_BLUE_CAR = 75;

/// Persistente Spielerdaten (savegame.dat)
struct SaveGame
{
    int totalStars;
    bool ownsRedCar;
    bool ownsBlueCar;
    int selectedColorId;   // 0=weiß, 1=rot, 2=lila
    bool isEnglish;
    bool isFullscreen;
    std::string lastPlayerName;
};

SaveGame LoadSaveGame();
void SaveGameData(const SaveGame &data);
void DeleteSaveData();

/// Erhöht die Geschwindigkeit pro Frame bis SPEED_MAX.
float GetCurrentSpeed(float currentSpeed, float deltaTime);

/// Lenkgeschwindigkeit skaliert mit der Weltgeschwindigkeit (max. 1400).
float GetDynamicPlayerSpeed(float currentWorldSpeed);

/// Gibt die Farbe zum Auto-ID zurück (0=weiß, 1=rot, 2=lila).
Color GetCarColor(int colorId);

#endif
