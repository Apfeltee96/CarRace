#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "player.h"
#include "scoreboard.h"
#include "ui.h"
#include "config.h"
#include "shop.h"
#include <vector>
#include <string>

enum GameState { MAIN_MENU, SHOP_MENU, DESCRIPTION, SCOREBOARD_MENU, SETTINGS, PLAYING, PAUSED, GAMEOVER, EXIT_PROMPT };

struct Obstacle { Rectangle rect; Color color; };
struct CollectableStar { Rectangle rect; bool active; };

class Game {
public:
    void Init();
    void Update();
    void Draw();
    void Cleanup();

private:
    void ResetObstacle(Obstacle &obs, float startY);
    void SpawnStar();
    void HandleMenuInput(Vector2 mousePoint);
    void HandleShopInput(Vector2 mousePoint);
    void HandleSettingsInput(Vector2 mousePoint);
    void UpdateGameLogic(float deltaTime);

    Texture2D carTextures[3];
    Texture2D obstacleTex;
    Texture2D starTex;
    RenderTexture2D target;

    GameState state;
    GameState previousState;
    SaveGame saveData;
    Player player;
    std::vector<Obstacle> obstacles;
    CollectableStar bonusStar;

    int currentScore;
    float currentSpeed;
    float totalTimeSurvived;
    int earnedStarsThisRound;
    char playerName[16];
    int letterCount;
    bool isNameSaved;

    Rectangle startBtn, scoreBtn, shopBtn, settingsBtn, descBtn, backMenuBtn;
    Rectangle langBtn, resBtn, nameChangeBtn, deleteDataBtn, backSetBtn;
    Rectangle btnPrimary, btnMenu;
};

#endif