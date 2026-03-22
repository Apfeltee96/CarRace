#include "level.h"

LevelData GetLevelData(int level) {
    LevelData data;
    data.levelNumber = level;

    // BUGFIX: Die Geschwindigkeiten knüpfen jetzt nahtlos aneinander an!
    switch (level) {
        case 1:
            data.targetTime = 20.0f;     
            data.baseSpeed = 180.0f;     // Endet bei: 180 + (20 * 1.5) = 210
            data.speedMultiplier = 1.5f; 
            break;
            
        case 2:
            data.targetTime = 30.0f;     
            data.baseSpeed = 210.0f;     // Startet exakt bei 210! Endet bei: 210 + (30 * 2.5) = 285
            data.speedMultiplier = 2.5f; 
            break;
            
        case 3:
            data.targetTime = 40.0f;     
            data.baseSpeed = 285.0f;     // Startet exakt bei 285! Endet bei: 285 + (40 * 3.5) = 425
            data.speedMultiplier = 3.5f; 
            break;
            
        case 4:
            data.targetTime = 50.0f;     
            data.baseSpeed = 425.0f;     // Startet exakt bei 425! Endet bei: 425 + (50 * 4.5) = 650
            data.speedMultiplier = 4.5f; 
            break;

        case 5:
            data.targetTime = 60.0f;     
            data.baseSpeed = 650.0f;     // Startet exakt bei 650! Endet bei: 650 + (60 * 5.5) = 980
            data.speedMultiplier = 5.5f; 
            break;
            
        default:
            data.targetTime = 60.0f;
            data.baseSpeed = 980.0f + ((level - 5) * 50.0f);
            data.speedMultiplier = 5.5f + ((level - 5) * 0.5f);
            break;
    }

    return data;
}