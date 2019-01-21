#ifndef LEVEL_H
#define LEVEL_H

#include <map>
#include "../../misc/enums.h"
#include "../base/tilemanager.h"
#include "../components/tilecomponent.h"

class Level
{
private:
    static bool levelLoaded;
    static SDL_Surface *loadedMap;
    static Uint32* loadedLevelPtr;
    static Uint32 levelWidth;
    static Uint32 levelHeight;
    static void print_bytes(const void *object, size_t size)
    {
        const unsigned char * const bytes =
            static_cast<const unsigned char *>(object);
        size_t i;

        printf("[ ");
        for (i = 0; i < size; i++) printf("%02x ", bytes[i]);
        printf("]\n");
    }

    static std::map<Uint32, Enums::TileType> TILEMAP;

    static Enums::TileType getType(Uint32 pixel)
    {
        // print_bytes(&pixel, sizeof(pixel));
        if (TILEMAP.count(pixel) == 1)
            return TILEMAP[pixel];
        return Enums::TileType::NOT_FOUND;
    }

    static const char* getName(Enums::TileType tile)
    {
        using namespace Enums;
        switch(tile)
        {
            case TileType::NONE:      return "Null";
            case TileType::DIRT:      return "Dirt";
            case TileType::GRASS:     return "Grass";
            case TileType::WATER:     return "Water";
            case TileType::NOT_FOUND: return "<Void>";
            case TileType::SAND:      return "Sand";
            case TileType::MARSH:     return "Marsh";
        }
    }

public:

    static void clearLevel()
    {
        SDL_FreeSurface(loadedMap);
        loadedLevelPtr = nullptr;
        levelLoaded = false;
        levelWidth = 0;
        levelHeight = 0;
    }

    static void loadLevel(const char* LevelName, TileManager& manager)
    {
        if (levelLoaded)
        {
            clearLevel();
        }
        SDL_Surface *surf = IMG_Load(LevelName);
        surf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA8888, 0);
        SDL_PixelFormat *fmt = surf->format;

        levelWidth = static_cast<int>(surf->w);
        levelHeight = static_cast<int>(surf->h);
        SDL_LockSurface(surf);
        Uint32* pixels = ((Uint32*)surf->pixels);
        SDL_UnlockSurface(surf);

        for (int y = 0; y < levelHeight; y++)
        {
            for (int x = 0; x < levelWidth; x++)
            {
                Uint32 pixel = (pixels[(levelWidth * y) + x]);
                manager.addTile(x, y, getType(pixel));
            }
        }

        // Smooth the world
        // REWORK:

        if (false)
        {
            for (int y = 0; y < levelHeight; y++)
            {
                for (int x = 0; x < levelWidth; x++)
                {
                    Uint32 pixel = (pixels[(levelWidth * y) + x]);
                    if (getType(pixel) != Enums::TileType::DIRT) continue;
                    if (x + 1 >= levelWidth || x - 1 < 0) continue;
                    if (y + 1 >= levelHeight || y - 1 < 0) continue;
                    Uint32 rpixel = (pixels[(levelWidth * y) + (x + 1)]);
                    Uint32 lpixel = (pixels[(levelWidth * y) + (x - 1)]);
                    Uint32 upixel = (pixels[(levelWidth * (y - 1)) + x]);
                    Uint32 dpixel = (pixels[(levelWidth * (y + 1)) + x]);

                    Uint32 rupixel = (pixels[(levelWidth * (y - 1)) + (x + 1)]);
                    Uint32 rdpixel = (pixels[(levelWidth * (y + 1)) + (x + 1)]);
                    Uint32 ldpixel = (pixels[(levelWidth * (y + 1)) + (x - 1)]);
                    Uint32 lupixel = (pixels[(levelWidth * (y - 1)) + (x - 1)]);

                    if (getType(pixel) != getType(rpixel) && getType(pixel) == getType(rupixel))
                        manager.addRoundedTile(x+1, y, getType(pixel), Enums::Direction::SOUTH_WEST);
                    if (getType(pixel) != getType(lpixel) && getType(pixel) == getType(ldpixel))
                        manager.addRoundedTile(x-1, y, getType(pixel), Enums::Direction::NORTH_EAST);
                    if (getType(pixel) != getType(rpixel) && getType(pixel) == getType(rdpixel))
                        manager.addRoundedTile(x+1, y, getType(pixel), Enums::Direction::NORTH_WEST);
                    if (getType(pixel) != getType(lpixel) && getType(pixel) == getType(lupixel))
                        manager.addRoundedTile(x-1, y, getType(pixel), Enums::Direction::SOUTH_EAST);
                }
            }
        }

        loadedLevelPtr = pixels;
        loadedMap = surf;
        levelLoaded = true;
        // SDL_FreeSurface(surf);
    }

    static Enums::TileType getTile(float x, float y)
    {
        int tileX = static_cast<int>(x) / 16;
        int tileY = static_cast<int>(y) / 16;
        if (tileX >= levelWidth || tileY >= levelHeight)
        {
            return Enums::TileType::NOT_FOUND;
        }
        Uint32 c = loadedLevelPtr[(levelWidth * tileY) + tileX];
        // printf("%d, %d --> %s (%d)\n", tileX, tileY, getName(getType(c)), c);
        return getType(c);
    }

    static Enums::TileType getTile(TransformComponent* t)
    {
        return getTile(
            t-> getPosition().x() + t-> getSize().x() / 2,
            t-> getPosition().y() + t-> getSize().y() / 2);
    }

    static int getLevelWidth()
    {
        return (int)levelWidth * 16;
    }

    static int getLevelHeight()
    {
        return (int)levelHeight * 16;
    }

};

bool Level::levelLoaded = false;
SDL_Surface * Level::loadedMap = nullptr;
Uint32* Level::loadedLevelPtr = nullptr;
Uint32 Level::levelWidth = 0;
Uint32 Level::levelHeight = 0;

std::map<Uint32, Enums::TileType> Level::TILEMAP = {
    {0xFFFFFFFF, Enums::TileType::NOT_FOUND},
    {0x000000FF, Enums::TileType::NONE},
    {0x7F3300FF, Enums::TileType::DIRT},
    {0x267F00FF, Enums::TileType::GRASS},
    {0xFFD800FF, Enums::TileType::SAND},
    {0x0026FFFF, Enums::TileType::WATER},
    {0x0094FFFF, Enums::TileType::MARSH},
};


#endif
