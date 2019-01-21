#ifndef HUMANOID_H
#define HUMANOID_H

#include "../base/ECS.h"
#include "../../misc/enums.h"
#include "components.h"

class HumanoidComponent : public Component
{
private:
    int maxHealth = 3;
    int health = 3;
    int I_FRAMES = 1000;
    int iFrameCounter = 0;

    Enums::PlayerMovementStatus movementStatus;
    Enums::PlayerActionStatus actionStatus;
    const char* filename;
    SpriteComponent* sprite;

public:
    HumanoidComponent() = default;
    HumanoidComponent(const char* filename)
    {
        this-> filename = filename;
    }

    void init() override
    {
        entity-> addComponent<SpriteComponent>(filename, 4, 0, 0, 10.0f);
        sprite = &entity->getComponent<SpriteComponent>();
        sprite -> setAnimated(false);
    }

    void setDirection(Enums::Direction facing)
    {
        switch(facing)
        {
            case Enums::Direction::NONE:
                sprite-> setSourceCol(0);
                sprite-> setAnimated(false);
                break;
            case Enums::Direction::NORTH:
                sprite-> setSourceRow(0);
                sprite-> setAnimated(true);
                break;
            case Enums::Direction::EAST:
                sprite-> setSourceRow(1);
                sprite-> setAnimated(true);
                sprite-> setFlipped(false);
                break;
            case Enums::Direction::SOUTH:
                sprite-> setSourceRow(2);
                sprite-> setAnimated(true);
                break;
            case Enums::Direction::WEST:
                sprite-> setSourceRow(1);
                sprite-> setAnimated(true);
                sprite-> setFlipped(true);
                break;
            default:
                sprite -> setAnimated(false);
        }
    }

    int getHealth()
    {
        return health;
    }

    int getMaxHealth()
    {
        return maxHealth;
    }

    void update(int ticks) override
    {
        if (iFrameCounter > 0)
        {
            iFrameCounter -= ticks;
        }
    }

    void damage(int hit)
    {
        if (iFrameCounter > 0) return;
        iFrameCounter = I_FRAMES;
        health -= hit;
        if (health <= 0)
        {
            printf("%s\n", "Humanoid has died!");
        }
    }

    ~HumanoidComponent(){}

};

#endif