#include "ui.h"
#include <string>

void DrawTextCentered(const char* text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, (1000 - textWidth) / 2, y, fontSize, color);
}

void DrawButton(Rectangle rect, const char* label, Vector2 mousePoint, Color baseCol, Color textCol = WHITE) {
    bool hover = CheckCollisionPointRec(mousePoint, rect);
    DrawRectangleRec(rect, hover ? LIGHTGRAY : baseCol);
    int tw = MeasureText(label, 20);
    DrawText(label, (int)(rect.x + (rect.width - tw) / 2), (int)(rect.y + 15), 20, hover ? BLACK : textCol);
}

void DrawHUD(const char* name, float time, int score, int stars, bool isEnglish) {
    DrawText(TextFormat(isEnglish ? "Driver: %s" : "Fahrer: %s", name), 20, 20, 20, RAYWHITE);
    DrawText(TextFormat(isEnglish ? "Score: %d" : "Punkte: %d", score), 20, 50, 20, RAYWHITE);
    DrawText(TextFormat(isEnglish ? "Time: %.2fs" : "Zeit: %.2fs", time), 20, 80, 20, RAYWHITE);
    DrawText(TextFormat(isEnglish ? "Stars: %d" : "Sterne: %d", stars), 20, 110, 20, GOLD);
}

void DrawMainMenu(const char* name, int letterCount, int framesCounter, Vector2 mousePoint, Rectangle startBtn, Rectangle scoreBtn, Rectangle shopBtn, Rectangle settingsBtn, Rectangle descBtn, int totalStars, bool nameSaved, bool isEnglish) {
    // ClearBackground entfernt
    DrawTextCentered("CAR RACE", 60, 60, GOLD);
    DrawText(TextFormat(isEnglish ? "Total Stars: %d" : "Gesamt Sterne: %d", totalStars), 20, 20, 20, GOLD);

    if (!nameSaved) {
        DrawTextCentered(isEnglish ? "ENTER YOUR NAME:" : "NAME EINGEBEN:", 180, 25, LIGHTGRAY);
        DrawRectangle(350, 220, 300, 40, RAYWHITE);
        DrawText(name, 360, 230, 20, BLACK);
        if (letterCount < 15 && (framesCounter / 20) % 2 == 0) 
            DrawText("_", 360 + MeasureText(name, 20), 230, 20, DARKGRAY);
    } else {
        DrawTextCentered(TextFormat(isEnglish ? "Welcome, %s!" : "Willkommen, %s!", name), 220, 30, RAYWHITE);
    }

    DrawButton(startBtn, isEnglish ? "START GAME" : "SPIEL STARTEN", mousePoint, DARKGREEN);
    DrawButton(scoreBtn, isEnglish ? "SCOREBOARD" : "BESTENLISTE", mousePoint, DARKBLUE);
    DrawButton(shopBtn, isEnglish ? "SHOP" : "LADEN", mousePoint, PURPLE);
    DrawButton(settingsBtn, isEnglish ? "SETTINGS" : "EINSTELLUNGEN", mousePoint, GRAY);
    DrawButton(descBtn, isEnglish ? "HOW TO PLAY" : "ANLEITUNG", mousePoint, DARKGRAY);
}

void DrawSettingsMenu(Vector2 mousePoint, Rectangle langBtn, Rectangle resBtn, Rectangle nameChangeBtn, Rectangle deleteDataBtn, Rectangle backBtn, bool isEnglish, bool isFullscreen) {
    // ClearBackground entfernt
    DrawTextCentered(isEnglish ? "SETTINGS" : "EINSTELLUNGEN", 80, 40, GOLD);
    DrawButton(langBtn, isEnglish ? "Language: EN" : "Sprache: DE", mousePoint, DARKGRAY);
    const char* resText = isFullscreen ? (isEnglish ? "Windowed" : "Fenstermodus") : (isEnglish ? "Fullscreen" : "Vollbildmodus");
    DrawButton(resBtn, resText, mousePoint, DARKGRAY);
    DrawButton(nameChangeBtn, isEnglish ? "Change Name" : "Name aendern", mousePoint, DARKGRAY);
    DrawButton(deleteDataBtn, isEnglish ? "Delete All Data" : "Daten loeschen", mousePoint, MAROON);
    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

void DrawScoreboardMenu(std::vector<ScoreEntry> scores, Vector2 mousePoint, Rectangle backBtn, bool isEnglish) {
    // Falls du den Hintergrund hier abdunkeln willst, mach das hier
    DrawTextCentered(isEnglish ? "TOP SCORES" : "BESTENLISTE", 50, 40, GOLD);

    for (int i = 0; i < (int)scores.size() && i < 10; i++) {
        Color c = (i == 0) ? GOLD : (i == 1) ? LIGHTGRAY : (i == 2) ? BROWN : WHITE;
        
        // WICHTIG: scores[i].name ist jetzt ein char-Array und kann direkt gezeichnet werden
        DrawText(TextFormat("%d. %s", i + 1, scores[i].name), 300, 150 + i * 40, 25, c);
        DrawText(TextFormat("%d", scores[i].score), 650, 150 + i * 40, 25, c);
    }
    
    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backBtn, bool isEnglish) {
    DrawTextCentered(isEnglish ? "HOW TO PLAY" : "ANLEITUNG", 100, 40, GOLD);
    const char* lines[] = {
        isEnglish ? "Use LEFT and RIGHT arrow keys to move." : "Nutze LINKS und RECHTS Pfeiltasten zum Lenken.",
        isEnglish ? "Avoid red obstacles!" : "Weiche den roten Hindernissen aus!",
        isEnglish ? "Collect yellow stars to buy new cars." : "Sammle gelbe Sterne fuer neue Autos.",
        isEnglish ? "The game gets faster over time!" : "Das Spiel wird mit der Zeit schneller!"
    };
    for (int i = 0; i < 4; i++) DrawTextCentered(lines[i], 250 + (i * 50), 20, RAYWHITE);
    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

void DrawPauseMenu(Vector2 mousePoint, Rectangle resumeBtn, Rectangle menuBtn, bool isEnglish) {
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.5f));
    DrawTextCentered(isEnglish ? "PAUSED" : "PAUSE", 250, 50, WHITE);
    DrawButton(resumeBtn, isEnglish ? "RESUME" : "WEITER", mousePoint, GRAY, BLACK);
    DrawButton(menuBtn, isEnglish ? "MENU" : "MENUE", mousePoint, GRAY, BLACK);
}

void DrawGameOverMenu(const char* name, int score, float time, int stars, Vector2 mousePoint, Rectangle menuBtn, bool isEnglish) {
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.8f));
    DrawTextCentered(isEnglish ? "GAME OVER" : "SPIEL VORBEI", 150, 60, RED);
    DrawTextCentered(TextFormat(isEnglish ? "Driver: %s" : "Fahrer: %s", name), 250, 25, WHITE);
    DrawTextCentered(TextFormat(isEnglish ? "Final Score: %d" : "Endstand: %d", score), 300, 30, GOLD);
    DrawTextCentered(TextFormat(isEnglish ? "Time Survived: %.2fs" : "Zeit: %.2fs", time), 350, 20, LIGHTGRAY);
    DrawTextCentered(TextFormat(isEnglish ? "Stars Collected: %d" : "Sterne gesammelt: %d", stars), 390, 20, GOLD);
    DrawButton(menuBtn, isEnglish ? "BACK TO MENU" : "HAUPTMENUE", mousePoint, GRAY, BLACK);
}

void DrawShopMenu(SaveGame data, Vector2 mousePoint, Rectangle redBtn, Rectangle whiteBtn, Rectangle backBtn, bool isEnglish, Texture2D carTexs[]) {
    DrawTextCentered(isEnglish ? "SHOP" : "LADEN", 60, 40, GOLD);
    DrawText(TextFormat(isEnglish ? "Stars: %d" : "Sterne: %d", data.totalStars), 20, 20, 25, GOLD);

    auto DrawShopItem = [&](int id, Rectangle btn, const char* labelTitle, int price, bool owned) {
        float scale = 0.5f;
        float texW = carTexs[id].width * scale;
        float texH = carTexs[id].height * scale;
        DrawTexturePro(carTexs[id], { 0, 0, (float)carTexs[id].width, (float)carTexs[id].height },
            { btn.x + (btn.width/2) - (texW/2), btn.y - texH - 20, texW, texH }, { 0, 0 }, 0.0f, WHITE);

        int tw = MeasureText(labelTitle, 20);
        DrawText(labelTitle, (int)(btn.x + (btn.width - tw) / 2), (int)(btn.y - texH - 50), 20, RAYWHITE);

        Color btnCol = owned ? DARKGREEN : (data.totalStars >= price ? DARKBLUE : MAROON);
        if (data.selectedColorId == id) btnCol = GOLD;
        const char* btnLabel = (data.selectedColorId == id) ? (isEnglish ? "SELECTED" : "GEWAEHLT") : 
                               (owned ? (isEnglish ? "EQUIP" : "NUTZEN") : TextFormat("%d Stars", price));
        DrawButton(btn, btnLabel, mousePoint, btnCol);
    };

    DrawShopItem(0, {150, 500, 200, 50}, isEnglish ? "START" : "START", 0, true);
    DrawShopItem(1, {400, 500, 200, 50}, isEnglish ? "FLITZER" : "FLITZER", 100, data.ownsRedCar);
    DrawShopItem(2, {650, 500, 200, 50}, isEnglish ? "PROFI" : "PROFI", 200, data.ownsPurpleCar);

    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}