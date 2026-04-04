#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include <algorithm>
#include <cstring>
#include <cmath>

// ============================================================
//  Interne Hilfsfunktionen für Straßenrandobjekte
// ============================================================
static void DrawTree(float x, float y, float s)
{
    // Stamm
    DrawRectangle((int)(x - 5 * s), (int)(y - 22 * s), (int)(10 * s), (int)(22 * s),
                  {101, 67, 33, 255});
    // Untere Blattkrone
    DrawCircle((int)x, (int)(y - 30 * s), (int)(22 * s), {20, 100, 20, 255});
    // Mittlere Blattkrone
    DrawCircle((int)(x - 10 * s), (int)(y - 42 * s), (int)(16 * s), {30, 130, 30, 255});
    DrawCircle((int)(x + 10 * s), (int)(y - 42 * s), (int)(16 * s), {30, 130, 30, 255});
    // Obere Blattkrone
    DrawCircle((int)x, (int)(y - 52 * s), (int)(18 * s), {50, 160, 50, 255});
}

static void DrawCactus(float x, float y, float s)
{
    Color cGreen = {34, 115, 34, 255};
    int bw = (int)(12 * s);
    int bh = (int)(50 * s);
    // Hauptkörper
    DrawRectangle((int)(x - bw / 2), (int)(y - bh), bw, bh, cGreen);
    // Linker Arm
    DrawRectangle((int)(x - bw / 2 - (int)(16 * s)), (int)(y - (int)(32 * s)),
                  (int)(16 * s), (int)(8 * s), cGreen);
    DrawRectangle((int)(x - bw / 2 - (int)(9 * s)), (int)(y - (int)(44 * s)),
                  (int)(9 * s), (int)(14 * s), cGreen);
    // Rechter Arm
    DrawRectangle((int)(x + bw / 2), (int)(y - (int)(26 * s)),
                  (int)(16 * s), (int)(8 * s), cGreen);
    DrawRectangle((int)(x + bw / 2 + (int)(7 * s)), (int)(y - (int)(38 * s)),
                  (int)(9 * s), (int)(14 * s), cGreen);
}

// ============================================================
//  Init
// ============================================================
void Game::Init()
{
    saveData = LoadSaveGame();
    InitWindow(1000, 800, "Car Race");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    // Icon setzen
    Image icon = LoadImage("icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    // Assets laden
    carTextures[0] = LoadTexture("assets/car_white.png");
    carTextures[1] = LoadTexture("assets/car_red.png");
    carTextures[2] = LoadTexture("assets/car_blue.png");
    obstacleTex = LoadTexture("assets/hindernis.png");
    starTex = LoadTexture("assets/star.png");
    clockTex = LoadTexture("assets/clock.png");
    shieldTex = LoadTexture("assets/shield.png");
    // Offscreen-RenderTexture für Auflösungsskalierung
    target = LoadRenderTexture(1000, 800);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
    if (saveData.isFullscreen && !IsWindowFullscreen())
        ToggleFullscreen();
    // --- Audio ---
    InitAudioDevice();
    musicVolume = 0.7f;
    musicLoaded = false;
    crashSoundLoaded = false;
    volumeDragging = false;
    if (FileExists("sounds/soundtrack.mp3"))
    {
        backgroundMusic = LoadMusicStream("sounds/soundtrack.mp3");
        backgroundMusic.looping = true;
        SetMusicVolume(backgroundMusic, musicVolume);
        PlayMusicStream(backgroundMusic);
        musicLoaded = true;
    }
    if (FileExists("sounds/crash.mp3"))
    {
        crashSound = LoadSound("sounds/crash.mp3");
        SetSoundVolume(crashSound, musicVolume);
        crashSoundLoaded = true;
    }
    // --- Straßenvisuals ---
    roadScrollOffset = 0.0f;
    InitSideObjects();
    // Startzustand
    state = MAIN_MENU;
    isNameSaved = false;
    framesCounter = 0;
    playerName[0] = '\0';
    if (!saveData.lastPlayerName.empty() && saveData.lastPlayerName != "Gast")
    {
        std::strncpy(playerName, saveData.lastPlayerName.c_str(), 15);
        isNameSaved = true;
    }
    playerName[15] = '\0';
    letterCount = static_cast<int>(std::strlen(playerName));
    // Highscore einmal laden (wird nach jedem Spielende aktualisiert)
    cachedTopScore = GetTopScore();
    // Button-Layouts (1000×800 Logikraum)
    float centerX = 1000 / 2.0f - 125.0f;
    startBtn = {centerX, 300, 250, 50};
    scoreBtn = {centerX, 360, 250, 50};
    shopBtn = {centerX, 420, 250, 50};
    settingsBtn = {centerX, 480, 250, 50};
    descBtn = {centerX, 540, 250, 50};
    backMenuBtn = {centerX, 680, 250, 50};
    langBtn = {centerX, 180, 250, 50};
    resBtn = {centerX, 250, 250, 50};
    nameChangeBtn = {centerX, 320, 250, 50};
    deleteDataBtn = {centerX, 390, 250, 50};
    backSetBtn = {centerX, 550, 250, 50};
    btnPrimary = {centerX, 400, 250, 50};
    btnMenu = {centerX, 480, 250, 50};
    // Shop-Buttons
    redCarBtn = {400, 500, 200, 50};
    blueCarBtn = {650, 500, 200, 50};
    // Hindernisse gleichmäßig über den oberen Bereich verteilen
    obstacles.resize(4);
    for (int i = 0; i < 4; i++)
        ResetObstacle(obstacles[i], -200.0f - (i * 400.0f));
    bonusStar = {{0, 0, 30, 30}, false};
    clockBuff = {{0, 0, 36, 36}, false};
    shieldBuff = {{0, 0, 48, 48}, false};
    buffActive = false;
    buffTimer = 0.0f;
    speedBeforeBuff = 0.0f;
    shieldActive = false;
    shieldTimer = 0.0f;
}

// ============================================================
//  Update
// ============================================================
void Game::Update()
{
    framesCounter++;
    // Musik-Stream jedes Frame aktualisieren (hält den Buffer gefüllt)
    if (musicLoaded)
        UpdateMusicStream(backgroundMusic);
    Vector2 mousePoint = GetScaledMouse();
    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (state == EXIT_PROMPT)
            state = previousState;
        else
        {
            previousState = state;
            state = EXIT_PROMPT;
        }
    }
    switch (state)
    {
    case EXIT_PROMPT:
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePoint, {380, 420, 100, 40}))
                Cleanup();
            if (CheckCollisionPointRec(mousePoint, {520, 420, 100, 40}))
                state = previousState;
        }
        break;
    case MAIN_MENU:
        HandleMenuInput(mousePoint);
        break;
    case SHOP_MENU:
        HandleShopInput(mousePoint);
        break;
    case SETTINGS:
        HandleSettingsInput(mousePoint);
        break;
    case PLAYING:
        UpdateGameLogic(GetFrameTime());
        break;
    case PAUSED:
    {
        // Lautstärke-Slider ziehen
        const Rectangle sliderTrack = {350, 575, 300, 16};
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if (volumeDragging || CheckCollisionPointRec(mousePoint, sliderTrack))
            {
                volumeDragging = true;
                musicVolume = std::clamp(
                    (mousePoint.x - sliderTrack.x) / sliderTrack.width, 0.0f, 1.0f);
                if (musicLoaded)
                    SetMusicVolume(backgroundMusic, musicVolume);
                if (crashSoundLoaded)
                    SetSoundVolume(crashSound, musicVolume);
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            volumeDragging = false;
            if (CheckCollisionPointRec(mousePoint, btnMenu))
                state = MAIN_MENU;
            if (CheckCollisionPointRec(mousePoint, btnPrimary))
                state = PLAYING;
        }
        break;
    }
    case SCOREBOARD_MENU:
    case DESCRIPTION:
    case GAMEOVER:
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePoint, backMenuBtn) ||
                CheckCollisionPointRec(mousePoint, btnMenu))
                state = MAIN_MENU;
        }
        break;
    default:
        break;
    }
}

//  Spiellogik
// ============================================================
void Game::UpdateGameLogic(float deltaTime)
{
    if (IsKeyPressed(KEY_P))
    {
        state = PAUSED;
        return;
    }
    totalTimeSurvived += deltaTime;
    currentScore = static_cast<int>(totalTimeSurvived * 50.0f);
    // Straßen-Scroll-Offset für Mittellinie & Leitplanken-Pfosten aktualisieren
    roadScrollOffset += currentSpeed * deltaTime;
    // Seitenrandobjekte (Bäume / Kakteen) nach unten scrollen und zurücksetzen
    for (auto &obj : sideObjects)
    {
        obj.y += currentSpeed * deltaTime;
        if (obj.y > 860.0f)
        {
            obj.y = (float)GetRandomValue(-600, -80);
            obj.size = 0.8f + GetRandomValue(0, 5) * 0.08f;
            obj.variant = GetRandomValue(0, 2);
        }
    }
    // Uhr-Buff Timer
    if (buffActive)
    {
        buffTimer -= deltaTime;
        if (buffTimer <= 0.0f)
        {
            buffActive = false;
            buffTimer = 0.0f;
            currentSpeed = speedBeforeBuff;
        }
    }
    // Schild-Buff Timer
    if (shieldActive)
    {
        shieldTimer -= deltaTime;
        if (shieldTimer <= 0.0f)
        {
            shieldActive = false;
            shieldTimer = 0.0f;
        }
    }
    // Geschwindigkeit nur erhöhen wenn kein Uhr-Buff aktiv
    if (!buffActive)
        currentSpeed = GetCurrentSpeed(currentSpeed, deltaTime);
    player.speed = GetDynamicPlayerSpeed(currentSpeed);
    // Spieler bewegen und auf Fahrbahn begrenzen
    if (IsKeyDown(KEY_LEFT))
        player.rect.x -= player.speed * deltaTime;
    if (IsKeyDown(KEY_RIGHT))
        player.rect.x += player.speed * deltaTime;
    player.rect.x = std::clamp(player.rect.x, 300.0f, 700.0f - player.rect.width);
    // Hindernisse bewegen und bei Ausfahrt zurücksetzen
    for (auto &obs : obstacles)
    {
        obs.rect.y += currentSpeed * deltaTime;
        if (obs.rect.y > 800.0f)
        {
            float highestY = 0.0f;
            for (const auto &o : obstacles)
                if (o.rect.y < highestY)
                    highestY = o.rect.y;
            ResetObstacle(obs, highestY - 400.0f);
        }
        // Kollision → nur Game Over wenn kein Schild aktiv
        if (CheckCollisionRecs(player.rect, obs.rect) && !shieldActive)
        {
            if (crashSoundLoaded)
                PlaySound(crashSound);
            AddOrUpdateScore(playerName, currentScore, totalTimeSurvived);
            saveData.totalStars += earnedStarsThisRound;
            SaveGameData(saveData);
            cachedTopScore = GetTopScore();
            state = GAMEOVER;
            return;
        }
    }
    // Bonusstern spawnen
    if (!bonusStar.active && GetRandomValue(0, 600) < 3)
        SpawnStar();
    if (bonusStar.active)
    {
        bonusStar.rect.y += currentSpeed * deltaTime;
        if (CheckCollisionRecs(player.rect, bonusStar.rect))
        {
            earnedStarsThisRound++;
            bonusStar.active = false;
        }
        if (bonusStar.rect.y > 800.0f)
            bonusStar.active = false;
    }
    // Uhr-Buff spawnen
    if (!clockBuff.active && !buffActive && GetRandomValue(0, 3000) < 2)
        SpawnClock();
    if (clockBuff.active)
    {
        clockBuff.rect.y += currentSpeed * deltaTime;
        if (CheckCollisionRecs(player.rect, clockBuff.rect))
        {
            speedBeforeBuff = currentSpeed;
            currentSpeed *= 0.4f;
            buffActive = true;
            buffTimer = 3.0f;
            clockBuff.active = false;
        }
        if (clockBuff.rect.y > 800.0f)
            clockBuff.active = false;
    }
    // Schild-Buff spawnen
    if (!shieldBuff.active && !shieldActive && GetRandomValue(0, 6000) < 2)
        SpawnShield();
    if (shieldBuff.active)
    {
        shieldBuff.rect.y += currentSpeed * deltaTime;
        if (CheckCollisionRecs(player.rect, shieldBuff.rect))
        {
            shieldActive = true;
            shieldTimer = 5.0f;
            shieldBuff.active = false;
        }
        if (shieldBuff.rect.y > 800.0f)
            shieldBuff.active = false;
    }
}

//  Eingabe-Handler
// ============================================================
void Game::HandleMenuInput(Vector2 mousePoint)
{
    if (!isNameSaved)
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if (key > 32 && key <= 125 && letterCount < 15)
            {
                playerName[letterCount] = static_cast<char>(key);
                playerName[letterCount + 1] = '\0';
                letterCount++;
            }
            key = GetCharPressed(); // Korrigiert: Hole nächsten Key in der Schleife
        }
        if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0)
            playerName[--letterCount] = '\0';
        if (IsKeyPressed(KEY_ENTER) && letterCount > 0)
        {
            saveData.lastPlayerName = playerName;
            SaveGameData(saveData);
            isNameSaved = true;
        }
    }
    if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        return;
    if (CheckCollisionPointRec(mousePoint, startBtn) && (letterCount > 0 || isNameSaved))
    {
        currentScore = 0;
        totalTimeSurvived = 0.0f;
        earnedStarsThisRound = 0;
        currentSpeed = SPEED_START;
        isNameSaved = true;
        saveData.lastPlayerName = playerName;
        SaveGameData(saveData);
        roadScrollOffset = 0.0f;
        InitSideObjects();
        Texture2D &cur = carTextures[saveData.selectedColorId];
        InitPlayer(player, 1000, 800, 45.0f,
                   45.0f * (static_cast<float>(cur.height) / cur.width),
                   GetCarColor(saveData.selectedColorId));
        // Korrigiert: Schleife zum Resetten der Hindernisse hinzugefügt
        for (int i = 0; i < (int)obstacles.size(); i++)
            ResetObstacle(obstacles[i], -200.0f - (i * 400.0f));

        bonusStar.active = false;
        clockBuff.active = false;
        shieldBuff.active = false;
        buffActive = false;
        buffTimer = 0.0f;
        speedBeforeBuff = 0.0f;
        shieldActive = false;
        shieldTimer = 0.0f;
        state = PLAYING;
    }
    else if (CheckCollisionPointRec(mousePoint, scoreBtn))
        state = SCOREBOARD_MENU;
    else if (CheckCollisionPointRec(mousePoint, shopBtn))
        state = SHOP_MENU;
    else if (CheckCollisionPointRec(mousePoint, settingsBtn))
        state = SETTINGS;
    else if (CheckCollisionPointRec(mousePoint, descBtn))
        state = DESCRIPTION;
}

void Game::HandleShopInput(Vector2 mousePoint)
{
    if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        return;
    if (CheckCollisionPointRec(mousePoint, backMenuBtn))
    {
        state = MAIN_MENU;
        return;
    }
    if (CheckCollisionPointRec(mousePoint, {150, 500, 200, 50}))
        saveData.selectedColorId = 0;
    if (CheckCollisionPointRec(mousePoint, redCarBtn))
    {
        if (saveData.ownsRedCar)
            saveData.selectedColorId = 1;
        else if (saveData.totalStars >= PRICE_RED_CAR)
        {
            saveData.totalStars -= PRICE_RED_CAR;
            saveData.ownsRedCar = true;
            saveData.selectedColorId = 1;
        }
    }
    if (CheckCollisionPointRec(mousePoint, blueCarBtn))
    {
        if (saveData.ownsBlueCar)
            saveData.selectedColorId = 2;
        else if (saveData.totalStars >= PRICE_BLUE_CAR)
        {
            saveData.totalStars -= PRICE_BLUE_CAR;
            saveData.ownsBlueCar = true;
            saveData.selectedColorId = 2;
        }
    }
    SaveGameData(saveData);
}

void Game::HandleSettingsInput(Vector2 mousePoint)
{
    if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        return;
    if (CheckCollisionPointRec(mousePoint, backSetBtn))
    {
        state = MAIN_MENU;
    }
    else if (CheckCollisionPointRec(mousePoint, langBtn))
    {
        saveData.isEnglish = !saveData.isEnglish;
        SaveGameData(saveData);
    }
    else if (CheckCollisionPointRec(mousePoint, resBtn))
    {
        saveData.isFullscreen = !saveData.isFullscreen;
        ToggleFullscreen();
        SaveGameData(saveData);
    }
    else if (CheckCollisionPointRec(mousePoint, nameChangeBtn))
    {
        isNameSaved = false;
        playerName[0] = '\0';
        letterCount = 0;
        state = MAIN_MENU;
    }
    else if (CheckCollisionPointRec(mousePoint, deleteDataBtn))
    {
        DeleteSaveData();
        ClearScoreboard();
        if (IsWindowFullscreen())
            ToggleFullscreen();
        saveData = {0, false, false, 0, saveData.isEnglish, false, ""};
        playerName[0] = '\0';
        letterCount = 0;
        isNameSaved = false;
        cachedTopScore = 0;
        state = MAIN_MENU;
    }
}

//  Hilfsmethoden
// ============================================================
Vector2 Game::GetScaledMouse() const
{
    float s = std::min(static_cast<float>(GetScreenWidth()) / 1000.0f,
                       static_cast<float>(GetScreenHeight()) / 800.0f);
    Vector2 m = GetMousePosition();
    return {
        (m.x - (GetScreenWidth() - 1000.0f * s) * 0.5f) / s,
        (m.y - (GetScreenHeight() - 800.0f * s) * 0.5f) / s};
}

void Game::ResetObstacle(Obstacle &obs, float startY)
{
    obs.rect = {
        static_cast<float>(GetRandomValue(310, 590)),
        startY,
        static_cast<float>(GetRandomValue(70, 110)),
        40.0f};
}

void Game::SpawnStar()
{
    bonusStar.rect = {static_cast<float>(GetRandomValue(320, 650)), -100.0f, 30.0f, 30.0f};
    bonusStar.active = true;
}

void Game::SpawnClock()
{
    clockBuff.rect = {static_cast<float>(GetRandomValue(320, 640)), -100.0f, 36.0f, 36.0f};
    clockBuff.active = true;
}

void Game::SpawnShield()
{
    shieldBuff.rect = {static_cast<float>(GetRandomValue(320, 640)), -100.0f, 48.0f, 48.0f};
    shieldBuff.active = true;
}

// ============================================================
//  Draw
// ============================================================
void Game::Draw()
{
    Vector2 mousePoint = GetScaledMouse();
    BeginTextureMode(target);
    DrawRoadBackground();
    bool inGame = (state == PLAYING || state == PAUSED ||
                   state == GAMEOVER || state == EXIT_PROMPT);
    if (inGame)
    {
        // Hindernisse
        for (const auto &obs : obstacles)
        {
            DrawTexturePro(obstacleTex,
                           {0, 0, static_cast<float>(obstacleTex.width), static_cast<float>(obstacleTex.height)},
                           obs.rect, {0, 0}, 0.0f, WHITE);
        }
        // Spielerauto
        Texture2D &cur = carTextures[saveData.selectedColorId];
        DrawTexturePro(cur,
                       {0, 0, static_cast<float>(cur.width), static_cast<float>(cur.height)},
                       player.rect, {0, 0}, 0.0f, WHITE);
        // Schild-Rahmen um das Auto
        if (shieldActive)
        {
            DrawRectangleLinesEx(
                {player.rect.x - 4, player.rect.y - 4,
                 player.rect.width + 8, player.rect.height + 8},
                4.0f, GOLD);
        }
        // Bonusstern
        if (bonusStar.active)
        {
            DrawTexturePro(starTex,
                           {0, 0, static_cast<float>(starTex.width), static_cast<float>(starTex.height)},
                           bonusStar.rect, {0, 0}, 0.0f, WHITE);
        }
        // Uhr-Buff Icon
        if (clockBuff.active)
        {
            DrawTexturePro(clockTex,
                           {0, 0, static_cast<float>(clockTex.width), static_cast<float>(clockTex.height)},
                           clockBuff.rect, {0, 0}, 0.0f, WHITE);
        }
        // Schild-Buff Icon
        if (shieldBuff.active)
        {
            DrawTexturePro(shieldTex,
                           {0, 0, static_cast<float>(shieldTex.width), static_cast<float>(shieldTex.height)},
                           shieldBuff.rect, {0, 0}, 0.0f, WHITE);
        }
        // HUD
        DrawHUD(playerName, totalTimeSurvived, currentScore,
                earnedStarsThisRound, saveData.isEnglish, starTex, cachedTopScore);
        // Uhr-Buff Anzeige
        if (buffActive)
        {
            DrawRectangleLinesEx({300, 0, 400, 800}, 4, Fade(SKYBLUE, 0.7f));
            float barMaxW = 380.0f;
            float barFill = barMaxW * (buffTimer / 3.0f);
            DrawRectangle(310, 50, (int)barMaxW, 12, Fade(DARKBLUE, 0.5f));
            DrawRectangle(310, 50, (int)barFill, 12, SKYBLUE);
            const char *buffLabel = saveData.isEnglish
                                        ? TextFormat("SLOW  %.1fs", buffTimer)
                                        : TextFormat("LANGSAM  %.1fs", buffTimer);
            DrawText(buffLabel, 355, 65, 18, SKYBLUE);
        }
        // Schild-Buff Anzeige
        if (shieldActive)
        {
            DrawRectangleLinesEx({300, 0, 400, 800}, 4, Fade(GOLD, 0.7f));
            float barMaxW = 380.0f;
            float barFill = barMaxW * (shieldTimer / 5.0f);
            DrawRectangle(310, 85, (int)barMaxW, 12, Fade(DARKBROWN, 0.5f));
            DrawRectangle(310, 85, (int)barFill, 12, GOLD);
            const char *shieldLabel = saveData.isEnglish
                                          ? TextFormat("SHIELD  %.1fs", shieldTimer)
                                          : TextFormat("SCHILD  %.1fs", shieldTimer);
            DrawText(shieldLabel, 350, 100, 18, GOLD);
        }
        if (state == PAUSED)
            DrawPauseMenu(mousePoint, btnPrimary, btnMenu, saveData.isEnglish, musicVolume);
        if (state == GAMEOVER)
            DrawGameOverMenu(playerName, currentScore, totalTimeSurvived,
                             earnedStarsThisRound, mousePoint, btnMenu, saveData.isEnglish);
    }
    // Menüs
    switch (state)
    {
    case MAIN_MENU:
        DrawMainMenu(playerName, letterCount, framesCounter, mousePoint,
                     startBtn, scoreBtn, shopBtn, settingsBtn, descBtn,
                     saveData.totalStars, isNameSaved, saveData.isEnglish);
        break;
    case SHOP_MENU:
        DrawShopMenu(saveData, mousePoint, redCarBtn, blueCarBtn,
                     backMenuBtn, saveData.isEnglish, carTextures);
        break;
    case SETTINGS:
        DrawSettingsMenu(mousePoint, langBtn, resBtn, nameChangeBtn,
                         deleteDataBtn, backSetBtn, saveData.isEnglish, saveData.isFullscreen);
        break;
    case SCOREBOARD_MENU:
        DrawScoreboardMenu(LoadScoreboard(), mousePoint, backMenuBtn, saveData.isEnglish);
        break;
    case DESCRIPTION:
        DrawDescriptionMenu(mousePoint, backMenuBtn, saveData.isEnglish);
        break;
    default:
        break;
    }
    // Exit-Dialog
    if (state == EXIT_PROMPT)
    {
        DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.7f));
        Rectangle box = {350, 300, 300, 200};
        DrawRectangleRec(box, RAYWHITE);
        DrawRectangleLinesEx(box, 3, GOLD);
        DrawTextCentered(saveData.isEnglish ? "EXIT GAME?" : "SPIEL BEENDEN?", 340, 25, DARKGRAY);
        Rectangle yBtn = {380, 420, 100, 40};
        Rectangle nBtn = {520, 420, 100, 40};
        DrawRectangleRec(yBtn, CheckCollisionPointRec(mousePoint, yBtn) ? RED : MAROON);
        DrawRectangleRec(nBtn, CheckCollisionPointRec(mousePoint, nBtn) ? GRAY : DARKGRAY);
        DrawText(saveData.isEnglish ? "YES" : "JA", (int)yBtn.x + 35, (int)yBtn.y + 10, 20, WHITE);
        DrawText(saveData.isEnglish ? "NO" : "NEIN", (int)nBtn.x + 35, (int)nBtn.y + 10, 20, WHITE);
    }
    EndTextureMode();

    // Offscreen-Buffer skaliert auf tatsächliche Fenstergröße zeichnen
    BeginDrawing();
    ClearBackground(BLACK);
    float finalScale = std::min(static_cast<float>(GetScreenWidth()) / 1000.0f,
                                static_cast<float>(GetScreenHeight()) / 800.0f);
    DrawTexturePro(target.texture,
                   {0, 0, static_cast<float>(target.texture.width), -static_cast<float>(target.texture.height)},
                   {(GetScreenWidth() - 1000.0f * finalScale) * 0.5f,
                    (GetScreenHeight() - 800.0f * finalScale) * 0.5f,
                    1000.0f * finalScale,
                    800.0f * finalScale},
                   {0, 0}, 0.0f, WHITE);
    EndDrawing();
}

//  Cleanup
// ============================================================
void Game::Cleanup()
{
    for (int i = 0; i < 3; i++)
        UnloadTexture(carTextures[i]);
    UnloadTexture(obstacleTex);
    UnloadTexture(starTex);
    UnloadTexture(clockTex);
    UnloadTexture(shieldTex);
    UnloadRenderTexture(target);
    if (musicLoaded)
    {
        StopMusicStream(backgroundMusic);
        UnloadMusicStream(backgroundMusic);
    }
    if (crashSoundLoaded)
        UnloadSound(crashSound);
    CloseAudioDevice();
    CloseWindow();
    exit(0);
}

// ============================================================
//  Straßenrandobjekte initialisieren
// ============================================================
void Game::InitSideObjects()
{
    sideObjects.clear();
    const float leftXs[] = {55.0f, 130.0f, 205.0f, 260.0f};
    const float rightXs[] = {740.0f, 795.0f, 865.0f, 945.0f};
    for (int i = 0; i < 4; i++)
    {
        sideObjects.push_back({leftXs[i],
                               (float)GetRandomValue(-700, 800),
                               0.8f + GetRandomValue(0, 5) * 0.08f,
                               GetRandomValue(0, 2)});
        sideObjects.push_back({rightXs[i],
                               (float)GetRandomValue(-700, 800),
                               0.8f + GetRandomValue(0, 5) * 0.08f,
                               GetRandomValue(0, 2)});
    }
}

// ============================================================
//  Straßenhintergrund zeichnen
// ============================================================
void Game::DrawRoadBackground()
{
    bool isDesert = (currentScore >= 5000);
    if (isDesert)
        ClearBackground({210, 180, 110, 255});
    else
        ClearBackground({28, 80, 28, 255});

    for (const auto &obj : sideObjects)
    {
        if (isDesert)
            DrawCactus(obj.x, obj.y, obj.size);
        else
            DrawTree(obj.x, obj.y, obj.size);
    }

    DrawRectangle(300, 0, 400, 800, {48, 48, 48, 255});
    DrawRectangle(297, 0, 6, 800, {210, 210, 210, 255});
    DrawRectangle(697, 0, 6, 800, {210, 210, 210, 255});
    DrawRectangle(280, 0, 18, 800, {150, 150, 155, 255});
    DrawRectangle(280, 0, 4, 800, {220, 225, 230, 255});
    DrawRectangle(702, 0, 18, 800, {150, 150, 155, 255});
    DrawRectangle(716, 0, 4, 800, {220, 225, 230, 255});

    const float postSpacing = 90.0f;
    float postStart = std::fmod(roadScrollOffset, postSpacing) - postSpacing;
    for (float py = postStart; py < 800.0f; py += postSpacing)
    {
        DrawRectangle(283, (int)py, 12, 14, {90, 90, 95, 255});
        DrawRectangle(705, (int)py, 12, 14, {90, 90, 95, 255});
    }

    const float dashLen = 40.0f, dashSpacing = 80.0f;
    Color dashColor = isDesert ? Color{255, 240, 100, 220} : Color{255, 255, 255, 200};
    float dashStart = std::fmod(roadScrollOffset, dashSpacing) - dashSpacing;
    for (float dy = dashStart; dy < 800.0f; dy += dashSpacing)
        DrawRectangle(498, (int)dy, 4, (int)dashLen, dashColor);
}