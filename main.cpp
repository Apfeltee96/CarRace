#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#include "scoreboard.h"
#include "player.h"
#include "ui.h" 
#include "shop.h" 
#include "config.h" 
#include <vector>
#include <string>
#include <cstring>

const int SCREEN_WIDTH = 1000; 
const int SCREEN_HEIGHT = 800;
const int ROAD_WIDTH = 400;
const int ROAD_OFFSET = (SCREEN_WIDTH - ROAD_WIDTH) / 2;
const int MAX_NAME_LENGTH = 15; 

struct Obstacle { Rectangle rect; Color color; };
struct CollectableStar { Rectangle rect; bool active; };
enum GameState { MAIN_MENU, SHOP_MENU, DESCRIPTION, SCOREBOARD_MENU, SETTINGS, PLAYING, PAUSED, GAMEOVER };

// TUNNEL VARIABLEN
float tunnelGapX = ROAD_OFFSET + 150; 
float tunnelGapWidth = 120.0f;        

bool IsPositionOccupied(Rectangle newRect, const std::vector<Obstacle>& obstacles) {
    for (const auto& obs : obstacles) {
        if (CheckCollisionRecs(newRect, {obs.rect.x - 10, obs.rect.y - 20, obs.rect.width + 20, obs.rect.height + 40})) return true;
    }
    return false;
}

void ResetObstacle(Obstacle &obs, float startY, const std::vector<Obstacle>& allObstacles, int score, int index) {
    if (score < 5000) {
        obs.rect.width = (float)GetRandomValue(70, 100);
        obs.rect.height = 40.0f;
        obs.rect.x = (float)GetRandomValue(ROAD_OFFSET + 5, (ROAD_OFFSET + ROAD_WIDTH) - (int)obs.rect.width - 5);
        obs.rect.y = startY; // Hier nutzen wir jetzt den festen Wert für einheitliche Abstände
    } 
    else {
        obs.rect.height = 60.0f;
        tunnelGapX += GetRandomValue(-30, 30);
        if (tunnelGapX < ROAD_OFFSET + 20) tunnelGapX = ROAD_OFFSET + 20;
        if (tunnelGapX > (ROAD_OFFSET + ROAD_WIDTH) - tunnelGapWidth - 20) tunnelGapX = (ROAD_OFFSET + ROAD_WIDTH) - tunnelGapWidth - 20;

        if (index % 2 == 0) {
            obs.rect.x = (float)ROAD_OFFSET;
            obs.rect.width = tunnelGapX - ROAD_OFFSET;
        } else {
            obs.rect.x = tunnelGapX + tunnelGapWidth;
            obs.rect.width = (ROAD_OFFSET + ROAD_WIDTH) - obs.rect.x;
        }
        obs.rect.y = startY;
    }
    obs.color = (score < 5000) ? RED : MAROON;
}

void SpawnStar(CollectableStar &star, const std::vector<Obstacle>& obstacles) {
    star.rect = { (float)GetRandomValue(ROAD_OFFSET + 50, ROAD_OFFSET + ROAD_WIDTH - 80), -100, 30, 30 };
    star.active = true;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Car Race");
    SetTargetFPS(60);

    SaveGame saveData = LoadSaveGame();
    Player player;
    InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT, GetCarColor(saveData.selectedColorId));

    int currentScore = 0;          
    float currentSpeed = SPEED_START; 
    float totalTimeSurvived = 0.0f; 
    int earnedStarsThisRound = 0; 
    char playerName[MAX_NAME_LENGTH + 1] = "\0"; 
    bool isNameSaved = false;

    if (!saveData.lastPlayerName.empty() && saveData.lastPlayerName != "Gast") {
        std::strcpy(playerName, saveData.lastPlayerName.c_str());
        isNameSaved = true; 
    }
    
    int letterCount = (int)std::strlen(playerName); 
    int framesCounter = 0; 

    // UI Rects
    Rectangle startButton = { SCREEN_WIDTH/2.0f - 100, 380, 200, 50 };
    Rectangle scoreBtn    = { SCREEN_WIDTH/2.0f - 100, 440, 200, 50 };
    Rectangle shopBtn     = { SCREEN_WIDTH/2.0f - 100, 500, 200, 50 };
    Rectangle settingsBtn = { SCREEN_WIDTH/2.0f - 100, 560, 200, 50 }; 
    Rectangle descButton  = { SCREEN_WIDTH/2.0f - 100, 620, 200, 50 };
    Rectangle langBtn         = { SCREEN_WIDTH/2.0f - 125, 200, 250, 50 };
    Rectangle deleteSaveBtn   = { SCREEN_WIDTH/2.0f - 125, 300, 250, 50 };
    Rectangle deleteScoreBtn  = { SCREEN_WIDTH/2.0f - 125, 400, 250, 50 };
    Rectangle backSettingsBtn = { SCREEN_WIDTH/2.0f - 125, 550, 250, 50 };
    Rectangle btnPrimary  = { SCREEN_WIDTH/2.0f - 125, 400, 250, 50 }; 
    Rectangle btnMenu     = { SCREEN_WIDTH/2.0f - 125, 480, 250, 50 }; 
    Rectangle backMenuBtn = { SCREEN_WIDTH/2.0f - 125, 680, 250, 50 };

    std::vector<Obstacle> obstacles(4);
    CollectableStar bonusStar = {{0,0,30,30}, false};
    GameState currentState = MAIN_MENU;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        framesCounter++;
        Vector2 mousePoint = GetMousePosition(); 

        if (currentState == MAIN_MENU) {
            if (!isNameSaved) {
                int key = GetCharPressed();
                while (key > 0) {
                    if ((key > 32) && (key <= 125) && (letterCount < MAX_NAME_LENGTH)) {
                        playerName[letterCount] = (char)key;
                        playerName[letterCount+1] = '\0';
                        letterCount++;
                    }
                    key = GetCharPressed(); 
                }
                if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) { letterCount--; playerName[letterCount] = '\0'; }
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, startButton) && (letterCount > 0 || isNameSaved)) {
                    currentScore = 0; totalTimeSurvived = 0.0f; earnedStarsThisRound = 0; currentSpeed = SPEED_START;
                    saveData.lastPlayerName = std::string(playerName); SaveGameData(saveData); isNameSaved = true; 
                    InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT, GetCarColor(saveData.selectedColorId));
                    
                    // --- EINHEITLICHE INITIALISIERUNG ---
                    // Jedes Hindernis startet genau 250 Pixel hinter dem anderen
                    for (int i = 0; i < 4; i++) {
                        ResetObstacle(obstacles[i], -150.0f - (i * 250.0f), obstacles, 0, i);
                    }
                    currentState = PLAYING;
                }
                else if (CheckCollisionPointRec(mousePoint, scoreBtn)) currentState = SCOREBOARD_MENU; 
                else if (CheckCollisionPointRec(mousePoint, shopBtn)) currentState = SHOP_MENU; 
                else if (CheckCollisionPointRec(mousePoint, settingsBtn)) currentState = SETTINGS;
                else if (CheckCollisionPointRec(mousePoint, descButton)) currentState = DESCRIPTION;
            }
        }
        else if (currentState == SETTINGS) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, backSettingsBtn)) currentState = MAIN_MENU;
                if (CheckCollisionPointRec(mousePoint, langBtn)) { saveData.isEnglish = !saveData.isEnglish; SaveGameData(saveData); }
                if (CheckCollisionPointRec(mousePoint, deleteSaveBtn)) {
                    DeleteSaveData(); saveData = {0, false, 0, "", saveData.isEnglish}; 
                    playerName[0] = '\0'; letterCount = 0; isNameSaved = false; currentState = MAIN_MENU; 
                }
                if (CheckCollisionPointRec(mousePoint, deleteScoreBtn)) ClearScoreboard();
            }
        }
        else if (currentState == PLAYING) {
            if (IsKeyPressed(KEY_P)) currentState = PAUSED;
            totalTimeSurvived += deltaTime; currentScore = (int)(totalTimeSurvived * 50.0f); 
            currentSpeed = GetCurrentSpeed(currentScore, currentSpeed, deltaTime);
            player.speed = GetPlayerSpeed(currentScore);

            if (IsKeyDown(KEY_LEFT)) player.rect.x -= player.speed * deltaTime;
            if (IsKeyDown(KEY_RIGHT)) player.rect.x += player.speed * deltaTime;
            if (player.rect.x < ROAD_OFFSET) player.rect.x = (float)ROAD_OFFSET;
            if (player.rect.x > ROAD_OFFSET + ROAD_WIDTH - player.rect.width) player.rect.x = (float)ROAD_OFFSET + ROAD_WIDTH - player.rect.width;

            for (int i = 0; i < 4; i++) {
                obstacles[i].rect.y += currentSpeed * deltaTime;
                if (obstacles[i].rect.y > SCREEN_HEIGHT) {
                    // --- EINHEITLICHER RESPAWN ---
                    // Das Hindernis spawnt oben mit einem festen Abstand zum "höchsten" Hindernis
                    float highestY = 0;
                    for (int j = 0; j < 4; j++) { if (obstacles[j].rect.y < highestY) highestY = obstacles[j].rect.y; }
                    
                    // Wir setzen es exakt 250 Pixel über das aktuell oberste Hindernis
                    ResetObstacle(obstacles[i], highestY - 250.0f, obstacles, currentScore, i);
                }
                if (CheckCollisionRecs(player.rect, obstacles[i].rect)) {
                    AddOrUpdateScore(playerName, currentScore, totalTimeSurvived);
                    saveData.totalStars += earnedStarsThisRound; SaveGameData(saveData);
                    currentState = GAMEOVER; 
                }
            }
            if (!bonusStar.active && GetRandomValue(0, 500) < 3) SpawnStar(bonusStar, obstacles);
            if (bonusStar.active) {
                bonusStar.rect.y += currentSpeed * deltaTime;
                if (CheckCollisionRecs(player.rect, bonusStar.rect)) { earnedStarsThisRound++; bonusStar.active = false; }
                if (bonusStar.rect.y > SCREEN_HEIGHT) bonusStar.active = false;
            }
        }
        else if (currentState == PAUSED || currentState == GAMEOVER) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, btnMenu)) currentState = MAIN_MENU;
                if (currentState == PAUSED && CheckCollisionPointRec(mousePoint, btnPrimary)) currentState = PLAYING;
            }
        }

        BeginDrawing();
        ClearBackground(DARKGREEN); 
        DrawRectangle(ROAD_OFFSET, 0, ROAD_WIDTH, SCREEN_HEIGHT, DARKGRAY);
        DrawLineEx({(float)ROAD_OFFSET, 0}, {(float)ROAD_OFFSET, (float)SCREEN_HEIGHT}, 5, WHITE); 
        DrawLineEx({(float)(ROAD_OFFSET + ROAD_WIDTH), 0}, {(float)(ROAD_OFFSET + ROAD_WIDTH), (float)SCREEN_HEIGHT}, 5, WHITE);

        if (currentState == PLAYING || currentState == PAUSED || currentState == GAMEOVER) {
            DrawRectangleRec(player.rect, player.color);
            for (int i = 0; i < 4; i++) DrawRectangleRec(obstacles[i].rect, obstacles[i].color);
            if (bonusStar.active) DrawPoly({bonusStar.rect.x + 15, bonusStar.rect.y + 15}, 5, 15, 0, YELLOW);
            DrawHUD(playerName, totalTimeSurvived, currentScore, earnedStarsThisRound, saveData.isEnglish);
            if (currentState == PAUSED) DrawPauseMenu(mousePoint, btnPrimary, btnMenu, saveData.isEnglish);
            else if (currentState == GAMEOVER) DrawGameOverMenu(playerName, currentScore, totalTimeSurvived, earnedStarsThisRound, mousePoint, btnMenu, saveData.isEnglish);
        } else {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));
            if (currentState == MAIN_MENU) DrawMainMenu(playerName, letterCount, framesCounter, mousePoint, startButton, scoreBtn, shopBtn, settingsBtn, descButton, saveData.totalStars, isNameSaved, saveData.isEnglish);
            else if (currentState == SETTINGS) DrawSettingsMenu(mousePoint, langBtn, deleteSaveBtn, deleteScoreBtn, backSettingsBtn, saveData.isEnglish);
            else if (currentState == SHOP_MENU) DrawShopMenu(saveData, mousePoint, {SCREEN_WIDTH/2.0f-100, 250, 200, 50}, {SCREEN_WIDTH/2.0f-100, 350, 200, 50}, backMenuBtn, saveData.isEnglish);
            else if (currentState == SCOREBOARD_MENU) DrawScoreboardMenu(LoadScoreboard(), mousePoint, backMenuBtn, saveData.isEnglish);
            else if (currentState == DESCRIPTION) DrawDescriptionMenu(mousePoint, backMenuBtn, saveData.isEnglish);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}