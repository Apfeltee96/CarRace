#include "raylib.h"
#include "level.h"
#include "scoreboard.h"
#include "player.h"
#include "ui.h" // NEU: Unser Interface-Manager!
#include <vector>
#include <string>

const int SCREEN_WIDTH = 800; 
const int SCREEN_HEIGHT = 800;
const int ROAD_WIDTH = 400;
const int ROAD_OFFSET = (SCREEN_WIDTH - ROAD_WIDTH) / 2;
const int MAX_NAME_LENGTH = 15; 

struct Obstacle {
    Rectangle rect;
    Color color;
};

enum GameState { MAIN_MENU, DESCRIPTION, SCOREBOARD_MENU, PLAYING, PAUSED, GAMEOVER };

void ResetObstacle(Obstacle &obs, float startY) {
    obs.rect.width = (float)GetRandomValue(60, 120);
    obs.rect.height = 40.0f;
    obs.rect.x = (float)GetRandomValue(ROAD_OFFSET, ROAD_OFFSET + ROAD_WIDTH - (int)obs.rect.width);
    obs.rect.y = startY; 
    obs.color = RED;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Car Race");
    SetTargetFPS(60);

    Player player;
    InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT);

    int currentLevel = 1;          
    float levelTimer = GetLevelDuration();        
    float currentSpeed = 400.0f; 
    float totalTimeSurvived = 0.0f; 

    char playerName[MAX_NAME_LENGTH + 1] = "\0"; 
    int letterCount = 0; 
    int framesCounter = 0; 

    Rectangle startButton = { SCREEN_WIDTH/2.0f - 100, 420, 200, 50 };
    Rectangle scoreBtn    = { SCREEN_WIDTH/2.0f - 100, 490, 200, 50 };
    Rectangle descButton  = { SCREEN_WIDTH/2.0f - 100, 560, 200, 50 };
    Rectangle inputBox    = { SCREEN_WIDTH/2.0f - 125, 300, 250, 50 };
    
    Rectangle btnPrimary  = { SCREEN_WIDTH/2.0f - 125, 400, 250, 50 }; 
    Rectangle btnMenu     = { SCREEN_WIDTH/2.0f - 125, 480, 250, 50 }; 

    std::vector<Obstacle> obstacles(4);
    GameState currentState = MAIN_MENU;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        framesCounter++;
        Vector2 mousePoint = GetMousePosition(); 

        // --- UPDATE ---
        if (currentState == MAIN_MENU) {
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

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, descButton)) currentState = DESCRIPTION;
                if (CheckCollisionPointRec(mousePoint, scoreBtn)) currentState = SCOREBOARD_MENU; 
                
                if (CheckCollisionPointRec(mousePoint, startButton) && letterCount > 0) {
                    currentLevel = 1;
                    levelTimer = GetLevelDuration();
                    totalTimeSurvived = 0.0f; 
                    
                    InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT);
                    
                    for (int i = 0; i < 4; i++) ResetObstacle(obstacles[i], -200.0f - (i * 350.0f));
                    currentState = PLAYING;
                }
            }
        }
        else if (currentState == DESCRIPTION || SCOREBOARD_MENU == currentState) {
            Rectangle backMenuBtn = { SCREEN_WIDTH/2.0f - 125, 680, 250, 50 };
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, backMenuBtn)) {
                currentState = MAIN_MENU;
            }
        }
        else if (currentState == PLAYING) {
            if (IsKeyPressed(KEY_P)) currentState = PAUSED;

            levelTimer -= deltaTime;
            totalTimeSurvived += deltaTime; 

            currentSpeed = CalculateObstacleSpeed(currentLevel, levelTimer); 

            if (levelTimer <= 0.0f) {
                currentLevel++;
                levelTimer = GetLevelDuration(); 
            }

            if (IsKeyDown(KEY_LEFT)) player.rect.x -= player.speed * deltaTime;
            if (IsKeyDown(KEY_RIGHT)) player.rect.x += player.speed * deltaTime;

            if (player.rect.x < ROAD_OFFSET) player.rect.x = ROAD_OFFSET;
            if (player.rect.x > ROAD_OFFSET + ROAD_WIDTH - player.rect.width) player.rect.x = ROAD_OFFSET + ROAD_WIDTH - player.rect.width;

            for (int i = 0; i < 4; i++) {
                obstacles[i].rect.y += currentSpeed * deltaTime;

                if (obstacles[i].rect.y > SCREEN_HEIGHT) {
                    float highestY = 0;
                    for (int j = 0; j < 4; j++) {
                        if (obstacles[j].rect.y < highestY) highestY = obstacles[j].rect.y;
                    }
                    float neuerAbstand = (float)GetRandomValue(300, 450);
                    ResetObstacle(obstacles[i], highestY - neuerAbstand); 
                }

                if (CheckCollisionRecs(player.rect, obstacles[i].rect)) {
                    AddOrUpdateScore(std::string(playerName), currentLevel, totalTimeSurvived);
                    currentState = GAMEOVER; 
                }
            }
        } 
        else if (currentState == PAUSED) {
            if (IsKeyPressed(KEY_P)) currentState = PLAYING;
            
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, btnPrimary)) {
                    currentState = PLAYING; 
                }
                else if (CheckCollisionPointRec(mousePoint, btnMenu)) {
                    AddOrUpdateScore(std::string(playerName), currentLevel, totalTimeSurvived);
                    currentState = MAIN_MENU;
                }
            }
        }
        else if (currentState == GAMEOVER) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, btnMenu)) {
                currentState = MAIN_MENU;
            }
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(DARKGREEN); // Rasen

        if (currentState == MAIN_MENU || currentState == DESCRIPTION || currentState == SCOREBOARD_MENU) {
            // Hintergrund für Menüs
            DrawRectangle(ROAD_OFFSET, 0, ROAD_WIDTH, SCREEN_HEIGHT, DARKGRAY);
            DrawLineEx({(float)ROAD_OFFSET, 0}, {(float)ROAD_OFFSET, (float)SCREEN_HEIGHT}, 5, WHITE); 
            DrawLineEx({(float)(ROAD_OFFSET + ROAD_WIDTH), 0}, {(float)(ROAD_OFFSET + ROAD_WIDTH), (float)SCREEN_HEIGHT}, 5, WHITE);
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));

            if (currentState == MAIN_MENU) {
                DrawMainMenu(playerName, letterCount, framesCounter, mousePoint, inputBox, startButton, scoreBtn, descButton);
            }
            else if (currentState == SCOREBOARD_MENU) {
                std::vector<ScoreEntry> scores = LoadScoreboard();
                Rectangle backMenuBtn = { SCREEN_WIDTH/2.0f - 125, 680, 250, 50 };
                DrawScoreboardMenu(scores, mousePoint, backMenuBtn);
            }
            else if (currentState == DESCRIPTION) {
                Rectangle backMenuBtn = { SCREEN_WIDTH/2.0f - 125, 680, 250, 50 };
                DrawDescriptionMenu(mousePoint, backMenuBtn);
            }
        }
        else {
            // Hintergrund für das Spiel
            DrawRectangle(ROAD_OFFSET, 0, ROAD_WIDTH, SCREEN_HEIGHT, DARKGRAY);
            DrawLineEx({(float)ROAD_OFFSET, 0}, {(float)ROAD_OFFSET, (float)SCREEN_HEIGHT}, 5, WHITE); 
            DrawLineEx({(float)(ROAD_OFFSET + ROAD_WIDTH), 0}, {(float)(ROAD_OFFSET + ROAD_WIDTH), (float)SCREEN_HEIGHT}, 5, WHITE);

            // Spieler und Hindernisse
            DrawRectangleRec(player.rect, player.color);
            for (int i = 0; i < 4; i++) DrawRectangleRec(obstacles[i].rect, obstacles[i].color);

            // NEU: Einfach ein einziger Aufruf für das gesamte HUD!
            DrawHUD(playerName, totalTimeSurvived, currentLevel, levelTimer);

            if (currentState == PAUSED) {
                DrawPauseMenu(mousePoint, btnPrimary, btnMenu);
            }
            else if (currentState == GAMEOVER) {
                DrawGameOverMenu(playerName, currentLevel, totalTimeSurvived, mousePoint, btnMenu);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}