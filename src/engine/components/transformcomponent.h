#ifndef TRANSFORM_CCOMPONENT_H
#define TRANSFORM_CCOMPONENT_H

#include "components.h"
#include "../base/ECS.h"
#include "../datatypes/vec.h"
#include "../../game.h"

class TransformComponent : public Component
{
private:
    Vec2 position;
    Vec2 velocity;

    Vec2 size = Vec2(16, 16);

public:

    TransformComponent()
    {
    }

    TransformComponent(float x, float y)
    {
        position = Vec2(x, y);
    }

    TransformComponent(float x, float y, int w, int h)
    {
        position = Vec2(x, y);
        size = Vec2(w, h);
    }

    Vec2& getPosition()
    {
        return position;
    }

    Vec2& getVelocity()
    {
        return velocity;
    }

    Vec2& getSize()
    {
        return size;
    }

    void init() override
    {
        velocity.x(0);
        velocity.y(0);
    }

    void update (int ticks) override
    {
        position += ((velocity) * (ticks / 18.0f));
    }

    bool isVisible()
    {
        SDL_Rect r;
        r.x = position.x() * GameCamera::getScale();
        r.y = position.y() * GameCamera::getScale();
        r.w = size.x() * GameCamera::getScale();
        r.h = size.y() * GameCamera::getScale();
        return SDL_HasIntersection(&r, GameCamera::getViewport());
    }

};


#endif