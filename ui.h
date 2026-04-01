#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <vector>
#include "scoreboard.h"
#include "config.h" // WICHTIG: Hier muss config.h rein, damit SaveGame bekannt ist

// Hilfsfunktionen
void DrawTextCentered(const char* text, int y, int fontSize, Color color);

// Menü- und HUD-Funktionen
// Stelle sicher, dass am Ende "int highScore" steht
void DrawHUD(const char* name, float time, int score, int stars, bool isEnglish, Texture2D starTex, int highScore);

void DrawMainMenu(const char* name, int letterCount, int framesCounter, Vector2 mousePoint, 
                  Rectangle startBtn, Rectangle scoreBtn, Rectangle shopBtn, 
                  Rectangle settingsBtn, Rectangle descBtn, int totalStars, 
                  bool nameSaved, bool isEnglish);

void DrawSettingsMenu(Vector2 mousePoint, Rectangle langBtn, Rectangle resBtn, 
                      Rectangle nameChangeBtn, Rectangle deleteDataBtn, Rectangle backBtn, 
                      bool isEnglish, bool isFullscreen);

void DrawScoreboardMenu(std::vector<ScoreEntry> scores, Vector2 mousePoint, 
                        Rectangle backBtn, bool isEnglish);

void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backBtn, bool isEnglish);

void DrawPauseMenu(Vector2 mousePoint, Rectangle resumeBtn, Rectangle menuBtn, bool isEnglish);
void DrawButton(Rectangle rect, const char* text, Vector2 mouse, Color baseColor, Color textColor);
void DrawGameOverMenu(const char* name, int score, float time, int stars, 
                      Vector2 mousePoint, Rectangle menuBtn, bool isEnglish);

void DrawShopMenu(SaveGame data, Vector2 mousePoint, Rectangle redBtn, Rectangle whiteBtn, 
                  Rectangle backBtn, bool isEnglish, Texture2D carTexs[]);
void DrawWorkshopBackground();

#endif