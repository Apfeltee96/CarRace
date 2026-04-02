#include "player.h"
#include "config.h"

void InitPlayer(Player& player, int screenWidth, int screenHeight,
                float width, float height, Color color) {
    player.rect.width  = width;
    player.rect.height = height;
    // Horizontal zentriert, 30px über dem unteren Rand
    player.rect.x = screenWidth  / 2.0f - width  / 2.0f;
    player.rect.y = screenHeight - height - 30.0f;
    player.color  = color;
    player.speed  = GetDynamicPlayerSpeed(SPEED_START); // Sicherer Startwert
}
