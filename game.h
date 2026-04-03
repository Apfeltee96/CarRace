#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "player.h"
#include "scoreboard.h"
#include "ui.h"
#include "config.h"
#include <vector>
#include <string>

// ============================================================
//  Spielzustände
// ============================================================

enum GameState
{
    MAIN_MENU,
    SHOP_MENU,
    DESCRIPTION,
    SCOREBOARD_MENU,
    SETTINGS,
    PLAYING,
    PAUSED,
    GAMEOVER,
    EXIT_PROMPT
};

// ============================================================
//  Spielobjekte
// ============================================================

/// Ein Hindernis auf der Fahrbahn.
struct Obstacle
{
    Rectangle rect;
    Color color;
};

/// Ein einsammelbarer Stern auf der Fahrbahn.
struct CollectableStar
{
    Rectangle rect;
    bool active;
};

// ============================================================
//  Hauptspielklasse
// ============================================================

class Game
{
public:
    /// Fenster öffnen, Assets laden, Startzustand setzen.
    void Init();

    /// Eingabe verarbeiten und Spiellogik für diesen Frame ausführen.
    void Update();

    /// Alles auf den Bildschirm zeichnen.
    void Draw();

    /// Assets freigeben und Fenster schließen.
    void Cleanup();

private:
    // --- Interne Hilfsmethoden ---

    /// Setzt ein Hindernis auf eine neue zufällige Position (startY = Startwert für Y).
    void ResetObstacle(Obstacle &obs, float startY);

    /// Spawnt einen Bonusstern an zufälliger X-Position.
    void SpawnStar();

    /// Skalierte Mausposition für 1000×800-Logikraum berechnen.
    Vector2 GetScaledMouse() const;

    // --- Eingabe-Handler pro Zustand ---
    void HandleMenuInput(Vector2 mousePoint);
    void HandleShopInput(Vector2 mousePoint);
    void HandleSettingsInput(Vector2 mousePoint);

    // --- Spiellogik ---
    void UpdateGameLogic(float deltaTime);

    // --- Assets ---
    Texture2D carTextures[3]; // [0]=weiß, [1]=rot, [2]=lila
    Texture2D obstacleTex;
    Texture2D starTex;
    RenderTexture2D target; // Offscreen-Buffer für Skalierung

    // --- Spielzustand ---
    GameState state;
    GameState previousState; // Für ESC-Toggle (EXIT_PROMPT)
    SaveGame saveData;
    Player player;

    std::vector<Obstacle> obstacles;
    CollectableStar bonusStar;

    int currentScore;
    float currentSpeed;
    float totalTimeSurvived;
    int earnedStarsThisRound;
    int framesCounter;  // Für blinkenden Cursor im Hauptmenü
    int cachedTopScore; // Gecachter Highscore (kein File-I/O pro Frame)

    char playerName[16];
    int letterCount;
    bool isNameSaved;

    // --- UI-Button-Rechtecke ---
    Rectangle startBtn, scoreBtn, shopBtn, settingsBtn, descBtn, backMenuBtn;
    Rectangle langBtn, resBtn, nameChangeBtn, deleteDataBtn, backSetBtn;
    Rectangle btnPrimary, btnMenu;
    Rectangle redCarBtn, blueCarBtn; // Shop-Buttons
};

#endif // GAME_H
