#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "components.h"
#include "../base/ECS.h"
#include "../../misc/enums.h"
#include "../datatypes/level.h"

class PlayerComponent : public Component
{
private:
    HumanoidComponent* humanoid;
    const char* filename;

public:
    PlayerComponent(){}
    PlayerComponent(const char* filename)
    {
        this-> filename = filename;
    }
    void init() override
    {
        if (entity-> hasComponent<HumanoidComponent>())
            humanoid = &entity-> getComponent<HumanoidComponent>();
        else
            humanoid = &entity-> addComponent<HumanoidComponent>(filename);
    }

    void setDirection(Enums::Direction facing)
    {
        humanoid-> setDirection(facing);
    }

    void update(int ticks) override
    {
        Vec2* t = &entity->getComponent<TransformComponent>().getPosition();
        int lw = Level::getLevelWidth();
        int lh = Level::getLevelHeight();
        if (t->x() < 0) t->x(0);
        if (t->y() < 0) t->y(0);
        if (t->x() + 16 > lw) t->x(lw - 16);
        if (t->y() + 16 > lh) t->y(lh - 16);
    }
};

#endif