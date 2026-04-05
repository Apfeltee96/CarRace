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
             (int)(rect.x + (rect.width  - tw)      / 2),
             (int)(rect.y + (rect.height - fontSize) / 2),
             fontSize, textColor);
}

// Zeichnet einen Ja/Nein-Button-Bereich für Bestätigungsdialoge.
// danger=true → roter Stil (Löschen/Beenden), false → goldener Stil.
static void DrawConfirmButtons(Vector2 mouse,
                                Rectangle yBtn, const char *yText, int yFontSize,
                                Rectangle nBtn, const char *nText,
                                bool danger)
{
    Color yBase   = danger ? MAROON                  : DARKGRAY;
    Color yHover  = danger ? RED                     : GOLD;
    Color yBorder = danger ? Color{255, 80, 80, 200} : GOLD;

    DrawRectangleRec(yBtn, CheckCollisionPointRec(mouse, yBtn) ? yHover : yBase);
    DrawRectangleRec(nBtn, CheckCollisionPointRec(mouse, nBtn) ? GRAY   : DARKGRAY);
    DrawRectangleLinesEx(yBtn, 2, yBorder);
    DrawRectangleLinesEx(nBtn, 2, {160, 160, 160, 200});

    int yw = MeasureText(yText, yFontSize);
    DrawText(yText, (int)(yBtn.x + (yBtn.width - yw) / 2), (int)(yBtn.y + 14), yFontSize, WHITE);
    int nw = MeasureText(nText, 22);
    DrawText(nText, (int)(nBtn.x + (nBtn.width - nw) / 2), (int)(nBtn.y + 14), 22, WHITE);
}

// ============================================================
//  HUD
// ============================================================

void DrawHUD(const char *name, float time, int score, int stars,
             bool isEnglish, Texture2D starTex, int highScore)
{
    constexpr int fontSize    = 22;
    constexpr int margin      = 20;
    constexpr int rightAlignX = 800;

    DrawText(TextFormat(isEnglish ? "Driver: %s" : "Fahrer: %s", name), margin, 20, fontSize, RAYWHITE);
    DrawText(TextFormat(isEnglish ? "Time: %.1fs" : "Zeit: %.1fs", time), margin, 50, fontSize, RAYWHITE);

    DrawText(isEnglish ? "Score:" : "Punkte:", rightAlignX, 20, fontSize, RAYWHITE);
    DrawText(TextFormat("%d", score), rightAlignX + 100, 20, fontSize, RAYWHITE);

    float iconScale = 25.0f / (float)starTex.height;
    DrawTextureEx(starTex, {(float)rightAlignX, 50.0f}, 0.0f, iconScale, WHITE);
    DrawText(TextFormat(": %d", stars), rightAlignX + 35, 50, fontSize, GOLD);
}

// ============================================================
//  Hauptmenü
// ============================================================

void DrawMainMenu(const char *name, int letterCount, int framesCounter,
                  Vector2 mousePoint, Rectangle startBtn, Rectangle scoreBtn,
                  Rectangle shopBtn, Rectangle settingsBtn, Rectangle descBtn,
                  int totalStars, bool nameSaved, bool isEnglish)
{
    DrawTextCentered(isEnglish ? "CAR RACE" : "AUTORENNEN", 100, 50, GOLD);
    DrawButton(startBtn,    isEnglish ? "START"       : "START",         mousePoint, GRAY, BLACK);
    DrawButton(scoreBtn,    isEnglish ? "SCORES"      : "BESTENLISTE",   mousePoint, GRAY, BLACK);
    DrawButton(shopBtn,     isEnglish ? "SHOP"        : "SHOP",          mousePoint, GRAY, BLACK);
    DrawButton(settingsBtn, isEnglish ? "SETTINGS"    : "EINSTELLUNGEN", mousePoint, GRAY, BLACK);
    DrawButton(descBtn,     isEnglish ? "HOW TO PLAY" : "STEUERUNG",     mousePoint, GRAY, BLACK);

    if (!nameSaved)
    {
        DrawText(isEnglish ? "Please enter name here:" : "Hier bitte Namen eingeben:", 350, 190, 18, RAYWHITE);
        DrawRectangle(350, 220, 300, 40, LIGHTGRAY);
        DrawRectangleLines(350, 220, 300, 40, BLACK);
        DrawText(name, 360, 230, 20, BLACK);
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

// ============================================================
//  Einstellungen
// ============================================================

void DrawSettingsMenu(Vector2 mousePoint, Rectangle langBtn, Rectangle resBtn,
                      Rectangle nameChangeBtn, Rectangle deleteDataBtn,
                      Rectangle backBtn, bool isEnglish, bool isFullscreen,
                      bool showNameChangeConfirm, bool showDeleteDataConfirm)
{
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.6f));
    DrawTextCentered(isEnglish ? "SETTINGS" : "EINSTELLUNGEN", 80, 50, GOLD);
    DrawButton(langBtn,       isEnglish ? "Language: English" : "Sprache: Deutsch",                        mousePoint, GRAY, BLACK);
    DrawButton(resBtn,        isFullscreen ? (isEnglish ? "Window Mode" : "Fenstermodus")
                                           : (isEnglish ? "Fullscreen"  : "Vollbild"),                     mousePoint, GRAY, BLACK);
    DrawButton(nameChangeBtn, isEnglish ? "Change Name"  : "Name aendern",                                 mousePoint, GRAY, BLACK);
    DrawButton(deleteDataBtn, isEnglish ? "DELETE DATA"  : "DATEN LOESCHEN",                               mousePoint, RED,  RAYWHITE);
    DrawButton(backBtn,       isEnglish ? "BACK"         : "ZURUECK",                                      mousePoint, GRAY, BLACK);

    if (showNameChangeConfirm)
    {
        DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.55f));
        Rectangle box = {300, 290, 400, 200};
        DrawRectangleRec(box, {25, 25, 25, 245});
        DrawRectangleLinesEx(box, 3, GOLD);
        DrawTextCentered(isEnglish ? "CHANGE NAME?"                : "NAMEN AENDERN?",            325, 26, RAYWHITE);
        DrawTextCentered(isEnglish ? "You will re-enter your name." : "Du gibst deinen Namen neu ein.", 363, 20, {180, 180, 180, 255});
        DrawConfirmButtons(mousePoint,
                           {330, 420, 150, 50}, isEnglish ? "YES" : "JA",   22,
                           {520, 420, 150, 50}, isEnglish ? "NO"  : "NEIN", false);
    }

    if (showDeleteDataConfirm)
    {
        DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.55f));
        Rectangle box = {240, 230, 520, 300};
        DrawRectangleRec(box, {25, 25, 25, 245});
        DrawRectangleLinesEx(box, 3, RED);
        DrawTextCentered(isEnglish ? "DELETE ALL DATA?"       : "ALLE DATEN LOESCHEN?",       260, 26, RED);
        DrawTextCentered(isEnglish ? "This will delete:"      : "Folgendes wird geloescht:",   300, 20, {200, 200, 200, 255});
        DrawTextCentered(isEnglish ? "- Player name"          : "- Spielername",               328, 20, RAYWHITE);
        DrawTextCentered(isEnglish ? "- Scoreboard"           : "- Bestenliste",               352, 20, RAYWHITE);
        DrawTextCentered(isEnglish ? "- All stars & cars"     : "- Alle Sterne & Autos",       376, 20, RAYWHITE);
        DrawTextCentered(isEnglish ? "This cannot be undone!" : "Nicht rueckgaengig machbar!", 406, 18, {255, 100, 100, 255});
        DrawConfirmButtons(mousePoint,
                           {290, 460, 150, 50}, isEnglish ? "YES, DELETE" : "JA, LOESCHEN", 20,
                           {480, 460, 150, 50}, isEnglish ? "NO"          : "NEIN",         true);
    }
}

// ============================================================
//  Shop-Menü
// ============================================================

void DrawWorkshopBackground()
{
    DrawRectangle(0, 0, 1000, 400, DARKGRAY);
    DrawRectangle(0, 400, 1000, 400, GRAY);
    DrawRectangle(0, 395, 1000, 10, BLACK);
    for (int i = 0; i < 10; i++)
        DrawRectangle(i * 100, 400, 5, 400, DARKGRAY);
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
        float gameScale = 45.0f / (float)carTexs[i].width;
        float shopScale = gameScale * 2.5f;
        float carW = carTexs[i].width  * shopScale;
        float carH = carTexs[i].height * shopScale;
        float carX = (btnXPositions[i] + btnWidth / 2.0f) - (carW / 2.0f);
        float carY = 350.0f - carH;

        DrawEllipse((int)(carX + carW / 2), (int)(carY + carH), 40, 15, Fade(BLACK, 0.5f));
        DrawTextureEx(carTexs[i], {carX, carY}, 0.0f, shopScale, WHITE);
    }

    DrawButton({150, 500, 200, 50}, isEnglish ? "SELECT" : "WAEHLEN", mousePoint,
               (data.selectedColorId == 0 ? GREEN : GRAY), BLACK);

    const char *redLabel  = data.ownsRedCar  ? (isEnglish ? "SELECT" : "WAEHLEN") : TextFormat("%d Stars", PRICE_RED_CAR);
    const char *blueLabel = data.ownsBlueCar ? (isEnglish ? "SELECT" : "WAEHLEN") : TextFormat("%d Stars", PRICE_BLUE_CAR);

    DrawButton(redBtn,  redLabel,  mousePoint,
               data.selectedColorId == 1 ? GREEN : (data.ownsRedCar  ? GRAY : MAROON),
               data.ownsRedCar  ? BLACK : WHITE);
    DrawButton(blueBtn, blueLabel, mousePoint,
               data.selectedColorId == 2 ? GREEN : (data.ownsBlueCar ? GRAY : DARKBLUE),
               data.ownsBlueCar ? BLACK : WHITE);

    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

// ============================================================
//  Steuerungsübersicht
// ============================================================

void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backBtn, bool isEnglish)
{
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.85f));
    DrawTextCentered(isEnglish ? "HOW TO PLAY" : "STEUERUNG", 60, 45, GOLD);

    Color sec = {180, 180, 180, 255};
    DrawTextCentered(isEnglish ? "-- CONTROLS --"    : "-- TASTEN --",    130, 22, sec);
    DrawTextCentered(isEnglish ? "LEFT / RIGHT ARROWS  -  Move car"
                               : "LINKS / RECHTS Pfeile  -  Fahrzeug lenken",                162, 22, RAYWHITE);
    DrawTextCentered(isEnglish ? "ESC  -  Pause menu (resume, menu, volume, quit)"
                               : "ESC  -  Pause-Menue (Weiter, Menue, Lautstaerke, Beenden)", 194, 22, RAYWHITE);

    DrawTextCentered(isEnglish ? "-- POWER-UPS --"   : "-- POWER-UPS --", 250, 22, sec);
    DrawTextCentered(isEnglish ? "CLOCK  -  Slows the game for 3 seconds"
                               : "UHR  -  Verlangsamt das Spiel fuer 3 Sekunden",            282, 22, {100, 220, 255, 255});
    DrawTextCentered(isEnglish ? "SHIELD  -  Protects from one obstacle (5 sec)"
                               : "SCHILD  -  Schuetzt vor einem Hindernis (5 Sek.)",         314, 22, GOLD);
    DrawTextCentered(isEnglish ? "STAR  -  Collect currency to buy new cars"
                               : "STERN  -  Sammelwaehrung fuer den Autoladen",              346, 22, {255, 230, 80, 255});

    DrawTextCentered(isEnglish ? "-- ENVIRONMENT --"  : "-- UMGEBUNG --", 396, 22, sec);
    DrawTextCentered(isEnglish ? "Forest road  -  default environment"
                               : "Waldstrasse  -  Standard-Umgebung",                        428, 22, {100, 200, 100, 255});
    DrawTextCentered(isEnglish ? "5000+ points  -  environment changes to DESERT!"
                               : "Ab 5000 Punkten  -  Umgebung wechselt zur WUESTE!",        460, 22, {230, 180, 60, 255});

    DrawTextCentered(isEnglish ? "-- SOUND --"        : "-- SOUND --",    510, 22, sec);
    DrawTextCentered(isEnglish ? "ESC  -  open pause menu with volume slider"
                               : "ESC  -  Pause-Menue mit Lautstaerke-Slider oeffnen",       542, 22, RAYWHITE);
    DrawTextCentered(isEnglish ? "Music starts when the game begins"
                               : "Musik startet erst beim Spielstart",                        574, 20, {160, 160, 160, 255});

    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}

// ============================================================
//  Pause-Menü
// ============================================================

void DrawPauseMenu(Vector2 mousePoint, Rectangle resumeBtn,
                   Rectangle menuBtn, Rectangle quitBtn,
                   bool isEnglish, float musicVolume,
                   bool showQuitConfirm, bool showPauseBackConfirm)
{
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.5f));
    Rectangle box = {250, 210, 500, 370};
    DrawRectangleRec(box, {20, 20, 20, 230});
    DrawRectangleLinesEx(box, 2, GOLD);
    DrawTextCentered(isEnglish ? "PAUSED" : "PAUSE", 228, 42, RAYWHITE);

    DrawButton(resumeBtn, isEnglish ? "RESUME"       : "WEITER",            mousePoint, GRAY,            BLACK);
    DrawButton(menuBtn,   isEnglish ? "BACK TO MENU" : "ZURUECK ZUM MENUE", mousePoint, GRAY,            BLACK);
    DrawButton(quitBtn,   isEnglish ? "QUIT GAME"    : "SPIEL BEENDEN",     mousePoint, {130, 30, 30, 255}, WHITE);

    // Lautstärke-Slider
    const char *volLabel = isEnglish ? "Volume:" : "Lautstaerke:";
    DrawText(volLabel, (1000 - MeasureText(volLabel, 20)) / 2, 495, 20, RAYWHITE);
    Rectangle track = {350, 520, 300, 16};
    DrawRectangleRec(track, {60, 60, 60, 255});
    DrawRectangle(350, 520, (int)(musicVolume * 300.0f), 16, {100, 180, 255, 255});
    DrawRectangleLinesEx(track, 2, {120, 120, 120, 255});
    int handleX = 350 + (int)(musicVolume * 300.0f);
    DrawCircle(handleX, 528, 10, WHITE);
    DrawCircleLines(handleX, 528, 10, LIGHTGRAY);

    if (showPauseBackConfirm)
    {
        DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.55f));
        Rectangle confirmBox = {300, 290, 400, 200};
        DrawRectangleRec(confirmBox, {25, 25, 25, 245});
        DrawRectangleLinesEx(confirmBox, 3, GOLD);
        DrawTextCentered(isEnglish ? "BACK TO MENU?"             : "ZURUECK ZUM MENUE?",           325, 26, RAYWHITE);
        DrawTextCentered(isEnglish ? "Current run will be lost."  : "Aktueller Lauf geht verloren.", 363, 20, {180, 180, 180, 255});
        DrawConfirmButtons(mousePoint,
                           {330, 420, 150, 50}, isEnglish ? "YES" : "JA",   22,
                           {520, 420, 150, 50}, isEnglish ? "NO"  : "NEIN", false);
    }

    if (showQuitConfirm)
    {
        DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.55f));
        Rectangle confirmBox = {300, 290, 400, 220};
        DrawRectangleRec(confirmBox, {25, 25, 25, 245});
        DrawRectangleLinesEx(confirmBox, 3, RED);
        DrawTextCentered(isEnglish ? "REALLY QUIT?"               : "WIRKLICH BEENDEN?",       330, 26, RAYWHITE);
        DrawTextCentered(isEnglish ? "All progress will be lost."  : "Fortschritt geht verloren.", 368, 20, {180, 180, 180, 255});
        DrawConfirmButtons(mousePoint,
                           {330, 420, 150, 50}, isEnglish ? "YES" : "JA",   22,
                           {520, 420, 150, 50}, isEnglish ? "NO"  : "NEIN", true);
    }
}

// ============================================================
//  Game Over
// ============================================================

void DrawGameOverMenu(const char *name, int score, float time, int stars,
                      Vector2 mousePoint, Rectangle menuBtn, bool isEnglish)
{
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.7f));
    DrawTextCentered(isEnglish ? "GAME OVER"             : "SPIEL VORBEI",          150, 60, RED);
    DrawTextCentered(TextFormat(isEnglish ? "Final Score: %d"     : "Endstand: %d",         score), 250, 30, RAYWHITE);
    DrawTextCentered(TextFormat(isEnglish ? "Stars Collected: %d" : "Sterne gesammelt: %d", stars), 300, 30, GOLD);
    DrawButton(menuBtn, isEnglish ? "MAIN MENU" : "HAUPTMENU", mousePoint, GRAY, BLACK);
}

// ============================================================
//  Bestenliste
// ============================================================

void DrawScoreboardMenu(const std::vector<ScoreEntry> &scores,
                        Vector2 mousePoint, Rectangle backBtn, bool isEnglish)
{
    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.8f));
    DrawTextCentered(isEnglish ? "TOP 10 SCORES" : "BESTENLISTE", 80, 50, GOLD);

    for (int i = 0; i < (int)scores.size(); i++)
    {
        Color c = (i == 0) ? GOLD : (i == 1) ? LIGHTGRAY : (i == 2) ? BROWN : WHITE;
        DrawText(TextFormat("%d. %s", i + 1, scores[i].name), 300, 180 + i * 40, 25, c);
        DrawText(TextFormat("%d", scores[i].score),            600, 180 + i * 40, 25, c);
    }
    DrawButton(backBtn, isEnglish ? "BACK" : "ZURUECK", mousePoint, GRAY, BLACK);
}
