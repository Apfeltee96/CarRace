#ifndef SHOP_H
#define SHOP_H

#include "raylib.h"
#include <string>
#include <vector>

struct SaveGame {
    int totalStars;
    bool ownsRedCar;
    int selectedColorId;
    std::string lastPlayerName;
    bool isEnglish;
};

// Funktionen für das Speichersystem
SaveGame LoadSaveGame();
void SaveGameData(const SaveGame& data);
void DeleteSaveData();

// Diese Funktion fehlte in der main.cpp
Color GetCarColor(int colorId);

#endif