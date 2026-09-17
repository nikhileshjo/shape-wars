#include "Game.h"

int main()
{
    GameEngine gameEngine;
    gameEngine.init("config/config.txt");
    gameEngine.run();
}