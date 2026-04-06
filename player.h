#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

// Spieler mit Auto
class Player
{
public:
    Rectangle rect{};
    float speed = 0.0f;

    // Spieler mittig
    void Init(int screenWidth, int screenHeight, float width, float height);

    // Tastatureingaben
    void Move(float deltaTime);

    // Zeichnet das Auto + Schildrahmen
    void Draw(Texture2D &tex, bool shieldActive) const;
};

#endif
