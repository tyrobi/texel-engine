#include <iostream>
#include <stdlib.h>
#include "src\game.h"

int main(int argc, char *argv[])
{
    Game* currentGame = new Game();
    currentGame -> start();
    delete currentGame;
    exit(0);
    return 0;
}
