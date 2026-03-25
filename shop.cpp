#include "shop.h"

void DrawShopMenu(SaveGame &saveData, Vector2 mousePoint, Rectangle redBtn, Rectangle purpleBtn, Rectangle backBtn, bool isEnglish) {
    ClearBackground(BLACK);
    
    DrawText(isEnglish ? "SHOP" : "LADEN", 400, 50, 40, GOLD);
    DrawText(TextFormat(isEnglish ? "Stars: %d" : "Sterne: %d", saveData.totalStars), 400, 100, 20, WHITE);

    // Blau (Standard)
    Rectangle blueBtn = { 400, 200, 200, 50 };
    DrawRectangleRec(blueBtn, saveData.selectedColorId == 0 ? DARKBLUE : BLUE);
    DrawText("Standard (Blue)", 410, 215, 20, WHITE);

    // Rot (100)
    DrawRectangleRec(redBtn, saveData.selectedColorId == 1 ? MAROON : RED);
    DrawText(saveData.ownsRedCar ? "Select Red" : "Buy Red (100)", 410, 315, 20, WHITE);

    // Lila (200)
    DrawRectangleRec(purpleBtn, saveData.selectedColorId == 2 ? DARKPURPLE : PURPLE);
    DrawText(saveData.ownsPurpleCar ? "Select Purple" : "Buy Purple (200)", 410, 415, 20, WHITE);

    // Zurück
    DrawRectangleRec(backBtn, GRAY);
    DrawText(isEnglish ? "BACK" : "ZURUECK", (int)backBtn.x + 50, (int)backBtn.y + 15, 20, BLACK);
}