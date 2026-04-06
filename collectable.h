#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "raylib.h"

enum class CollectableType
{
    Star,
    Clock,
    Shield
};

// Ein Sammelobjekt
struct Collectable
{
    Rectangle rect{};
    bool active = false;
    CollectableType type;

    // Spawnt das Objekt an zufälligem X
    void Spawn(float minX, float maxX, float size);

    // Bewegt das Objekt nach unten
    void UpdatePosition(float worldSpeed, float deltaTime);
};

#endif
