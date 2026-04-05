#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "player.h"
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

struct CollectableStar   { Rectangle rect; bool active; };
struct CollectableClock  { Rectangle rect; bool active; }; // verlangsamt das Spiel kurz
struct CollectableShield { Rectangle rect; bool active; }; // schützt vor einem Treffer

// Straßenrandobjekt
struct RoadSideObj
{
    float x, y;
    float size;
    int variant;
};

class Game
{
public:
    void Init();
    void Update();
    void Draw();
    void Cleanup();

    void ResetObstacle(Obstacle &obs, float startY);
    void SpawnStar();
    void SpawnClock();
    void SpawnShield();
    Vector2 GetScaledMouse() const;
    void InitSideObjects();
    void DrawRoadBackground();

    void HandleMenuInput(Vector2 mousePoint);
    void HandleShopInput(Vector2 mousePoint);
    void HandleSettingsInput(Vector2 mousePoint);
    void UpdateGameLogic(float deltaTime);

    // --- Assets ---
    Texture2D carTextures[3];
    Texture2D obstacleTex;
    Texture2D starTex;
    Texture2D clockTex;
    Texture2D shieldTex;
    RenderTexture2D target;

    // --- Audio ---
    Music backgroundMusic;
    Sound crashSound;
    float musicVolume;
    bool musicLoaded;
    bool crashSoundLoaded;
    bool volumeDragging;

    // --- Straßenvisuals ---
    float roadScrollOffset;
    std::vector<RoadSideObj> sideObjects;

    // --- Spielzustand ---
    GameState state;
    GameState previousState;
    SaveGame saveData;
    Player player;
    std::vector<Obstacle> obstacles;
    CollectableStar   bonusStar;
    CollectableClock  clockBuff;
    CollectableShield shieldBuff;

    int currentScore;
    float currentSpeed;
    float totalTimeSurvived;
    int earnedStarsThisRound;
    int framesCounter;
    int cachedTopScore;

    bool buffActive;
    float buffTimer;
    float speedBeforeBuff;
    bool shieldActive;
    float shieldTimer;

    char playerName[16];
    int letterCount;
    bool isNameSaved;

    // --- UI-Buttons ---
    Rectangle startBtn, scoreBtn, shopBtn, settingsBtn, descBtn, backMenuBtn;
    Rectangle langBtn, resBtn, nameChangeBtn, deleteDataBtn, backSetBtn;
    Rectangle btnPrimary, btnMenu;
    Rectangle redCarBtn, blueCarBtn;
    Rectangle pauseResumeBtn, pauseMenuBtn, pauseQuitBtn;

    // --- Bestätigungsdialoge ---
    bool showQuitConfirm;
    bool showPauseBackConfirm;
    bool showNameChangeConfirm;
    bool showDeleteDataConfirm;
};

#endif // GAME_H
