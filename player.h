#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

struct Player
{
    Rectangle rect;
    float speed;
    Color color;
};

/// Initialisiert den Spieler mittig am unteren Bildschirmrand.
void InitPlayer(Player &player, int screenWidth, int screenHeight,
                float width, float height, Color color);

#endif // PLAYER_H
