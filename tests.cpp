#include "config.h"
#include <cassert>
#include <cstdio>


//Einfache Unit-Tests für die Funktionen in config.h
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
    assert(fast >= slow && "Schnellere Welt → schnellere Lenkung erwartet");

    printf("[OK] test_GetDynamicPlayerSpeed\n");
}

int main()
{
    test_GetCurrentSpeed();
    test_GetDynamicPlayerSpeed();
    printf("Alle Tests bestanden.\n");
    return 0;
}
