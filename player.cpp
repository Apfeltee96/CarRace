#include "player.h"

void InitPlayer(Player &player, int screenWidth, int screenHeight, float width, float height, Color color) {
    player.rect.width = width;
    player.rect.height = height;
    player.rect.x = screenWidth / 2.0f - player.rect.width / 2.0f;
    player.rect.y = screenHeight - player.rect.height - 30.0f;
    player.color = color;
}