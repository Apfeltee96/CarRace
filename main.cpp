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

// --- KONSTANTEN ---
const int SCREEN_WIDTH = 1000; 
const int SCREEN_HEIGHT = 800;
const int ROAD_WIDTH = 400;
const int ROAD_OFFSET = (SCREEN_WIDTH - ROAD_WIDTH) / 2;
const int MAX_NAME_LENGTH = 15; 

struct Obstacle {
    Rectangle rect;
    Color color;
};

enum GameState { MAIN_MENU, SHOP_MENU, DESCRIPTION, SCOREBOARD_MENU, SETTINGS, PLAYING, PAUSED, GAMEOVER };

void ResetObstacle(Obstacle &obs, float startY) {
    obs.rect.width = (float)GetRandomValue(60, 120);
    obs.rect.height = 40.0f;
    obs.rect.x = (float)GetRandomValue(ROAD_OFFSET, ROAD_OFFSET + ROAD_WIDTH - (int)obs.rect.width);
    obs.rect.y = startY; 
    obs.color = RED;
}

int main() {
    // 1. Initialisierung
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Car Race");
    SetTargetFPS(60);

    SaveGame saveData = LoadSaveGame();
    Player player;
    InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT, GetCarColor(saveData.selectedColorId));

    // 2. Spiel-Variablen
    int currentScore = 0;          
    float currentSpeed = SPEED_START; 
    float totalTimeSurvived = 0.0f; 
    int earnedStarsThisRound = 0; 
    float messageTimer = 0.0f;
    bool messageShown = false;

    char playerName[MAX_NAME_LENGTH + 1] = "\0"; 
    bool isNameSaved = false;
    if (!saveData.lastPlayerName.empty() && saveData.lastPlayerName != "Gast") {
        std::strcpy(playerName, saveData.lastPlayerName.c_str());
        isNameSaved = true; 
    }
    
    int letterCount = (int)std::strlen(playerName); 
    int framesCounter = 0; 

    // --- UI RECTANGLES ---
    Rectangle startButton = { SCREEN_WIDTH/2.0f - 100, 380, 200, 50 };
    Rectangle scoreBtn    = { SCREEN_WIDTH/2.0f - 100, 440, 200, 50 };
    Rectangle shopBtn     = { SCREEN_WIDTH/2.0f - 100, 500, 200, 50 };
    Rectangle settingsBtn = { SCREEN_WIDTH/2.0f - 100, 560, 200, 50 }; 
    Rectangle descButton  = { SCREEN_WIDTH/2.0f - 100, 620, 200, 50 };
    
    // Buttons für das Settings-Menü
    Rectangle deleteSaveBtn   = { SCREEN_WIDTH/2.0f - 125, 250, 250, 50 };
    Rectangle deleteScoreBtn  = { SCREEN_WIDTH/2.0f - 125, 370, 250, 50 };
    Rectangle backSettingsBtn = { SCREEN_WIDTH/2.0f - 125, 550, 250, 50 };

    Rectangle inputBox    = { SCREEN_WIDTH/2.0f - 125, 280, 250, 50 };
    Rectangle btnPrimary  = { SCREEN_WIDTH/2.0f - 125, 400, 250, 50 }; 
    Rectangle btnMenu     = { SCREEN_WIDTH/2.0f - 125, 480, 250, 50 }; 
    Rectangle backMenuBtn = { SCREEN_WIDTH/2.0f - 125, 680, 250, 50 };

    std::vector<Obstacle> obstacles(4);
    GameState currentState = MAIN_MENU;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        framesCounter++;
        Vector2 mousePoint = GetMousePosition(); 

        // --- UPDATE LOGIK ---
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
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (letterCount > 0) {
                        letterCount--;
                        playerName[letterCount] = '\0';
                    }
                }
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, descButton)) currentState = DESCRIPTION;
                else if (CheckCollisionPointRec(mousePoint, scoreBtn)) currentState = SCOREBOARD_MENU; 
                else if (CheckCollisionPointRec(mousePoint, shopBtn)) currentState = SHOP_MENU; 
                else if (CheckCollisionPointRec(mousePoint, settingsBtn)) currentState = SETTINGS;
                
                if (isNameSaved) {
                    const char* welcomeText = TextFormat("Willkommen zurueck, %s!", playerName);
                    int welcomeX = (int)(SCREEN_WIDTH/2 - MeasureText(welcomeText, 25)/2);
                    Rectangle editBtn = { (float)(welcomeX + MeasureText(welcomeText, 25) + 15), (float)(inputBox.y + 10), 80, 25 };
                    if (CheckCollisionPointRec(mousePoint, editBtn)) {
                        isNameSaved = false; 
                        letterCount = (int)std::strlen(playerName); 
                    }
                }

                if (CheckCollisionPointRec(mousePoint, startButton) && (letterCount > 0 || isNameSaved)) {
                    currentScore = 0;
                    totalTimeSurvived = 0.0f;
                    earnedStarsThisRound = 0;
                    messageTimer = 0.0f;
                    messageShown = false; 
                    saveData.lastPlayerName = std::string(playerName);
                    SaveGameData(saveData);
                    isNameSaved = true; 
                    InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT, GetCarColor(saveData.selectedColorId));
                    for (int i = 0; i < 4; i++) ResetObstacle(obstacles[i], -200.0f - (i * 350.0f));
                    currentState = PLAYING;
                }
            }
        }
        else if (currentState == SETTINGS) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, backSettingsBtn)) currentState = MAIN_MENU;
                
                // SPIELSTAND LÖSCHEN
                if (CheckCollisionPointRec(mousePoint, deleteSaveBtn)) {
                    DeleteSaveData(); 
                    saveData = {0, false, 0, ""}; 
                    playerName[0] = '\0';
                    letterCount = 0;
                    isNameSaved = false;
                    currentState = MAIN_MENU; 
                }

                // SCOREBOARD LÖSCHEN
                if (CheckCollisionPointRec(mousePoint, deleteScoreBtn)) {
                    ClearScoreboard();
                }
            }
        }
        else if (currentState == SHOP_MENU || currentState == DESCRIPTION || currentState == SCOREBOARD_MENU) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, backMenuBtn)) {
                currentState = MAIN_MENU;
            }
        }
        else if (currentState == PLAYING) {
            if (IsKeyPressed(KEY_P)) currentState = PAUSED;
            totalTimeSurvived += deltaTime; 
            currentScore = (int)(totalTimeSurvived * 50.0f); 
            currentSpeed = GetCurrentSpeed(currentScore);
            earnedStarsThisRound = CalculateStars(currentScore);
            player.speed = GetPlayerSpeed(currentScore);

            if (IsKeyDown(KEY_LEFT)) player.rect.x -= player.speed * deltaTime;
            if (IsKeyDown(KEY_RIGHT)) player.rect.x += player.speed * deltaTime;

            if (player.rect.x < ROAD_OFFSET) player.rect.x = (float)ROAD_OFFSET;
            if (player.rect.x > ROAD_OFFSET + ROAD_WIDTH - player.rect.width) player.rect.x = (float)ROAD_OFFSET + ROAD_WIDTH - player.rect.width;

            for (int i = 0; i < 4; i++) {
                obstacles[i].rect.y += currentSpeed * deltaTime;
                if (obstacles[i].rect.y > SCREEN_HEIGHT) {
                    float highestY = 0;
                    for (int j = 0; j < 4; j++) { if (obstacles[j].rect.y < highestY) highestY = obstacles[j].rect.y; }
                    ResetObstacle(obstacles[i], highestY - (float)GetRandomValue(350, 500)); 
                }
                if (CheckCollisionRecs(player.rect, obstacles[i].rect)) {
                    AddOrUpdateScore(std::string(playerName), currentScore, totalTimeSurvived);
                    saveData.totalStars += earnedStarsThisRound;
                    SaveGameData(saveData);
                    currentState = GAMEOVER; 
                }
            }
        }
        else if (currentState == PAUSED || currentState == GAMEOVER) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, btnMenu)) currentState = MAIN_MENU;
            if (currentState == PAUSED && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, btnPrimary)) currentState = PLAYING;
        }

        // --- ZEICHNEN ---
        BeginDrawing();
        ClearBackground(DARKGREEN); 

        if (currentState != PLAYING && currentState != PAUSED && currentState != GAMEOVER) {
            DrawRectangle(ROAD_OFFSET, 0, ROAD_WIDTH, SCREEN_HEIGHT, DARKGRAY);
            DrawLineEx({(float)ROAD_OFFSET, 0}, {(float)ROAD_OFFSET, (float)SCREEN_HEIGHT}, 5, WHITE); 
            DrawLineEx({(float)(ROAD_OFFSET + ROAD_WIDTH), 0}, {(float)(ROAD_OFFSET + ROAD_WIDTH), (float)SCREEN_HEIGHT}, 5, WHITE);
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));

            if (currentState == MAIN_MENU) DrawMainMenu(playerName, letterCount, framesCounter, mousePoint, inputBox, startButton, scoreBtn, shopBtn, settingsBtn, descButton, saveData.totalStars, isNameSaved);
            else if (currentState == SETTINGS) DrawSettingsMenu(mousePoint, deleteSaveBtn, deleteScoreBtn, backSettingsBtn);
            else if (currentState == SHOP_MENU) DrawShopMenu(saveData, mousePoint, {SCREEN_WIDTH/2.0f-100, 250, 200, 50}, {SCREEN_WIDTH/2.0f-100, 350, 200, 50}, backMenuBtn);
            else if (currentState == SCOREBOARD_MENU) DrawScoreboardMenu(LoadScoreboard(), mousePoint, backMenuBtn);
            else if (currentState == DESCRIPTION) DrawDescriptionMenu(mousePoint, backMenuBtn);
        }
        else {
            DrawRectangle(ROAD_OFFSET, 0, ROAD_WIDTH, SCREEN_HEIGHT, DARKGRAY);
            DrawLineEx({(float)ROAD_OFFSET, 0}, {(float)ROAD_OFFSET, (float)SCREEN_HEIGHT}, 5, WHITE); 
            DrawLineEx({(float)(ROAD_OFFSET + ROAD_WIDTH), 0}, {(float)(ROAD_OFFSET + ROAD_WIDTH), (float)SCREEN_HEIGHT}, 5, WHITE);
            DrawRectangleRec(player.rect, player.color);
            for (int i = 0; i < 4; i++) DrawRectangleRec(obstacles[i].rect, obstacles[i].color);
            DrawHUD(playerName, totalTimeSurvived, currentScore, earnedStarsThisRound);
            if (currentState == PAUSED) DrawPauseMenu(mousePoint, btnPrimary, btnMenu);
            else if (currentState == GAMEOVER) DrawGameOverMenu(playerName, currentScore, totalTimeSurvived, earnedStarsThisRound, mousePoint, btnMenu);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}