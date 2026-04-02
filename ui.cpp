#include "ui.h"

// ============================================================
//  Hilfsfunktionen
// ============================================================

void DrawTextCentered(const char* text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, (1000 - textWidth) / 2, y, fontSize, color);
}

void DrawButton(Rectangle rect, const char* text, Vector2 mouse,
                Color baseColor, Color textColor) {
    bool hover = CheckCollisionPointRec(mouse, rect);
    DrawRectangleRec(rect, hover ? GOLD : baseColor);
    DrawRectangleLinesEx(rect, 2, textColor);

    constexpr int fontSize = 20;
    int tw = MeasureText(text, fontSize);
    DrawText(text,
        static_cast<int>(rect.x + (rect.width  - tw)       / 2),
        static_cast<int>(rect.y + (rect.height - fontSize)  / 2),
        fontSize, textColor);
}

// ============================================================
//  HUD
// ============================================================

void DrawHUD(const char* name, float time, int score, int stars,
             bool isEnglish, Texture2D starTex, int highScore) {
    constexpr int fontSize    = 22;
    constexpr int margin      = 20;
    constexpr int rightAlignX = 800;

    // Links oben: Fahrername & Zeit
    DrawText(TextFormat(isEnglish ? "Driver: %s" : "Fahrer: %s", name),
             margin, 20, fontSize, RAYWHITE);
    DrawText(TextFormat(isEnglish ? "Time: %.2fs" : "Zeit: %.2fs", time),
             margin, 50, fontSize, RAYWHITE);

    // Rechts oben: Punkte
    DrawText(isEnglish ? "Score:" : "Punkte:", rightAlignX, 20, fontSize, RAYWHITE);
    Color scoreColor = (score > highScore) ? GOLD : RAYWHITE;
    DrawText(TextFormat("%d", score), rightAlignX + 80, 20, fontSize, scoreColor);

    // Rechts: Sterne
    constexpr float iconHeight = 25.0f;
    float iconScale = iconHeight / static_cast<float>(starTex.height);
    DrawTextureEx(starTex, { static_cast<float>(rightAlignX), 50.0f }, 0.0f, iconScale, WHITE);
    DrawText(TextFormat(": %d", stars), rightAlignX + 35, 50, fontSize, GOLD);
}

// ============================================================
//  Hauptmenü
// ============================================================

void DrawMainMenu(const char* name, int letterCount, int framesCounter,
                  Vector2 mousePoint, Rectangle startBtn, Rectangle scoreBtn,
                  Rectangle shopBtn, Rectangle settingsBtn, Rectangle descBtn,
                  int totalStars, bool nameSaved, bool isEnglish) {

    DrawTextCentered(isEnglish ? "CAR RACE" : "AUTORENNEN", 100, 50, GOLD);

    DrawButton(startBtn,    isEnglish ? "START"      : "START",          mousePoint, GRAY, BLACK);
    DrawButton(scoreBtn,    isEnglish ? "SCORES"     : "BESTENLISTE",    mousePoint, GRAY, BLACK);
    DrawButton(shopBtn,     isEnglish ? "SHOP"       : "SHOP",           mousePoint, GRAY, BLACK);
    DrawButton(settingsBtn, isEnglish ? "SETTINGS"   : "EINSTELLUNGEN",  mousePoint, GRAY, BLACK);
    DrawButton(descBtn,     isEnglish ? "HOW TO PLAY": "STEUERUNG",      mousePoint, GRAY, BLACK);

    if (!nameSaved) {
        // Namenseingabefeld
        DrawRectangle(350, 220, 300, 40, LIGHTGRAY);
        DrawRectangleLines(350, 220, 300, 40, BLACK);
        DrawText(name, 360, 230, 20, BLACK);
        // Blinkender Cursor (wechselt alle 30 Frames)
        if (((framesCounter / 30) % 2) == 0)
            DrawRectangle(360 + MeasureText(name, 20), 225, 2, 30, DARKGRAY);
    } else {
        DrawTextCentered(
            TextFormat(isEnglish ? "Welcome, %s!" : "Willkommen, %s!", name),
            220, 25, RAYWHITE);
    }

    DrawText(TextFormat(isEnglish ? "Stars: %d" : "Sterne: %d", totalStars),
             820, 20, 20, GOLD);
}

// ============================================================
//  Einstellungsmenü
// ============================================================

void DrawSettingsMenu(Vector2 mousePoint, Rectangle langBtn, Rectangle resBtn,
                      Rectangle nameChangeBtn, Rectangle deleteDataBtn,
                      Rectangle backBtn, bool isEnglish, bool isFullscreen) {

    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.6f));
    DrawTextCentered(isEnglish ? "SETTINGS" : "EINSTELLUNGEN", 80, 50, GOLD);

    DrawButton(langBtn,       isEnglish ? "Language: English"  : "Sprache: Deutsch",    mousePoint, GRAY, BLACK);
    DrawButton(resBtn,        isFullscreen
                                  ? (isEnglish ? "Window Mode" : "Fenstermodus")
                                  : (isEnglish ? "Fullscreen"  : "Vollbild"),            mousePoint, GRAY, BLACK);
    DrawButton(nameChangeBtn, isEnglish ? "Change Name"        : "Name aendern",         mousePoint, GRAY, BLACK);
    DrawButton(deleteDataBtn, isEnglish ? "DELETE DATA"        : "DATEN LOESCHEN",       mousePoint, RED,  RAYWHITE);
    DrawButton(backBtn,       isEnglish ? "BACK"               : "ZURUECK",              mousePoint, GRAY, BLACK);
}

// ============================================================
//  Shop-Menü
// ============================================================

void DrawWorkshopBackground() {
    DrawRectangle(0, 0, 1000, 400, DARKGRAY);   // Wand
    DrawRectangle(0, 400, 1000, 400, GRAY);      // Boden
    DrawRectangle(0, 395, 1000, 10, BLACK);      // Wandkante

    // Perspektivische Bodenlinien (vertikal)
    for (int i = 0; i < 10; i++)
        DrawLine(i * 100, 400, i * 100 - 50, 800, DARKGRAY);
    // Horizontale Bodenlinien
    for (int i = 0; i < 4; i++)
        DrawLine(0, 400 + i * 100, 1000, 400 + i * 100, DARKGRAY);

    // Gelber Sicherheitsstreifen an der Wand
    for (int i = 0; i < 20; i++)
        DrawRectangle(i * 60, 370, 30, 25, GOLD);
}

void DrawShopMenu(const SaveGame& data, Vector2 mousePoint,
                  Rectangle redBtn, Rectangle blueBtn, Rectangle backBtn,
                  bool isEnglish, Texture2D carTexs[]) {

    DrawWorkshopBackground();
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.3f));

    DrawTextCentered(isEnglish ? "Carshop" : "Autoladen", 50, 50, GOLD);
    DrawText(TextFormat(isEnglish ? "Stars: %d" : "Sterne: %d", data.totalStars),
             820, 20, 25, GOLD);

    // Auto-Positionen und Buttons sind symmetrisch über 3 Slots
    const float btnXPositions[3] = { 150.0f, 400.0f, 650.0f };
    constexpr float btnWidth     = 200.0f;

    // Autos zentriert über ihren jeweiligen Button zeichnen
    for (int i = 0; i < 3; i++) {
        float gameScale        = 45.0f / static_cast<float>(carTexs[i].width);
        float shopScale        = gameScale * 2.0f;
        float carW             = carTexs[i].width  * shopScale;
        float carH             = carTexs[i].height * shopScale;
        float carX             = (btnXPositions[i] + btnWidth / 2.0f) - (carW / 2.0f);
        float carY             = 350.0f - carH;

        // Schatten
        DrawEllipse(static_cast<int>(carX + carW / 2),
                    static_cast<int>(carY + carH),
                    30, 10, Fade(BLACK, 0.5f));
        DrawTextureEx(carTexs[i], { carX, carY }, 0.0f, shopScale, WHITE);
    }

    // Weißes Auto (immer verfügbar)
    DrawButton({ 150, 500, 200, 50 },
               isEnglish ? "SELECT" : "WAEHLEN",
               mousePoint,
               data.selectedColorId == 0 ? GREEN : GRAY, BLACK);

    // Rotes Auto
    const char* redLabel = data.ownsRedCar
        ? (isEnglish ? "SELECT" : "WAEHLEN")
        : TextFormat("%d Stars", PRICE_RED_CAR);
    DrawButton(redBtn, redLabel, mousePoint,
               data.selectedColorId == 1 ? GREEN : GRAY, BLACK);

    // Lila Auto
    const char* blueLabel = data.ownsPurpleCar
        ? (isEnglish ? "SELECT" : "WAEHLEN")
        : TextFormat("%d Stars", PRICE_PURPLE_CAR);
    DrawButton(blueBtn, blueLabel, mousePoint,
               data.selectedColorId == 2 ? GREEN : GRAY, BLACK);

    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

// ============================================================
//  Steuerungsübersicht
// ============================================================

void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backBtn, bool isEnglish) {
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.8f));
    DrawTextCentered(isEnglish ? "CONTROLS" : "STEUERUNG", 100, 50, GOLD);

    DrawTextCentered(isEnglish ? "Use LEFT/RIGHT ARROWS to move"    : "Nutze LINKS/RECHTS PFEILE zum Bewegen", 250, 25, RAYWHITE);
    DrawTextCentered(isEnglish ? "Collect stars to buy new cars"     : "Sammle Sterne fuer neue Autos",         300, 25, RAYWHITE);
    DrawTextCentered(isEnglish ? "Avoid obstacles!"                  : "Weiche Hindernissen aus!",              350, 25, RAYWHITE);

    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

// ============================================================
//  Pausemenü
// ============================================================

void DrawPauseMenu(Vector2 mousePoint, Rectangle resumeBtn,
                   Rectangle menuBtn, bool isEnglish) {
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.5f));
    DrawTextCentered(isEnglish ? "PAUSED" : "PAUSE", 250, 50, RAYWHITE);
    DrawButton(resumeBtn, isEnglish ? "RESUME" : "WEITER", mousePoint, GRAY, BLACK);
    DrawButton(menuBtn,   isEnglish ? "MENU"   : "MENÜ",   mousePoint, GRAY, BLACK);
}

// ============================================================
//  Game-Over-Bildschirm
// ============================================================

void DrawGameOverMenu(const char* name, int score, float time, int stars,
                      Vector2 mousePoint, Rectangle menuBtn, bool isEnglish) {
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.7f));
    DrawTextCentered(isEnglish ? "GAME OVER" : "SPIEL VORBEI", 150, 60, RED);
    DrawTextCentered(TextFormat(isEnglish ? "Final Score: %d"       : "Endstand: %d",          score), 250, 30, RAYWHITE);
    DrawTextCentered(TextFormat(isEnglish ? "Stars Collected: %d"   : "Sterne gesammelt: %d",  stars), 300, 30, GOLD);
    DrawButton(menuBtn, isEnglish ? "MAIN MENU" : "HAUPTMENU", mousePoint, GRAY, BLACK);
}

// ============================================================
//  Bestenliste
// ============================================================

void DrawScoreboardMenu(const std::vector<ScoreEntry>& scores,
                        Vector2 mousePoint, Rectangle backBtn, bool isEnglish) {
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.8f));
    DrawTextCentered(isEnglish ? "TOP 10 SCORES" : "BESTENLISTE", 80, 50, GOLD);

    for (int i = 0; i < static_cast<int>(scores.size()); i++) {
        // Gold → Silber → Bronze → Weiß
        Color c = (i == 0) ? GOLD : (i == 1) ? LIGHTGRAY : (i == 2) ? BROWN : WHITE;
        DrawText(TextFormat("%d. %s", i + 1, scores[i].name), 300, 180 + i * 40, 25, c);
        DrawText(TextFormat("%d", scores[i].score),            600, 180 + i * 40, 25, c);
    }

    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}
