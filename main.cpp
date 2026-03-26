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

void ResetObstacle(Obstacle &obs, float startY) {
    obs.rect.width = (float)GetRandomValue(70, 110);
    obs.rect.height = 40.0f;
    obs.rect.x = (float)GetRandomValue(ROAD_OFFSET + 10, (ROAD_OFFSET + ROAD_WIDTH) - (int)obs.rect.width - 10);
    obs.rect.y = startY; 
    obs.color = RED;
}

void SpawnStar(CollectableStar &star, const std::vector<Obstacle>& obstacles) {
    bool positionSafe = false;
    int attempts = 0;
    while (!positionSafe && attempts < 20) {
        star.rect = { (float)GetRandomValue(ROAD_OFFSET + 20, ROAD_OFFSET + ROAD_WIDTH - 50), -100, 30, 30 };
        positionSafe = true;
        for (const auto& obs : obstacles) {
            Rectangle dangerZone = { obs.rect.x - 50, obs.rect.y - 50, obs.rect.width + 100, obs.rect.height + 100 };
            if (CheckCollisionRecs(star.rect, dangerZone)) { positionSafe = false; break; }
        }
        attempts++;
    }
    star.active = true;
}

int main() {
    // 1. Erst Daten laden, dann Fenster initialisieren
    SaveGame saveData = LoadSaveGame();
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Car Race");
    
    // Vollbild-Check beim Start
    if (saveData.isFullscreen && !IsWindowFullscreen()) {
        ToggleFullscreen();
    }
    
    SetTargetFPS(60);

    Player player;
    InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT, GetCarColor(saveData.selectedColorId));

    int currentScore = 0;          
    float currentSpeed = SPEED_START; 
    float totalTimeSurvived = 0.0f; 
    int earnedStarsThisRound = 0;
    char playerName[MAX_NAME_LENGTH + 1] = "\0"; 
    bool isNameSaved = false;

    if (!saveData.lastPlayerName.empty() && saveData.lastPlayerName != "Gast") {
        std::strncpy(playerName, saveData.lastPlayerName.c_str(), MAX_NAME_LENGTH);
        isNameSaved = true; 
    }
    
    int letterCount = (int)std::strlen(playerName); 

    // --- BUTTON DEFINITIONEN ---
    float centerX = SCREEN_WIDTH / 2.0f - 125;
    float menuCenterX = SCREEN_WIDTH / 2.0f - 100;

    // Hauptmenü
    Rectangle startButton = { menuCenterX, 300, 200, 50 };
    Rectangle scoreBtn    = { menuCenterX, 360, 200, 50 };
    Rectangle shopBtn     = { menuCenterX, 420, 200, 50 };
    Rectangle settingsBtn = { menuCenterX, 480, 200, 50 }; 
    Rectangle descButton  = { menuCenterX, 540, 200, 50 };

    // Einstellungen (Settings)
    Rectangle langBtn         = { centerX, 180, 250, 50 };
    Rectangle resBtn          = { centerX, 250, 250, 50 }; 
    Rectangle nameChangeBtn   = { centerX, 320, 250, 50 }; 
    Rectangle deleteDataBtn   = { centerX, 390, 250, 50 }; 
    Rectangle backSettingsBtn = { centerX, 550, 250, 50 };

    // Spiel-Over / Pause
    Rectangle btnPrimary  = { centerX, 400, 250, 50 }; 
    Rectangle btnMenu     = { centerX, 480, 250, 50 }; 
    Rectangle backMenuBtn = { centerX, 680, 250, 50 };

    std::vector<Obstacle> obstacles(4); 
    CollectableStar bonusStar = {{0,0,30,30}, false};
    GameState currentState = MAIN_MENU;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        Vector2 mousePoint = GetMousePosition(); 

        if (currentState == MAIN_MENU) {
            if (!isNameSaved) {
                int key = GetCharPressed();
                while (key > 0) {
                    if ((key > 32) && (key <= 125) && (letterCount < MAX_NAME_LENGTH)) {
                        playerName[letterCount] = (char)key; playerName[letterCount+1] = '\0'; letterCount++;
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
                    for (int i = 0; i < (int)obstacles.size(); i++) ResetObstacle(obstacles[i], -200.0f - (i * 400.0f));
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
                
                if (CheckCollisionPointRec(mousePoint, langBtn)) { 
                    saveData.isEnglish = !saveData.isEnglish; 
                    SaveGameData(saveData); 
                }

                // AUFLÖSUNG UMSCHALTEN
                if (CheckCollisionPointRec(mousePoint, resBtn)) {
                    saveData.isFullscreen = !saveData.isFullscreen;
                    ToggleFullscreen();
                    SaveGameData(saveData);
                }
                
                if (CheckCollisionPointRec(mousePoint, nameChangeBtn)) {
                    isNameSaved = false;
                    playerName[0] = '\0';
                    letterCount = 0;
                    currentState = MAIN_MENU;
                }

                if (CheckCollisionPointRec(mousePoint, deleteDataBtn)) {
                    DeleteSaveData();
                    ClearScoreboard();
                    if (IsWindowFullscreen()) ToggleFullscreen();
                    saveData = { 0, false, false, 0, saveData.isEnglish, false, "" };
                    playerName[0] = '\0';
                    letterCount = 0;
                    isNameSaved = false;
                    currentState = MAIN_MENU;
                }
            }
        }
        else if (currentState == SHOP_MENU) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, backMenuBtn)) currentState = MAIN_MENU;
                
                // Shop Logik Buttons
                if (CheckCollisionPointRec(mousePoint, {400, 200, 200, 50})) { saveData.selectedColorId = 0; SaveGameData(saveData); }
                if (CheckCollisionPointRec(mousePoint, {400, 300, 200, 50})) {
                    if (saveData.ownsRedCar) { saveData.selectedColorId = 1; }
                    else if (saveData.totalStars >= 100) { saveData.totalStars -= 100; saveData.ownsRedCar = true; saveData.selectedColorId = 1; }
                    SaveGameData(saveData);
                }
                if (CheckCollisionPointRec(mousePoint, {400, 400, 200, 50})) {
                    if (saveData.ownsPurpleCar) { saveData.selectedColorId = 2; }
                    else if (saveData.totalStars >= 200) { saveData.totalStars -= 200; saveData.ownsPurpleCar = true; saveData.selectedColorId = 2; }
                    SaveGameData(saveData);
                }
                InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT, GetCarColor(saveData.selectedColorId));
            }
        }
        else if (currentState == SCOREBOARD_MENU || currentState == DESCRIPTION) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, backMenuBtn)) currentState = MAIN_MENU;
        }
        else if (currentState == PLAYING) {
            if (IsKeyPressed(KEY_P)) currentState = PAUSED;
            totalTimeSurvived += deltaTime; currentScore = (int)(totalTimeSurvived * 50.0f); 
            currentSpeed = GetCurrentSpeed(currentScore, currentSpeed, deltaTime);
            player.speed = GetDynamicPlayerSpeed(currentSpeed);

            if (IsKeyDown(KEY_LEFT)) player.rect.x -= player.speed * deltaTime;
            if (IsKeyDown(KEY_RIGHT)) player.rect.x += player.speed * deltaTime;
            if (player.rect.x < ROAD_OFFSET) player.rect.x = (float)ROAD_OFFSET;
            if (player.rect.x > (ROAD_OFFSET + ROAD_WIDTH) - player.rect.width) player.rect.x = (float)(ROAD_OFFSET + ROAD_WIDTH) - player.rect.width;

            for (int i = 0; i < (int)obstacles.size(); i++) {
                obstacles[i].rect.y += currentSpeed * deltaTime;
                if (obstacles[i].rect.y > SCREEN_HEIGHT) {
                    float highestY = 0;
                    for (int j = 0; j < (int)obstacles.size(); j++) { if (obstacles[j].rect.y < highestY) highestY = obstacles[j].rect.y; }
                    ResetObstacle(obstacles[i], highestY - 400.0f);
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

        if (currentState == PLAYING || currentState == PAUSED || currentState == GAMEOVER) {
            for (int i = 0; i < (int)obstacles.size(); i++) DrawRectangleRec(obstacles[i].rect, obstacles[i].color);
            DrawRectangleRec(player.rect, player.color);
            if (bonusStar.active) DrawPoly({bonusStar.rect.x + 15, bonusStar.rect.y + 15}, 5, 15, 0, YELLOW);
            DrawHUD(playerName, totalTimeSurvived, currentScore, earnedStarsThisRound, saveData.isEnglish);
            if (currentState == PAUSED) DrawPauseMenu(mousePoint, btnPrimary, btnMenu, saveData.isEnglish);
            else if (currentState == GAMEOVER) DrawGameOverMenu(playerName, currentScore, totalTimeSurvived, earnedStarsThisRound, mousePoint, btnMenu, saveData.isEnglish);
        } else {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));
            if (currentState == MAIN_MENU) DrawMainMenu(playerName, letterCount, 0, mousePoint, startButton, scoreBtn, shopBtn, settingsBtn, descButton, saveData.totalStars, isNameSaved, saveData.isEnglish);
            else if (currentState == SETTINGS) DrawSettingsMenu(mousePoint, langBtn, resBtn, nameChangeBtn, deleteDataBtn, backSettingsBtn, saveData.isEnglish, saveData.isFullscreen);
            else if (currentState == SHOP_MENU) DrawShopMenu(saveData, mousePoint, {400, 300, 200, 50}, {400, 400, 200, 50}, backMenuBtn, saveData.isEnglish);
            else if (currentState == SCOREBOARD_MENU) DrawScoreboardMenu(LoadScoreboard(), mousePoint, backMenuBtn, saveData.isEnglish);
            else if (currentState == DESCRIPTION) DrawDescriptionMenu(mousePoint, backMenuBtn, saveData.isEnglish);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}