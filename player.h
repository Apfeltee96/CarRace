#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

// Hier lagern wir den Bauplan für den Spieler aus
struct Player {
    Rectangle rect;
    float speed;
    Color color;
};

// Diese Funktion bereitet das Auto für den Start vor
void InitPlayer(Player &player, int screenWidth, int screenHeight);

#endif