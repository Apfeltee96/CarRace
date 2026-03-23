#include "player.h"

void InitPlayer(Player &player, int screenWidth, int screenHeight, Color carColor) {
    player.rect.width = 60;
    player.rect.height = 60; 
    player.speed = 850.0f; 
    player.color = carColor; // Malt das Auto in der gekauften Farbe an
    
    player.rect.x = (screenWidth / 2.0f) - (player.rect.width / 2.0f);
    player.rect.y = screenHeight - 150.0f;
}