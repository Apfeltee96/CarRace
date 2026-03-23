#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"

struct Player {
    Rectangle rect;
    float speed;
    Color color;
};

// NEU: Wir übergeben jetzt die Farbe, die im Shop ausgewählt wurde!
void InitPlayer(Player &player, int screenWidth, int screenHeight, Color carColor);

#endif