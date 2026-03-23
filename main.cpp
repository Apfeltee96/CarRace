#include "raylib.h"
#include "scoreboard.h"
#include "player.h"
#include "ui.h" 
#include "shop.h" 
#include "config.h" // NEU: Unsere zentrale Schaltstelle
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

enum GameState { MAIN_MENU, SHOP_MENU, DESCRIPTION, SCOREBOARD_MENU, PLAYING, PAUSED, GAMEOVER };

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

    char playerName[MAX_NAME_LENGTH + 1] = "\0"; 
    bool isNameSaved = false;
    if (!saveData.lastPlayerName.empty() && saveData.lastPlayerName != "Gast") {
        std::strcpy(playerName, saveData.lastPlayerName.c_str());
        isNameSaved = true; 
    }
    
    int letterCount = (int)std::strlen(playerName); 
    int framesCounter = 0; 

    // 3. UI-Definitionen (Rectangles)
    Rectangle startButton = { SCREEN_WIDTH/2.0f - 100, 400, 200, 50 };
    Rectangle scoreBtn    = { SCREEN_WIDTH/2.0f - 100, 470, 200, 50 };
    Rectangle shopBtn     = { SCREEN_WIDTH/2.0f - 100, 540, 200, 50 };
    Rectangle descButton  = { SCREEN_WIDTH/2.0f - 100, 610, 200, 50 };
    Rectangle inputBox    = { SCREEN_WIDTH/2.0f - 125, 280, 250, 50 };
    Rectangle btnPrimary  = { SCREEN_WIDTH/2.0f - 125, 400, 250, 50 }; 
    Rectangle btnMenu     = { SCREEN_WIDTH/2.0f - 125, 480, 250, 50 }; 
    Rectangle shopBtnBlue = { SCREEN_WIDTH/2.0f - 100, 250, 200, 50 };
    Rectangle shopBtnRed  = { SCREEN_WIDTH/2.0f - 100, 350, 200, 50 };
    Rectangle backMenuBtn = { SCREEN_WIDTH/2.0f - 125, 680, 250, 50 };

    std::vector<Obstacle> obstacles(4);
    GameState currentState = MAIN_MENU;

    // 4. Game Loop
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
                    letterCount--;
                    if (letterCount < 0) letterCount = 0;
                    playerName[letterCount] = '\0';
                }
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, descButton)) currentState = DESCRIPTION;
                if (CheckCollisionPointRec(mousePoint, scoreBtn)) currentState = SCOREBOARD_MENU; 
                if (CheckCollisionPointRec(mousePoint, shopBtn)) currentState = SHOP_MENU; 
                
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
                    saveData.lastPlayerName = std::string(playerName);
                    SaveGameData(saveData);
                    isNameSaved = true; 
                    
                    InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT, GetCarColor(saveData.selectedColorId));
                    for (int i = 0; i < 4; i++) ResetObstacle(obstacles[i], -200.0f - (i * 350.0f));
                    currentState = PLAYING;
                }
            }
        }
        else if (currentState == SHOP_MENU) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, backMenuBtn)) currentState = MAIN_MENU;
                if (CheckCollisionPointRec(mousePoint, shopBtnBlue)) {
                    saveData.selectedColorId = 0;
                    SaveGameData(saveData);
                }
                if (CheckCollisionPointRec(mousePoint, shopBtnRed)) {
                    if (saveData.ownsRedCar) {
                        saveData.selectedColorId = 1;
                        SaveGameData(saveData);
                    } else if (saveData.totalStars >= PRICE_RED_CAR) { 
                        saveData.totalStars -= PRICE_RED_CAR;
                        saveData.ownsRedCar = true;
                        saveData.selectedColorId = 1; 
                        SaveGameData(saveData);
                    }
                }
            }
        }
        else if (currentState == DESCRIPTION || currentState == SCOREBOARD_MENU) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, backMenuBtn)) {
                currentState = MAIN_MENU;
            }
        }
      else if (currentState == PLAYING) {
            if (IsKeyPressed(KEY_P)) currentState = PAUSED;

            totalTimeSurvived += deltaTime; 
            currentScore = (int)(totalTimeSurvived * 50.0f); 

            // --- UPDATE DER GESCHWINDIGKEITEN ---
            currentSpeed = GetCurrentSpeed(currentScore);
            earnedStarsThisRound = CalculateStars(currentScore);
            
            // NEU: Die horizontale Bewegungsgeschwindigkeit anpassen!
            player.speed = GetPlayerSpeed(currentScore);
            // -------------------------------------

            // Steuerung nutzt nun den aktualisierten player.speed
            if (IsKeyDown(KEY_LEFT)) player.rect.x -= player.speed * deltaTime;
            if (IsKeyDown(KEY_RIGHT)) player.rect.x += player.speed * deltaTime;
            
            // ... (Rest des Codes wie bisher)

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
        else if (currentState == PAUSED) {
            if (IsKeyPressed(KEY_P)) currentState = PLAYING;
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, btnPrimary)) currentState = PLAYING; 
                else if (CheckCollisionPointRec(mousePoint, btnMenu)) {
                    AddOrUpdateScore(std::string(playerName), currentScore, totalTimeSurvived);
                    currentState = MAIN_MENU;
                }
            }
        }
        else if (currentState == GAMEOVER) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, btnMenu)) {
                currentState = MAIN_MENU;
            }
        }

        // --- DRAW LOGIK ---
        BeginDrawing();
        ClearBackground(DARKGREEN); 

        if (currentState == MAIN_MENU || currentState == DESCRIPTION || currentState == SCOREBOARD_MENU || currentState == SHOP_MENU) {
            DrawRectangle(ROAD_OFFSET, 0, ROAD_WIDTH, SCREEN_HEIGHT, DARKGRAY);
            DrawLineEx({(float)ROAD_OFFSET, 0}, {(float)ROAD_OFFSET, (float)SCREEN_HEIGHT}, 5, WHITE); 
            DrawLineEx({(float)(ROAD_OFFSET + ROAD_WIDTH), 0}, {(float)(ROAD_OFFSET + ROAD_WIDTH), (float)SCREEN_HEIGHT}, 5, WHITE);
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));

            if (currentState == MAIN_MENU) DrawMainMenu(playerName, letterCount, framesCounter, mousePoint, inputBox, startButton, scoreBtn, shopBtn, descButton, saveData.totalStars, isNameSaved);
            else if (currentState == SHOP_MENU) DrawShopMenu(saveData, mousePoint, shopBtnBlue, shopBtnRed, backMenuBtn);
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