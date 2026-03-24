#include "ui.h"

void DrawHUD(const char* playerName, float totalTimeSurvived, int score, int earnedStars) {
    int sw = GetScreenWidth();
    DrawText(playerName, 20, 20, 25, LIGHTGRAY);
    DrawText(TextFormat("%.1fs", totalTimeSurvived), 20, 50, 22, WHITE);

    const char* scoreText = TextFormat("PUNKTE: %i", score);
    DrawText(scoreText, sw - MeasureText(scoreText, 30) - 20, 20, 30, GOLD);

    const char* starText = TextFormat("+ %i Sterne", earnedStars);
    DrawText(starText, sw - MeasureText(starText, 25) - 20, 55, 25, YELLOW);

    // --- NEU: Milestone-Anzeige (Blinkender Text) ---
    if (score >= 1000 && score < 1150) {
        DrawText("SPEED UP!", sw/2 - MeasureText("SPEED UP!", 40)/2, 300, 40, ORANGE);
    }
    else if (score >= 2500 && score < 2650) {
        DrawText("TURBO MODE!", sw/2 - MeasureText("TURBO MODE!", 40)/2, 300, 40, RED);
    }
    else if (score >= 5000 && score < 5150) {
        DrawText("SONIC SPEED!", sw/2 - MeasureText("SONIC SPEED!", 40)/2, 300, 40, MAGENTA);
    }
}

void DrawMainMenu(const char* playerName, int letterCount, int framesCounter, Vector2 mousePoint, Rectangle inputBox, Rectangle startButton, Rectangle scoreBtn, Rectangle shopBtn, Rectangle descButton, int totalStars, bool isNameSaved) {
    int sw = GetScreenWidth();
    const char* titleText = "CAR RACE";
    DrawText(titleText, (int)(sw/2 - MeasureText(titleText, 45)/2), 120, 45, ORANGE);
    
    DrawText(TextFormat("Sterne: %i", totalStars), 20, 20, 25, GOLD);
    
    if (!isNameSaved) {
        DrawText("Dein Name:", (int)inputBox.x, (int)(inputBox.y - 30), 20, WHITE);
        DrawRectangleRec(inputBox, LIGHTGRAY);
        DrawRectangleLines((int)inputBox.x, (int)inputBox.y, (int)inputBox.width, (int)inputBox.height, DARKBLUE);
        DrawText(playerName, (int)inputBox.x + 10, (int)inputBox.y + 15, 20, MAROON);

        if (letterCount < 15) { 
            if (((framesCounter/30)%2) == 0) DrawText("_", (int)(inputBox.x + 10 + MeasureText(playerName, 20)), (int)(inputBox.y + 15), 20, MAROON);
        }
        if (letterCount == 0) DrawText("Bitte Namen eingeben!", (int)(inputBox.x + 15), (int)(inputBox.y + 60), 20, RED);

        Color startColor = (letterCount > 0) ? (CheckCollisionPointRec(mousePoint, startButton) ? GREEN : DARKGREEN) : GRAY;
        DrawRectangleRec(startButton, startColor);
        DrawText("SPIEL STARTEN", (int)(startButton.x + 15), (int)(startButton.y + 15), 20, WHITE);
    } 
    else {
        // Spieler ist bekannt: Begrüßung
        const char* welcomeText = TextFormat("Willkommen zurueck, %s!", playerName);
        int welcomeX = (int)(sw/2 - MeasureText(welcomeText, 25)/2);
        int welcomeY = (int)(inputBox.y + 10);
        DrawText(welcomeText, welcomeX, welcomeY, 25, LIGHTGRAY);

        // NEU: Kleiner "Aendern"-Button rechts neben dem Namen
        Rectangle editBtn = { (float)(welcomeX + MeasureText(welcomeText, 25) + 15), (float)welcomeY, 80, 25 };
        Color editColor = CheckCollisionPointRec(mousePoint, editBtn) ? RAYWHITE : GRAY;
        DrawRectangleLinesEx(editBtn, 1, editColor);
        DrawText("Aendern", (int)editBtn.x + 5, (int)editBtn.y + 5, 15, editColor);

        // Start-Button
        Color startColor = CheckCollisionPointRec(mousePoint, startButton) ? GREEN : DARKGREEN;
        DrawRectangleRec(startButton, startColor);
        DrawText("NOCHMAL SPIELEN", (int)(startButton.x + 8), (int)(startButton.y + 15), 20, WHITE);
    }

    Color sColor = CheckCollisionPointRec(mousePoint, scoreBtn) ? GOLD : ORANGE;
    DrawRectangleRec(scoreBtn, sColor);
    DrawText("BESTENLISTE", (int)(scoreBtn.x + 30), (int)(scoreBtn.y + 15), 20, WHITE);

    Color shColor = CheckCollisionPointRec(mousePoint, shopBtn) ? PURPLE : VIOLET;
    DrawRectangleRec(shopBtn, shColor);
    DrawText("ZUM SHOP", (int)(shopBtn.x + 50), (int)(shopBtn.y + 15), 20, WHITE);

    Color descColor = CheckCollisionPointRec(mousePoint, descButton) ? BLUE : DARKBLUE;
    DrawRectangleRec(descButton, descColor);
    DrawText("DESCRIPTION", (int)(descButton.x + 30), (int)(descButton.y + 15), 20, WHITE);
}

void DrawShopMenu(const SaveGame& saveData, Vector2 mousePoint, Rectangle btnBlue, Rectangle btnRed, Rectangle backMenuBtn) {
    int sw = GetScreenWidth();
    DrawText("SHOP", (int)(sw/2 - MeasureText("SHOP", 40)/2), 100, 40, GOLD);
    const char* starsText = TextFormat("Deine Sterne: %i", saveData.totalStars);
    DrawText(starsText, (int)(sw/2 - MeasureText(starsText, 25)/2), 160, 25, YELLOW);

    DrawRectangle((int)(btnBlue.x - 60), (int)(btnBlue.y + 10), 30, 30, BLUE); 
    Color colorBlueBtn = CheckCollisionPointRec(mousePoint, btnBlue) ? LIGHTGRAY : GRAY;
    if (saveData.selectedColorId == 0) colorBlueBtn = GREEN;
    DrawRectangleRec(btnBlue, colorBlueBtn);
    DrawText(saveData.selectedColorId == 0 ? "AUSGERUESTET" : "AUSRUESTEN", (int)(btnBlue.x + 20), (int)(btnBlue.y + 15), 20, WHITE);

    DrawRectangle((int)(btnRed.x - 60), (int)(btnRed.y + 10), 30, 30, RED); 
    Color colorRedBtn = CheckCollisionPointRec(mousePoint, btnRed) ? LIGHTGRAY : GRAY;
    if (saveData.selectedColorId == 1) colorRedBtn = GREEN;
    DrawRectangleRec(btnRed, colorRedBtn);
    if (saveData.ownsRedCar) {
        DrawText(saveData.selectedColorId == 1 ? "AUSGERUESTET" : "AUSRUESTEN", (int)(btnRed.x + 20), (int)(btnRed.y + 15), 20, WHITE);
    } else {
        DrawText("KAUFEN (15 Sterne)", (int)(btnRed.x + 5), (int)(btnRed.y + 15), 20, saveData.totalStars >= 15 ? WHITE : RED);
    }

    Color backColor = CheckCollisionPointRec(mousePoint, backMenuBtn) ? RED : MAROON;
    DrawRectangleRec(backMenuBtn, backColor);
    DrawText("ZURUECK", (int)(backMenuBtn.x + 75), (int)(backMenuBtn.y + 15), 20, WHITE);
}

void DrawScoreboardMenu(const std::vector<ScoreEntry>& scores, Vector2 mousePoint, Rectangle backMenuBtn) {
    DrawText("TOP 10 SPIELER", 250, 150, 40, GOLD); 
    if (scores.empty()) {
        DrawText("Noch keine Eintraege vorhanden!", 220, 300, 20, LIGHTGRAY);
    } else {
        for (int i = 0; i < (int)scores.size(); i++) {
            DrawText(TextFormat("%i. %s", i+1, scores[i].name.c_str()), 160, 250 + (i * 35), 22, WHITE);
            DrawText(TextFormat("%i Pkt.", scores[i].score), 410, 250 + (i * 35), 22, ORANGE);
            DrawText(TextFormat("(%.1fs)", scores[i].timeSurvived), 540, 250 + (i * 35), 22, LIGHTGRAY);
        }
    }
    Color backColor = CheckCollisionPointRec(mousePoint, backMenuBtn) ? RED : MAROON;
    DrawRectangleRec(backMenuBtn, backColor);
    DrawText("ZURUECK", (int)(backMenuBtn.x + 75), (int)(backMenuBtn.y + 15), 20, WHITE);
}

void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backMenuBtn) {
    DrawText("SPIELANLEITUNG", 240, 150, 40, ORANGE);
    DrawText("Ziel:", 200, 250, 25, YELLOW);
    DrawText("Sammle so viele Punkte wie moeglich!", 200, 290, 20, WHITE);
    DrawText("Sterne:", 200, 340, 25, GOLD);
    DrawText("Fuer 500 Punkte erhaeltst du 1 Stern fuer den Shop!", 200, 380, 20, WHITE);
    
    Color backColor = CheckCollisionPointRec(mousePoint, backMenuBtn) ? RED : MAROON;
    DrawRectangleRec(backMenuBtn, backColor);
    DrawText("ZURUECK", (int)(backMenuBtn.x + 75), (int)(backMenuBtn.y + 15), 20, WHITE);
}

void DrawPauseMenu(Vector2 mousePoint, Rectangle btnPrimary, Rectangle btnMenu) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.6f));
    DrawText("PAUSIERT", 290, 280, 45, WHITE); 
    
    Color primColor = CheckCollisionPointRec(mousePoint, btnPrimary) ? GREEN : DARKGREEN;
    DrawRectangleRec(btnPrimary, primColor);
    DrawText("WEITER SPIELEN", (int)(btnPrimary.x + 40), (int)(btnPrimary.y + 15), 20, WHITE);

    Color menuColor = CheckCollisionPointRec(mousePoint, btnMenu) ? GRAY : DARKGRAY;
    DrawRectangleRec(btnMenu, menuColor);
    DrawText("HAUPTMENUE", (int)(btnMenu.x + 60), (int)(btnMenu.y + 15), 20, WHITE);
}

void DrawGameOverMenu(const char* playerName, int score, float totalTimeSurvived, int earnedStars, Vector2 mousePoint, Rectangle btnMenu) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.7f));
    DrawText("CRASH!", 320, 250, 50, RED); 
    
    const char* overText1 = TextFormat("Schade %s, du hast", playerName);
    DrawText(overText1, (int)(sw/2 - MeasureText(overText1, 25)/2), 330, 25, LIGHTGRAY);
    
    const char* overText2 = TextFormat("%i Punkte in %.1fs gesammelt.", score, totalTimeSurvived);
    DrawText(overText2, (int)(sw/2 - MeasureText(overText2, 25)/2), 365, 25, LIGHTGRAY);
    
    const char* starText = TextFormat("Du hast %i Sterne verdient!", earnedStars);
    DrawText(starText, (int)(sw/2 - MeasureText(starText, 25)/2), 410, 25, GOLD); 

    Color menuColor = CheckCollisionPointRec(mousePoint, btnMenu) ? GRAY : DARKGRAY;
    DrawRectangleRec(btnMenu, menuColor);
    DrawText("HAUPTMENUE", (int)(btnMenu.x + 60), (int)(btnMenu.y + 15), 20, WHITE);
}
void DrawSpecialMessage(const char* message) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    
    // Ein schickes Rechteck in der Mitte
    Rectangle msgBox = { (float)(sw/2 - 250), (float)(sh/2 - 50), 500, 100 };
    
    // Hintergrund mit leichtem Rot-Stich für die Liebe ;)
    DrawRectangleRec(msgBox, Fade(MAROON, 0.8f));
    DrawRectangleLinesEx(msgBox, 3, GOLD);
    
    // Der Text
    int fontSize = 25;
    int textWidth = MeasureText(message, fontSize);
    DrawText(message, (int)(sw/2 - textWidth/2), (int)(sh/2 - 12), fontSize, WHITE);
}