#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "scoreboard.h"
#include "config.h"
#include <vector>

// --- Hilfsfunktionen ---
void DrawTextCentered(const char *text, int y, int fontSize, Color color);
void DrawButton(Rectangle rect, const char *text, Vector2 mouse,
                Color baseColor, Color textColor);

// --- HUD & In-Game UI ---
void DrawHUD(const char *name, float time, int score, int stars,
             bool isEnglish, Texture2D starTex, int highScore);

void DrawPauseMenu(Vector2 mousePoint, Rectangle resumeBtn,
                   Rectangle menuBtn, Rectangle quitBtn,
                   bool isEnglish, float musicVolume,
                   bool showQuitConfirm, bool showPauseBackConfirm);

// --- Menüs ---
void DrawMainMenu(const char *name, int letterCount, int framesCounter,
                  Vector2 mousePoint, Rectangle startBtn, Rectangle scoreBtn,
                  Rectangle shopBtn, Rectangle settingsBtn, Rectangle descBtn,
                  int totalStars, bool nameSaved, bool isEnglish);

void DrawSettingsMenu(Vector2 mousePoint, Rectangle langBtn, Rectangle resBtn,
                      Rectangle nameChangeBtn, Rectangle deleteDataBtn,
                      Rectangle backBtn, bool isEnglish, bool isFullscreen,
                      bool showNameChangeConfirm, bool showDeleteDataConfirm);

void DrawShopMenu(const SaveGame &data, Vector2 mousePoint,
                  Rectangle redBtn, Rectangle blueBtn, Rectangle backBtn,
                  bool isEnglish, Texture2D carTexs[]);

void DrawWorkshopBackground();
void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backBtn, bool isEnglish);

void DrawGameOverMenu(const char *name, int score, float time, int stars,
                      Vector2 mousePoint, Rectangle menuBtn, bool isEnglish);

void DrawScoreboardMenu(const std::vector<ScoreEntry> &scores,
                        Vector2 mousePoint, Rectangle backBtn, bool isEnglish);

#endif 
