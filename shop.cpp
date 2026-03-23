#include "shop.h"
#include <fstream>

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

int CalculateStars(int score) {
    int stars = 0;

    if (score < 1000) {
        // Phase 1: Normales Sammeln
        stars = score / 500; 
    } 
    else if (score < 3000) {
        // Phase 2: Wer die 1000 knackt, sammelt schneller!
        // Die ersten 1000 geben 2 Sterne + alles darüber gibt doppelt.
        stars = 2 + ((score - 1000) / 250); 
    } 
    else {
        // Phase 3: Profi-Bereich! Wer über 3000 kommt, wird mit Sternen überschüttet.
        // 10 Sterne (aus Phase 1+2) + alles darüber gibt massiv Punkte.
        stars = 10 + ((score - 3000) / 100); 
    }

    return stars;
}
Color GetCarColor(int colorId) {
    if (colorId == 1) return RED;
    return BLUE; 
}