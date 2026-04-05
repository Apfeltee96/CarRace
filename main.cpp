#include "game.h"

// Hauptprogramm
int main()
{
    Game game;
    game.Init();

    while (!WindowShouldClose())
    {
        game.Update();
        game.Draw();
    }

    game.Cleanup();
    return 0;
}
