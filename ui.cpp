#include "ui.h"

// ============================================================
//  Hilfsfunktionen
// ============================================================
void DrawTextCentered(const char *text, int y, int fontSize, Color color)
{
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, (1000 - textWidth) / 2, y, fontSize, color);
}

void DrawButton(Rectangle rect, const char *text, Vector2 mouse,
                Color baseColor, Color textColor)
{
    bool hover = CheckCollisionPointRec(mouse, rect);
    DrawRectangleRec(rect, hover ? GOLD : baseColor);
    DrawRectangleLinesEx(rect, 2, textColor);
    constexpr int fontSize = 20;
    int tw = MeasureText(text, fontSize);
    DrawText(text,
             static_cast<int>(rect.x + (rect.width - tw) / 2),
             static_cast<int>(rect.y + (rect.height - fontSize) / 2),
             fontSize, textColor);
}

// ============================================================
//  HUD
// ============================================================
void DrawHUD(const char *name, float time, int score, int stars,
             bool isEnglish, Texture2D starTex, int highScore)
{
    constexpr int fontSize = 22;
    constexpr int margin = 20;
    constexpr int rightAlignX = 800;

    // Links oben: Fahrername & Zeit
    DrawText(TextFormat(isEnglish ? "Driver: %s" : "Fahrer: %s", name), margin, 20, fontSize, RAYWHITE);
    DrawText(TextFormat(isEnglish ? "Time: %.1fs" : "Zeit: %.1fs", time), margin, 50, fontSize, RAYWHITE);

    // Rechts oben: Punkte
    DrawText(isEnglish ? "Score:" : "Punkte:", rightAlignX, 20, fontSize, RAYWHITE);
    DrawText(TextFormat("%d", score), rightAlignX + 100, 20, fontSize, RAYWHITE);

    // Rechts: Sterne
    constexpr float iconHeight = 25.0f;
    float iconScale = iconHeight / static_cast<float>(starTex.height);
    DrawTextureEx(starTex, {static_cast<float>(rightAlignX), 50.0f}, 0.0f, iconScale, WHITE);
    DrawText(TextFormat(": %d", stars), rightAlignX + 35, 50, fontSize, GOLD);
}

//  Hauptmenü
// ============================================================
void DrawMainMenu(const char *name, int letterCount, int framesCounter,
                  Vector2 mousePoint, Rectangle startBtn, Rectangle scoreBtn,
                  Rectangle shopBtn, Rectangle settingsBtn, Rectangle descBtn,
                  int totalStars, bool nameSaved, bool isEnglish)
{
    DrawTextCentered(isEnglish ? "CAR RACE" : "AUTORENNEN", 100, 50, GOLD);
    DrawButton(startBtn, isEnglish ? "START" : "START", mousePoint, GRAY, BLACK);
    DrawButton(scoreBtn, isEnglish ? "SCORES" : "BESTENLISTE", mousePoint, GRAY, BLACK);
    DrawButton(shopBtn, isEnglish ? "SHOP" : "SHOP", mousePoint, GRAY, BLACK);
    DrawButton(settingsBtn, isEnglish ? "SETTINGS" : "EINSTELLUNGEN", mousePoint, GRAY, BLACK);
    DrawButton(descBtn, isEnglish ? "HOW TO PLAY" : "STEUERUNG", mousePoint, GRAY, BLACK);

    if (!nameSaved)
    {
        const char *label = isEnglish ? "Please enter name here:" : "Hier bitte Namen eingeben:";
        DrawText(label, 350, 190, 18, RAYWHITE);
        DrawRectangle(350, 220, 300, 40, LIGHTGRAY);
        DrawRectangleLines(350, 220, 300, 40, BLACK);
        DrawText(name, 360, 230, 20, BLACK);

        // Blinkender Cursor
        if (((framesCounter / 30) % 2) == 0)
            DrawRectangle(360 + MeasureText(name, 20), 225, 2, 30, DARKGRAY);
    }
    else
    {
        DrawTextCentered(
            TextFormat(isEnglish ? "Welcome, %s!" : "Willkommen, %s!", name),
            220, 25, RAYWHITE);
    }
}

void DrawSettingsMenu(Vector2 mousePoint, Rectangle langBtn, Rectangle resBtn,
                      Rectangle nameChangeBtn, Rectangle deleteDataBtn,
                      Rectangle backBtn, bool isEnglish, bool isFullscreen)
{
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.6f));
    DrawTextCentered(isEnglish ? "SETTINGS" : "EINSTELLUNGEN", 80, 50, GOLD);
    DrawButton(langBtn, isEnglish ? "Language: English" : "Sprache: Deutsch", mousePoint, GRAY, BLACK);
    DrawButton(resBtn, isFullscreen ? (isEnglish ? "Window Mode" : "Fenstermodus") : (isEnglish ? "Fullscreen" : "Vollbild"), mousePoint, GRAY, BLACK);
    DrawButton(nameChangeBtn, isEnglish ? "Change Name" : "Name aendern", mousePoint, GRAY, BLACK);
    DrawButton(deleteDataBtn, isEnglish ? "DELETE DATA" : "DATEN LOESCHEN", mousePoint, RED, RAYWHITE);
    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

// ============================================================
//  Shop-Menü
// ============================================================
void DrawWorkshopBackground()
{
    DrawRectangle(0, 0, 1000, 400, DARKGRAY); // Wand
    DrawRectangle(0, 400, 1000, 400, GRAY);   // Boden
    DrawRectangle(0, 395, 1000, 10, BLACK);   // Wandkante
    // Perspektivische Bodenlinien (vertikal)
    for (int i = 0; i < 10; i++)
        DrawRectangle(i * 100, 400, 5, 400, DARKGRAY); // Vereinfachte Linien
}

void DrawShopMenu(const SaveGame &data, Vector2 mousePoint,
                  Rectangle redBtn, Rectangle blueBtn, Rectangle backBtn,
                  bool isEnglish, Texture2D carTexs[])
{
    DrawWorkshopBackground();
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.3f));
    DrawText(TextFormat(isEnglish ? "Stars: %d" : "Sterne: %d", data.totalStars), 820, 20, 25, GOLD);

    const float btnXPositions[3] = {150.0f, 400.0f, 650.0f};
    constexpr float btnWidth = 200.0f;

    for (int i = 0; i < 3; i++)
    {
        float gameScale = 45.0f / static_cast<float>(carTexs[i].width);
        float shopScale = gameScale * 2.5f;
        float carW = carTexs[i].width * shopScale;
        float carH = carTexs[i].height * shopScale;
        float carX = (btnXPositions[i] + btnWidth / 2.0f) - (carW / 2.0f);
        float carY = 350.0f - carH;

        DrawEllipse(static_cast<int>(carX + carW / 2),
                    static_cast<int>(carY + carH),
                    40, 15, Fade(BLACK, 0.5f));
        DrawTextureEx(carTexs[i], {carX, carY}, 0.0f, shopScale, WHITE);
    }

    // Buttons
    DrawButton({150, 500, 200, 50}, isEnglish ? "SELECT" : "WAEHLEN", mousePoint, (data.selectedColorId == 0 ? GREEN : GRAY), BLACK);

    const char *redLabel = data.ownsRedCar ? (isEnglish ? "SELECT" : "WAEHLEN") : TextFormat("%d Stars", PRICE_RED_CAR);
    DrawButton(redBtn, redLabel, mousePoint, (data.selectedColorId == 1 ? GREEN : (data.ownsRedCar ? GRAY : MAROON)), (data.ownsRedCar ? BLACK : WHITE));

    const char *blueLabel = data.ownsBlueCar ? (isEnglish ? "SELECT" : "WAEHLEN") : TextFormat("%d Stars", PRICE_BLUE_CAR);
    DrawButton(blueBtn, blueLabel, mousePoint, (data.selectedColorId == 2 ? GREEN : (data.ownsBlueCar ? GRAY : DARKBLUE)), (data.ownsBlueCar ? BLACK : WHITE));

    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

//  Steuerungsübersicht
// ============================================================
void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backBtn, bool isEnglish)
{
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.85f));
    DrawTextCentered(isEnglish ? "HOW TO PLAY" : "STEUERUNG", 60, 45, GOLD);

    Color secColor = {180, 180, 180, 255};
    DrawTextCentered(isEnglish ? "-- CONTROLS --" : "-- TASTEN --", 130, 22, secColor);
    DrawTextCentered(isEnglish ? "LEFT / RIGHT ARROWS  -  Move car" : "LINKS / RECHTS Pfeile  -  Fahrzeug lenken", 162, 22, RAYWHITE);
    DrawTextCentered(isEnglish ? "P  -  Pause / Resume" : "P  -  Pause / Fortsetzen", 194, 22, RAYWHITE);
    DrawTextCentered(isEnglish ? "ESC  -  Menu / Quit" : "ESC  -  Menue / Beenden", 226, 22, RAYWHITE);

    DrawTextCentered(isEnglish ? "-- POWER-UPS --" : "-- POWER-UPS --", 276, 22, secColor);
    DrawTextCentered(isEnglish ? "CLOCK  -  Slows the game for 3 seconds" : "UHR  -  Verlangsamt das Spiel fuer 3 Sekunden", 308, 22, {100, 220, 255, 255});
    DrawTextCentered(isEnglish ? "SHIELD  -  Protects from one obstacle (5 sec)" : "SCHILD  -  Schuetzt vor einem Hindernis (5 Sek.)", 340, 22, GOLD);
    DrawTextCentered(isEnglish ? "STAR  -  Collect currency to buy new cars" : "STERN  -  Sammelwaehrung fuer den Autoladen", 372, 22, {255, 230, 80, 255});

    DrawTextCentered(isEnglish ? "-- ENVIRONMENT --" : "-- UMGEBUNG --", 422, 22, secColor);
    DrawTextCentered(isEnglish ? "Forest road  -  default environment" : "Waldstrasse  -  Standard-Umgebung", 454, 22, {100, 200, 100, 255});
    DrawTextCentered(isEnglish ? "5000+ points  -  environment changes to DESERT!" : "Ab 5000 Punkten  -  Umgebung wechselt zur WUESTE!", 486, 22, {230, 180, 60, 255});

    DrawTextCentered(isEnglish ? "-- SOUND --" : "-- SOUND --", 536, 22, secColor);
    DrawTextCentered(isEnglish ? "Pause (P)  -  open volume slider in pause menu" : "Pause (P)  -  Lautstaerke-Slider im Pause-Menue", 568, 22, RAYWHITE);

    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

void DrawPauseMenu(Vector2 mousePoint, Rectangle resumeBtn,
                   Rectangle menuBtn, bool isEnglish, float musicVolume)
{
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.5f));
    Rectangle box = {250, 220, 500, 400};
    DrawRectangleRec(box, {20, 20, 20, 220});
    DrawRectangleLinesEx(box, 2, GOLD);
    DrawTextCentered(isEnglish ? "PAUSED" : "PAUSE", 250, 50, RAYWHITE);

    DrawButton(resumeBtn, isEnglish ? "RESUME" : "WEITER", mousePoint, GRAY, BLACK);
    DrawButton(menuBtn, isEnglish ? "MENU" : "MENÜ", mousePoint, GRAY, BLACK);

    const char *volLabel = isEnglish ? "Volume:" : "Lautstaerke:";
    int labelW = MeasureText(volLabel, 20);
    DrawText(volLabel, (1000 - labelW) / 2, 543, 20, RAYWHITE);

    Rectangle track = {350, 575, 300, 16};
    DrawRectangleRec(track, {60, 60, 60, 255});
    DrawRectangle(350, 575, (int)(musicVolume * 300.0f), 16, {100, 180, 255, 255});
    DrawRectangleLinesEx(track, 2, {120, 120, 120, 255});

    int handleX = 350 + (int)(musicVolume * 300.0f);
    DrawCircle(handleX, 583, 10, WHITE);
    DrawCircleLines(handleX, 583, 10, LIGHTGRAY);
}

void DrawGameOverMenu(const char *name, int score, float time, int stars,
                      Vector2 mousePoint, Rectangle menuBtn, bool isEnglish)
{
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.7f));
    DrawTextCentered(isEnglish ? "GAME OVER" : "SPIEL VORBEI", 150, 60, RED);
    DrawTextCentered(TextFormat(isEnglish ? "Final Score: %d" : "Endstand: %d", score), 250, 30, RAYWHITE);
    DrawTextCentered(TextFormat(isEnglish ? "Stars Collected: %d" : "Sterne gesammelt: %d", stars), 300, 30, GOLD);
    DrawButton(menuBtn, isEnglish ? "MAIN MENU" : "HAUPTMENU", mousePoint, GRAY, BLACK);
}

void DrawScoreboardMenu(const std::vector<ScoreEntry> &scores,
                        Vector2 mousePoint, Rectangle backBtn, bool isEnglish)
{
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.8f));
    DrawTextCentered(isEnglish ? "TOP 10 SCORES" : "BESTENLISTE", 80, 50, GOLD);

    for (int i = 0; i < static_cast<int>(scores.size()); i++)
    {
        Color c = (i == 0) ? GOLD : (i == 1) ? LIGHTGRAY
                                : (i == 2)   ? BROWN
                                             : WHITE;
        DrawText(TextFormat("%d. %s", i + 1, scores[i].name), 300, 180 + i * 40, 25, c);
        DrawText(TextFormat("%d", scores[i].score), 600, 180 + i * 40, 25, c);
    }
    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}