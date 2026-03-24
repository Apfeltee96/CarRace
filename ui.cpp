#include "ui.h"
#include <string>

// --- HUD ZEICHNEN ---
void DrawHUD(const char* playerName, float totalTimeSurvived, int score, int earnedStars) {
    int sw = GetScreenWidth();
    DrawText(playerName, 20, 20, 25, LIGHTGRAY);
    DrawText(TextFormat("%.1fs", totalTimeSurvived), 20, 50, 22, WHITE);

    const char* scoreText = TextFormat("PUNKTE: %i", score);
    DrawText(scoreText, sw - MeasureText(scoreText, 30) - 20, 20, 30, GOLD);

    const char* starText = TextFormat("+ %i Sterne", earnedStars);
    DrawText(starText, sw - MeasureText(starText, 25) - 20, 55, 25, YELLOW);
}

// --- HAUPTMENÜ ---
void DrawMainMenu(const char* playerName, int letterCount, int framesCounter, Vector2 mousePoint, 
                  Rectangle inputBox, Rectangle startButton, Rectangle scoreBtn, 
                  Rectangle shopBtn, Rectangle settingsBtn, Rectangle descBtn, 
                  int totalStars, bool isNameSaved) 
{
    int sw = GetScreenWidth();
    DrawText("CAR RACE", sw/2 - MeasureText("CAR RACE", 60)/2, 60, 60, GOLD);
    DrawText(TextFormat("Gesamtsterne: %i", totalStars), sw/2 - MeasureText(TextFormat("Gesamtsterne: %i", totalStars), 20)/2, 130, 20, YELLOW);

    if (!isNameSaved) {
        DrawRectangleRec(inputBox, RAYWHITE);
        DrawRectangleLinesEx(inputBox, 2, CheckCollisionPointRec(mousePoint, inputBox) ? GOLD : GRAY);
        DrawText(playerName, (int)inputBox.x + 10, (int)inputBox.y + 15, 20, BLACK);
        if (letterCount < 15 && (framesCounter / 20) % 2 == 0) 
            DrawText("_", (int)inputBox.x + 15 + MeasureText(playerName, 20), (int)inputBox.y + 15, 20, BLACK);
    } else {
        const char* welcome = TextFormat("Willkommen, %s!", playerName);
        DrawText(welcome, sw/2 - MeasureText(welcome, 25)/2, (int)inputBox.y + 10, 25, LIGHTGRAY);
    }

    // --- START BUTTON ---
    bool hoverStart = CheckCollisionPointRec(mousePoint, startButton);
    DrawRectangleRec(startButton, hoverStart ? GREEN : DARKGREEN);
    DrawText("SPIEL STARTEN", (int)(startButton.x + startButton.width/2 - MeasureText("SPIEL STARTEN", 20)/2), (int)(startButton.y + 15), 20, WHITE);

    // --- SCOREBOARD BUTTON ---
    bool hoverScore = CheckCollisionPointRec(mousePoint, scoreBtn);
    DrawRectangleRec(scoreBtn, hoverScore ? GRAY : DARKGRAY);
    DrawText("SCOREBOARD", (int)(scoreBtn.x + scoreBtn.width/2 - MeasureText("SCOREBOARD", 20)/2), (int)(scoreBtn.y + 15), 20, WHITE);

    // --- SHOP BUTTON ---
    bool hoverShop = CheckCollisionPointRec(mousePoint, shopBtn);
    Color shopBase = {180, 100, 0, 255}; // Dunkelorange
    DrawRectangleRec(shopBtn, hoverShop ? ORANGE : shopBase);
    DrawText("SHOP", (int)(shopBtn.x + shopBtn.width/2 - MeasureText("SHOP", 20)/2), (int)(shopBtn.y + 15), 20, WHITE);

    // --- SETTINGS BUTTON ---
    bool hoverSett = CheckCollisionPointRec(mousePoint, settingsBtn);
    DrawRectangleRec(settingsBtn, hoverSett ? BLUE : DARKBLUE);
    DrawText("EINSTELLUNGEN", (int)(settingsBtn.x + settingsBtn.width/2 - MeasureText("EINSTELLUNGEN", 20)/2), (int)(settingsBtn.y + 15), 20, WHITE);

    // --- ANLEITUNG BUTTON ---
    bool hoverDesc = CheckCollisionPointRec(mousePoint, descBtn);
    DrawRectangleRec(descBtn, hoverDesc ? DARKGRAY : BLACK);
    DrawText("ANLEITUNG", (int)(descBtn.x + descBtn.width/2 - MeasureText("ANLEITUNG", 20)/2), (int)(descBtn.y + 15), 20, WHITE);
}

// --- EINSTELLUNGEN ---
void DrawSettingsMenu(Vector2 mousePoint, Rectangle deleteSaveBtn, Rectangle deleteScoreBtn, Rectangle backBtn) {
    int sw = GetScreenWidth();
    DrawRectangle(0, 0, sw, GetScreenHeight(), Fade(BLACK, 0.8f));
    
    DrawText("EINSTELLUNGEN", sw/2 - MeasureText("EINSTELLUNGEN", 40)/2, 100, 40, GOLD);

    // Hilfsfunktion für Buttons im Settings-Menü
    auto DrawSettingBtn = [&](Rectangle r, const char* t, Color c) {
        bool hover = CheckCollisionPointRec(mousePoint, r);
        DrawRectangleRec(r, hover ? ColorAlpha(c, 0.8f) : c);
        DrawText(t, (int)(r.x + r.width/2 - MeasureText(t, 20)/2), (int)(r.y + 15), 20, WHITE);
    };

    // 1. Spielstand löschen
    DrawText("Sterne & Autos:", sw/2 - 150, 220, 20, GRAY);
    DrawSettingBtn(deleteSaveBtn, "SPIELSTAND LOESCHEN", MAROON);

    // 2. Scoreboard löschen
    DrawText("Bestenliste:", sw/2 - 150, 340, 20, GRAY);
    DrawSettingBtn(deleteScoreBtn, "SCOREBOARD LEEREN", DARKBLUE);

    // 3. Zurück
    DrawSettingBtn(backBtn, "ZURUECK", DARKGRAY);
}

// --- WEITERE MENÜS (DESCRIPTION & SCOREBOARD) ---
void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backMenuBtn) {
    int sw = GetScreenWidth();
    DrawText("ANLEITUNG", sw/2 - MeasureText("ANLEITUNG", 40)/2, 150, 40, GOLD);
    DrawText("- Weiche den roten Hindernissen aus", sw/2 - 150, 250, 20, WHITE);
    DrawText("- Sammle Punkte für Sterne", sw/2 - 150, 280, 20, WHITE);
    DrawText("- Schalte neue Autos im Shop frei", sw/2 - 150, 310, 20, WHITE);

    bool hover = CheckCollisionPointRec(mousePoint, backMenuBtn);
    DrawRectangleRec(backMenuBtn, hover ? GRAY : DARKGRAY);
    DrawText("ZURUECK", (int)backMenuBtn.x + 75, (int)backMenuBtn.y + 15, 20, WHITE);
}

void DrawScoreboardMenu(const std::vector<ScoreEntry>& scores, Vector2 mousePoint, Rectangle backMenuBtn) {
    int sw = GetScreenWidth();
    DrawText("TOP SCORES", sw/2 - MeasureText("TOP SCORES", 40)/2, 100, 40, GOLD);

    for (int i = 0; i < (int)scores.size(); i++) {
        const char* entry = TextFormat("%i. %s - %i Pkt", i + 1, scores[i].name.c_str(), scores[i].score);
        DrawText(entry, sw/2 - 150, 180 + (i * 35), 22, LIGHTGRAY);
    }

    bool hover = CheckCollisionPointRec(mousePoint, backMenuBtn);
    DrawRectangleRec(backMenuBtn, hover ? GRAY : DARKGRAY);
    DrawText("ZURUECK", (int)backMenuBtn.x + 75, (int)backMenuBtn.y + 15, 20, WHITE);
}

// --- IN-GAME MENÜS ---
void DrawPauseMenu(Vector2 mousePoint, Rectangle btnPrimary, Rectangle btnMenu) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    DrawText("PAUSE", GetScreenWidth()/2 - MeasureText("PAUSE", 40)/2, 250, 40, WHITE);

    DrawRectangleRec(btnPrimary, CheckCollisionPointRec(mousePoint, btnPrimary) ? GREEN : DARKGREEN);
    DrawText("WEITER", (int)btnPrimary.x + 75, (int)btnPrimary.y + 15, 20, WHITE);

    DrawRectangleRec(btnMenu, CheckCollisionPointRec(mousePoint, btnMenu) ? GRAY : DARKGRAY);
    DrawText("HAUPTMENUE", (int)btnMenu.x + 55, (int)btnMenu.y + 15, 20, WHITE);
}

void DrawGameOverMenu(const char* playerName, int score, float totalTimeSurvived, int earnedStars, Vector2 mousePoint, Rectangle btnMenu) {
    int sw = GetScreenWidth();
    DrawText("GAME OVER", sw/2 - MeasureText("GAME OVER", 50)/2, 200, 50, RED);
    DrawText(TextFormat("Score: %i", score), sw/2 - 60, 280, 25, WHITE);
    DrawText(TextFormat("Sterne verdient: %i", earnedStars), sw/2 - 100, 320, 25, YELLOW);

    DrawRectangleRec(btnMenu, CheckCollisionPointRec(mousePoint, btnMenu) ? GRAY : DARKGRAY);
    DrawText("HAUPTMENUE", (int)btnMenu.x + 55, (int)btnMenu.y + 15, 20, WHITE);
}

void DrawSpecialMessage(const char* message) {
    int sw = GetScreenWidth();
    DrawRectangle(sw/2 - 260, 340, 520, 80, Fade(MAROON, 0.8f));
    DrawRectangleLines(sw/2 - 260, 340, 520, 80, GOLD);
    DrawText(message, sw/2 - MeasureText(message, 22)/2, 370, 22, WHITE);
}

void DrawShopMenu(const SaveGame& saveData, Vector2 mousePoint, Rectangle btnBlue, Rectangle btnRed, Rectangle backMenuBtn) {
    int sw = GetScreenWidth();
    DrawText("SHOP", sw/2 - MeasureText("SHOP", 40)/2, 100, 40, GOLD);
    DrawText(TextFormat("Deine Sterne: %i", saveData.totalStars), sw/2 - 80, 160, 20, YELLOW);

    // Blaues Auto (Start)
    DrawRectangleRec(btnBlue, CheckCollisionPointRec(mousePoint, btnBlue) ? SKYBLUE : BLUE);
    DrawText("BLAU (Standard)", (int)btnBlue.x + 25, (int)btnBlue.y + 15, 18, WHITE);
    if (saveData.selectedColorId == 0) DrawText("AKTIV", (int)btnBlue.x + 210, (int)btnBlue.y + 15, 20, GREEN);

    // Rotes Auto
    DrawRectangleRec(btnRed, CheckCollisionPointRec(mousePoint, btnRed) ? RED : MAROON);
    const char* redText = saveData.ownsRedCar ? "ROT (Besitzt)" : "ROT (15 Sterne)";
    DrawText(redText, (int)btnRed.x + 25, (int)btnRed.y + 15, 18, WHITE);
    if (saveData.selectedColorId == 1) DrawText("AKTIV", (int)btnRed.x + 210, (int)btnRed.y + 15, 20, GREEN);

    DrawRectangleRec(backMenuBtn, CheckCollisionPointRec(mousePoint, backMenuBtn) ? GRAY : DARKGRAY);
    DrawText("ZURUECK", (int)backMenuBtn.x + 75, (int)backMenuBtn.y + 15, 20, WHITE);
}