#ifndef GAME_WINDOW_C
#define GAME_WINDOW_C

#include "game.h"
#include <SDL.h>

SDL_Window* GameWindow::window = nullptr;

GameWindow::GameWindow()
{
    GameWindow::window = SDL_CreateWindow(
        "Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        320, 240, 0);
}

GameWindow::GameWindow(const char* name, int width, int height)
{
    GameWindow::window = SDL_CreateWindow(
        name,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height, 0);
}

SDL_Window* GameWindow::get()
{
    return GameWindow::window;
}

void GameWindow::destroy()
{
    SDL_DestroyWindow(GameWindow::window);
}


#endif