#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <vector>
#include "scoreboard.h"

void DrawHUD(const char* name, float time, int score, int stars, bool isEnglish);
void DrawMainMenu(const char* name, int letterCount, int framesCounter, Vector2 mousePoint, Rectangle startBtn, Rectangle scoreBtn, Rectangle shopBtn, Rectangle settingsBtn, Rectangle descBtn, int totalStars, bool nameSaved, bool isEnglish);
void DrawSettingsMenu(Vector2 mousePoint, Rectangle langBtn, Rectangle nameChangeBtn, Rectangle deleteDataBtn, Rectangle backBtn, bool isEnglish);
void DrawScoreboardMenu(std::vector<ScoreEntry> scores, Vector2 mousePoint, Rectangle backBtn, bool isEnglish);
void DrawDescriptionMenu(Vector2 mousePoint, Rectangle backBtn, bool isEnglish);
void DrawPauseMenu(Vector2 mousePoint, Rectangle resumeBtn, Rectangle menuBtn, bool isEnglish);
void DrawGameOverMenu(const char* name, int score, float time, int stars, Vector2 mousePoint, Rectangle menuBtn, bool isEnglish);

#endif