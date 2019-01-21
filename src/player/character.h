#ifndef CHARACTER_H
#define CHARACTER_H
#include "..\mobs\humanoid.h"
#include "..\player\inventory.h"
#include "..\items\accessory.h"

class Character : public Humanoid
{
private:
    Inventory inventory;
    Accessory equippedAccessories;

public:
    Character()
    {

    };

    ~Character()
    {

    };
};
#endif