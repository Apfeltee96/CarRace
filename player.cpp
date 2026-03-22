#include "player.h"

void InitPlayer(Player &player, int screenWidth, int screenHeight) {
    player.rect.width = 60;
    player.rect.height = 60; 
    player.speed = 850.0f; 
    player.color = BLUE; // Das tauschen wir später gegen ein Bild aus!
    
    // Setzt das Auto sauber auf die Startposition
    player.rect.x = (screenWidth / 2.0f) - (player.rect.width / 2.0f);
    player.rect.y = screenHeight - 150.0f;
}