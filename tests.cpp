#include "config.h"
#include "collectable.h"
#include <cassert>
#include <cstdio>

static void test_GetCurrentSpeed()
{
    float increased = GetCurrentSpeed(SPEED_START, 1.0f);
    assert(increased > SPEED_START && "Speed sollte nach 1s gestiegen sein");

    float capped = GetCurrentSpeed(SPEED_MAX, 10.0f);
    assert(capped == SPEED_MAX && "Speed darf SPEED_MAX nicht überschreiten");

    float nearMax = GetCurrentSpeed(SPEED_MAX - 1.0f, 100.0f);
    assert(nearMax == SPEED_MAX && "Speed soll bei SPEED_MAX gekappt werden");

    printf("[OK] test_GetCurrentSpeed\n");
}

static void test_GetDynamicPlayerSpeed()
{
    constexpr float STEER_MAX = 1400.0f;

    float atStart = GetDynamicPlayerSpeed(SPEED_START);
    assert(atStart > 0.0f && "Lenkgeschwindigkeit muss positiv sein");
    assert(atStart <= STEER_MAX && "Lenkgeschwindigkeit darf STEER_MAX nicht überschreiten");

    float atMax = GetDynamicPlayerSpeed(100000.0f);
    assert(atMax == STEER_MAX && "Lenkgeschwindigkeit soll bei STEER_MAX gekappt werden");

    float slow = GetDynamicPlayerSpeed(0.0f);
    float fast = GetDynamicPlayerSpeed(500.0f);
    assert(fast >= slow && "Schnellere Welt -> schnellere Lenkung erwartet");

    printf("[OK] test_GetDynamicPlayerSpeed\n");
}

static void test_Collectable_Spawn()
{
    Collectable c;
    c.type = CollectableType::Star;
    c.Spawn(320, 650, 30.0f);

    assert(c.active && "Nach Spawn muss active == true sein");
    assert(c.rect.width == 30.0f && "Breite muss der Spawn-Groesse entsprechen");
    assert(c.rect.height == 30.0f && "Hoehe muss der Spawn-Groesse entsprechen");
    assert(c.rect.x >= 320.0f && "X muss >= minX sein");
    assert(c.rect.x <= 650.0f && "X muss <= maxX sein");
    assert(c.rect.y < 0.0f && "Item soll ueber dem Bildschirm starten");

    printf("[OK] test_Collectable_Spawn\n");
}

static void test_Collectable_UpdatePosition()
{
    Collectable c;
    c.type = CollectableType::Clock;
    c.Spawn(320, 640, 36.0f);

    float startY = c.rect.y;
    c.UpdatePosition(400.0f, 1.0f);
    assert(c.rect.y > startY && "Item soll sich nach unten bewegen");

    // Simuliere, bis es den Bildschirm verlässt
    for (int i = 0; i < 200; i++)
        c.UpdatePosition(400.0f, 0.05f);
    assert(!c.active && "Item soll deaktiviert werden wenn es den Bildschirm verlaesst");

    printf("[OK] test_Collectable_UpdatePosition\n");
}

int main()
{
    test_GetCurrentSpeed();
    test_GetDynamicPlayerSpeed();
    test_Collectable_Spawn();
    test_Collectable_UpdatePosition();
    printf("Alle Tests bestanden.\n");
    return 0;
}
