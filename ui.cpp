#include "ui.h"
#include "level.h"

void DrawHUD(const char* playerName, float totalTimeSurvived, int currentLevel, float levelTimer) {
    int sw = GetScreenWidth();
    DrawText(playerName, 20, 20, 25, LIGHTGRAY);
    DrawText(TextFormat("%.1fs", totalTimeSurvived), 20, 50, 22, WHITE);

    const char* levelText = TextFormat("Level: %i", currentLevel);
    DrawText(levelText, sw - MeasureText(levelText, 30) - 20, 20, 30, GetLevelColor(currentLevel));
    
    const char* timeLeftText = TextFormat("Zeit: 00:%02i", (int)levelTimer);
    Color timeColor = (levelTimer <= 5.0f) ? RED : WHITE; 
    DrawText(timeLeftText, sw - MeasureText(timeLeftText, 22) - 20, 55, 22, timeColor);
}

void DrawMainMenu(const char* playerName, int letterCount, int framesCounter, Vector2 mousePoint, Rectangle inputBox, Rectangle startButton, Rectangle scoreBtn, Rectangle descButton) {
    int sw = GetScreenWidth();
    const char* titleText = "CAR RACE";
    DrawText(titleText, sw/2 - MeasureText(titleText, 45)/2, 150, 45, ORANGE);
    
    DrawText("Dein Name:", (int)inputBox.x, (int)inputBox.y - 30, 20, WHITE);
    DrawRectangleRec(inputBox, LIGHTGRAY);
    DrawRectangleLines((int)inputBox.x, (int)inputBox.y, (int)inputBox.width, (int)inputBox.height, DARKBLUE);
    DrawText(playerName, (int)inputBox.x + 10, (int)inputBox.y + 15, 20, MAROON);

    if (letterCount < 15) { 
        if (((framesCounter/30)%2) == 0) DrawText("_", (int)inputBox.x + 10 + MeasureText(playerName, 20), (int)inputBox.y + 15, 20, MAROON);
    }

    if (letterCount == 0) DrawText("Bitte Namen eingeben!", (int)inputBox.x + 15, (int)inputBox.y + 60, 20, RED);

    Color startColor = (letterCount > 0) ? (CheckCollisionPointRec(mousePoint, startButton) ? GREEN : DARKGREEN) : GRAY;
    DrawRectangleRec(startButton, startColor);
    DrawText("SPIEL STARTEN", (int)startButton.x + 15, (int)startButton.y + 15, 20, WHITE);

    Color sColor = CheckCollisionPointRec(mousePoint, scoreBtn) ? GOLD : ORANGE;
    DrawRectangleRec(scoreBtn, sColor);
    DrawText("BESTENLISTE", (int)scoreBtn.x + 30, (int)scoreBtn.y + 15, 20, WHITE);

    Color descColor = CheckCollisionPointRec(mousePoint, descButton) ? BLUE : DARKBLUE;
    DrawRectangleRec(descButton, descColor);
    DrawText("DESCRIPTION", (int)descButton.x + 30, (int)descButton.y + 15, 20, WHITE);
}

void DrawScoreboardMenu(const std::vector<ScoreEntry>& scores, Vector2 mousePoint, Rectangle backMenuBtn) {
    DrawText("TOP 10 SPIELER", 250, 150, 40, GOLD); 
    if (scores.empty()) {
        DrawText("Noch keine Eintraege vorhanden!", 220, 300, 20, LIGHTGRAY);
    } else {
        for (int i = 0; i < (int)scores.size(); i++) {
            DrawText(TextFormat("%i. %s", i+1, scores[i].name.c_str()), 200, 250 + (i * 35), 22, WHITE);
            DrawText(TextFormat("Lvl %i", scores[i].level), 430, 250 + (i * 35), 22, ORANGE);
            DrawText(TextFormat("(%.1fs)", scores[i].timeSurvived), 510, 250 + (i * 35), 22, LIGHTGRAY);
        }
    }
    Color backColor = CheckCollisionPointRec(mousePoint, backMenuBtn) ? RED : MAROON;
    DrawRectangleRec(backMenuBtn, backColor);
    DrawText("ZURUECK", (int)backMenuBtn.x + 75, (int)backMenuBtn.y + 15, 20, WHITE);
}

void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backMenuBtn) {
    DrawText("SPIELANLEITUNG", 240, 150, 40, ORANGE);
    DrawText("Ziel:", 200, 250, 25, YELLOW);
    DrawText("Ueberlebe so lange wie moeglich!", 200, 290, 20, WHITE);
    DrawText("Steuerung:", 200, 360, 25, YELLOW);
    DrawText("[Pfeil Links / Rechts]: Auto bewegen", 200, 400, 20, WHITE);
    DrawText("[P]: Spiel pausieren", 200, 430, 20, WHITE);
    
    Color backColor = CheckCollisionPointRec(mousePoint, backMenuBtn) ? RED : MAROON;
    DrawRectangleRec(backMenuBtn, backColor);
    DrawText("ZURUECK", (int)backMenuBtn.x + 75, (int)backMenuBtn.y + 15, 20, WHITE);
}

void DrawPauseMenu(Vector2 mousePoint, Rectangle btnPrimary, Rectangle btnMenu) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.6f));
    DrawText("PAUSIERT", 290, 280, 45, WHITE); 
    
    Color primColor = CheckCollisionPointRec(mousePoint, btnPrimary) ? GREEN : DARKGREEN;
    DrawRectangleRec(btnPrimary, primColor);
    DrawText("WEITER SPIELEN", (int)btnPrimary.x + 40, (int)btnPrimary.y + 15, 20, WHITE);

    Color menuColor = CheckCollisionPointRec(mousePoint, btnMenu) ? GRAY : DARKGRAY;
    DrawRectangleRec(btnMenu, menuColor);
    DrawText("HAUPTMENUE", (int)btnMenu.x + 60, (int)btnMenu.y + 15, 20, WHITE);
}

void DrawGameOverMenu(const char* playerName, int currentLevel, float totalTimeSurvived, Vector2 mousePoint, Rectangle btnMenu) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.7f));
    DrawText("CRASH!", 320, 250, 50, RED); 
    
    const char* overText1 = TextFormat("Schade %s, du bist in", playerName);
    DrawText(overText1, sw/2 - MeasureText(overText1, 25)/2, 330, 25, LIGHTGRAY);
    
    const char* overText2 = TextFormat("Level %i nach %.1fs gecrasht.", currentLevel, totalTimeSurvived);
    DrawText(overText2, sw/2 - MeasureText(overText2, 25)/2, 365, 25, LIGHTGRAY);
    
    DrawText("Ergebnis im Scoreboard gespeichert!", 195, 410, 20, GREEN); 

    Color menuColor = CheckCollisionPointRec(mousePoint, btnMenu) ? GRAY : DARKGRAY;
    DrawRectangleRec(btnMenu, menuColor);
    DrawText("HAUPTMENUE", (int)btnMenu.x + 60, (int)btnMenu.y + 15, 20, WHITE);
}