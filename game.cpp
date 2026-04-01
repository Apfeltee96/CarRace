#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include <algorithm>
#include <cstring>

void Game::Init() {
    saveData = LoadSaveGame();
    InitWindow(1000, 800, "Car Race");
    SetExitKey(KEY_NULL);

    carTextures[0] = LoadTexture("assets/car_white.png");
    carTextures[1] = LoadTexture("assets/car_red.png");
    carTextures[2] = LoadTexture("assets/car_blue.png");
    obstacleTex = LoadTexture("assets/hindernis.png");
    starTex = LoadTexture("assets/star.png");

    target = LoadRenderTexture(1000, 800);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    if (saveData.isFullscreen && !IsWindowFullscreen()) ToggleFullscreen();
    SetTargetFPS(60);

    state = MAIN_MENU;
    isNameSaved = false;
    if (!saveData.lastPlayerName.empty() && saveData.lastPlayerName != "Gast") {
        std::strncpy(playerName, saveData.lastPlayerName.c_str(), 15);
        isNameSaved = true;
    }
    playerName[15] = '\0';
    letterCount = (int)std::strlen(playerName);

    float centerX = 1000 / 2.0f - 125;
    startBtn    = { 400, 300, 200, 50 };
    scoreBtn    = { 400, 360, 200, 50 };
    shopBtn     = { 400, 420, 200, 50 };
    settingsBtn = { 400, 480, 200, 50 };
    descBtn     = { 400, 540, 200, 50 };
    backMenuBtn = { 375, 680, 250, 50 };
    
    langBtn = { centerX, 180, 250, 50 };
    resBtn  = { centerX, 250, 250, 50 };
    nameChangeBtn = { centerX, 320, 250, 50 };
    deleteDataBtn = { centerX, 390, 250, 50 };
    backSetBtn = { centerX, 550, 250, 50 };
    
    btnPrimary = { centerX, 400, 250, 50 };
    btnMenu    = { centerX, 480, 250, 50 };

    obstacles.resize(4);
    for (int i = 0; i < 4; i++) ResetObstacle(obstacles[i], -200.0f - (i * 400.0f));
    bonusStar = {{0,0,30,30}, false};
}

void Game::Update() {
    float deltaTime = GetFrameTime();
    Vector2 realMouse = GetMousePosition();
    
    float scale = std::min((float)GetScreenWidth()/1000.0f, (float)GetScreenHeight()/800.0f);
    Vector2 mousePoint = {
        (realMouse.x - (GetScreenWidth() - (1000.0f * scale)) * 0.5f) / scale,
        (realMouse.y - (GetScreenHeight() - (800.0f * scale)) * 0.5f) / scale
    };

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (state == EXIT_PROMPT) state = previousState;
        else { previousState = state; state = EXIT_PROMPT; }
    }

    if (state == EXIT_PROMPT) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePoint, { 380, 420, 100, 40 })) Cleanup(); 
            if (CheckCollisionPointRec(mousePoint, { 520, 420, 100, 40 })) state = previousState; 
        }
    }
    else if (state == MAIN_MENU) HandleMenuInput(mousePoint);
    else if (state == SHOP_MENU) HandleShopInput(mousePoint);
    else if (state == SETTINGS) HandleSettingsInput(mousePoint);
    else if (state == PLAYING) UpdateGameLogic(deltaTime);
    else if (state == SCOREBOARD_MENU || state == DESCRIPTION || state == PAUSED || state == GAMEOVER) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePoint, backMenuBtn) || CheckCollisionPointRec(mousePoint, btnMenu)) state = MAIN_MENU;
            if (state == PAUSED && CheckCollisionPointRec(mousePoint, btnPrimary)) state = PLAYING;
        }
    }
}

void Game::UpdateGameLogic(float deltaTime) {
    if (IsKeyPressed(KEY_P)) state = PAUSED;
    totalTimeSurvived += deltaTime;
    currentScore = (int)(totalTimeSurvived * 50.0f);
    currentSpeed = GetCurrentSpeed(currentScore, currentSpeed, deltaTime);
    player.speed = GetDynamicPlayerSpeed(currentSpeed);

    if (IsKeyDown(KEY_LEFT)) player.rect.x -= player.speed * deltaTime;
    if (IsKeyDown(KEY_RIGHT)) player.rect.x += player.speed * deltaTime;
    player.rect.x = std::clamp(player.rect.x, 300.0f, 700.0f - player.rect.width);

    for (int i = 0; i < (int)obstacles.size(); i++) {
        obstacles[i].rect.y += currentSpeed * deltaTime;
        
        if (obstacles[i].rect.y > 800) {
            float highestY = 0;
            for (const auto& obs : obstacles) if (obs.rect.y < highestY) highestY = obs.rect.y;
            ResetObstacle(obstacles[i], highestY - 400.0f);
        }

        if (CheckCollisionRecs(player.rect, obstacles[i].rect)) {
            AddOrUpdateScore(playerName, currentScore, totalTimeSurvived);
            saveData.totalStars += earnedStarsThisRound; SaveGameData(saveData);
            state = GAMEOVER;
        }
    }
    if (!bonusStar.active && GetRandomValue(0, 500) < 3) SpawnStar();
    if (bonusStar.active) {
        bonusStar.rect.y += currentSpeed * deltaTime;
        if (CheckCollisionRecs(player.rect, bonusStar.rect)) { earnedStarsThisRound++; bonusStar.active = false; }
        if (bonusStar.rect.y > 800) bonusStar.active = false;
    }
}

void Game::HandleMenuInput(Vector2 mousePoint) {
    if (!isNameSaved) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key > 32) && (key <= 125) && (letterCount < 15)) {
                playerName[letterCount] = (char)key; playerName[letterCount + 1] = '\0'; letterCount++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) { letterCount--; playerName[letterCount] = '\0'; }
        if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
            saveData.lastPlayerName = playerName; SaveGameData(saveData); isNameSaved = true;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePoint, startBtn) && (letterCount > 0 || isNameSaved)) {
            currentScore = 0; totalTimeSurvived = 0; earnedStarsThisRound = 0; currentSpeed = SPEED_START;
            isNameSaved = true; saveData.lastPlayerName = playerName; SaveGameData(saveData);
            Texture2D cur = carTextures[saveData.selectedColorId];
            InitPlayer(player, 1000, 800, 45, 45 * ((float)cur.height/cur.width), GetCarColor(saveData.selectedColorId));
            for (int i = 0; i < 4; i++) ResetObstacle(obstacles[i], -200.0f - (i * 400.0f));
            state = PLAYING;
        }
        else if (CheckCollisionPointRec(mousePoint, scoreBtn)) state = SCOREBOARD_MENU;
        else if (CheckCollisionPointRec(mousePoint, shopBtn)) state = SHOP_MENU;
        else if (CheckCollisionPointRec(mousePoint, settingsBtn)) state = SETTINGS;
        else if (CheckCollisionPointRec(mousePoint, descBtn)) state = DESCRIPTION;
    }
}

void Game::HandleShopInput(Vector2 mousePoint) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePoint, backMenuBtn)) state = MAIN_MENU;
        
        if (CheckCollisionPointRec(mousePoint, {150, 500, 200, 50})) { saveData.selectedColorId = 0; }
        
        if (CheckCollisionPointRec(mousePoint, {400, 500, 200, 50})) {
            if (saveData.ownsRedCar) saveData.selectedColorId = 1;
            else if (saveData.totalStars >= 50) { 
                saveData.totalStars -= 50; 
                saveData.ownsRedCar = true; 
                saveData.selectedColorId = 1; 
            }
        }
        
        if (CheckCollisionPointRec(mousePoint, {650, 500, 200, 50})) {
            if (saveData.ownsPurpleCar) saveData.selectedColorId = 2;
            else if (saveData.totalStars >= 150) { 
                saveData.totalStars -= 150; 
                saveData.ownsPurpleCar = true; 
                saveData.selectedColorId = 2; 
            }
        }
        SaveGameData(saveData);
    }
}

void Game::HandleSettingsInput(Vector2 mousePoint) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePoint, backSetBtn)) state = MAIN_MENU;
        if (CheckCollisionPointRec(mousePoint, langBtn)) { saveData.isEnglish = !saveData.isEnglish; SaveGameData(saveData); }
        if (CheckCollisionPointRec(mousePoint, resBtn)) { saveData.isFullscreen = !saveData.isFullscreen; ToggleFullscreen(); SaveGameData(saveData); }
        if (CheckCollisionPointRec(mousePoint, nameChangeBtn)) { isNameSaved = false; playerName[0] = '\0'; letterCount = 0; state = MAIN_MENU; }
        if (CheckCollisionPointRec(mousePoint, deleteDataBtn)) {
            DeleteSaveData(); ClearScoreboard(); if (IsWindowFullscreen()) ToggleFullscreen();
            saveData = { 0, false, false, 0, saveData.isEnglish, false, "" };
            playerName[0] = '\0'; letterCount = 0; isNameSaved = false; state = MAIN_MENU;
        }
    }
}

void Game::ResetObstacle(Obstacle &obs, float startY) {
    obs.rect = { (float)GetRandomValue(310, 590), startY, (float)GetRandomValue(70, 110), 40 };
}

void Game::SpawnStar() {
    bonusStar.rect = { (float)GetRandomValue(320, 650), -100, 30, 30 };
    bonusStar.active = true;
}

void Game::Draw() {
    float s = std::min((float)GetScreenWidth()/1000.0f, (float)GetScreenHeight()/800.0f);
    Vector2 m = GetMousePosition();
    Vector2 mp = { (m.x - (GetScreenWidth() - (1000.0f * s)) * 0.5f) / s, (m.y - (GetScreenHeight() - (800.0f * s)) * 0.5f) / s };

    BeginTextureMode(target);
    ClearBackground(DARKGREEN);
    DrawRectangle(300, 0, 400, 800, DARKGRAY);

    if (state == PLAYING || state == PAUSED || state == GAMEOVER || state == EXIT_PROMPT) {
        for (auto &obs : obstacles) DrawTexturePro(obstacleTex, {0,0,(float)obstacleTex.width,(float)obstacleTex.height}, obs.rect, {0,0}, 0, WHITE);
        Texture2D cur = carTextures[saveData.selectedColorId];
        DrawTexturePro(cur, {0,0,(float)cur.width,(float)cur.height}, player.rect, {0,0}, 0, WHITE);
        if (bonusStar.active) DrawTexturePro(starTex, {0,0,(float)starTex.width,(float)starTex.height}, bonusStar.rect, {0,0}, 0, WHITE);
        
        // FIX: Highscore (GetTopScore) am Ende hinzugefügt
       // Ersetze in game.cpp den Aufruf:
// DrawHUD(..., GetTopScore());

// Durch diesen Block:
std::vector<ScoreEntry> tempScores = LoadScoreboard();
int topScore = tempScores.empty() ? 0 : tempScores[0].score; // Nimmt den ersten (besten) Eintrag
// In Game::Draw()
DrawHUD(playerName, totalTimeSurvived, currentScore, earnedStarsThisRound, saveData.isEnglish, starTex, GetTopScore());
        
        if (state == PAUSED) DrawPauseMenu(mp, btnPrimary, btnMenu, saveData.isEnglish);
        if (state == GAMEOVER) DrawGameOverMenu(playerName, currentScore, totalTimeSurvived, earnedStarsThisRound, mp, btnMenu, saveData.isEnglish);
    }

    if (state == MAIN_MENU) DrawMainMenu(playerName, letterCount, 0, mp, startBtn, scoreBtn, shopBtn, settingsBtn, descBtn, saveData.totalStars, isNameSaved, saveData.isEnglish);
    else if (state == SHOP_MENU) DrawShopMenu(saveData, mp, {400, 500, 200, 50}, {650, 500, 200, 50}, backMenuBtn, saveData.isEnglish, carTextures);
    else if (state == SETTINGS) DrawSettingsMenu(mp, langBtn, resBtn, nameChangeBtn, deleteDataBtn, backSetBtn, saveData.isEnglish, saveData.isFullscreen);
    else if (state == SCOREBOARD_MENU) DrawScoreboardMenu(LoadScoreboard(), mp, backMenuBtn, saveData.isEnglish);
    else if (state == DESCRIPTION) DrawDescriptionMenu(mp, backMenuBtn, saveData.isEnglish);

    if (state == EXIT_PROMPT) {
        DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.7f));
        Rectangle box = { 350, 300, 300, 200 };
        DrawRectangleRec(box, RAYWHITE);
        DrawRectangleLinesEx(box, 3, GOLD);
        DrawTextCentered(saveData.isEnglish ? "EXIT GAME?" : "SPIEL BEENDEN?", 340, 25, DARKGRAY);
        
        Rectangle yBtn = { 380, 420, 100, 40 };
        Rectangle nBtn = { 520, 420, 100, 40 };
        
        bool hoverY = CheckCollisionPointRec(mp, yBtn);
        DrawRectangleRec(yBtn, hoverY ? RED : MAROON);
        DrawText(saveData.isEnglish ? "YES" : "JA", (int)yBtn.x + 35, (int)yBtn.y + 10, 20, WHITE);
        
        bool hoverN = CheckCollisionPointRec(mp, nBtn);
        DrawRectangleRec(nBtn, hoverN ? LIGHTGRAY : GRAY);
        DrawText(saveData.isEnglish ? "NO" : "NEIN", (int)nBtn.x + 35, (int)nBtn.y + 10, 20, BLACK);
    }
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    float finalScale = std::min((float)GetScreenWidth() / 1000.0f, (float)GetScreenHeight() / 800.0f);
    DrawTexturePro(target.texture, 
        {0, 0, (float)target.texture.width, (float)-target.texture.height},
        {(GetScreenWidth() - 1000.0f * finalScale) * 0.5f, 
         (GetScreenHeight() - 800.0f * finalScale) * 0.5f, 
         1000.0f * finalScale, 800.0f * finalScale}, 
        {0, 0}, 0.0f, WHITE);
    EndDrawing();
}

void Game::Cleanup() {
    for (int i = 0; i < 3; i++) UnloadTexture(carTextures[i]);
    UnloadTexture(obstacleTex); UnloadTexture(starTex);
    UnloadRenderTexture(target);
    CloseWindow();
    exit(0);
}