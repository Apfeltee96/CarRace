#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "scoreboard.h"
#include "config.h"
#include <vector>

// ============================================================
//  Hilfsfunktionen
// ============================================================

/// Zeichnet Text horizontal zentriert bei gegebenem Y-Wert.
void DrawTextCentered(const char* text, int y, int fontSize, Color color);

/// Zeichnet einen Button mit Hover-Effekt (gold bei Hover).
void DrawButton(Rectangle rect, const char* text, Vector2 mouse,
                Color baseColor, Color textColor);

// ============================================================
//  HUD (während des Spiels)
// ============================================================

/// Zeichnet das HUD mit Fahrername, Zeit, Score, Sternen und Highscore.
void DrawHUD(const char* name, float time, int score, int stars,
             bool isEnglish, Texture2D starTex, int highScore);

// ============================================================
//  Menüs
// ============================================================

/// Hauptmenü mit Namenseingabe und Navigationsbuttons.
void DrawMainMenu(const char* name, int letterCount, int framesCounter,
                  Vector2 mousePoint, Rectangle startBtn, Rectangle scoreBtn,
                  Rectangle shopBtn, Rectangle settingsBtn, Rectangle descBtn,
                  int totalStars, bool nameSaved, bool isEnglish);

/// Einstellungsmenü (Sprache, Vollbild, Name, Datenlöschung).
void DrawSettingsMenu(Vector2 mousePoint, Rectangle langBtn, Rectangle resBtn,
                      Rectangle nameChangeBtn, Rectangle deleteDataBtn,
                      Rectangle backBtn, bool isEnglish, bool isFullscreen);

/// Shop-Menü mit Werkstatt-Hintergrund und Autoauswahl.
/// @param data       Aktueller Spielstand (wird für Besitz/Auswahl gelesen)
/// @param carTexs    Array mit den drei Auto-Texturen [weiß, rot, lila]
void DrawShopMenu(const SaveGame& data, Vector2 mousePoint,
                  Rectangle redBtn, Rectangle blueBtn, Rectangle backBtn,
                  bool isEnglish, Texture2D carTexs[]);

/// Hintergrund der Werkstatt (Wand, Boden, Fliesen, Sicherheitsstreifen).
void DrawWorkshopBackground();

/// Steuerungsübersicht.
void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backBtn, bool isEnglish);

/// Pausemenü.
void DrawPauseMenu(Vector2 mousePoint, Rectangle resumeBtn,
                   Rectangle menuBtn, bool isEnglish);

/// Game-Over-Bildschirm.
void DrawGameOverMenu(const char* name, int score, float time, int stars,
                      Vector2 mousePoint, Rectangle menuBtn, bool isEnglish);

/// Bestenliste (Top 10).
void DrawScoreboardMenu(const std::vector<ScoreEntry>& scores,
                        Vector2 mousePoint, Rectangle backBtn, bool isEnglish);

#endif // UI_H
