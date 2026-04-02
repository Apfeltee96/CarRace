#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"
#include <string>

// ============================================================
//  Spielkonstanten
// ============================================================

constexpr float SPEED_START        = 400.0f;   // Startgeschwindigkeit der Hindernisse
constexpr float SPEED_MAX          = 1600.0f;  // Maximale Hindernisgeschwindigkeit
constexpr float SPEED_ACCELERATION = 2.5f;     // Beschleunigung pro Sekunde

constexpr int   PRICE_RED_CAR      = 30;       // Sternepreis für das rote Auto
constexpr int   PRICE_BLUE_CAR   = 75;      // Sternepreis für das lila Auto

// ============================================================
//  Speicherstand
// ============================================================

/// Enthält alle persistenten Spielerdaten, die in savegame.dat gespeichert werden.
struct SaveGame {
    int         totalStars;       // Gesammelte Sterne insgesamt
    bool        ownsRedCar;       // Rotes Auto freigeschaltet?
    bool        ownsBlueCar;    // Lila Auto freigeschaltet?
    int         selectedColorId;  // Aktiv ausgewähltes Auto (0=weiß, 1=rot, 2=lila)
    bool        isEnglish;        // Sprache: true=Englisch, false=Deutsch
    bool        isFullscreen;     // Vollbildmodus aktiv?
    std::string lastPlayerName;   // Zuletzt verwendeter Spielername
};

// ============================================================
//  Funktionsdeklarationen
// ============================================================

/// Lädt den Speicherstand aus savegame.dat.
/// Gibt Standardwerte zurück, wenn keine Datei existiert.
SaveGame LoadSaveGame();

/// Speichert den Spielstand in savegame.dat.
void SaveGameData(const SaveGame& data);

/// Löscht savegame.dat vollständig.
void DeleteSaveData();

/// Berechnet die neue Hindernisgeschwindigkeit basierend auf verstrichener Zeit.
/// @param currentSpeed  Aktuelle Geschwindigkeit dieses Frames
/// @param deltaTime     Zeit seit dem letzten Frame (Sekunden)
/// @return              Neue (ggf. erhöhte) Geschwindigkeit
float GetCurrentSpeed(float currentSpeed, float deltaTime);

/// Gibt die Lenkgeschwindigkeit des Spielers zurück, skaliert mit der Weltgeschwindigkeit.
float GetDynamicPlayerSpeed(float currentWorldSpeed);

/// Gibt die Farbe zurück, die zum Auto mit der gegebenen ID gehört.
Color GetCarColor(int colorId);

#endif // CONFIG_H
