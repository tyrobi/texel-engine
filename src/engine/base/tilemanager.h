#ifndef TILE_MANAGER_H
#define TILE_MANAGER_H

#include "../components/components.h"
#include "../../misc/enums.h"
#include "ECS.h"

class TileManager : private Manager
{

public:
    void cleanse()
    {
        this-> refresh();
    }

    void addTile(int x, int y, Enums::TileType type)
    {
        auto& t = this-> addEntity();
        t.addComponent<TileComponent>(x, y, type);
        t.addGroup(Enums::GroupLabels::SUB_MAP);
    }

    void addRoundedTile(int x, int y, Enums::TileType type, Enums::Direction direction)
    {
        auto& t = this-> addEntity();
        t.addComponent<TileComponent>(x, y, type, direction);
        t.addGroup(Enums::GroupLabels::MAP);
    }

    void render()
    {
        this-> draw();
    }

    void render(Enums::GroupLabels group)
    {
        this-> draw(group);
    }

    void update(int ticks)
    {
        Manager::update(ticks);
    }

    void reset() override
    {
        Manager::reset();
    }
};
#endif