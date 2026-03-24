#include "shop.h"
#include "config.h"
#include <fstream>
#include <cstdio> // Für remove()

const char* SAVE_FILE = "savegame.txt";

SaveGame LoadSaveGame() {
    SaveGame data = {0, false, 0, ""}; 
    std::ifstream file(SAVE_FILE);
    if (file.is_open()) {
        file >> data.totalStars >> data.ownsRedCar >> data.selectedColorId >> data.lastPlayerName;
        file.close();
    }
    return data;
}

void SaveGameData(const SaveGame& data) {
    std::ofstream file(SAVE_FILE);
    if (file.is_open()) {
        // Falls kein Name eingegeben wurde, speichern wir einen Platzhalter
        std::string nameToSave = data.lastPlayerName.empty() ? "Gast" : data.lastPlayerName;
        file << data.totalStars << " " << data.ownsRedCar << " " << data.selectedColorId << " " << nameToSave;
        file.close();
    }
}




void DeleteSaveData() {
    remove(SAVE_FILE); // Löscht die savegame.txt physisch von der Festplatte
}


Color GetCarColor(int colorId) {
    if (colorId == 1) return RED;
    return BLUE; 
}