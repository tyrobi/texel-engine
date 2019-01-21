#ifndef GAME_CAMERA_IMPL
#define GAME_CAMERA_IMPL
#include "game.h"

SDL_Rect GameCamera::viewport = SDL_Rect();
Vec2 GameCamera::position = Vec2();
Vec2 GameCamera::offset = Vec2();
float GameCamera::scaling = 2.0f;

void GameCamera::setSize()
{
    int wi, he;

    SDL_GetWindowSize(GameWindow::get(), &wi, &he);
    setSize(wi, he);
}
void GameCamera::setSize(int w, int h)
{
    viewport.w = w;
    viewport.h = h;

    int wi, he;

    SDL_GetWindowSize(GameWindow::get(), &wi, &he);
    offset = Vec2((wi - viewport.w) / 2, (he - viewport.h) / 2);
}

void GameCamera::setPosition(Vec2& pos)
{
    following = nullptr;
    position = pos;
}

Vec2 GameCamera::getPosition()
{
    return Vec2(viewport.x, viewport.y);
}

Vec2& GameCamera::getOffset()
{
    return offset;
}

void GameCamera::setFollowing(Entity& e)
{
    following = &e;
}

void GameCamera::update()
{
    if (following != nullptr)
    {
        TransformComponent* t = &following->getComponent<TransformComponent>();
        viewport.x = (t-> getPosition().x() * scaling) + (t-> getSize().x() * scaling / 2) - ((viewport.w) / 2);
        viewport.y = (t-> getPosition().y() * scaling) + (t-> getSize().y() * scaling / 2) - ((viewport.h) / 2);

        int lw, lh;
        lw = static_cast<int>(Level::getLevelWidth() * GameCamera::getScale());
        lh = static_cast<int>(Level::getLevelHeight() * GameCamera::getScale());

        if (viewport.w > lw)
            viewport.x = (lw / 2) - (viewport.w / 2);
        else
        {
            if (viewport.x < 0) viewport.x = 0;
            if (viewport.x + viewport.w > lw) viewport.x = lw - viewport.w;
        }

        if (viewport.h > lh)
            viewport.y = (lh / 2) - (viewport.h / 2);
        else
        {
            if (viewport.y < 0) viewport.y = 0;
            if (viewport.y + viewport.h > lh) viewport.y = lh - viewport.h;
        }
    }
}
#endif