#include "ui.h"
#include <string>

// --- HILFSFUNKTIONEN ---

void DrawTextCentered(const char* text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, (1000 - textWidth) / 2, y, fontSize, color);
}

void DrawButton(Rectangle rect, const char* text, Vector2 mouse, Color baseColor, Color textColor) {
    bool hover = CheckCollisionPointRec(mouse, rect);
    // Button wird GOLD bei Hover, sonst baseColor
    DrawRectangleRec(rect, hover ? GOLD : baseColor);
    DrawRectangleLinesEx(rect, 2, textColor);
    
    int fontSize = 20;
    int tw = MeasureText(text, fontSize);
    DrawText(text, (int)(rect.x + (rect.width - tw) / 2), (int)(rect.y + (rect.height - fontSize) / 2), fontSize, textColor);
}

// --- MENÜS & HUD ---

void DrawHUD(const char* name, float time, int score, int stars, bool isEnglish, Texture2D starTex) {
    // Linke Seite: Nur der Name des Fahrers bleibt hier
    DrawText(TextFormat(isEnglish ? "Driver: %s" : "Fahrer: %s", name), 20, 20, 22, RAYWHITE);

    // Rechte Seite: Punkte und Zeit
    // Wir berechnen die Position: 1000 (Breite) - Textbreite - 20 (Abstand zum Rand)
    const char* scoreText = TextFormat(isEnglish ? "Score: %d" : "Punkte: %d", score);
    const char* timeText = TextFormat(isEnglish ? "Time: %.2fs" : "Zeit: %.2fs", time);
    
    int scoreWidth = MeasureText(scoreText, 22);
    int timeWidth = MeasureText(timeText, 22);

    // Zeichnen am rechten Rand
    DrawText(scoreText, 1000 - scoreWidth - 20, 20, 22, RAYWHITE);
    DrawText(timeText, 1000 - timeWidth - 20, 50, 22, RAYWHITE);
    
    // Sterne (Ebenfalls rechts, unter der Zeit)
    const char* starCountText = TextFormat(": %d", stars);
    int starTextWidth = MeasureText(starCountText, 22);
    float iconHeight = 25.0f;
    float iconScale = iconHeight / (float)starTex.height;
    float iconWidth = starTex.width * iconScale;

    // Position für Stern-Icon und Text rechts bündig
    float starsX = 1000 - starTextWidth - iconWidth - 20;
    DrawTextureEx(starTex, { starsX, 80 }, 0.0f, iconScale, WHITE);
    DrawText(starCountText, (int)(starsX + iconWidth + 5), 80, 22, GOLD);
}

void DrawMainMenu(const char* name, int letterCount, int framesCounter, Vector2 mousePoint, 
                  Rectangle startBtn, Rectangle scoreBtn, Rectangle shopBtn, 
                  Rectangle settingsBtn, Rectangle descBtn, int totalStars, 
                  bool nameSaved, bool isEnglish) {
    
    DrawTextCentered(isEnglish ? "CAR RACE" : "AUTORENNEN", 100, 50, GOLD);

    // Buttons mit 5 Argumenten
    DrawButton(startBtn, isEnglish ? "START" : "START", mousePoint, GRAY, BLACK);
    DrawButton(scoreBtn, isEnglish ? "SCORES" : "BESTENLISTE", mousePoint, GRAY, BLACK);
    DrawButton(shopBtn, isEnglish ? "SHOP" : "SHOP", mousePoint, GRAY, BLACK);
    DrawButton(settingsBtn, isEnglish ? "SETTINGS" : "EINSTELLUNGEN", mousePoint, GRAY, BLACK);
    DrawButton(descBtn, isEnglish ? "HOW TO PLAY" : "STEUERUNG", mousePoint, GRAY, BLACK);

    if (!nameSaved) {
        DrawRectangle(350, 220, 300, 40, LIGHTGRAY);
        DrawRectangleLines(350, 220, 300, 40, BLACK);
        DrawText(name, 360, 230, 20, BLACK);
        // Cursor blinken
        if (((framesCounter / 30) % 2) == 0) 
            DrawRectangle(360 + MeasureText(name, 20), 225, 2, 30, DARKGRAY);
    } else {
        DrawTextCentered(TextFormat(isEnglish ? "Welcome, %s!" : "Willkommen, %s!", name), 220, 25, RAYWHITE);
    }
    DrawText(TextFormat(isEnglish ? "Stars: %d" : "Sterne: %d", totalStars), 820, 20, 20, GOLD);
}

void DrawSettingsMenu(Vector2 mousePoint, Rectangle langBtn, Rectangle resBtn, 
                      Rectangle nameChangeBtn, Rectangle deleteDataBtn, Rectangle backBtn, 
                      bool isEnglish, bool isFullscreen) {
    
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.6f));
    DrawTextCentered(isEnglish ? "SETTINGS" : "EINSTELLUNGEN", 80, 50, GOLD);

    DrawButton(langBtn, isEnglish ? "Language: English" : "Sprache: Deutsch", mousePoint, GRAY, BLACK);
    DrawButton(resBtn, isFullscreen ? (isEnglish ? "Window Mode" : "Fenstermodus") : (isEnglish ? "Fullscreen" : "Vollbild"), mousePoint, GRAY, BLACK);
    DrawButton(nameChangeBtn, isEnglish ? "Change Name" : "Name aendern", mousePoint, GRAY, BLACK);
    DrawButton(deleteDataBtn, isEnglish ? "DELETE DATA" : "DATEN LOESCHEN", mousePoint, RED, RAYWHITE);
    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

void DrawShopMenu(SaveGame data, Vector2 mousePoint, Rectangle redBtn, Rectangle blueBtn, 
                  Rectangle backBtn, bool isEnglish, Texture2D carTexs[]) {
    
    // 1. Werkstatt-Hintergrund statt einfachem Schwarz
    DrawWorkshopBackground();
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.3f)); // Leicht abdunkeln für UI-Lesbarkeit

    DrawTextCentered(isEnglish ? "Carshop" : "Autoladen", 50, 50, GOLD);
    DrawText(TextFormat(isEnglish ? "Stars: %d" : "Sterne: %d", data.totalStars), 820, 20, 25, GOLD);

    // Definition der Button-X-Positionen für die Ausrichtung
    float btnXPositions[3] = { 150.0f, 400.0f, 650.0f };
    float btnWidth = 200.0f;

    for (int i = 0; i < 3; i++) {
        // Skalierung wie im Spiel
        float gameScale = 45.0f / (float)carTexs[i].width;
        float shopDisplayScale = gameScale * 2.0f; // Etwas größer im Shop (Faktor 2.0)

        // Mittige Position berechnen:
        // (Button-Start + halbe Button-Breite) - (halbe Auto-Breite skaliert)
        float carWidthScaled = carTexs[i].width * shopDisplayScale;
        float carHeightScaled = carTexs[i].height * shopDisplayScale;
        
        float carX = (btnXPositions[i] + btnWidth / 2.0f) - (carWidthScaled / 2.0f);
        float carY = 350.0f - carHeightScaled; // Auto steht "auf" der Linie bei Y=350

        // Schatten unter dem Auto
        DrawEllipse((int)(carX + carWidthScaled/2), (int)(carY + carHeightScaled), 30, 10, Fade(BLACK, 0.5f));
        
        // Auto zeichnen
        DrawTextureEx(carTexs[i], { carX, carY }, 0, shopDisplayScale, WHITE);
    }

    // Die Buttons (X-Werte müssen zu btnXPositions passen)
    DrawButton({150, 500, 200, 50}, isEnglish ? "SELECT" : "WAEHLEN", mousePoint, data.selectedColorId == 0 ? GREEN : GRAY, BLACK);
    
    const char* redLabel = data.ownsRedCar ? (isEnglish ? "SELECT" : "WAEHLEN") : "100 Stars";
    DrawButton(redBtn, redLabel, mousePoint, data.selectedColorId == 1 ? GREEN : GRAY, BLACK);

    const char* blueLabel = data.ownsPurpleCar ? (isEnglish ? "SELECT" : "WAEHLEN") : "200 Stars";
    DrawButton(blueBtn, blueLabel, mousePoint, data.selectedColorId == 2 ? GREEN : GRAY, BLACK);

    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}
void DrawWorkshopBackground() {
    // Wand (Dunkelgrau)
    DrawRectangle(0, 0, 1000, 400, DARKGRAY);
    // Boden (Helleres Grau/Beton-Optik)
    DrawRectangle(0, 400, 1000, 400, GRAY);
    
    // Horizontale Linie (Übergang Wand/Boden)
    DrawRectangle(0, 395, 1000, 10, BLACK);

    // Ein paar "Fliesen-Linien" auf dem Boden für die Perspektive
    for (int i = 0; i < 10; i++) {
        DrawLine(i * 100, 400, i * 100 - 50, 800, DARKGRAY);
    }
    for (int i = 0; i < 4; i++) {
        DrawLine(0, 400 + i * 100, 1000, 400 + i * 100, DARKGRAY);
    }

    // Ein gelber "Sicherheitsstreifen" an der Wand
    for (int i = 0; i < 20; i++) {
        DrawRectangle(i * 60, 370, 30, 25, GOLD);
    }
}

void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backBtn, bool isEnglish) {
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.8f));
    DrawTextCentered(isEnglish ? "STEUERUNG" : "STEUERUNG", 100, 50, GOLD);
    
    Color txt = RAYWHITE;
    DrawTextCentered(isEnglish ? "Use LEFT/RIGHT ARROWS to move" : "Nutze LINKS/RECHTS PFEILE zum Bewegen", 250, 25, txt);
    DrawTextCentered(isEnglish ? "Collect stars to buy new cars" : "Sammle Sterne fuer neue Autos", 300, 25, txt);
    DrawTextCentered(isEnglish ? "Avoid obstacles!" : "Weiche Hindernissen aus!", 350, 25, txt);

    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

void DrawPauseMenu(Vector2 mousePoint, Rectangle resumeBtn, Rectangle menuBtn, bool isEnglish) {
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.5f));
    DrawTextCentered(isEnglish ? "PAUSED" : "PAUSE", 250, 50, RAYWHITE);
    DrawButton(resumeBtn, isEnglish ? "RESUME" : "WEITER", mousePoint, GRAY, BLACK);
    DrawButton(menuBtn, isEnglish ? "MENU" : "MENÜ", mousePoint, GRAY, BLACK);
}

void DrawGameOverMenu(const char* name, int score, float time, int stars, 
                      Vector2 mousePoint, Rectangle menuBtn, bool isEnglish) {
    
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.7f));
    DrawTextCentered(isEnglish ? "GAME OVER" : "SPIEL VORBEI", 150, 60, RED);
    
    DrawTextCentered(TextFormat(isEnglish ? "Final Score: %d" : "Endstand: %d", score), 250, 30, RAYWHITE);
    DrawTextCentered(TextFormat(isEnglish ? "Stars Collected: %d" : "Sterne gesammelt: %d", stars), 300, 30, GOLD);
    
    DrawButton(menuBtn, isEnglish ? "MAIN MENU" : "HAUPTMENU", mousePoint, GRAY, BLACK);
}

void DrawScoreboardMenu(std::vector<ScoreEntry> scores, Vector2 mousePoint, 
                        Rectangle backBtn, bool isEnglish) {
    
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.8f));
    DrawTextCentered(isEnglish ? "TOP 10 SCORES" : "BESTENLISTE", 80, 50, GOLD);

    for (int i = 0; i < (int)scores.size(); i++) {
        Color c = (i == 0) ? GOLD : (i == 1) ? LIGHTGRAY : (i == 2) ? BROWN : WHITE;
        DrawText(TextFormat("%d. %s", i + 1, scores[i].name), 300, 180 + i * 40, 25, c);
        DrawText(TextFormat("%d", scores[i].score), 600, 180 + i * 40, 25, c);
    }

    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}