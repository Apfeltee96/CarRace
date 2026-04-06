#include "player.h"
#include "config.h"
#include <algorithm>

void Player::Init(int screenWidth, int screenHeight, float width, float height)
{
    rect.width = width;
    rect.height = height;
    // Horizontal zentriert
    rect.x = screenWidth / 2.0f - width / 2.0f;
    rect.y = screenHeight - height - 30.0f;
    speed = GetDynamicPlayerSpeed(SPEED_START);
}

void Player::Move(float deltaTime)
{
    if (IsKeyDown(KEY_LEFT))
        rect.x -= speed * deltaTime;
    if (IsKeyDown(KEY_RIGHT))
        rect.x += speed * deltaTime;
    rect.x = std::clamp(rect.x, 300.0f, 700.0f - rect.width);
}

void Player::Draw(Texture2D &tex, bool shieldActive) const
{
    DrawTexturePro(tex,
                   {0, 0, (float)tex.width, (float)tex.height},
                   rect, {0, 0}, 0.0f, WHITE);

    if (shieldActive)
        DrawRectangleLinesEx(
            {rect.x - 4, rect.y - 4, rect.width + 8, rect.height + 8},
            4.0f, GOLD);
}
