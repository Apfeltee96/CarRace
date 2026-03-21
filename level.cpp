#include "level.h"

LevelData GetLevelData(int level) {
    LevelData data;
    data.levelNumber = level;

    // Hier kannst du jedes Level ganz individuell designen (Hardcoden)!
    switch (level) {
        case 1:
            data.targetTime = 60.0f;     // 60 Sekunden überleben
            data.baseSpeed = 200.0f;     // Gemütlicher Start
            data.speedMultiplier = 3.0f; // Normaler Geschwindigkeits-Anstieg
            break;
            
        case 2:
            data.targetTime = 60.0f;
            data.baseSpeed = 250.0f;     // Startet direkt schneller
            data.speedMultiplier = 4.0f; // Wird schneller schwer
            break;
            
        case 3:
            data.targetTime = 45.0f;     // Kürzeres Level (nur 45 Sekunden)...
            data.baseSpeed = 350.0f;     // ...aber dafür von Anfang an extrem schnell!
            data.speedMultiplier = 5.0f;
            break;
            
        default:
            // Dynamischer Algorithmus für Level 4, 5, 6, 7... (Endlos)
            // Falls du kein spezielles Design gemacht hast, berechnet das Spiel die Werte einfach selbst
            data.targetTime = 60.0f;
            data.baseSpeed = 200.0f + (level * 30.0f);
            data.speedMultiplier = 3.0f + (level * 0.5f);
            break;
    }

    return data;
}