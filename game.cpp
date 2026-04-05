#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include <algorithm>
#include <cstring>
#include <cmath>

// Zeichnet eine Textur gestreckt auf dst
static void DrawTexFull(Texture2D tex, Rectangle dst)
{
    DrawTexturePro(tex,
                   {0, 0, (float)tex.width, (float)tex.height},
                   dst, {0, 0}, 0.0f, WHITE);
}

static void DrawTree(float x, float y, float s)
{
    DrawRectangle((int)(x - 5 * s), (int)(y - 22 * s), (int)(10 * s), (int)(22 * s), {101, 67, 33, 255});

    DrawCircle((int)x, (int)(y - 30 * s), (int)(22 * s), {20, 100, 20, 255});
    DrawCircle((int)(x - 10 * s), (int)(y - 42 * s), (int)(16 * s), {30, 130, 30, 255});
    DrawCircle((int)(x + 10 * s), (int)(y - 42 * s), (int)(16 * s), {30, 130, 30, 255});
    DrawCircle((int)x, (int)(y - 52 * s), (int)(18 * s), {50, 160, 50, 255});
}

static void DrawCactus(float x, float y, float s)
{
    Color cGreen = {34, 115, 34, 255};
    int bw = (int)(12 * s), bh = (int)(50 * s);
    DrawRectangle((int)(x - bw / 2), (int)(y - bh), bw, bh, cGreen);
    DrawRectangle((int)(x - bw / 2 - (int)(16 * s)), (int)(y - (int)(32 * s)),
                  (int)(16 * s), (int)(8 * s), cGreen);
    DrawRectangle((int)(x - bw / 2 - (int)(9 * s)), (int)(y - (int)(44 * s)),
                  (int)(9 * s), (int)(14 * s), cGreen);
    DrawRectangle((int)(x + bw / 2), (int)(y - (int)(26 * s)),
                  (int)(16 * s), (int)(8 * s), cGreen);
    DrawRectangle((int)(x + bw / 2 + (int)(7 * s)), (int)(y - (int)(38 * s)),
                  (int)(9 * s), (int)(14 * s), cGreen);
}

// --- Init ---

void Game::Init()
{
    saveData = LoadSaveGame();
    InitWindow(1000, 800, "Car Race");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    Image icon = LoadImage("icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    carTextures[0] = LoadTexture("assets/car_white.png");
    carTextures[1] = LoadTexture("assets/car_red.png");
    carTextures[2] = LoadTexture("assets/car_blue.png");
    obstacleTex = LoadTexture("assets/hindernis.png");
    starTex = LoadTexture("assets/star.png");
    clockTex = LoadTexture("assets/clock.png");
    shieldTex = LoadTexture("assets/shield.png");

    target = LoadRenderTexture(1000, 800);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    if (saveData.isFullscreen && !IsWindowFullscreen())
        ToggleFullscreen();

    // Audio
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
        musicLoaded = true;
    }
    if (FileExists("sounds/crash.mp3"))
    {
        crashSound = LoadSound("sounds/crash.mp3");
        SetSoundVolume(crashSound, musicVolume);
        crashSoundLoaded = true;
    }

    roadScrollOffset = 0.0f;
    InitSideObjects();

    // Startzustand
    state = MAIN_MENU;
    showQuitConfirm = false;
    showPauseBackConfirm = false;
    showNameChangeConfirm = false;
    showDeleteDataConfirm = false;
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
    cachedTopScore = GetTopScore();

    // Button-Layouts (alle zentriert, 250px breit)
    float cx = 1000 / 2.0f - 125.0f;
    startBtn = {cx, 300, 250, 50};
    scoreBtn = {cx, 360, 250, 50};
    shopBtn = {cx, 420, 250, 50};
    settingsBtn = {cx, 480, 250, 50};
    descBtn = {cx, 540, 250, 50};
    backMenuBtn = {cx, 680, 250, 50};
    langBtn = {cx, 180, 250, 50};
    resBtn = {cx, 250, 250, 50};
    nameChangeBtn = {cx, 320, 250, 50};
    deleteDataBtn = {cx, 390, 250, 50};
    backSetBtn = {cx, 550, 250, 50};
    btnPrimary = {cx, 400, 250, 50};
    btnMenu = {cx, 480, 250, 50};
    pauseResumeBtn = {cx, 280, 250, 50};
    pauseMenuBtn = {cx, 350, 250, 50};
    pauseQuitBtn = {cx, 420, 250, 50};
    redCarBtn = {400, 500, 200, 50};
    blueCarBtn = {650, 500, 200, 50};

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

// --- Update ---

void Game::Update()
{
    framesCounter++;
    if (musicLoaded)
        UpdateMusicStream(backgroundMusic);

    Vector2 mousePoint = GetScaledMouse();

    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (state == PLAYING)
        {
            if (musicLoaded)
                PauseMusicStream(backgroundMusic);
            state = PAUSED;
            showQuitConfirm = showPauseBackConfirm = false;
        }
        else if (state == PAUSED)
        {
            if (showQuitConfirm)
                showQuitConfirm = false;
            else if (showPauseBackConfirm)
                showPauseBackConfirm = false;
            else
            {
                if (musicLoaded)
                    ResumeMusicStream(backgroundMusic);
                state = PLAYING;
            }
        }
        else if (state == SETTINGS)
        {
            if (showNameChangeConfirm)
                showNameChangeConfirm = false;
            else if (showDeleteDataConfirm)
                showDeleteDataConfirm = false;
            else
            {
                previousState = state;
                state = EXIT_PROMPT;
            }
        }
        else if (state == EXIT_PROMPT)
            state = previousState;
        else if (state != GAMEOVER)
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
        const Rectangle sliderTrack = {350, 520, 300, 16};
        bool anyConfirm = showQuitConfirm || showPauseBackConfirm;

        if (!anyConfirm)
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                if (volumeDragging || CheckCollisionPointRec(mousePoint, sliderTrack))
                {
                    volumeDragging = true;
                    musicVolume = std::clamp((mousePoint.x - sliderTrack.x) / sliderTrack.width, 0.0f, 1.0f);
                    if (musicLoaded)
                        SetMusicVolume(backgroundMusic, musicVolume);
                    if (crashSoundLoaded)
                        SetSoundVolume(crashSound, musicVolume);
                }
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                volumeDragging = false;
                if (CheckCollisionPointRec(mousePoint, pauseResumeBtn))
                {
                    if (musicLoaded)
                        ResumeMusicStream(backgroundMusic);
                    state = PLAYING;
                }
                else if (CheckCollisionPointRec(mousePoint, pauseMenuBtn))
                    showPauseBackConfirm = true;
                else if (CheckCollisionPointRec(mousePoint, pauseQuitBtn))
                    showQuitConfirm = true;
            }
        }
        else if (showPauseBackConfirm)
        {
            const Rectangle yBtn = {330, 420, 150, 50};
            const Rectangle nBtn = {520, 420, 150, 50};
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(mousePoint, yBtn))
                {
                    showPauseBackConfirm = false;
                    if (musicLoaded)
                        StopMusicStream(backgroundMusic);
                    state = MAIN_MENU;
                }
                if (CheckCollisionPointRec(mousePoint, nBtn))
                    showPauseBackConfirm = false;
            }
        }
        else if (showQuitConfirm)
        {
            const Rectangle yBtn = {330, 420, 150, 50};
            const Rectangle nBtn = {520, 420, 150, 50};
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(mousePoint, yBtn))
                    Cleanup();
                if (CheckCollisionPointRec(mousePoint, nBtn))
                    showQuitConfirm = false;
            }
        }
        break;
    }

    case SCOREBOARD_MENU:
    case DESCRIPTION:
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            if (CheckCollisionPointRec(mousePoint, backMenuBtn))
                state = MAIN_MENU;
        break;

    case GAMEOVER:
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            if (CheckCollisionPointRec(mousePoint, btnMenu))
            {
                if (musicLoaded)
                    StopMusicStream(backgroundMusic);
                state = MAIN_MENU;
            }
        break;

    default:
        break;
    }
}

// --- Spiellogik ---

void Game::UpdateGameLogic(float deltaTime)
{
    totalTimeSurvived += deltaTime;
    currentScore = static_cast<int>(totalTimeSurvived * 50.0f);
    roadScrollOffset += currentSpeed * deltaTime;

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

    // Uhr-Buff: Verlangsamt das Spiel kurzzeitig
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

    // Schild-Buff: Schützt vor einem Treffer
    if (shieldActive)
    {
        shieldTimer -= deltaTime;
        if (shieldTimer <= 0.0f)
        {
            shieldActive = false;
            shieldTimer = 0.0f;
        }
    }

    if (!buffActive)
        currentSpeed = GetCurrentSpeed(currentSpeed, deltaTime);
    player.speed = GetDynamicPlayerSpeed(currentSpeed);

    if (IsKeyDown(KEY_LEFT))
        player.rect.x -= player.speed * deltaTime;
    if (IsKeyDown(KEY_RIGHT))
        player.rect.x += player.speed * deltaTime;
    player.rect.x = std::clamp(player.rect.x, 300.0f, 700.0f - player.rect.width);

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
        if (CheckCollisionRecs(player.rect, obs.rect) && !shieldActive)
        {
            if (musicLoaded)
                StopMusicStream(backgroundMusic);
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

    // Stern einsammeln
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

    // Uhr einsammeln
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

    // Schild einsammeln
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

// --- Eingabe-Handler ---

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
            key = GetCharPressed();
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
                   45.0f * ((float)cur.height / cur.width),
                   GetCarColor(saveData.selectedColorId));

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

        if (musicLoaded)
        {
            StopMusicStream(backgroundMusic);
            SeekMusicStream(backgroundMusic, 0.0f);
            PlayMusicStream(backgroundMusic);
        }
        showQuitConfirm = showPauseBackConfirm = false;
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

    if (showNameChangeConfirm)
    {
        const Rectangle yBtn = {330, 420, 150, 50};
        const Rectangle nBtn = {520, 420, 150, 50};
        if (CheckCollisionPointRec(mousePoint, yBtn))
        {
            showNameChangeConfirm = false;
            isNameSaved = false;
            playerName[0] = '\0';
            letterCount = 0;
            state = MAIN_MENU;
        }
        else if (CheckCollisionPointRec(mousePoint, nBtn))
            showNameChangeConfirm = false;
        return;
    }
    if (showDeleteDataConfirm)
    {
        const Rectangle yBtn = {290, 460, 150, 50};
        const Rectangle nBtn = {480, 460, 150, 50};
        if (CheckCollisionPointRec(mousePoint, yBtn))
        {
            showDeleteDataConfirm = false;
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
        else if (CheckCollisionPointRec(mousePoint, nBtn))
            showDeleteDataConfirm = false;
        return;
    }

    if (CheckCollisionPointRec(mousePoint, backSetBtn))
        state = MAIN_MENU;
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
        showNameChangeConfirm = true;
    else if (CheckCollisionPointRec(mousePoint, deleteDataBtn))
        showDeleteDataConfirm = true;
}

// --- Hilfsmethoden ---

Vector2 Game::GetScaledMouse() const
{
    float s = std::min((float)GetScreenWidth() / 1000.0f, (float)GetScreenHeight() / 800.0f);
    Vector2 m = GetMousePosition();
    return {
        (m.x - (GetScreenWidth() - 1000.0f * s) * 0.5f) / s,
        (m.y - (GetScreenHeight() - 800.0f * s) * 0.5f) / s};
}

void Game::ResetObstacle(Obstacle &obs, float startY)
{
    obs.rect = {
        (float)GetRandomValue(310, 590),
        startY,
        (float)GetRandomValue(70, 110),
        40.0f};
}

void Game::SpawnStar()
{
    bonusStar.rect = {(float)GetRandomValue(320, 650), -100.0f, 30.0f, 30.0f};
    bonusStar.active = true;
}
void Game::SpawnClock()
{
    clockBuff.rect = {(float)GetRandomValue(320, 640), -100.0f, 36.0f, 36.0f};
    clockBuff.active = true;
}
void Game::SpawnShield()
{
    shieldBuff.rect = {(float)GetRandomValue(320, 640), -100.0f, 48.0f, 48.0f};
    shieldBuff.active = true;
}

// --- Draw ---

void Game::Draw()
{
    Vector2 mousePoint = GetScaledMouse();
    BeginTextureMode(target);
    DrawRoadBackground();

    bool inGame = (state == PLAYING || state == PAUSED ||
                   state == GAMEOVER || state == EXIT_PROMPT);
    if (inGame)
    {
        for (const auto &obs : obstacles)
            DrawTexFull(obstacleTex, obs.rect);

        Texture2D &cur = carTextures[saveData.selectedColorId];
        DrawTexFull(cur, player.rect);

        // Schild-Rahmen um Spieler
        if (shieldActive)
            DrawRectangleLinesEx({player.rect.x - 4, player.rect.y - 4,
                                  player.rect.width + 8, player.rect.height + 8},
                                 4.0f, GOLD);

        if (bonusStar.active)
            DrawTexFull(starTex, bonusStar.rect);
        if (clockBuff.active)
            DrawTexFull(clockTex, clockBuff.rect);
        if (shieldBuff.active)
            DrawTexFull(shieldTex, shieldBuff.rect);

        DrawHUD(playerName, totalTimeSurvived, currentScore,
                earnedStarsThisRound, saveData.isEnglish, starTex, cachedTopScore);

        // Uhr-Buff Anzeige
        if (buffActive)
        {
            DrawRectangleLinesEx({300, 0, 400, 800}, 4, Fade(SKYBLUE, 0.7f));
            float fill = 380.0f * (buffTimer / 3.0f);
            DrawRectangle(310, 50, 380, 12, Fade(DARKBLUE, 0.5f));
            DrawRectangle(310, 50, (int)fill, 12, SKYBLUE);
            DrawText(saveData.isEnglish ? TextFormat("SLOW  %.1fs", buffTimer)
                                        : TextFormat("LANGSAM  %.1fs", buffTimer),
                     355, 65, 18, SKYBLUE);
        }

        // Schild-Buff Anzeige
        if (shieldActive)
        {
            DrawRectangleLinesEx({300, 0, 400, 800}, 4, Fade(GOLD, 0.7f));
            float fill = 380.0f * (shieldTimer / 5.0f);
            DrawRectangle(310, 85, 380, 12, Fade(DARKBROWN, 0.5f));
            DrawRectangle(310, 85, (int)fill, 12, GOLD);
            DrawText(saveData.isEnglish ? TextFormat("SHIELD  %.1fs", shieldTimer)
                                        : TextFormat("SCHILD  %.1fs", shieldTimer),
                     350, 100, 18, GOLD);
        }

        if (state == PAUSED)
            DrawPauseMenu(mousePoint, pauseResumeBtn, pauseMenuBtn, pauseQuitBtn,
                          saveData.isEnglish, musicVolume, showQuitConfirm, showPauseBackConfirm);
        if (state == GAMEOVER)
            DrawGameOverMenu(playerName, currentScore, totalTimeSurvived,
                             earnedStarsThisRound, mousePoint, btnMenu, saveData.isEnglish);
    }

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
                         deleteDataBtn, backSetBtn, saveData.isEnglish, saveData.isFullscreen,
                         showNameChangeConfirm, showDeleteDataConfirm);
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

    // Auf den Bildschirm skaliert ausgeben
    BeginDrawing();
    ClearBackground(BLACK);
    float finalScale = std::min((float)GetScreenWidth() / 1000.0f, (float)GetScreenHeight() / 800.0f);
    DrawTexturePro(target.texture,
                   {0, 0, (float)target.texture.width, -(float)target.texture.height},
                   {(GetScreenWidth() - 1000.0f * finalScale) * 0.5f,
                    (GetScreenHeight() - 800.0f * finalScale) * 0.5f,
                    1000.0f * finalScale, 800.0f * finalScale},
                   {0, 0}, 0.0f, WHITE);
    EndDrawing();
}

// --- Cleanup ---

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

// --- Straßenrandobjekte ---

void Game::InitSideObjects()
{
    sideObjects.clear();
    const float leftXs[] = {55.0f, 130.0f, 205.0f, 260.0f};
    const float rightXs[] = {740.0f, 795.0f, 865.0f, 945.0f};
    for (int i = 0; i < 4; i++)
    {
        sideObjects.push_back({leftXs[i], (float)GetRandomValue(-700, 800),
                               0.8f + GetRandomValue(0, 5) * 0.08f, GetRandomValue(0, 2)});
        sideObjects.push_back({rightXs[i], (float)GetRandomValue(-700, 800),
                               0.8f + GetRandomValue(0, 5) * 0.08f, GetRandomValue(0, 2)});
    }
}

void Game::DrawRoadBackground()
{
    bool isDesert = (currentScore >= 5000);
    ClearBackground(isDesert ? Color{210, 180, 110, 255} : Color{28, 80, 28, 255});

    for (const auto &obj : sideObjects)
    {
        if (isDesert)
            DrawCactus(obj.x, obj.y, obj.size);
        else
            DrawTree(obj.x, obj.y, obj.size);
    }

    // Fahrbahn
    DrawRectangle(300, 0, 400, 800, {48, 48, 48, 255});
    // Fahrbahnmarkierungen
    DrawRectangle(297, 0, 6, 800, {210, 210, 210, 255});
    DrawRectangle(697, 0, 6, 800, {210, 210, 210, 255});
    DrawRectangle(280, 0, 18, 800, {150, 150, 155, 255});
    DrawRectangle(280, 0, 4, 800, {220, 225, 230, 255});
    DrawRectangle(702, 0, 18, 800, {150, 150, 155, 255});
    DrawRectangle(716, 0, 4, 800, {220, 225, 230, 255});

    // Scrollende Randpfosten
    const float postSpacing = 90.0f;
    float postStart = std::fmod(roadScrollOffset, postSpacing) - postSpacing;
    for (float py = postStart; py < 800.0f; py += postSpacing)
    {
        DrawRectangle(283, (int)py, 12, 14, {90, 90, 95, 255});
        DrawRectangle(705, (int)py, 12, 14, {90, 90, 95, 255});
    }

    // Scrollende Mittellinie
    const float dashLen = 40.0f, dashSpacing = 80.0f;
    Color dashColor = isDesert ? Color{255, 240, 100, 220} : Color{255, 255, 255, 200};
    float dashStart = std::fmod(roadScrollOffset, dashSpacing) - dashSpacing;
    for (float dy = dashStart; dy < 800.0f; dy += dashSpacing)
        DrawRectangle(498, (int)dy, 4, (int)dashLen, dashColor);
}
