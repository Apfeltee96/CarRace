#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "scoreboard.h"
#include "shop.h"
#include <vector>

void DrawHUD(const char* playerName, float time, int score, int stars, bool isEnglish);
void DrawMainMenu(const char* name, int letters, int frames, Vector2 mouse, Rectangle start, Rectangle score, Rectangle shop, Rectangle settings, Rectangle desc, int stars, bool saved, bool isEnglish);
void DrawSettingsMenu(Vector2 mouse, Rectangle lang, Rectangle delSave, Rectangle delScore, Rectangle back, bool isEnglish);
void DrawShopMenu(const SaveGame& data, Vector2 mouse, Rectangle btnB, Rectangle btnR, Rectangle back, bool isEnglish);
void DrawScoreboardMenu(const std::vector<ScoreEntry>& scores, Vector2 mouse, Rectangle back, bool isEnglish);
void DrawDescriptionMenu(Vector2 mouse, Rectangle back, bool isEnglish);
void DrawPauseMenu(Vector2 mouse, Rectangle btnP, Rectangle btnM, bool isEnglish);
void DrawGameOverMenu(const char* name, int score, float time, int stars, Vector2 mouse, Rectangle btnM, bool isEnglish);

#endif