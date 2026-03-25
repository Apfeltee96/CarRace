#include "shop.h"
#include <fstream>

const char* SAVE_FILE = "savegame.dat";

SaveGame LoadSaveGame() {
    SaveGame data = { 0, false, 0, "Gast", false };
    std::ifstream file(SAVE_FILE, std::ios::binary);
    if (file.is_open()) {
        file.read((char*)&data.totalStars, sizeof(int));
        file.read((char*)&data.ownsRedCar, sizeof(bool));
        file.read((char*)&data.selectedColorId, sizeof(int));
        
        int nameLen = 0;
        file.read((char*)&nameLen, sizeof(int));
        char* buffer = new char[nameLen + 1];
        file.read(buffer, nameLen);
        buffer[nameLen] = '\0';
        data.lastPlayerName = buffer;
        delete[] buffer;

        file.read((char*)&data.isEnglish, sizeof(bool)); // Sprache laden
        file.close();
    }
    return data;
}

void SaveGameData(const SaveGame& data) {
    std::ofstream file(SAVE_FILE, std::ios::binary);
    if (file.is_open()) {
        file.write((char*)&data.totalStars, sizeof(int));
        file.write((char*)&data.ownsRedCar, sizeof(bool));
        file.write((char*)&data.selectedColorId, sizeof(int));
        
        int nameLen = (int)data.lastPlayerName.length();
        file.write((char*)&nameLen, sizeof(int));
        file.write(data.lastPlayerName.c_str(), nameLen);

        file.write((char*)&data.isEnglish, sizeof(bool)); // Sprache speichern
        file.close();
    }
}

void DeleteSaveData() {
    remove(SAVE_FILE);
}

Color GetCarColor(int colorId) {
    if (colorId == 1) return RED;
    return BLUE;
}