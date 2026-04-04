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
    SETTINGS,        // Hinzugefügt
    SCOREBOARD_MENU, // Hinzugefügt
    DESCRIPTION,
    PLAYING,    // Hinzugefügt
    PAUSED,     // Hinzugefügt
    GAMEOVER,   // Hinzugefügt
    EXIT_PROMPT // Hinzugefügt
}; // Klammer und Semikolon fehlten

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

/// Eine einsammelbare Uhr auf der Fahrbahn (verlangsamt das Spiel kurz).
struct CollectableClock
{
    Rectangle rect;
    bool active;
};

/// Ein einsammelbares Schild auf der Fahrbahn (schützt vor Hindernissen kurz).
struct CollectableShield
{
    Rectangle rect;
    bool active;
};

/// Ein Straßenrandobjekt (Baum im Wald oder Kaktus in der Wüste), scrollt mit der Straße.
struct RoadSideObj
{
    float x, y;
    float size;  // Skalierungsfaktor
    int variant; // Variante für optische Abwechslung (0-2)
};

// ============================================================
//  Hauptspielklasse
// ============================================================
class Game
{
public:
    /// Fenster öffnen, Assets laden, Startzustand setzen.
    void Init();

    /// Haupt-Update-Loop (Eingabe & Logik-Verteilung).
    void Update();

    /// Haupt-Draw-Loop.
    void Draw();

    /// Ressourcen freigeben und Fenster schließen.
    void Cleanup();

    // --- Interne Hilfsmethoden ---
    /// Setzt ein Hindernis auf eine neue zufällige Position.
    void ResetObstacle(Obstacle &obs, float startY);
    /// Spawnt einen Bonusstern an zufälliger X-Position.
    void SpawnStar();
    /// Spawnt eine Uhr an zufälliger X-Position.
    void SpawnClock();
    /// Spawnt ein Schild an zufälliger X-Position.
    void SpawnShield();
    /// Skalierte Mausposition für 1000x800-Logikraum berechnen.
    Vector2 GetScaledMouse() const;
    /// Straßenrandobjekte (Bäume / Kakteen) initialisieren.
    void InitSideObjects();
    /// Hintergrund mit Straße, Randobjekten, Leitplanken und Mittellinie zeichnen.
    void DrawRoadBackground();

    // --- Eingabe-Handler pro Zustand ---
    void HandleMenuInput(Vector2 mousePoint);
    void HandleShopInput(Vector2 mousePoint);
    void HandleSettingsInput(Vector2 mousePoint); // Hinzugefügt
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
    CollectableStar bonusStar;
    CollectableClock clockBuff;
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

    // --- UI-Button-Rechtecke ---
    Rectangle startBtn, scoreBtn, shopBtn, settingsBtn, descBtn, backMenuBtn;
    Rectangle langBtn, resBtn, nameChangeBtn, deleteDataBtn, backSetBtn;
    Rectangle btnPrimary, btnMenu;
    Rectangle redCarBtn, blueCarBtn;
};

#endif // GAME_H