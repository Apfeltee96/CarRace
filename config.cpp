#include "config.h"
#include <fstream>
#include <cstdio>

const char* SAVE_FILE = "savegame.dat";

SaveGame LoadSaveGame() {
    SaveGame data = { 0, false, false, 0, false, "Gast" };
    std::ifstream file(SAVE_FILE, std::ios::binary);
    if (file.is_open()) {
        file.read((char*)&data.totalStars, sizeof(int));
        file.read((char*)&data.ownsRedCar, sizeof(bool));
        file.read((char*)&data.ownsPurpleCar, sizeof(bool));
        file.read((char*)&data.selectedColorId, sizeof(int));
        
        int nameLen = 0;
        if (file.read((char*)&nameLen, sizeof(int))) {
            char* buffer = new char[nameLen + 1];
            file.read(buffer, nameLen);
            buffer[nameLen] = '\0';
            data.lastPlayerName = std::string(buffer);
            delete[] buffer;
        }
        file.read((char*)&data.isEnglish, sizeof(bool));
        file.close();
    }
    return data;
}

void SaveGameData(SaveGame data) {
    std::ofstream file(SAVE_FILE, std::ios::binary);
    if (file.is_open()) {
        file.write((char*)&data.totalStars, sizeof(int));
        file.write((char*)&data.ownsRedCar, sizeof(bool));
        file.write((char*)&data.ownsPurpleCar, sizeof(bool));
        file.write((char*)&data.selectedColorId, sizeof(int));
        
        int nameLen = (int)data.lastPlayerName.length();
        file.write((char*)&nameLen, sizeof(int));
        file.write(data.lastPlayerName.c_str(), nameLen);
        file.write((char*)&data.isEnglish, sizeof(bool));
        file.close();
    }
}

void DeleteSaveData() {
    std::remove(SAVE_FILE);
}

float GetCurrentSpeed(int score, float currentFrameSpeed, float deltaTime) {
    const float MAX_SPEED = 1600.0f; 
    const float ACCELERATION_RATE = 2.5f; 
    if (currentFrameSpeed < MAX_SPEED) {
        currentFrameSpeed += ACCELERATION_RATE * deltaTime;
    }
    return currentFrameSpeed;
}

float GetDynamicPlayerSpeed(float currentWorldSpeed) {
    float baseSteerSpeed = 500.0f;
    float dynamicSteer = baseSteerSpeed + (currentWorldSpeed * 0.5f);
    if (dynamicSteer > 1400.0f) dynamicSteer = 1400.0f;
    return dynamicSteer;
}

int CalculateStars(int score) {
    return score / 1000;
}

Color GetCarColor(int colorId) {
    if (colorId == 1) return RED;
    if (colorId == 2) return PURPLE;
    return BLUE;
}