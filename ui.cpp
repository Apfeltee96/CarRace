#include "ui.h"

void DrawHUD(const char* playerName, float time, int score, int stars, bool isEnglish) {
    int sw = GetScreenWidth();
    DrawText(playerName, 20, 20, 25, LIGHTGRAY);
    DrawText(TextFormat("%.1fs", time), 20, 50, 22, WHITE);
    DrawText(TextFormat(isEnglish ? "SCORE: %i" : "PUNKTE: %i", score), sw - 250, 20, 30, GOLD);
    DrawText(TextFormat("+ %i %s", stars, isEnglish ? "Stars" : "Sterne"), sw - 250, 55, 25, YELLOW);
}

void DrawMainMenu(const char* name, int letters, int frames, Vector2 mouse, Rectangle start, Rectangle score, Rectangle shop, Rectangle settings, Rectangle desc, int stars, bool saved, bool isEnglish) {
    int sw = GetScreenWidth();
    DrawText("CAR RACE", sw/2 - MeasureText("CAR RACE", 60)/2, 60, 60, GOLD);
    DrawText(TextFormat(isEnglish ? "Total Stars: %i" : "Gesamtsterne: %i", stars), sw/2 - 80, 130, 20, YELLOW);

    if (!saved) {
        DrawRectangleRec({(float)sw/2-125, 280, 250, 50}, RAYWHITE);
        DrawText(name, sw/2 - 115, 295, 20, BLACK);
        DrawText(isEnglish ? "Enter Name:" : "Name eingeben:", sw/2 - 125, 250, 20, LIGHTGRAY);
    } else {
        DrawText(TextFormat(isEnglish ? "Welcome, %s!" : "Willkommen, %s!", name), sw/2 - 100, 295, 25, LIGHTGRAY);
    }

    auto Btn = [&](Rectangle r, const char* tDe, const char* tEn, Color c, Color h) {
        bool hv = CheckCollisionPointRec(mouse, r);
        DrawRectangleRec(r, hv ? h : c);
        const char* txt = isEnglish ? tEn : tDe;
        DrawText(txt, (int)(r.x + r.width/2 - MeasureText(txt, 20)/2), (int)(r.y + 15), 20, WHITE);
    };

    Btn(start, "SPIEL STARTEN", "START GAME", DARKGREEN, GREEN);
    Btn(score, "BESTENLISTE", "SCOREBOARD", DARKGRAY, GRAY);
    Btn(shop, "SHOP", "SHOP", {180, 100, 0, 255}, ORANGE);
    Btn(settings, "EINSTELLUNGEN", "SETTINGS", DARKBLUE, BLUE);
    Btn(desc, "ANLEITUNG", "HOW TO PLAY", BLACK, DARKGRAY);
}

void DrawSettingsMenu(Vector2 mouse, Rectangle lang, Rectangle delSave, Rectangle delScore, Rectangle back, bool isEnglish) {
    int sw = GetScreenWidth();
    DrawText(isEnglish ? "SETTINGS" : "EINSTELLUNGEN", sw/2 - 100, 100, 40, GOLD);
    
    auto SBtn = [&](Rectangle r, const char* t, Color c) {
        bool hv = CheckCollisionPointRec(mouse, r);
        DrawRectangleRec(r, hv ? ColorAlpha(c, 0.8f) : c);
        DrawText(t, (int)(r.x + r.width/2 - MeasureText(t, 20)/2), (int)(r.y + 15), 20, WHITE);
    };

    SBtn(lang, isEnglish ? "Language: EN" : "Sprache: DE", BLUE);
    SBtn(delSave, isEnglish ? "DELETE SAVE" : "SPIELSTAND LOESCHEN", MAROON);
    SBtn(delScore, isEnglish ? "CLEAR SCORES" : "BESTENLISTE LEEREN", DARKPURPLE);
    SBtn(back, isEnglish ? "BACK" : "ZURUECK", DARKGRAY);
}

void DrawShopMenu(const SaveGame& data, Vector2 mouse, Rectangle btnB, Rectangle btnR, Rectangle back, bool isEnglish) {
    DrawText("SHOP", GetScreenWidth()/2 - 50, 100, 40, GOLD);
    DrawText(TextFormat(isEnglish ? "Stars: %i" : "Sterne: %i", data.totalStars), GetScreenWidth()/2 - 50, 160, 20, YELLOW);
    
    DrawRectangleRec(btnB, BLUE); 
    DrawText(isEnglish ? "BLUE (Default)" : "BLAU (Standard)", (int)btnB.x + 10, (int)btnB.y + 15, 18, WHITE);
    
    DrawRectangleRec(btnR, data.ownsRedCar ? RED : MAROON);
    DrawText(data.ownsRedCar ? (isEnglish ? "RED (Owned)" : "ROT (Besitzt)") : (isEnglish ? "RED (15 Stars)" : "ROT (15 Sterne)"), (int)btnR.x + 10, (int)btnR.y + 15, 18, WHITE);

    DrawRectangleRec(back, DARKGRAY);
    DrawText(isEnglish ? "BACK" : "ZURUECK", (int)back.x + 75, (int)back.y + 15, 20, WHITE);
}

void DrawScoreboardMenu(const std::vector<ScoreEntry>& scores, Vector2 mouse, Rectangle back, bool isEnglish) {
    DrawText(isEnglish ? "TOP SCORES" : "BESTENLISTE", GetScreenWidth()/2 - 100, 100, 40, GOLD);
    for (int i = 0; i < (int)scores.size(); i++) 
        DrawText(TextFormat("%i. %s - %i", i+1, scores[i].name.c_str(), scores[i].score), GetScreenWidth()/2 - 100, 200 + i*30, 20, WHITE);
    DrawRectangleRec(back, DARKGRAY);
    DrawText(isEnglish ? "BACK" : "ZURUECK", (int)back.x + 75, (int)back.y + 15, 20, WHITE);
}

void DrawDescriptionMenu(Vector2 mouse, Rectangle back, bool isEnglish) {
    int sw = GetScreenWidth();
    DrawText(isEnglish ? "HOW TO PLAY" : "ANLEITUNG", sw/2 - 100, 150, 40, GOLD);
    DrawText(isEnglish ? "- Avoid Red Cars" : "- Weiche roten Autos aus", sw/2 - 150, 250, 20, WHITE);
    DrawText(isEnglish ? "- Collect Yellow Stars" : "- Sammle gelbe Sterne", sw/2 - 150, 280, 20, WHITE);
    DrawRectangleRec(back, DARKGRAY);
    DrawText(isEnglish ? "BACK" : "ZURUECK", (int)back.x + 75, (int)back.y + 15, 20, WHITE);
}

void DrawPauseMenu(Vector2 mouse, Rectangle btnP, Rectangle btnM, bool isEnglish) {
    DrawRectangle(0,0,GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    DrawText(isEnglish ? "PAUSED" : "PAUSE", GetScreenWidth()/2-50, 250, 40, WHITE);
    DrawRectangleRec(btnP, GREEN); DrawText(isEnglish ? "RESUME" : "WEITER", (int)btnP.x+70, (int)btnP.y+15, 20, WHITE);
    DrawRectangleRec(btnM, GRAY); DrawText(isEnglish ? "MENU" : "HAUPTMENUE", (int)btnM.x+60, (int)btnM.y+15, 20, WHITE);
}

void DrawGameOverMenu(const char* name, int score, float time, int stars, Vector2 mouse, Rectangle btnM, bool isEnglish) {
    DrawText("GAME OVER", GetScreenWidth()/2-120, 200, 50, RED);
    DrawText(TextFormat(isEnglish ? "Score: %i" : "Punkte: %i", score), GetScreenWidth()/2-60, 280, 25, WHITE);
    DrawRectangleRec(btnM, GRAY); DrawText(isEnglish ? "MENU" : "HAUPTMENUE", (int)btnM.x+60, (int)btnM.y+15, 20, WHITE);
}