#ifndef GAME_RENDERER_C
#define GAME_RENDERER_C

#include "game.h"

SDL_Renderer* GameRenderer::renderer = nullptr;

GameRenderer::GameRenderer()
{
    if (GameWindow::get() == nullptr)
        GameWindow();
    GameRenderer::renderer = SDL_CreateRenderer(GameWindow::get(), -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");  // make the scaled rendering look smoother.
    // SDL_RenderSetLogicalSize(sdlRenderer, 640, 480);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

}
SDL_Renderer* GameRenderer::get()
{
    return GameRenderer::renderer;
}

void GameRenderer::destroy()
{
    SDL_DestroyRenderer(GameRenderer::renderer);
}

#endif