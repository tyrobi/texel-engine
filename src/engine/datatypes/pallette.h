#ifndef COLOR_PALLETTE_H
#define COLOR_PALLETTE_H

#include <SDL.h>

class Pallette
{
protected:
    std::map<SDL_Color, SDL_Color> colors = {};
public:
    Pallette();
    const SDL_Color& getMappedColor(const SDL_Color& col)
    {
        if (colors.count(col) == 0)
            return SDL_Color(0, 0, 0, 255);
        return colors[col];
    }

    void addColor(SDL_Color target, SDL_Color appliedColor)
    {
        if (colors.count(target) == 1)
        {
            printf("%s\n", "this color has already been mapped");
        }
        colors[target] = appliedColor;
    }
    ~Pallette();

};
#endif