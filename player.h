#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"

struct Player {
    Rectangle rect;
    float speed;
    Color color;
};

// In player.h
void InitPlayer(Player &player, int screenWidth, int screenHeight, float width, float height, Color color);
void InitPlayerEx(Player &player, int screenWidth, int screenHeight, float width, float height, Color color); // Die neue

#endif