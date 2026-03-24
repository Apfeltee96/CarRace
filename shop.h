#ifndef SHOP_H
#define SHOP_H
#include "raylib.h"
#include <string> // Wichtig für den Namen!

struct SaveGame {
    int totalStars;
    bool ownsRedCar;
    int selectedColorId;
    std::string lastPlayerName; // NEU: Merkt sich den letzten Spieler!
};

SaveGame LoadSaveGame();
void SaveGameData(const SaveGame& data);
Color GetCarColor(int colorId);
void DeleteSaveData();

#endif