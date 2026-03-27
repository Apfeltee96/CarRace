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
#include <algorithm>

const int SCREEN_WIDTH = 1000; 
const int SCREEN_HEIGHT = 800;
const int ROAD_WIDTH = 400;
const int ROAD_OFFSET = (SCREEN_WIDTH - ROAD_WIDTH) / 2;
const int MAX_NAME_LENGTH = 15; 

struct Obstacle { Rectangle rect; Color color; };
struct CollectableStar { Rectangle rect; bool active; };
enum GameState { MAIN_MENU, SHOP_MENU, DESCRIPTION, SCOREBOARD_MENU, SETTINGS, PLAYING, PAUSED, GAMEOVER, EXIT_PROMPT };

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
    SaveGame saveData = LoadSaveGame();
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Car Race");
    SetExitKey(KEY_NULL); 

    // NEUE REIHENFOLGE DER TEXTUREN
    Texture2D carTextures[3];
    carTextures[0] = LoadTexture("assets/car_white.png"); // Start Auto
    carTextures[1] = LoadTexture("assets/car_red.png");   // Mittel
    carTextures[2] = LoadTexture("assets/car_blue.png");  // Premium (200 Sterne)
    
    Texture2D obstacleTex = LoadTexture("assets/hindernis.png");
    Texture2D starTex     = LoadTexture("assets/star.png");

    RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    if (saveData.isFullscreen && !IsWindowFullscreen()) ToggleFullscreen();
    SetTargetFPS(60);

    Player player;
    // Initialisierung: Größe passt sich der gewählten Textur an
    Texture2D currentTex = carTextures[saveData.selectedColorId];
    float cw = 45.0f; 
    float ch = cw * ((float)currentTex.height / (float)currentTex.width);
    InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT, cw, ch, GetCarColor(saveData.selectedColorId));

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

    float centerX = SCREEN_WIDTH / 2.0f - 125;
    Rectangle startButton = { SCREEN_WIDTH / 2.0f - 100, 300, 200, 50 };
    Rectangle scoreBtn    = { SCREEN_WIDTH / 2.0f - 100, 360, 200, 50 };
    Rectangle shopBtn     = { SCREEN_WIDTH / 2.0f - 100, 420, 200, 50 };
    Rectangle settingsBtn = { SCREEN_WIDTH / 2.0f - 100, 480, 200, 50 }; 
    Rectangle descButton  = { SCREEN_WIDTH / 2.0f - 100, 540, 200, 50 };
    Rectangle backMenuBtn = { SCREEN_WIDTH / 2.0f - 125, 680, 250, 50 };
    
    Rectangle langBtn       = { centerX, 180, 250, 50 };
    Rectangle resBtn        = { centerX, 250, 250, 50 }; 
    Rectangle nameChangeBtn = { centerX, 320, 250, 50 }; 
    Rectangle deleteDataBtn = { centerX, 390, 250, 50 }; 
    Rectangle backSetBtn    = { centerX, 550, 250, 50 };

    Rectangle btnPrimary = { centerX, 400, 250, 50 }; 
    Rectangle btnMenu    = { centerX, 480, 250, 50 }; 

    std::vector<Obstacle> obstacles(4); 
    CollectableStar bonusStar = {{0,0,30,30}, false};
    GameState currentState = MAIN_MENU;
    GameState previousState = MAIN_MENU;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        Vector2 realMouse = GetMousePosition();
        float scale = std::min((float)GetScreenWidth()/SCREEN_WIDTH, (float)GetScreenHeight()/SCREEN_HEIGHT);
        Vector2 mousePoint = {
            (realMouse.x - (GetScreenWidth() - (SCREEN_WIDTH * scale)) * 0.5f) / scale,
            (realMouse.y - (GetScreenHeight() - (SCREEN_HEIGHT * scale)) * 0.5f) / scale
        };

        if (IsKeyPressed(KEY_ESCAPE)) {
            if (currentState == EXIT_PROMPT) currentState = previousState;
            else { previousState = currentState; currentState = EXIT_PROMPT; }
        }

        if (currentState == EXIT_PROMPT) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, { SCREEN_WIDTH/2.0f - 110, 400, 100, 40 })) break; 
                if (CheckCollisionPointRec(mousePoint, { SCREEN_WIDTH/2.0f + 10, 400, 100, 40 })) currentState = previousState;
            }
        }
   else if (currentState == MAIN_MENU) {
            if (!isNameSaved) {
                // --- BUCHSTABEN EINGEBEN ---
                int key = GetCharPressed();
                while (key > 0) {
                    if ((key > 32) && (key <= 125) && (letterCount < MAX_NAME_LENGTH)) {
                        playerName[letterCount] = (char)key;
                        playerName[letterCount + 1] = '\0';
                        letterCount++;
                    }
                    key = GetCharPressed();
                }

                // --- LÖSCHEN MIT BACKSPACE ---
                if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                    letterCount--;
                    playerName[letterCount] = '\0';
                }

                // --- BESTÄTIGEN MIT ENTER ---
                if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                    saveData.lastPlayerName = std::string(playerName);
                    SaveGameData(saveData); 
                    isNameSaved = true; 
                }
            } // Ende von if (!isNameSaved)

            // --- MAUS KLICKS (Darf nicht in isNameSaved verschachtelt sein!) ---
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, startButton) && (letterCount > 0 || isNameSaved)) {
                    currentScore = 0; totalTimeSurvived = 0.0f; earnedStarsThisRound = 0; currentSpeed = SPEED_START;
                    saveData.lastPlayerName = std::string(playerName); SaveGameData(saveData); isNameSaved = true; 
                    
                    Texture2D cur = carTextures[saveData.selectedColorId];
                    float w = 45.0f; float h = w * ((float)cur.height / (float)cur.width);
                    InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT, w, h, GetCarColor(saveData.selectedColorId));
                    
                    for (int i = 0; i < (int)obstacles.size(); i++) ResetObstacle(obstacles[i], -200.0f - (i * 400.0f));
                    currentState = PLAYING;
                }
                else if (CheckCollisionPointRec(mousePoint, scoreBtn)) currentState = SCOREBOARD_MENU; 
                else if (CheckCollisionPointRec(mousePoint, shopBtn)) currentState = SHOP_MENU; 
                else if (CheckCollisionPointRec(mousePoint, settingsBtn)) currentState = SETTINGS;
                else if (CheckCollisionPointRec(mousePoint, descButton)) currentState = DESCRIPTION;
            }
        } // Ende von currentState == MAIN_MENU
        else if (currentState == SETTINGS) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, backSetBtn)) currentState = MAIN_MENU;
                if (CheckCollisionPointRec(mousePoint, langBtn)) { saveData.isEnglish = !saveData.isEnglish; SaveGameData(saveData); }
                if (CheckCollisionPointRec(mousePoint, resBtn)) {
                    saveData.isFullscreen = !saveData.isFullscreen;
                    ToggleFullscreen();
                    SaveGameData(saveData);
                }
                if (CheckCollisionPointRec(mousePoint, nameChangeBtn)) { isNameSaved = false; playerName[0] = '\0'; letterCount = 0; currentState = MAIN_MENU; }
                if (CheckCollisionPointRec(mousePoint, deleteDataBtn)) {
                    DeleteSaveData(); ClearScoreboard(); if (IsWindowFullscreen()) ToggleFullscreen();
                    saveData = { 0, false, false, 0, saveData.isEnglish, false, "" };
                    playerName[0] = '\0'; letterCount = 0; isNameSaved = false; currentState = MAIN_MENU;
                }
            }
        }
        else if (currentState == SHOP_MENU) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, backMenuBtn)) currentState = MAIN_MENU;
                // Weiß (ID 0) - Jetzt das Startauto
                if (CheckCollisionPointRec(mousePoint, {150, 500, 200, 50})) { saveData.selectedColorId = 0; SaveGameData(saveData); }
                // Rot (ID 1) - Bleibt bei 100 Sternen
                if (CheckCollisionPointRec(mousePoint, {400, 500, 200, 50})) {
                    if (saveData.ownsRedCar) saveData.selectedColorId = 1;
                    else if (saveData.totalStars >= 100) { saveData.totalStars -= 100; saveData.ownsRedCar = true; saveData.selectedColorId = 1; }
                    SaveGameData(saveData);
                }
                // Blau (ID 2) - Jetzt Premium für 200 Sterne
                if (CheckCollisionPointRec(mousePoint, {650, 500, 200, 50})) {
                    if (saveData.ownsPurpleCar) saveData.selectedColorId = 2; 
                    else if (saveData.totalStars >= 200) { saveData.totalStars -= 200; saveData.ownsPurpleCar = true; saveData.selectedColorId = 2; }
                    SaveGameData(saveData);
                }
                Texture2D cur = carTextures[saveData.selectedColorId];
                float w = 45.0f; float h = w * ((float)cur.height / (float)cur.width);
                InitPlayer(player, SCREEN_WIDTH, SCREEN_HEIGHT, w, h, GetCarColor(saveData.selectedColorId));
            }
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
                    for (int j = 0; j < (int)obstacles.size(); j++) if (obstacles[j].rect.y < highestY) highestY = obstacles[j].rect.y;
                    ResetObstacle(obstacles[i], highestY - 400.0f);
                }
                if (CheckCollisionRecs(player.rect, obstacles[i].rect)) {
    // Sicherstellen, dass der Name nicht leer ist, bevor wir speichern
    const char* saveName = (std::strlen(playerName) > 0) ? playerName : "Gast";
    
    AddOrUpdateScore(saveName, currentScore, totalTimeSurvived);
    saveData.totalStars += earnedStarsThisRound; 
    SaveGameData(saveData);
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
        else if (currentState == SCOREBOARD_MENU || currentState == DESCRIPTION || currentState == PAUSED || currentState == GAMEOVER) {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePoint, backMenuBtn) || CheckCollisionPointRec(mousePoint, btnMenu)) currentState = MAIN_MENU;
                if (currentState == PAUSED && CheckCollisionPointRec(mousePoint, btnPrimary)) currentState = PLAYING;
            }
        }

        // --- ZEICHNEN ---
BeginTextureMode(target); 
    // 1. IMMER den Hintergrund (Wiese & Straße) zeichnen
    ClearBackground(DARKGREEN); 
    DrawRectangle(ROAD_OFFSET, 0, ROAD_WIDTH, SCREEN_HEIGHT, DARKGRAY);

    // 2. JE NACH ZUSTAND WEITERE DINGE ZEICHNEN
    if (currentState == PLAYING || currentState == PAUSED || currentState == GAMEOVER || currentState == EXIT_PROMPT) {
        // Hindernisse, Auto und Sterne (Spiel-Objekte)
        for (int i = 0; i < (int)obstacles.size(); i++) {
            DrawTexturePro(obstacleTex, {0,0,(float)obstacleTex.width,(float)obstacleTex.height}, obstacles[i].rect, {0,0}, 0.0f, WHITE);
        }
        Texture2D cur = carTextures[saveData.selectedColorId];
        DrawTexturePro(cur, {0,0,(float)cur.width,(float)cur.height}, player.rect, {0,0}, 0.0f, WHITE);
        if (bonusStar.active) DrawTexturePro(starTex, {0,0,(float)starTex.width,(float)starTex.height}, bonusStar.rect, {0,0}, 0.0f, WHITE);
        
        DrawHUD(playerName, totalTimeSurvived, currentScore, earnedStarsThisRound, saveData.isEnglish);
        
        if (currentState == PAUSED) DrawPauseMenu(mousePoint, btnPrimary, btnMenu, saveData.isEnglish);
        else if (currentState == GAMEOVER) DrawGameOverMenu(playerName, currentScore, totalTimeSurvived, earnedStarsThisRound, mousePoint, btnMenu, saveData.isEnglish);
    } 
    
    // 3. MENÜS ÜBER DEN HINTERGRUND LEGEN
    // Wir zeichnen die Menüs hier, aber OHNE ClearBackground in der ui.cpp aufzurufen!
    if (currentState == MAIN_MENU) {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.4f)); // Optional: Hintergrund leicht abdunkeln
        DrawMainMenu(playerName, letterCount, 0, mousePoint, startButton, scoreBtn, shopBtn, settingsBtn, descButton, saveData.totalStars, isNameSaved, saveData.isEnglish);
    }
    else if (currentState == SETTINGS) {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));
        DrawSettingsMenu(mousePoint, langBtn, resBtn, nameChangeBtn, deleteDataBtn, backSetBtn, saveData.isEnglish, saveData.isFullscreen);
    }
    else if (currentState == SHOP_MENU) {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));
        DrawShopMenu(saveData, mousePoint, {400, 500, 200, 50}, {650, 500, 200, 50}, backMenuBtn, saveData.isEnglish, carTextures);
    }
    else if (currentState == SCOREBOARD_MENU) {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));
        DrawScoreboardMenu(LoadScoreboard(), mousePoint, backMenuBtn, saveData.isEnglish);
    }
    else if (currentState == DESCRIPTION) {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));
        DrawDescriptionMenu(mousePoint, backMenuBtn, saveData.isEnglish);
    }

            if (currentState == EXIT_PROMPT) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
                Rectangle box = { SCREEN_WIDTH/2.0f - 150, SCREEN_HEIGHT/2.0f - 100, 300, 200 };
                DrawRectangleRec(box, RAYWHITE);
                DrawRectangleLinesEx(box, 3, GOLD);
                DrawText(saveData.isEnglish ? "EXIT?" : "BEENDEN?", (int)box.x+80, (int)box.y+40, 20, DARKGRAY);
                Rectangle yBtn = { SCREEN_WIDTH/2.0f - 110, 400, 100, 40 }, nBtn = { SCREEN_WIDTH/2.0f + 10, 400, 100, 40 };
                DrawRectangleRec(yBtn, CheckCollisionPointRec(mousePoint, yBtn) ? RED : MAROON);
                DrawText(saveData.isEnglish ? "YES" : "JA", (int)yBtn.x+35, (int)yBtn.y+10, 20, WHITE);
                DrawRectangleRec(nBtn, CheckCollisionPointRec(mousePoint, nBtn) ? LIGHTGRAY : GRAY);
                DrawText(saveData.isEnglish ? "NO" : "NEIN", (int)nBtn.x+35, (int)nBtn.y+10, 20, BLACK);
            }
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(target.texture, {0,0,(float)target.texture.width,(float)-target.texture.height},
                           {(GetScreenWidth()-(SCREEN_WIDTH*scale))*0.5f,(GetScreenHeight()-(SCREEN_HEIGHT*scale))*0.5f,SCREEN_WIDTH*scale,SCREEN_HEIGHT*scale},
                           {0,0}, 0.0f, WHITE);
        EndDrawing();
    }

    for (int i = 0; i < 3; i++) UnloadTexture(carTextures[i]);
    UnloadTexture(obstacleTex); UnloadTexture(starTex);
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}