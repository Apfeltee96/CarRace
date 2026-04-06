#include "game.h"
#include <algorithm>
#include <cmath>

static void DrawTexFull(Texture2D tex, Rectangle dst)
{
    DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height}, dst, {0, 0}, 0.0f, WHITE);
}

// Baum
static void DrawTree(float x, float y, float s)
{
    DrawRectangle((int)(x - 5 * s), (int)(y - 22 * s), (int)(10 * s), (int)(22 * s), {101, 67, 33, 255});
    DrawCircle((int)x, (int)(y - 30 * s), 22.0f * s, {20, 100, 20, 255});
    DrawCircle((int)(x - 10 * s), (int)(y - 42 * s), 16.0f * s, {30, 130, 30, 255});
    DrawCircle((int)(x + 10 * s), (int)(y - 42 * s), 16.0f * s, {30, 130, 30, 255});
    DrawCircle((int)x, (int)(y - 52 * s), 18.0f * s, {50, 160, 50, 255});
}
// Kaktus
static void DrawCactus(float x, float y, float s)
{
    Color cGreen = {34, 115, 34, 255};
    int bw = (int)(12 * s), bh = (int)(50 * s);
    DrawRectangle((int)(x - bw / 2), (int)(y - bh), bw, bh, cGreen);
    DrawRectangle((int)(x - bw / 2 - (int)(16 * s)), (int)(y - (int)(32 * s)), (int)(16 * s), (int)(8 * s), cGreen);
    DrawRectangle((int)(x - bw / 2 - (int)(9 * s)), (int)(y - (int)(44 * s)), (int)(9 * s), (int)(14 * s), cGreen);
    DrawRectangle((int)(x + bw / 2), (int)(y - (int)(26 * s)), (int)(16 * s), (int)(8 * s), cGreen);
    DrawRectangle((int)(x + bw / 2 + (int)(7 * s)), (int)(y - (int)(38 * s)), (int)(9 * s), (int)(14 * s), cGreen);
}
// UFO
static void DrawUFO(float x, float y, float s)
{
    DrawEllipse((int)x, (int)(y + 4 * s), 26.0f * s, 7.0f * s, {80, 80, 100, 255});
    DrawCircle((int)(x - 12 * s), (int)(y + 7 * s), 4.0f * s, {255, 140, 0, 200});
    DrawCircle((int)x, (int)(y + 9 * s), 5.0f * s, {255, 100, 0, 200});
    DrawCircle((int)(x + 12 * s), (int)(y + 7 * s), 4.0f * s, {255, 140, 0, 200});
    DrawEllipse((int)x, (int)y, 28.0f * s, 9.0f * s, {160, 160, 185, 255});
    DrawEllipseLines((int)x, (int)y, 28.0f * s, 9.0f * s, {220, 220, 255, 180});
    DrawEllipse((int)x, (int)(y - 7 * s), 13.0f * s, 10.0f * s, {80, 200, 255, 210});
    DrawEllipseLines((int)x, (int)(y - 7 * s), 13.0f * s, 10.0f * s, {180, 240, 255, 255});
    DrawCircle((int)(x - 18 * s), (int)(y + 2 * s), 3.0f * s, {0, 255, 120, 220});
    DrawCircle((int)(x + 18 * s), (int)(y + 2 * s), 3.0f * s, {255, 50, 50, 220});
}
// Planet
static void DrawPlanet(float x, float y, float s, int variant)
{
    Color colors[] = {{220, 80, 80, 255}, {80, 160, 255, 255}, {255, 180, 60, 255}, {180, 80, 255, 255}};
    Color c = colors[variant % 4];
    float r = 14.0f * s;
    DrawCircle((int)x, (int)y, r, c);
    DrawCircle((int)(x + r * 0.25f), (int)(y - r * 0.25f), r * 0.55f, Fade(WHITE, 0.15f));
    DrawCircleLines((int)x, (int)y, r, Fade(WHITE, 0.3f));
    if (variant % 2 == 1)
        DrawEllipseLines((int)x, (int)y, 22.0f * s, 5.0f * s, Fade(WHITE, 0.5f));
}

// --- Init ---

void Game::Init()
{
    saveData = LoadSaveGame();
    InitWindow(SCREEN_W, SCREEN_H, "Car Race");
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

    target = LoadRenderTexture(SCREEN_W, SCREEN_H);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    if (saveData.isFullscreen && !IsWindowFullscreen())
        ToggleFullscreen();

    InitAudioDevice();
    musicVolume = saveData.musicVolume;
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

    state = MAIN_MENU;
    showQuitConfirm = false;
    showPauseBackConfirm = false;
    showNameChangeConfirm = false;
    showDeleteDataConfirm = false;
    isNameSaved = false;
    framesCounter = 0;
    playerName.clear();

    if (!saveData.lastPlayerName.empty() && saveData.lastPlayerName != "Gast")
    {
        playerName = saveData.lastPlayerName.substr(0, 15);
        isNameSaved = true;
    }
    letterCount = static_cast<int>(playerName.size());
    cachedTopScore = GetTopScore();

    const float cx = SCREEN_W / 2.0f - 125.0f;
    buttons.startBtn = {cx, 300, 250, 50};
    buttons.scoreBtn = {cx, 360, 250, 50};
    buttons.shopBtn = {cx, 420, 250, 50};
    buttons.settingsBtn = {cx, 480, 250, 50};
    buttons.descBtn = {cx, 540, 250, 50};
    buttons.backMenuBtn = {cx, 680, 250, 50};
    buttons.langBtn = {cx, 180, 250, 50};
    buttons.resBtn = {cx, 250, 250, 50};
    buttons.nameChangeBtn = {cx, 320, 250, 50};
    buttons.deleteDataBtn = {cx, 390, 250, 50};
    buttons.effectsBtn = {cx, 450, 250, 40};
    buttons.backSetBtn = {cx, 610, 250, 50};
    buttons.btnPrimary = {cx, 400, 250, 50};
    buttons.btnMenu = {cx, 480, 250, 50};
    buttons.pauseResumeBtn = {cx, 280, 250, 50};
    buttons.pauseMenuBtn = {cx, 350, 250, 50};
    buttons.pauseQuitBtn = {cx, 420, 250, 50};
    buttons.redCarBtn = {400, 500, 200, 50};
    buttons.blueCarBtn = {650, 500, 200, 50};

    obstacles.resize(4);
    for (int i = 0; i < 4; i++)
        ResetObstacle(obstacles[i], -200.0f - (i * 400.0f));

    bonusStar = {{0, 0, 30, 30}, false, CollectableType::Star};
    clockBuff = {{0, 0, 36, 36}, false, CollectableType::Clock};
    shieldBuff = {{0, 0, 48, 48}, false, CollectableType::Shield};

    buffActive = false;
    buffTimer = 0.0f;
    speedBeforeBuff = 0.0f;
    shieldActive = false;
    shieldTimer = 0.0f;
}

// --- Update ---

void Game::Update()
{
    if (shouldClose)
        return;

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
        const Rectangle effBtn = {375, 558, 250, 35};
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
                    saveData.musicVolume = musicVolume;
                }
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (volumeDragging)
                {
                    volumeDragging = false;
                    SaveGameData(saveData);
                }
                else if (CheckCollisionPointRec(mousePoint, buttons.pauseResumeBtn))
                {
                    if (musicLoaded)
                        ResumeMusicStream(backgroundMusic);
                    state = PLAYING;
                }
                else if (CheckCollisionPointRec(mousePoint, buttons.pauseMenuBtn))
                    showPauseBackConfirm = true;
                else if (CheckCollisionPointRec(mousePoint, buttons.pauseQuitBtn))
                    showQuitConfirm = true;
                else if (CheckCollisionPointRec(mousePoint, effBtn))
                {
                    saveData.effectsEnabled = !saveData.effectsEnabled;
                    SaveGameData(saveData);
                }
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
            if (CheckCollisionPointRec(mousePoint, buttons.backMenuBtn))
                state = MAIN_MENU;
        break;

    case GAMEOVER:
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            if (CheckCollisionPointRec(mousePoint, buttons.btnMenu))
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

// --- Neue Runde starten ---

void Game::StartNewGame()
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
    player.Init(SCREEN_W, SCREEN_H,
                45.0f, 45.0f * ((float)cur.height / cur.width));

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
        SetMusicVolume(backgroundMusic, musicVolume);
        PlayMusicStream(backgroundMusic);
    }
    showQuitConfirm = showPauseBackConfirm = false;
    state = PLAYING;
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
    player.Move(deltaTime);

    // Hindernisse
    for (auto &obs : obstacles)
    {
        obs.rect.y += currentSpeed * deltaTime;
        if (obs.rect.y > (float)SCREEN_H)
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

    // Collectables
    if (!bonusStar.active && GetRandomValue(0, 600) < 3)
        bonusStar.Spawn(320, 650, 30.0f);
    bonusStar.UpdatePosition(currentSpeed, deltaTime);
    if (bonusStar.active && CheckCollisionRecs(player.rect, bonusStar.rect))
    {
        earnedStarsThisRound++;
        bonusStar.active = false;
    }

    if (!clockBuff.active && !buffActive && GetRandomValue(0, 3000) < 2)
        clockBuff.Spawn(320, 640, 36.0f);
    clockBuff.UpdatePosition(currentSpeed, deltaTime);
    if (clockBuff.active && CheckCollisionRecs(player.rect, clockBuff.rect))
    {
        speedBeforeBuff = currentSpeed;
        currentSpeed *= 0.4f;
        buffActive = true;
        buffTimer = 3.0f;
        clockBuff.active = false;
    }

    if (!shieldBuff.active && !shieldActive && GetRandomValue(0, 6000) < 2)
        shieldBuff.Spawn(320, 640, 48.0f);
    shieldBuff.UpdatePosition(currentSpeed, deltaTime);
    if (shieldBuff.active && CheckCollisionRecs(player.rect, shieldBuff.rect))
    {
        shieldActive = true;
        shieldTimer = 5.0f;
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
                playerName += static_cast<char>(key);
                letterCount++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0)
        {
            playerName.pop_back();
            --letterCount;
        }
        if (IsKeyPressed(KEY_ENTER) && letterCount > 0)
        {
            saveData.lastPlayerName = playerName;
            SaveGameData(saveData);
            isNameSaved = true;
        }
    }

    if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        return;

    if (CheckCollisionPointRec(mousePoint, buttons.startBtn) && (letterCount > 0 || isNameSaved))
        StartNewGame();
    else if (CheckCollisionPointRec(mousePoint, buttons.scoreBtn))
        state = SCOREBOARD_MENU;
    else if (CheckCollisionPointRec(mousePoint, buttons.shopBtn))
        state = SHOP_MENU;
    else if (CheckCollisionPointRec(mousePoint, buttons.settingsBtn))
        state = SETTINGS;
    else if (CheckCollisionPointRec(mousePoint, buttons.descBtn))
        state = DESCRIPTION;
}

void Game::HandleShopInput(Vector2 mousePoint)
{
    if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        return;

    if (CheckCollisionPointRec(mousePoint, buttons.backMenuBtn))
    {
        state = MAIN_MENU;
        return;
    }
    if (CheckCollisionPointRec(mousePoint, {150, 500, 200, 50}))
        saveData.selectedColorId = 0;

    if (CheckCollisionPointRec(mousePoint, buttons.redCarBtn))
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
    if (CheckCollisionPointRec(mousePoint, buttons.blueCarBtn))
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
    const Rectangle settingsSlider = {350, 525, 300, 16};
    if (!showNameChangeConfirm && !showDeleteDataConfirm)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if (volumeDragging || CheckCollisionPointRec(mousePoint, settingsSlider))
            {
                volumeDragging = true;
                musicVolume = std::clamp((mousePoint.x - settingsSlider.x) / settingsSlider.width, 0.0f, 1.0f);
                if (musicLoaded)
                    SetMusicVolume(backgroundMusic, musicVolume);
                if (crashSoundLoaded)
                    SetSoundVolume(crashSound, musicVolume);
                saveData.musicVolume = musicVolume;
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && volumeDragging)
        {
            volumeDragging = false;
            SaveGameData(saveData);
        }
    }

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
            playerName.clear();
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
            saveData = {0, false, false, 0, saveData.isEnglish, false, "", 0.2f, true};
            playerName.clear();
            letterCount = 0;
            isNameSaved = false;
            cachedTopScore = 0;
            musicVolume = 0.2f;
            if (musicLoaded)
                SetMusicVolume(backgroundMusic, musicVolume);
            if (crashSoundLoaded)
                SetSoundVolume(crashSound, musicVolume);
            state = MAIN_MENU;
        }
        else if (CheckCollisionPointRec(mousePoint, nBtn))
            showDeleteDataConfirm = false;
        return;
    }

    if (CheckCollisionPointRec(mousePoint, buttons.backSetBtn))
        state = MAIN_MENU;
    else if (CheckCollisionPointRec(mousePoint, buttons.langBtn))
    {
        saveData.isEnglish = !saveData.isEnglish;
        SaveGameData(saveData);
    }
    else if (CheckCollisionPointRec(mousePoint, buttons.resBtn))
    {
        saveData.isFullscreen = !saveData.isFullscreen;
        ToggleFullscreen();
        SaveGameData(saveData);
    }
    else if (CheckCollisionPointRec(mousePoint, buttons.effectsBtn))
    {
        saveData.effectsEnabled = !saveData.effectsEnabled;
        SaveGameData(saveData);
    }
    else if (CheckCollisionPointRec(mousePoint, buttons.nameChangeBtn))
        showNameChangeConfirm = true;
    else if (CheckCollisionPointRec(mousePoint, buttons.deleteDataBtn))
        showDeleteDataConfirm = true;
}

// --- Hilfsmethoden ---

Vector2 Game::GetScaledMouse() const
{
    float s = std::min((float)GetScreenWidth() / (float)SCREEN_W,
                       (float)GetScreenHeight() / (float)SCREEN_H);
    Vector2 m = GetMousePosition();
    return {
        (m.x - (GetScreenWidth() - (float)SCREEN_W * s) * 0.5f) / s,
        (m.y - (GetScreenHeight() - (float)SCREEN_H * s) * 0.5f) / s};
}

void Game::ResetObstacle(Obstacle &obs, float startY)
{
    obs.rect = {
        (float)GetRandomValue(310, 590),
        startY,
        (float)GetRandomValue(70, 110),
        40.0f};
}

// --- Draw ---

void Game::Draw()
{
    if (shouldClose)
        return;

    Vector2 mousePoint = GetScaledMouse();
    BeginTextureMode(target);
    DrawRoadBackground();

    bool gameWasActive = (previousState == PLAYING || previousState == PAUSED || previousState == GAMEOVER);
    bool inGame = (state == PLAYING || state == PAUSED || state == GAMEOVER ||
                   (state == EXIT_PROMPT && gameWasActive));

    if (inGame)
    {
        for (const auto &obs : obstacles)
            DrawTexFull(obstacleTex, obs.rect);

        player.Draw(carTextures[saveData.selectedColorId], shieldActive);

        if (bonusStar.active)
            DrawTexFull(starTex, bonusStar.rect);
        if (clockBuff.active)
            DrawTexFull(clockTex, clockBuff.rect);
        if (shieldBuff.active)
            DrawTexFull(shieldTex, shieldBuff.rect);

        DrawHUD(playerName, totalTimeSurvived, currentScore,
                earnedStarsThisRound, saveData.isEnglish, starTex, cachedTopScore);

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
            DrawPauseMenu(mousePoint,
                          buttons.pauseResumeBtn, buttons.pauseMenuBtn, buttons.pauseQuitBtn,
                          saveData.isEnglish, musicVolume, saveData.effectsEnabled,
                          showQuitConfirm, showPauseBackConfirm);
        if (state == GAMEOVER)
            DrawGameOverMenu(playerName, currentScore, totalTimeSurvived,
                             earnedStarsThisRound, mousePoint, buttons.btnMenu, saveData.isEnglish);
    }

    switch (state)
    {
    case MAIN_MENU:
        DrawMainMenu(playerName, letterCount, framesCounter, mousePoint,
                     buttons.startBtn, buttons.scoreBtn, buttons.shopBtn,
                     buttons.settingsBtn, buttons.descBtn,
                     saveData.totalStars, isNameSaved, saveData.isEnglish);
        break;
    case SHOP_MENU:
        DrawShopMenu(saveData, mousePoint, buttons.redCarBtn, buttons.blueCarBtn,
                     buttons.backMenuBtn, saveData.isEnglish, carTextures);
        break;
    case SETTINGS:
        DrawSettingsMenu(mousePoint,
                         buttons.langBtn, buttons.resBtn, buttons.nameChangeBtn,
                         buttons.deleteDataBtn, buttons.backSetBtn, buttons.effectsBtn,
                         saveData.isEnglish, saveData.isFullscreen,
                         musicVolume, saveData.effectsEnabled,
                         showNameChangeConfirm, showDeleteDataConfirm);
        break;
    case SCOREBOARD_MENU:
        DrawScoreboardMenu(LoadScoreboard(), mousePoint, buttons.backMenuBtn, saveData.isEnglish);
        break;
    case DESCRIPTION:
        DrawDescriptionMenu(mousePoint, buttons.backMenuBtn, saveData.isEnglish);
        break;
    default:
        break;
    }

    if (state == EXIT_PROMPT)
    {
        DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.55f));
        Rectangle box = {300, 290, 400, 200};
        DrawRectangleRec(box, {25, 25, 25, 245});
        DrawRectangleLinesEx(box, 3, RED);
        DrawTextCentered(saveData.isEnglish ? "EXIT GAME?" : "SPIEL BEENDEN?", 325, 26, RAYWHITE);
        Rectangle yBtn = {330, 420, 150, 50};
        Rectangle nBtn = {520, 420, 150, 50};
        DrawRectangleRec(yBtn, CheckCollisionPointRec(mousePoint, yBtn) ? RED : MAROON);
        DrawRectangleRec(nBtn, CheckCollisionPointRec(mousePoint, nBtn) ? GRAY : DARKGRAY);
        DrawRectangleLinesEx(yBtn, 2, {255, 80, 80, 200});
        DrawRectangleLinesEx(nBtn, 2, {160, 160, 160, 200});
        int yw = MeasureText(saveData.isEnglish ? "YES" : "JA", 22);
        int nw = MeasureText(saveData.isEnglish ? "NO" : "NEIN", 22);
        DrawText(saveData.isEnglish ? "YES" : "JA",
                 (int)(yBtn.x + (yBtn.width - yw) / 2), (int)(yBtn.y + 14), 22, WHITE);
        DrawText(saveData.isEnglish ? "NO" : "NEIN",
                 (int)(nBtn.x + (nBtn.width - nw) / 2), (int)(nBtn.y + 14), 22, WHITE);
    }

    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    float finalScale = std::min((float)GetScreenWidth() / (float)SCREEN_W,
                                (float)GetScreenHeight() / (float)SCREEN_H);
    DrawTexturePro(target.texture,
                   {0, 0, (float)target.texture.width, -(float)target.texture.height},
                   {(GetScreenWidth() - (float)SCREEN_W * finalScale) * 0.5f,
                    (GetScreenHeight() - (float)SCREEN_H * finalScale) * 0.5f,
                    (float)SCREEN_W * finalScale, (float)SCREEN_H * finalScale},
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
    shouldClose = true;
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
    bool isSpace = (currentScore >= 10000);

    if (isSpace)
    {
        ClearBackground({5, 5, 20, 255});

        for (int i = 0; i < 90; i++)
        {
            int sx = (i * 139 + 23) % SCREEN_W;
            float sy = std::fmod(i * 91.7f + roadScrollOffset * 0.15f, (float)SCREEN_H);
            unsigned char br = (unsigned char)(120 + (i * 53) % 135);
            int sz = (i % 4 == 0) ? 2 : 1;
            DrawRectangle(sx, (int)sy, sz, sz, {br, br, br, br});
        }

        for (const auto &obj : sideObjects)
        {
            if (obj.variant == 2)
                DrawUFO(obj.x, obj.y, obj.size);
            else
                DrawPlanet(obj.x, obj.y, obj.size, obj.variant);
        }

        if (saveData.effectsEnabled)
        {
            Color rainbow[] = {
                {255, 0, 0, 160}, {255, 127, 0, 160}, {255, 255, 0, 160}, {0, 255, 0, 160}, {0, 100, 255, 160}, {148, 0, 211, 160}};
            float stripeH = 50.0f;
            float rbStart = std::fmod(roadScrollOffset, stripeH * 6) - stripeH * 6;
            for (float ry = rbStart; ry < (float)SCREEN_H; ry += stripeH)
            {
                int ci = ((int)(ry / stripeH) % 6 + 6) % 6;
                DrawRectangle(300, (int)ry, 400, (int)stripeH, rainbow[ci]);
            }
            DrawRectangle(300, 0, 400, SCREEN_H, {0, 0, 10, 100});

            float t = (float)framesCounter * 0.04f;
            Color glowL = {(unsigned char)(127 + 127 * sinf(t)),
                           (unsigned char)(127 + 127 * sinf(t + 2.1f)),
                           (unsigned char)(127 + 127 * sinf(t + 4.2f)), 255};
            Color glowR = {(unsigned char)(127 + 127 * sinf(t + 1.0f)),
                           (unsigned char)(127 + 127 * sinf(t + 3.1f)),
                           (unsigned char)(127 + 127 * sinf(t + 5.2f)), 255};
            DrawRectangle(295, 0, 8, SCREEN_H, glowL);
            DrawRectangle(697, 0, 8, SCREEN_H, glowR);
        }
        else
        {
            DrawRectangle(300, 0, 400, SCREEN_H, {30, 30, 50, 255});
            DrawRectangle(295, 0, 8, SCREEN_H, {100, 100, 200, 255});
            DrawRectangle(697, 0, 8, SCREEN_H, {100, 100, 200, 255});
        }
    }
    else
    {
        ClearBackground(isDesert ? Color{210, 180, 110, 255} : Color{28, 80, 28, 255});

        for (const auto &obj : sideObjects)
        {
            if (isDesert)
                DrawCactus(obj.x, obj.y, obj.size);
            else
                DrawTree(obj.x, obj.y, obj.size);
        }

        DrawRectangle(300, 0, 400, SCREEN_H, {48, 48, 48, 255});
        DrawRectangle(297, 0, 6, SCREEN_H, {210, 210, 210, 255});
        DrawRectangle(697, 0, 6, SCREEN_H, {210, 210, 210, 255});
        DrawRectangle(280, 0, 18, SCREEN_H, {150, 150, 155, 255});
        DrawRectangle(280, 0, 4, SCREEN_H, {220, 225, 230, 255});
        DrawRectangle(702, 0, 18, SCREEN_H, {150, 150, 155, 255});
        DrawRectangle(716, 0, 4, SCREEN_H, {220, 225, 230, 255});

        const float postSpacing = 90.0f;
        float postStart = std::fmod(roadScrollOffset, postSpacing) - postSpacing;
        for (float py = postStart; py < (float)SCREEN_H; py += postSpacing)
        {
            DrawRectangle(283, (int)py, 12, 14, {90, 90, 95, 255});
            DrawRectangle(705, (int)py, 12, 14, {90, 90, 95, 255});
        }
    }
}
