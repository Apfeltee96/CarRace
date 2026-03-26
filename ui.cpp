#include "ui.h"
#include <string>

// Hilfsfunktion für zentrierten Text
void DrawTextCentered(const char* text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, (GetScreenWidth() - textWidth) / 2, y, fontSize, color);
}

void DrawHUD(const char* name, float time, int score, int stars, bool isEnglish) {
    DrawText(TextFormat(isEnglish ? "Driver: %s" : "Fahrer: %s", name), 20, 20, 20, RAYWHITE);
    DrawText(TextFormat(isEnglish ? "Score: %d" : "Punkte: %d", score), 20, 50, 20, RAYWHITE);
    DrawText(TextFormat(isEnglish ? "Time: %.2fs" : "Zeit: %.2fs", time), 20, 80, 20, RAYWHITE);
    DrawText(TextFormat(isEnglish ? "Stars: %d" : "Sterne: %d", stars), 20, 110, 20, GOLD);
}

void DrawMainMenu(const char* name, int letterCount, int framesCounter, Vector2 mousePoint, Rectangle startBtn, Rectangle scoreBtn, Rectangle shopBtn, Rectangle settingsBtn, Rectangle descBtn, int totalStars, bool nameSaved, bool isEnglish) {
    ClearBackground(DARKGRAY);
    
    DrawTextCentered("CAR RACE", 60, 60, GOLD);
    DrawText(TextFormat(isEnglish ? "Total Stars: %d" : "Gesamt Sterne: %d", totalStars), 20, 20, 20, GOLD);

    if (!nameSaved) {
        DrawTextCentered(isEnglish ? "ENTER YOUR NAME:" : "NAME EINGEBEN:", 180, 25, LIGHTGRAY);
        DrawRectangle(GetScreenWidth()/2 - 150, 220, 300, 40, RAYWHITE);
        DrawText(name, GetScreenWidth()/2 - 140, 230, 20, BLACK);
        if (letterCount < 15 && (framesCounter / 20) % 2 == 0) {
            DrawText("_", GetScreenWidth()/2 - 140 + MeasureText(name, 20), 230, 20, DARKGRAY);
        }
    } else {
        DrawTextCentered(TextFormat(isEnglish ? "Welcome, %s!" : "Willkommen, %s!", name), 220, 30, RAYWHITE);
    }

    auto DrawBtn = [&](Rectangle rect, const char* label, Color col) {
        bool hover = CheckCollisionPointRec(mousePoint, rect);
        DrawRectangleRec(rect, hover ? LIGHTGRAY : col);
        int tw = MeasureText(label, 20);
        DrawText(label, (int)(rect.x + (rect.width - tw) / 2), (int)(rect.y + 15), 20, hover ? BLACK : WHITE);
    };

    // Wir ordnen die Buttons sauber untereinander an
    DrawBtn(startBtn,    isEnglish ? "START GAME" : "SPIEL STARTEN", DARKGREEN);
    DrawBtn(scoreBtn,    isEnglish ? "SCOREBOARD" : "BESTENLISTE", DARKBLUE);
    DrawBtn(shopBtn,     isEnglish ? "SHOP" : "LADEN / SHOP", PURPLE); // Hier ist er!
    DrawBtn(settingsBtn, isEnglish ? "SETTINGS" : "EINSTELLUNGEN", GRAY);
    DrawBtn(descBtn,     isEnglish ? "HOW TO PLAY" : "ANLEITUNG", DARKGRAY);
}
void DrawSettingsMenu(Vector2 mousePoint, Rectangle langBtn, Rectangle resBtn, Rectangle nameChangeBtn, Rectangle deleteDataBtn, Rectangle backBtn, bool isEnglish, bool isFullscreen) {
    ClearBackground(BLACK);
    DrawTextCentered(isEnglish ? "SETTINGS" : "EINSTELLUNGEN", 80, 40, GOLD);

    auto DrawBtn = [&](Rectangle rect, const char* label, Color col, Color textCol) {
        bool hover = CheckCollisionPointRec(mousePoint, rect);
        DrawRectangleRec(rect, hover ? LIGHTGRAY : col);
        int tw = MeasureText(label, 20);
        DrawText(label, (int)(rect.x + (rect.width - tw) / 2), (int)(rect.y + 15), 20, textCol);
    };

    DrawBtn(langBtn, isEnglish ? "Language: EN" : "Sprache: DE", DARKGRAY, WHITE);
    
    // NEU: Auflösungs-Button
    const char* resText = isFullscreen ? (isEnglish ? "Mode: Fullscreen" : "Modus: Vollbild") 
                                       : (isEnglish ? "Mode: Windowed (1000x800)" : "Modus: Fenster (1000x800)");
    DrawBtn(resBtn, resText, DARKGRAY, WHITE);

    DrawBtn(nameChangeBtn, isEnglish ? "Change Name" : "Name aendern", DARKGRAY, WHITE);
    DrawBtn(deleteDataBtn, isEnglish ? "Delete All Data" : "Daten loeschen", MAROON, WHITE);
    DrawBtn(backBtn, isEnglish ? "BACK" : "ZURUECK", GRAY, BLACK);
}

void DrawScoreboardMenu(std::vector<ScoreEntry> scores, Vector2 mousePoint, Rectangle backBtn, bool isEnglish) {
    ClearBackground(BLACK);
    DrawTextCentered(isEnglish ? "TOP SCORES" : "BESTENLISTE", 50, 40, GOLD);

    for (int i = 0; i < (int)scores.size() && i < 10; i++) {
        Color c = (i == 0) ? GOLD : (i == 1) ? LIGHTGRAY : (i == 2) ? BROWN : WHITE;
        DrawText(TextFormat("%d. %s", i + 1, scores[i].name), 300, 150 + i * 40, 25, c);
        DrawText(TextFormat("%d", scores[i].score), 650, 150 + i * 40, 25, c);
    }

    bool hover = CheckCollisionPointRec(mousePoint, backBtn);
    DrawRectangleRec(backBtn, hover ? LIGHTGRAY : GRAY);
    DrawText(isEnglish ? "BACK" : "ZURUECK", (int)backBtn.x + (backBtn.width - MeasureText(isEnglish ? "BACK" : "ZURUECK", 20))/2, (int)backBtn.y + 15, 20, BLACK);
}

void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backBtn, bool isEnglish) {
    ClearBackground(BLACK);
    DrawTextCentered(isEnglish ? "HOW TO PLAY" : "ANLEITUNG", 100, 40, GOLD);

    const char* line1 = isEnglish ? "Use LEFT and RIGHT arrow keys to move." : "Nutze LINKS und RECHTS Pfeiltasten zum Lenken.";
    const char* line2 = isEnglish ? "Avoid red obstacles!" : "Weiche den roten Hindernissen aus!";
    const char* line3 = isEnglish ? "Collect yellow stars to buy new cars." : "Sammle gelbe Sterne fuer neue Autos.";
    const char* line4 = isEnglish ? "The game gets faster over time!" : "Das Spiel wird mit der Zeit schneller!";

    DrawTextCentered(line1, 250, 20, RAYWHITE);
    DrawTextCentered(line2, 300, 20, RAYWHITE);
    DrawTextCentered(line3, 350, 20, RAYWHITE);
    DrawTextCentered(line4, 400, 20, RAYWHITE);

    bool hover = CheckCollisionPointRec(mousePoint, backBtn);
    DrawRectangleRec(backBtn, hover ? LIGHTGRAY : GRAY);
    DrawText(isEnglish ? "BACK" : "ZURUECK", (int)backBtn.x + (backBtn.width - MeasureText(isEnglish ? "BACK" : "ZURUECK", 20))/2, (int)backBtn.y + 15, 20, BLACK);
}

void DrawPauseMenu(Vector2 mousePoint, Rectangle resumeBtn, Rectangle menuBtn, bool isEnglish) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    DrawTextCentered(isEnglish ? "PAUSED" : "PAUSE", 250, 50, WHITE);

    bool h1 = CheckCollisionPointRec(mousePoint, resumeBtn);
    DrawRectangleRec(resumeBtn, h1 ? LIGHTGRAY : GRAY);
    DrawText(isEnglish ? "RESUME" : "WEITER", (int)resumeBtn.x + (resumeBtn.width - MeasureText(isEnglish ? "RESUME" : "WEITER", 20))/2, (int)resumeBtn.y + 15, 20, BLACK);

    bool h2 = CheckCollisionPointRec(mousePoint, menuBtn);
    DrawRectangleRec(menuBtn, h2 ? LIGHTGRAY : GRAY);
    DrawText(isEnglish ? "MENU" : "MENUE", (int)menuBtn.x + (menuBtn.width - MeasureText(isEnglish ? "MENU" : "MENUE", 20))/2, (int)menuBtn.y + 15, 20, BLACK);
}

void DrawGameOverMenu(const char* name, int score, float time, int stars, Vector2 mousePoint, Rectangle menuBtn, bool isEnglish) {
    // FIX: DARKBORDER durch Fade(BLACK, 0.8f) ersetzt
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
    DrawTextCentered(isEnglish ? "GAME OVER" : "SPIEL VORBEI", 150, 60, RED);
    
    DrawTextCentered(TextFormat(isEnglish ? "Driver: %s" : "Fahrer: %s", name), 250, 25, WHITE);
    DrawTextCentered(TextFormat(isEnglish ? "Final Score: %d" : "Endstand: %d", score), 300, 30, GOLD);
    DrawTextCentered(TextFormat(isEnglish ? "Time Survived: %.2fs" : "Zeit: %.2fs", time), 350, 20, LIGHTGRAY);
    DrawTextCentered(TextFormat(isEnglish ? "Stars Collected: %d" : "Sterne gesammelt: %d", stars), 390, 20, GOLD);

    bool hover = CheckCollisionPointRec(mousePoint, menuBtn);
    DrawRectangleRec(menuBtn, hover ? LIGHTGRAY : GRAY);
    DrawText(isEnglish ? "BACK TO MENU" : "HAUPTMENUE", (int)menuBtn.x + (menuBtn.width - MeasureText(isEnglish ? "BACK TO MENU" : "HAUPTMENUE", 20))/2, (int)menuBtn.y + 15, 20, BLACK);
}