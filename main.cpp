#include "raylib.h"
#include "level.h"
#include "scoreboard.h"
#include <vector>
#include <string>

// NEU: Das Fenster ist jetzt breiter (800 statt 600). Die Straße bleibt bei 400!
// Dadurch haben wir links und rechts jeweils 200 Pixel Grasfläche für das HUD.
const int SCREEN_WIDTH = 800; 
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

enum GameState { MAIN_MENU, DESCRIPTION, SCOREBOARD_MENU, PLAYING, PAUSED, GAMEOVER, GAME_WON };

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
    player.rect.width = 60;
    player.rect.height = 100;
    player.speed = 650.0f;
    player.color = BLUE;

    int currentLevel = 1;          
    LevelData activeLevelData = GetLevelData(currentLevel); 
    float timeLeft = activeLevelData.targetTime;        
    float currentSpeed = activeLevelData.baseSpeed; 
    
    float totalTimeSurvived = 0.0f; 

    char playerName[MAX_NAME_LENGTH + 1] = "\0"; 
    int letterCount = 0; 
    int framesCounter = 0; 

    // Die Buttons zentrieren sich automatisch durch SCREEN_WIDTH/2.0f
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
                    activeLevelData = GetLevelData(currentLevel);
                    timeLeft = activeLevelData.targetTime;
                    totalTimeSurvived = 0.0f; 
                    
                    player.rect.x = (SCREEN_WIDTH / 2) - (player.rect.width / 2);
                    player.rect.y = SCREEN_HEIGHT - 150;
                    for (int i = 0; i < 4; i++) ResetObstacle(obstacles[i], -200.0f - (i * 350.0f));
                    currentState = PLAYING;
                }
            }
        }
        else if (currentState == DESCRIPTION || currentState == SCOREBOARD_MENU) {
            Rectangle backMenuBtn = { SCREEN_WIDTH/2.0f - 125, 680, 250, 50 };
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, backMenuBtn)) {
                currentState = MAIN_MENU;
            }
        }
        else if (currentState == PLAYING) {
            if (IsKeyPressed(KEY_P)) currentState = PAUSED;

            timeLeft -= deltaTime;
            totalTimeSurvived += deltaTime; 

            currentSpeed = activeLevelData.baseSpeed + ((activeLevelData.targetTime - timeLeft) * activeLevelData.speedMultiplier); 

            if (timeLeft <= 0.0f) {
                if (currentLevel == 5) {
                    timeLeft = 0.0f;   
                    currentState = GAME_WON; 
                } else {
                    currentLevel++;
                    activeLevelData = GetLevelData(currentLevel);
                    timeLeft = activeLevelData.targetTime;
                }
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
        else if (currentState == GAME_WON) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, btnMenu)) {
                AddOrUpdateScore(std::string(playerName), 5, totalTimeSurvived);
                currentState = MAIN_MENU;
            }
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(DARKGREEN);

        if (currentState == MAIN_MENU || currentState == DESCRIPTION || currentState == SCOREBOARD_MENU) {
            DrawRectangle(ROAD_OFFSET, 0, ROAD_WIDTH, SCREEN_HEIGHT, DARKGRAY);
            DrawLineEx({(float)ROAD_OFFSET, 0}, {(float)ROAD_OFFSET, (float)SCREEN_HEIGHT}, 5, WHITE); 
            DrawLineEx({(float)(ROAD_OFFSET + ROAD_WIDTH), 0}, {(float)(ROAD_OFFSET + ROAD_WIDTH), (float)SCREEN_HEIGHT}, 5, WHITE);
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));

            if (currentState == MAIN_MENU) {
                const char* titleText = "CAR RACE";
                DrawText(titleText, SCREEN_WIDTH/2 - MeasureText(titleText, 45)/2, 150, 45, ORANGE);
                
                // NEU: "(ohne Leerzeichen)" wurde entfernt
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

                Color sColor = CheckCollisionPointRec(mousePoint, scoreBtn) ? GOLD : ORANGE;
                DrawRectangleRec(scoreBtn, sColor);
                DrawText("BESTENLISTE", (int)scoreBtn.x + 30, (int)scoreBtn.y + 15, 20, WHITE);

                Color descColor = CheckCollisionPointRec(mousePoint, descButton) ? BLUE : DARKBLUE;
                DrawRectangleRec(descButton, descColor);
                DrawText("DESCRIPTION", (int)descButton.x + 30, (int)descButton.y + 15, 20, WHITE);
            }
            else if (currentState == SCOREBOARD_MENU) {
                // Alle X-Werte um +100 angepasst, damit es auf 800px Breite mittig bleibt
                DrawText("TOP 10 SPIELER", 250, 150, 40, GOLD); 
                
                std::vector<ScoreEntry> scores = LoadScoreboard();
                if (scores.empty()) {
                    DrawText("Noch keine Eintraege vorhanden!", 220, 300, 20, LIGHTGRAY);
                } else {
                    for (size_t i = 0; i < scores.size(); i++) {
                        DrawText(TextFormat("%d. %s", i+1, scores[i].name.c_str()), 200, 250 + (i * 35), 22, WHITE);
                        DrawText(TextFormat("Lvl %d", scores[i].level), 430, 250 + (i * 35), 22, ORANGE);
                        DrawText(TextFormat("(%.1fs)", scores[i].timeSurvived), 510, 250 + (i * 35), 22, LIGHTGRAY);
                    }
                }

                Rectangle backMenuBtn = { SCREEN_WIDTH/2.0f - 125, 680, 250, 50 };
                Color backColor = CheckCollisionPointRec(mousePoint, backMenuBtn) ? RED : MAROON;
                DrawRectangleRec(backMenuBtn, backColor);
                DrawText("ZURUECK", (int)backMenuBtn.x + 75, (int)backMenuBtn.y + 15, 20, WHITE);
            }
            else if (currentState == DESCRIPTION) {
                // Alle X-Werte um +100 angepasst
                DrawText("SPIELANLEITUNG", 240, 150, 40, ORANGE);
                DrawText("Ziel:", 200, 250, 25, YELLOW);
                DrawText("Ueberlebe, bis der Timer auf 0 faellt.", 200, 290, 20, WHITE);
                DrawText("Steuerung:", 200, 360, 25, YELLOW);
                DrawText("[Pfeil Links / Rechts]: Auto bewegen", 200, 400, 20, WHITE);
                DrawText("[P]: Spiel pausieren", 200, 430, 20, WHITE);
                
                Rectangle backMenuBtn = { SCREEN_WIDTH/2.0f - 125, 680, 250, 50 };
                Color backColor = CheckCollisionPointRec(mousePoint, backMenuBtn) ? RED : MAROON;
                DrawRectangleRec(backMenuBtn, backColor);
                DrawText("ZURUECK", (int)backMenuBtn.x + 75, (int)backMenuBtn.y + 15, 20, WHITE);
            }
        }
        else {
            DrawRectangle(ROAD_OFFSET, 0, ROAD_WIDTH, SCREEN_HEIGHT, DARKGRAY);
            DrawLineEx({(float)ROAD_OFFSET, 0}, {(float)ROAD_OFFSET, (float)SCREEN_HEIGHT}, 5, WHITE); 
            DrawLineEx({(float)(ROAD_OFFSET + ROAD_WIDTH), 0}, {(float)(ROAD_OFFSET + ROAD_WIDTH), (float)SCREEN_HEIGHT}, 5, WHITE);

            DrawRectangleRec(player.rect, player.color);
            for (int i = 0; i < 4; i++) DrawRectangleRec(obstacles[i].rect, obstacles[i].color);

            // --- PERFEKTIONIERTES HUD ---
            
            // LINKS Oben: Name und exakt darunter die reinen Sekunden
            DrawText(playerName, 20, 20, 25, LIGHTGRAY);
            DrawText(TextFormat("%.1fs", totalTimeSurvived), 20, 50, 22, WHITE);

            // RECHTS Oben: Level und exakt darunter die verbleibende Zeit
            const char* levelText = TextFormat("Level: %i", currentLevel);
            Color lvlColor = (timeLeft > activeLevelData.targetTime - 1.5f) ? GREEN : ORANGE;
            DrawText(levelText, SCREEN_WIDTH - MeasureText(levelText, 30) - 20, 20, 30, lvlColor);
            
            const char* timeLeftText = TextFormat("Zeit: 00:%02i", (int)timeLeft);
            DrawText(timeLeftText, SCREEN_WIDTH - MeasureText(timeLeftText, 22) - 20, 55, 22, WHITE);
            // ---------------------------

            if (currentState == PAUSED) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));
                DrawText("PAUSIERT", 290, 280, 45, WHITE); // X + 100
                
                Color primColor = CheckCollisionPointRec(mousePoint, btnPrimary) ? GREEN : DARKGREEN;
                DrawRectangleRec(btnPrimary, primColor);
                DrawText("WEITER SPIELEN", (int)btnPrimary.x + 40, (int)btnPrimary.y + 15, 20, WHITE);

                Color menuColor = CheckCollisionPointRec(mousePoint, btnMenu) ? GRAY : DARKGRAY;
                DrawRectangleRec(btnMenu, menuColor);
                DrawText("HAUPTMENUE", (int)btnMenu.x + 60, (int)btnMenu.y + 15, 20, WHITE);
            }
            else if (currentState == GAMEOVER) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
                DrawText("CRASH!", 320, 250, 50, RED); // X + 100
                
                const char* overText1 = TextFormat("Schade %s, du bist in", playerName);
                DrawText(overText1, SCREEN_WIDTH/2 - MeasureText(overText1, 25)/2, 330, 25, LIGHTGRAY);
                
                const char* overText2 = TextFormat("Level %i nach %.1fs gecrasht.", currentLevel, totalTimeSurvived);
                DrawText(overText2, SCREEN_WIDTH/2 - MeasureText(overText2, 25)/2, 365, 25, LIGHTGRAY);
                
                DrawText("Ergebnis im Scoreboard gespeichert!", 195, 410, 20, GREEN); // X + 100

                Color menuColor = CheckCollisionPointRec(mousePoint, btnMenu) ? GRAY : DARKGRAY;
                DrawRectangleRec(btnMenu, menuColor);
                DrawText("HAUPTMENUE", (int)btnMenu.x + 60, (int)btnMenu.y + 15, 20, WHITE);
            }
            else if (currentState == GAME_WON) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.8f));
                DrawText("HERZLICHEN GLUECKWUNSCH!", 130, 250, 35, GOLD); // X + 100
                DrawText("Du hast alle 5 Level von", 250, 320, 25, WHITE); // X + 100
                
                const char* wonText = TextFormat("Car Race in %.1fs bestanden!", totalTimeSurvived);
                DrawText(wonText, SCREEN_WIDTH/2 - MeasureText(wonText, 25)/2, 360, 25, WHITE);

                Color menuColor = CheckCollisionPointRec(mousePoint, btnMenu) ? GRAY : DARKGRAY;
                DrawRectangleRec(btnMenu, menuColor);
                DrawText("HAUPTMENUE", (int)btnMenu.x + 60, (int)btnMenu.y + 15, 20, WHITE);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}