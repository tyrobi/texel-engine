#ifndef TILE_COMPONENT_H
#define TILE_COMPONENT_H

#include "components.h"
#include "../base/ECS.h"
#include "../../misc/enums.h"

class TileComponent : public Component
{
protected:

    int x, y = 0;
    Enums::TileType type;
    Enums::Direction direction;
    bool rounded = false;
    SDL_Texture *texture = nullptr;
    int frames = 0;
    int count = 0;
    bool animated = false;
    float speed = 1.0f;
    int animIndex = 0;
    SDL_Rect src, dest;
    TransformComponent *transform = nullptr;

    void loadSprite(const char* tex, int frames, int x, int y, float speed, bool randomAnimFrame)
    {
        if (rounded)
            texture = TextureManager::loadClippedTexture(tex, Vec2(x, y), direction);
        else
            texture = TextureManager::loadTexture(tex);

        src.x = x * 16;
        src.y = y * 16;
        src.w = src.h = dest.w = dest.h = 16;

        dest.x = this-> x * 16;
        dest.y = this-> y * 16;
        animated = (frames > 0);
        if (animated)
        {
            this-> frames = frames;
            speed = (speed > 0)? speed : 1.0f;
            this-> speed = 1000.0f / speed;
            if (randomAnimFrame)
                animIndex = rand() % frames;
        }
    }
    void loadSprite(const char* tex, int frames, int x, int y, float speed)
    {
        loadSprite(tex, 0, x, y, 0, false);
    }
    void loadSprite(const char* tex, int x, int y)
    {
        loadSprite(tex, 0, x, y, 0);
    }

    int getRand(int a, int b)
    {
        return (rand() % (b - a)) + a;
    }

public:
    TileComponent(){};
    virtual ~TileComponent(){}

    TileComponent(int x, int y, Enums::TileType type)
    {
        this-> x = x;
        this-> y = y;
        this-> type = type;
        rounded = false;
    }

    TileComponent(int x, int y, Enums::TileType type, Enums::Direction direction)
    {
        this-> x = x;
        this-> y = y;
        this-> type = type;
        rounded = true;
        this-> direction = direction;
    }

    virtual void init() override
    {
        using namespace Enums;

        const char* sheetStatic = "res/tiles/tilesheet.png";
        const char* sheetAnimated = "res/tiles/animated.png";
        switch(type)
        {
            case TileType::NONE:
                loadSprite(sheetStatic, 1, 0);
                break;
            case TileType::GRASS:
                loadSprite(sheetStatic, 2, getRand(0, 2));
                break;
            case TileType::DIRT:
                loadSprite(sheetStatic, 3, getRand(0, 2));
                break;
            case TileType::SAND:
                loadSprite(sheetStatic, 4, 0);
                break;
            case TileType::WATER:
                loadSprite(sheetAnimated, 8, 0, 0, 10.0f, true);
                break;
            case TileType::MARSH:
                loadSprite(sheetAnimated, 8, 0, 1, 10.0f, true);
                break;
            default:
                loadSprite(sheetStatic, 0, 0);
                break;
        }

        transform = &entity-> addComponent<TransformComponent>();
        transform-> getPosition().x((x * 16));
        transform-> getPosition().y((y * 16));
    }

    virtual void update (int ticks) override
    {
        if (animated)
        {
            count += ticks;
            if (count >= speed)
            {
                count -= static_cast<int>(speed);
                ++animIndex %= frames;
                src.x = 16 * animIndex;
            }
        }
    }

    virtual void draw() override
    {
        if (transform->isVisible())
            TextureManager::draw(texture, src, dest);
    }
};

#endif