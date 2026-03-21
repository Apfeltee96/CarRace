#include "raylib.h"
#include <vector>
#include <string>

// --- KONSTANTEN ---
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;
const int ROAD_WIDTH = 400;
const int ROAD_OFFSET = (SCREEN_WIDTH - ROAD_WIDTH) / 2;
const int MAX_NAME_LENGTH = 15; 

struct Player {
    Rectangle rect;
    float speed;
    Color color;
};

struct Obstacle {
    Rectangle rect;
    Color color;
};

enum GameState { MAIN_MENU, DESCRIPTION, PLAYING, PAUSED, GAMEOVER, LEVEL_CLEARED };

void ResetObstacle(Obstacle &obs, float startY) {
    obs.rect.width = (float)GetRandomValue(60, 120);
    obs.rect.height = 40.0f;
    obs.rect.x = (float)GetRandomValue(ROAD_OFFSET, ROAD_OFFSET + ROAD_WIDTH - (int)obs.rect.width);
    obs.rect.y = startY; 
    obs.color = RED;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CarRace - Level Menue");
    SetTargetFPS(60);

    Player player;
    player.rect.width = 60;
    player.rect.height = 100;
    player.rect.x = (SCREEN_WIDTH / 2) - (player.rect.width / 2);
    player.rect.y = SCREEN_HEIGHT - 150;
    player.speed = 400.0f;
    player.color = BLUE;

    int currentLevel = 1;          
    float timeLeft = 60.0f;        
    float baseSpeed = 200.0f;      
    float currentSpeed = baseSpeed; 

    char playerName[MAX_NAME_LENGTH + 1] = "\0"; 
    int letterCount = 0; 
    int framesCounter = 0; 

    // Alle Button-Rechtecke definieren
    Rectangle startButton = { SCREEN_WIDTH/2.0f - 100, 450, 200, 50 };
    Rectangle descButton = { SCREEN_WIDTH/2.0f - 100, 520, 200, 50 };
    Rectangle backButton = { SCREEN_WIDTH/2.0f - 100, 650, 200, 50 };
    Rectangle inputBox = { SCREEN_WIDTH/2.0f - 125, 300, 250, 50 };
    
    // NEU: Buttons für den Level-Geschafft-Bildschirm
    Rectangle nextLevelButton = { SCREEN_WIDTH/2.0f - 125, 400, 250, 50 };
    Rectangle returnMenuButton = { SCREEN_WIDTH/2.0f - 125, 480, 250, 50 };

    std::vector<Obstacle> obstacles(4);
    
    GameState currentState = MAIN_MENU;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        framesCounter++;

        Vector2 mousePoint = GetMousePosition(); 

        // --- UPDATE (Logik) ---
        if (currentState == MAIN_MENU) {
            
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_NAME_LENGTH)) {
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
                if (CheckCollisionPointRec(mousePoint, descButton)) {
                    currentState = DESCRIPTION;
                }
                
                if (CheckCollisionPointRec(mousePoint, startButton) && letterCount > 0) {
                    currentLevel = 1;
                    timeLeft = 60.0f;
                    player.rect.x = (SCREEN_WIDTH / 2) - (player.rect.width / 2);
                    for (int i = 0; i < 4; i++) {
                        ResetObstacle(obstacles[i], -200.0f - (i * 350.0f));
                    }
                    currentState = PLAYING;
                }
            }
        }
        else if (currentState == DESCRIPTION) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, backButton)) {
                currentState = MAIN_MENU;
            }
        }
        else if (currentState == PLAYING) {
            if (IsKeyPressed(KEY_P)) {
                currentState = PAUSED;
            }

            timeLeft -= deltaTime;

            float levelBonusSpeed = (currentLevel - 1) * 20.0f; 
            currentSpeed = baseSpeed + levelBonusSpeed + ((60.0f - timeLeft) * 3.0f); 

            if (timeLeft <= 0.0f) {
                timeLeft = 0.0f;                 
                currentState = LEVEL_CLEARED; 
            }

            if (IsKeyDown(KEY_LEFT)) player.rect.x -= player.speed * deltaTime;
            if (IsKeyDown(KEY_RIGHT)) player.rect.x += player.speed * deltaTime;

            if (player.rect.x < ROAD_OFFSET) player.rect.x = ROAD_OFFSET;
            if (player.rect.x > ROAD_OFFSET + ROAD_WIDTH - player.rect.width) {
                player.rect.x = ROAD_OFFSET + ROAD_WIDTH - player.rect.width;
            }

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
                    currentState = GAMEOVER; 
                }
            }
        } 
        else if (currentState == PAUSED) {
            if (IsKeyPressed(KEY_P)) currentState = PLAYING;
        }
        else if (currentState == GAMEOVER) {
            if (IsKeyPressed(KEY_ENTER)) {
                currentState = MAIN_MENU;
            }
        }
        // NEU: Logik für den LEVEL_CLEARED Bildschirm
        else if (currentState == LEVEL_CLEARED) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                // Klick auf "Nächstes Level"
                if (CheckCollisionPointRec(mousePoint, nextLevelButton)) {
                    currentLevel++; 
                    timeLeft = 60.0f; 
                    player.rect.x = (SCREEN_WIDTH / 2) - (player.rect.width / 2); 
                    for (int i = 0; i < 4; i++) {
                        ResetObstacle(obstacles[i], -200.0f - (i * 350.0f)); 
                    }
                    currentState = PLAYING;
                }
                // Klick auf "Hauptmenü"
                else if (CheckCollisionPointRec(mousePoint, returnMenuButton)) {
                    currentState = MAIN_MENU;
                }
            }
        }

        // --- DRAW (Zeichnen) ---
        BeginDrawing();
        ClearBackground(DARKGREEN);

        if (currentState == MAIN_MENU || currentState == DESCRIPTION) {
            DrawRectangle(ROAD_OFFSET, 0, ROAD_WIDTH, SCREEN_HEIGHT, DARKGRAY);
            DrawLineEx({(float)ROAD_OFFSET, 0}, {(float)ROAD_OFFSET, (float)SCREEN_HEIGHT}, 5, WHITE); 
            DrawLineEx({(float)(ROAD_OFFSET + ROAD_WIDTH), 0}, {(float)(ROAD_OFFSET + ROAD_WIDTH), (float)SCREEN_HEIGHT}, 5, WHITE);
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));

            if (currentState == MAIN_MENU) {
                DrawText("CarRace", 120, 150, 40, ORANGE);
                
                DrawText("Dein Name:", (int)inputBox.x, (int)inputBox.y - 30, 20, WHITE);
                DrawRectangleRec(inputBox, LIGHTGRAY);
                DrawRectangleLines((int)inputBox.x, (int)inputBox.y, (int)inputBox.width, (int)inputBox.height, DARKBLUE);
                DrawText(playerName, (int)inputBox.x + 10, (int)inputBox.y + 15, 20, MAROON);

                if (letterCount < MAX_NAME_LENGTH) {
                    if (((framesCounter/30)%2) == 0) DrawText("_", (int)inputBox.x + 10 + MeasureText(playerName, 20), (int)inputBox.y + 15, 20, MAROON);
                }

                if (letterCount == 0) DrawText("Bitte Namen eingeben!", (int)inputBox.x + 15, (int)inputBox.y + 60, 20, RED);

                Color startColor = (letterCount > 0) ? (CheckCollisionPointRec(mousePoint, startButton) ? GREEN : DARKGREEN) : GRAY;
                DrawRectangleRec(startButton, startColor);
                DrawText("SPIEL STARTEN", (int)startButton.x + 15, (int)startButton.y + 15, 20, WHITE);

                Color descColor = CheckCollisionPointRec(mousePoint, descButton) ? BLUE : DARKBLUE;
                DrawRectangleRec(descButton, descColor);
                DrawText("DESCRIPTION", (int)descButton.x + 30, (int)descButton.y + 15, 20, WHITE);
            }
            else if (currentState == DESCRIPTION) {
                DrawText("SPIELANLEITUNG", 140, 150, 40, ORANGE);
                DrawText("Ziel:", 100, 250, 25, YELLOW);
                DrawText("Ueberlebe 60 Sekunden, um", 100, 290, 20, WHITE);
                DrawText("in das naechste Level zu kommen.", 100, 320, 20, WHITE);
                DrawText("Steuerung:", 100, 380, 25, YELLOW);
                DrawText("[Pfeil Links / Rechts]: Auto bewegen", 100, 420, 20, WHITE);
                DrawText("[P]: Spiel pausieren", 100, 450, 20, WHITE);
                DrawText("Achtung: Das Spiel wird im", 100, 520, 20, RED);
                DrawText("Verlauf immer schneller!", 100, 550, 20, RED);

                Color backColor = CheckCollisionPointRec(mousePoint, backButton) ? RED : MAROON;
                DrawRectangleRec(backButton, backColor);
                DrawText("ZURUECK", (int)backButton.x + 50, (int)backButton.y + 15, 20, WHITE);
            }
        }
        else {
            DrawRectangle(ROAD_OFFSET, 0, ROAD_WIDTH, SCREEN_HEIGHT, DARKGRAY);
            DrawLineEx({(float)ROAD_OFFSET, 0}, {(float)ROAD_OFFSET, (float)SCREEN_HEIGHT}, 5, WHITE); 
            DrawLineEx({(float)(ROAD_OFFSET + ROAD_WIDTH), 0}, {(float)(ROAD_OFFSET + ROAD_WIDTH), (float)SCREEN_HEIGHT}, 5, WHITE);

            DrawRectangleRec(player.rect, player.color);

            for (int i = 0; i < 4; i++) {
                DrawRectangleRec(obstacles[i].rect, obstacles[i].color);
            }

            DrawText(TextFormat("Zeit: 00:%02i", (int)timeLeft), 20, 20, 30, WHITE);
            
            const char* levelText = TextFormat("Level: %i", currentLevel);
            DrawText(levelText, SCREEN_WIDTH - MeasureText(levelText, 30) - 20, 20, 30, ORANGE);
            DrawText(playerName, SCREEN_WIDTH/2 - MeasureText(playerName, 20)/2, 25, 20, LIGHTGRAY);

            if (currentState == PAUSED) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.5f));
                DrawText("PAUSIERT", 190, 350, 45, WHITE);
                DrawText("Druecke 'P' zum Weiterfahren", 130, 420, 20, LIGHTGRAY);
            }
            else if (currentState == GAMEOVER) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
                DrawText("CRASH!", 220, 300, 50, RED);
                DrawText(TextFormat("Schade %s, du bist in", playerName), 100, 380, 25, LIGHTGRAY);
                DrawText(TextFormat("Level %i gecrasht.", currentLevel), 180, 415, 25, LIGHTGRAY);
                DrawText("Druecke ENTER fuer das Hauptmenue", 70, 520, 25, WHITE);
            }
            // NEU: Zeichnen der neuen Buttons im LEVEL_CLEARED Bildschirm
            else if (currentState == LEVEL_CLEARED) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
                DrawText(TextFormat("LEVEL %i GESCHAFFT!", currentLevel), 80, 250, 45, GREEN);
                
                // Button 1: Nächstes Level
                Color nextColor = CheckCollisionPointRec(mousePoint, nextLevelButton) ? GREEN : DARKGREEN;
                DrawRectangleRec(nextLevelButton, nextColor);
                DrawText("NAECHSTES LEVEL", (int)nextLevelButton.x + 35, (int)nextLevelButton.y + 15, 20, WHITE);

                // Button 2: Zurück ins Hauptmenü
                Color menuColor = CheckCollisionPointRec(mousePoint, returnMenuButton) ? GRAY : DARKGRAY;
                DrawRectangleRec(returnMenuButton, menuColor);
                DrawText("HAUPTMENUE", (int)returnMenuButton.x + 60, (int)returnMenuButton.y + 15, 20, WHITE);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}