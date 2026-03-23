#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"

// --- GESCHWINDIGKEITEN ---
const float SPEED_START  = 400.0f;
const float SPEED_STEP_1 = 550.0f;
const float SPEED_STEP_2 = 700.0f;
const float SPEED_SONIC  = 900.0f;
const float SPEED_LIGHT  = 1800.0f;

// --- MEILENSTEINE (PUNKTE) ---
const int MILESTONE_1 = 1000;
const int MILESTONE_2 = 2500;
const int MILESTONE_3 = 5000;
const int MILESTONE_LIGHT = 10000;

// --- SHOP PREISE ---
const int PRICE_RED_CAR = 15;

// --- HORIZONTALE GESCHWINDIGKEIT (Links/Rechts) ---
const float PLAYER_SPEED_NORMAL = 700.0f;
const float PLAYER_SPEED_FAST   = 950.0f;
const float PLAYER_SPEED_LIGHT  = 1300.0f;

// Neue Funktion, um die passende Spieler-Geschwindigkeit zu holen
float GetPlayerSpeed(int score);

// Logik-Funktionen
float GetCurrentSpeed(int score);
int CalculateStars(int score);

#endif