#ifndef TILE_H
#define TILE_H

#include "../../misc/enums.h"
#include <string>
#include <map>

class BaseTile
{
protected:
    SDL_Texture* sourceTex;
    SDL_Rect src;
    const char* name;

public:

    BaseTile() : sourceTex(nullptr){}

    void render(SDL_Rect *dest)
    {
        if (sourceTex != nullptr)
            SDL_RenderCopy(GameRenderer::get(), sourceTex, &src, dest);
        else
            printf("Render requested, but no texture has been provided\n");
    }

    virtual ~BaseTile()
    {
        SDL_DestroyTexture(sourceTex);
    }

    virtual void load()
    {
        printf("Base Load called\n");
    }

    virtual const char * getName()
    {
        return name;
    }
};


class StaticTile : public BaseTile
{
private:
    bool textureLoaded = false;
public:

    StaticTile(StaticTile * tile)
    {
        src.x = tile -> src.x;
        src.y = tile -> src.y;
        src.w = tile -> src.w;
        src.h = tile -> src.h;

        this-> name = tile -> name;

        sourceTex = tile -> sourceTex;
    }

    StaticTile(const char* name, int x, int y, Enums::CollisionType collision, Enums::MaterialType material)
    {
        src.x = 16 * x;
        src.y = 16 * y;

        src.w = 16;
        src.h = 16;

        this-> name = name;

        sourceTex = nullptr;
    }

    StaticTile(const char* name, int x, int y, Enums::CollisionType collision)
    : StaticTile(name, x, y, collision, Enums::MaterialType::SOLID)
    {
    }

    StaticTile(const char* name, int x, int y)
    : StaticTile(name, x, y, Enums::CollisionType::FLOOR)
    {
    }

    virtual void load() override
    {
        if (!textureLoaded)
            sourceTex = TextureManager::loadTexture("res/tiles/tilesheet.png");
        textureLoaded = true;
    }

    ~StaticTile()
    {

    }

};

class Tile
{
private:
    BaseTile *tile;
    int x, y;
    SDL_Rect dest;
    static StaticTile NULL_TILE;
    static StaticTile HOLE;
    static StaticTile GRASS;
    static StaticTile DIRT;
    static StaticTile SAND;

public:

    Tile(BaseTile *tile, int x, int y)
    {
        this -> tile = tile;
        this -> x = x;
        this -> y = y;
        dest.x = x * 32;
        dest.y = y * 32;
        dest.w = 32;
        dest.h = 32;
    }

    static const std::map<Uint32, BaseTile*> TILEMAP;

    static BaseTile* convertFromColor(Uint32 src)
    {
        BaseTile *newTile = nullptr;
        if (TILEMAP.count(src) >= 1 )
            return TILEMAP.at(src);
        return &NULL_TILE;
    }

    static Tile* generateTile(Uint32 source, int x, int y)
    {
        BaseTile* tile = convertFromColor(source);
        return new Tile(tile, x, y);
    }

    void loadTile()
    {
        tile -> load();
    }

    void render()
    {
        tile-> render(&dest);
    }

    ~Tile()
    {
        delete tile;
    }
};

StaticTile Tile::NULL_TILE = StaticTile("Void", 0, 0, Enums::CollisionType::WALL);
StaticTile Tile::HOLE = StaticTile("Hole", 1, 0, Enums::CollisionType::WALL);
StaticTile Tile::GRASS = StaticTile("Grass", 2, 0, Enums::CollisionType::FLOOR);
StaticTile Tile::DIRT = StaticTile("Dirt", 3, 0, Enums::CollisionType::FLOOR);
StaticTile Tile::SAND = StaticTile("Sand", 4, 0, Enums::CollisionType::FLOOR);

const std::map<Uint32, BaseTile*> Tile::TILEMAP = {
    {0xFFFFFFFF, &Tile::NULL_TILE},
    {0x000000FF, &Tile::HOLE},
    {0x7F3300FF, &Tile::DIRT},
    {0x267F00FF, &Tile::GRASS},
    {0xFFD800FF, &Tile::SAND},

};

#endif
