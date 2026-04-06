#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "player.h"
#include "collectable.h"
#include "scoreboard.h"
#include "ui.h"
#include "config.h"
#include <vector>
#include <string>

enum GameState
{
    MAIN_MENU,
    SHOP_MENU,
    SETTINGS,
    SCOREBOARD_MENU,
    DESCRIPTION,
    PLAYING,
    PAUSED,
    GAMEOVER,
    EXIT_PROMPT
};

struct Obstacle
{
    Rectangle rect;
    Color color;
};

struct RoadSideObj
{
    float x, y;
    float size;
    int variant;
};

// Positionen aller UI-Buttons
struct ButtonLayout
{
    Rectangle startBtn, scoreBtn, shopBtn, settingsBtn, descBtn, backMenuBtn;
    Rectangle langBtn, resBtn, nameChangeBtn, deleteDataBtn, backSetBtn, effectsBtn;
    Rectangle btnPrimary, btnMenu;
    Rectangle redCarBtn, blueCarBtn;
    Rectangle pauseResumeBtn, pauseMenuBtn, pauseQuitBtn;
};

class Game
{
public:
    void Init();
    void Update();
    void Draw();
    void Cleanup();

    /// Gibt true zurück, sobald Cleanup() aufgerufen wurde.
    bool ShouldClose() const { return shouldClose; }

private:
    void ResetObstacle(Obstacle &obs, float startY);
    Vector2 GetScaledMouse() const;
    void InitSideObjects();
    void DrawRoadBackground();

    void HandleMenuInput(Vector2 mousePoint);
    void HandleShopInput(Vector2 mousePoint);
    void HandleSettingsInput(Vector2 mousePoint);
    void UpdateGameLogic(float deltaTime);
    void StartNewGame();

    // --- Assets ---
    Texture2D carTextures[3]{};
    Texture2D obstacleTex{};
    Texture2D starTex{};
    Texture2D clockTex{};
    Texture2D shieldTex{};
    RenderTexture2D target{};

    // --- Audio ---
    Music backgroundMusic{};
    Sound crashSound{};
    float musicVolume = 0.2f;
    bool musicLoaded = false;
    bool crashSoundLoaded = false;
    bool volumeDragging = false;

    // --- Straßenvisuals ---
    float roadScrollOffset = 0.0f;
    std::vector<RoadSideObj> sideObjects;

    // --- Spielzustand ---
    GameState state = MAIN_MENU;
    GameState previousState = MAIN_MENU;
    SaveGame saveData{};
    Player player{};
    std::vector<Obstacle> obstacles;

    Collectable bonusStar{{}, false, CollectableType::Star};
    Collectable clockBuff{{}, false, CollectableType::Clock};
    Collectable shieldBuff{{}, false, CollectableType::Shield};

    int currentScore = 0;
    float currentSpeed = SPEED_START;
    float totalTimeSurvived = 0.0f;
    int earnedStarsThisRound = 0;
    int framesCounter = 0;
    int cachedTopScore = 0;

    bool buffActive = false;
    float buffTimer = 0.0f;
    float speedBeforeBuff = 0.0f;
    bool shieldActive = false;
    float shieldTimer = 0.0f;

    std::string playerName;
    int letterCount = 0;
    bool isNameSaved = false;

    // --- UI-Buttons ---
    ButtonLayout buttons{};

    // --- Bestätigungsdialoge ---
    bool showQuitConfirm = false;
    bool showPauseBackConfirm = false;
    bool showNameChangeConfirm = false;
    bool showDeleteDataConfirm = false;

    bool shouldClose = false;
};

#endif
