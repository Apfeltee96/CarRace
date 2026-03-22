#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "scoreboard.h"
#include <vector>

// Das HUD (Heads-Up Display) im Spiel
void DrawHUD(const char* playerName, float totalTimeSurvived, int currentLevel, float levelTimer);

// Die verschiedenen Menüs
void DrawMainMenu(const char* playerName, int letterCount, int framesCounter, Vector2 mousePoint, Rectangle inputBox, Rectangle startButton, Rectangle scoreBtn, Rectangle descButton);
void DrawScoreboardMenu(const std::vector<ScoreEntry>& scores, Vector2 mousePoint, Rectangle backMenuBtn);
void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backMenuBtn);
void DrawPauseMenu(Vector2 mousePoint, Rectangle btnPrimary, Rectangle btnMenu);
void DrawGameOverMenu(const char* playerName, int currentLevel, float totalTimeSurvived, Vector2 mousePoint, Rectangle btnMenu);

#endif