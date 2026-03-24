#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "scoreboard.h"
#include "shop.h" 
#include <vector>

// Das HUD (Heads-Up Display) im Spiel
void DrawHUD(const char* playerName, float totalTimeSurvived, int score, int earnedStars);

// Die verschiedenen Menüs
void DrawMainMenu(const char* playerName, int letterCount, int framesCounter, Vector2 mousePoint, 
                  Rectangle inputBox, Rectangle startButton, Rectangle scoreBtn, 
                  Rectangle shopBtn, Rectangle settingsBtn, Rectangle descBtn, 
                  int totalStars, bool isNameSaved);
void DrawShopMenu(const SaveGame& saveData, Vector2 mousePoint, Rectangle btnBlue, Rectangle btnRed, Rectangle backMenuBtn);
void DrawScoreboardMenu(const std::vector<ScoreEntry>& scores, Vector2 mousePoint, Rectangle backMenuBtn);
void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backMenuBtn);
void DrawPauseMenu(Vector2 mousePoint, Rectangle btnPrimary, Rectangle btnMenu);
void DrawGameOverMenu(const char* playerName, int score, float totalTimeSurvived, int earnedStars, Vector2 mousePoint, Rectangle btnMenu);
void DrawSpecialMessage(const char* message);
void DrawSettingsMenu(Vector2 mousePoint, Rectangle deleteSaveBtn, Rectangle deleteScoreBtn, Rectangle backBtn);

#endif