#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../../game.h"
#include "../visuals/textureManager.h"

class GameObject
{
private:
    int xPos, yPos;
    SDL_Texture* objTex;
    SDL_Rect src, dest;

public:

    GameObject(const char* tex, int x, int y)
    {
        xPos = x;
        yPos = y;
        src = SDL_Rect();
        dest = SDL_Rect();
        src.w = 16;
        src.h = 16;
        dest.w = 32;
        dest.h = 32;

        this-> objTex = TextureManager::loadTexture(tex);

    }

    ~GameObject()
    {
        SDL_DestroyTexture(objTex);
    }

    void update(int ticks)
    {
        dest.x = xPos;
        dest.y = yPos;
    }

    void render()
    {
        SDL_RenderCopy(GameRenderer::get(), objTex, &src, &dest);
    }

};

#endif
