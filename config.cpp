
#include "config.h"
#include <fstream>
#include <filesystem>
#include <algorithm>

static const char* SAVE_FILE = "savegame.dat";

// ============================================================
//  Speicherstand laden / speichern
// ============================================================

SaveGame LoadSaveGame() {
    // Standardwerte falls keine Datei existiert
    SaveGame data = { 0, false, false, 0, false, false, "Gast" };

    std::ifstream file(SAVE_FILE, std::ios::binary);
    if (!file.is_open()) return data;

    // Reihenfolge muss exakt mit SaveGameData übereinstimmen
    file.read(reinterpret_cast<char*>(&data.totalStars),      sizeof(int));
    file.read(reinterpret_cast<char*>(&data.ownsRedCar),      sizeof(bool));
    file.read(reinterpret_cast<char*>(&data.ownsBlueCar),   sizeof(bool));
    file.read(reinterpret_cast<char*>(&data.selectedColorId), sizeof(int));

    // String wird als Länge + Inhalt gespeichert
    int nameLen = 0;
    if (file.read(reinterpret_cast<char*>(&nameLen), sizeof(int)) && nameLen > 0) {
        std::string name(nameLen, '\0');
        file.read(name.data(), nameLen);
        data.lastPlayerName = std::move(name);
    }

    file.read(reinterpret_cast<char*>(&data.isEnglish),    sizeof(bool));
    file.read(reinterpret_cast<char*>(&data.isFullscreen), sizeof(bool));

    return data;
}

void SaveGameData(const SaveGame& data) {
    std::ofstream file(SAVE_FILE, std::ios::binary);
    if (!file.is_open()) return;

    file.write(reinterpret_cast<const char*>(&data.totalStars),      sizeof(int));
    file.write(reinterpret_cast<const char*>(&data.ownsRedCar),      sizeof(bool));
    file.write(reinterpret_cast<const char*>(&data.ownsBlueCar),   sizeof(bool));
    file.write(reinterpret_cast<const char*>(&data.selectedColorId), sizeof(int));

    int nameLen = static_cast<int>(data.lastPlayerName.length());
    file.write(reinterpret_cast<const char*>(&nameLen), sizeof(int));
    file.write(data.lastPlayerName.c_str(), nameLen);

    file.write(reinterpret_cast<const char*>(&data.isEnglish),    sizeof(bool));
    file.write(reinterpret_cast<const char*>(&data.isFullscreen), sizeof(bool));
}

void DeleteSaveData() {
    std::filesystem::remove(SAVE_FILE);
}

// ============================================================
//  Spielmechanik-Hilfsfunktionen
// ============================================================

float GetCurrentSpeed(float currentSpeed, float deltaTime) {
    // Geschwindigkeit steigt linear bis SPEED_MAX
    return std::min(currentSpeed + SPEED_ACCELERATION * deltaTime, SPEED_MAX);
}

float GetDynamicPlayerSpeed(float currentWorldSpeed) {
    // Lenkgeschwindigkeit wächst mit der Weltgeschwindigkeit, aber maximal 1400
    constexpr float BASE_STEER   = 500.0f;
    constexpr float STEER_FACTOR = 0.5f;
    constexpr float STEER_MAX    = 1400.0f;
    return std::min(BASE_STEER + currentWorldSpeed * STEER_FACTOR, STEER_MAX);
}

Color GetCarColor(int colorId) {
    switch (colorId) {
        case 1:  return RED;
        case 2:  return BLUE;
        default: return WHITE;  // colorId 0 = weißes Auto
    }
}
