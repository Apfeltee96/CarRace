#include "game.h"

// Hauptprogramm
int main()
{
    Game game;
    game.Init();

    while (!WindowShouldClose() && !game.ShouldClose())
    {
        game.Update();
        game.Draw();
    }

    game.Cleanup();
    return 0;
}
