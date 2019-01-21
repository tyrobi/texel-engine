#ifndef COLLISION_H
#define COLLISION_H

class Collision
{
public:
    static bool AABB(const SDL_Rect& rec1, const SDL_Rect& rec2)
    {
        return SDL_HasIntersection(&rec1, &rec2);
    }

};

#endif