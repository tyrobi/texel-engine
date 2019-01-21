#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "components.h"
#include "../base/ECS.h"

#include "../../engine/visuals/textureManager.h"

class SpriteComponent : public Component
{
private:
    SDL_Texture* tex;
    SDL_Rect src, dest;

    TransformComponent* transform;

    int index, count, numFrames, srcX, srcY = 0;
    float speed;
    bool animated = false;
    bool flipX, flipY;

public:
    SpriteComponent() = default;
    SpriteComponent(const char* texture)
    {
        loadTexture(texture);
    }

    SpriteComponent(const char* texture, int x, int y)
    {
        srcX = x;
        srcY = y;
        loadTexture(texture);
    }

    SpriteComponent(const char* texture, int frames, int x, int y, float speed)
    {
        loadTexture(texture);
        numFrames = frames;
        srcX = x;
        srcY = y;
        this->speed = 1000.0f / speed;
        animated = true;
    }

    SpriteComponent(const char* texture, int frames, int x, int y, float speed, bool random)
    {
        loadTexture(texture);
        numFrames = frames;
        srcX = x;
        srcY = y;
        this->speed = 1000.0f / speed;
        animated = true;
        if (random)
            index = rand() % numFrames;
    }

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
            entity->addComponent<TransformComponent>();

        transform = &entity->getComponent<TransformComponent>();
        src.w = 16;
        src.h = 16;
        src.x = srcX * 16;
        src.y = srcY * 16;
        dest.w = transform->getSize().x();
        dest.h = transform->getSize().y();
        count = 0;
        flipX = false;
        flipY = false;
    }

    void setFlipped(bool x, bool y)
    {
        flipX = x;
        flipY = y;
    }
    void setFlipped(bool x)
    {
        flipX = x;
    }

    void loadTexture(const char* tex)
    {
        this-> tex = TextureManager::loadTexture(tex);
    }

    ~SpriteComponent()
    {

    }

    void setSourceRow(int row)
    {
        src.y = 16 * (row);
    }
    void setSourceCol(int col)
    {
        src.x = 16 * (col);
        count = 0;
        index = col;
    }
    void setAnimated(bool animated)
    {
        this-> animated = animated;
    }

    void update(int ticks) override
    {
        dest.x = transform->getPosition().x();
        dest.y = transform->getPosition().y();
        dest.w = transform->getSize().x();
        dest.h = transform->getSize().y();

        if (animated)
        {
            count += ticks;
            if (count >= speed)
            {
                count -= static_cast<int>(speed);
                ++index %= numFrames;
                src.x = 16 * index;
            }
        }
    }

    void draw() override
    {
        if (transform->isVisible())
        {
            TextureManager::draw(tex, src, dest, flipX, flipY);
        }
    }

};

#endif