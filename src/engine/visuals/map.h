#ifndef MAP_H
#define MAP_H

#include <vector>
#include "tile.h"

class Map
{
private:
    std::vector<Tile*> map;

    void print_bytes(const void *object, size_t size)
    {
        // This is for C++; in C just drop the static_cast<>() and assign.
        const unsigned char * const bytes = static_cast<const unsigned char *>(object);
        size_t i;

        printf("[ ");

        for(i = 0; i < size; i++)
            printf("%02x ", bytes[i]);

        printf("]\n");
    }

public:
    Map()
    {

    }

    ~Map()
    {

    }

    void loadMap(const char* mapName)
    {
        SDL_Surface *surf = IMG_Load(mapName);
        surf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA8888, 0);
        SDL_PixelFormat *fmt = surf->format;

        SDL_LockSurface(surf);
        Uint32 *pixels = ((Uint32*)surf->pixels);
        SDL_UnlockSurface(surf);

        // std::cout << "Texture takes " << unsigned(fmt->BytesPerPixel) << " bytes to represent" << std::endl;

        for (int y = 0; y < surf->h; y++)
        {
            for (int x = 0; x < surf->w; x++)
            {
                Uint32 pixel = (pixels[(surf->w * y) + x]);
                // print_bytes(&pixel, sizeof pixel);
                Tile *t = Tile::generateTile(pixel, x, y);
                map.push_back(t);
                t->loadTile();
            }
        }

        SDL_FreeSurface(surf);
    }

    void drawMap()
    {
        for (Tile *t : map)
        {
            t->render();
        }
    }
};


#endif
