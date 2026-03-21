#include "level.h"

LevelData GetLevelData(int level) {
    LevelData data;
    data.levelNumber = level;

    switch (level) {
        case 1:
            data.targetTime = 20.0f;     // 20 Sekunden
            data.baseSpeed = 180.0f;     // Sehr langsamer, entspannter Start
            data.speedMultiplier = 1.5f; // Steigt nur ganz leicht an
            break;
            
        case 2:
            data.targetTime = 30.0f;     // 30 Sekunden
            data.baseSpeed = 230.0f;     // Etwas flotter
            data.speedMultiplier = 2.5f; // Spürbarer Anstieg
            break;
            
        case 3:
            data.targetTime = 40.0f;     // 40 Sekunden
            data.baseSpeed = 280.0f;     // Zügig
            data.speedMultiplier = 3.5f; // Fordernd
            break;
            
        case 4:
            data.targetTime = 50.0f;     // 50 Sekunden
            data.baseSpeed = 330.0f;     // Schnell
            data.speedMultiplier = 4.5f; // Schwer auszuweichen
            break;

        case 5:
            data.targetTime = 60.0f;     // 1 Minute (Das Finale)
            data.baseSpeed = 380.0f;     // Sehr schnell von Anfang an
            data.speedMultiplier = 5.5f; // Hardcore-Modus
            break;
            
        default:
            // Endlos-Modus: Falls jemand über Level 5 hinauskommt
            data.targetTime = 60.0f;
            data.baseSpeed = 380.0f + ((level - 5) * 30.0f);
            data.speedMultiplier = 5.5f + ((level - 5) * 0.5f);
            break;
    }

    return data;
}