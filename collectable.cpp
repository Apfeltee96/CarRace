#include "collectable.h"

void Collectable::Spawn(float minX, float maxX, float size)
{
    rect = {(float)GetRandomValue((int)minX, (int)maxX), -100.0f, size, size};
    active = true;
}

void Collectable::UpdatePosition(float worldSpeed, float deltaTime)
{
    if (!active)
        return;
    rect.y += worldSpeed * deltaTime;
    if (rect.y > 800.0f)
        active = false;
}
