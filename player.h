#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

// ============================================================
//  Spieler-Struktur
// ============================================================

/// Repräsentiert das Spielerauto mit Position, Größe und Geschwindigkeit.
struct Player {
    Rectangle rect;   // Position und Größe (x, y, width, height)
    float     speed;  // Aktuelle Lenkgeschwindigkeit (Pixel/Sekunde)
    Color     color;  // Farbe (wird für Tinting der Textur verwendet)
};

// ============================================================
//  Funktionsdeklarationen
// ============================================================

/// Initialisiert den Spieler mittig am unteren Bildschirmrand.
/// @param player       Zu initialisierender Spieler
/// @param screenWidth  Breite des Spielfeldes
/// @param screenHeight Höhe des Spielfeldes
/// @param width        Breite des Autos in Pixeln
/// @param height       Höhe des Autos in Pixeln
/// @param color        Farbe / Tint des Autos
void InitPlayer(Player& player, int screenWidth, int screenHeight,
                float width, float height, Color color);

#endif // PLAYER_H
