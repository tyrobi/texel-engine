#ifndef DOOR_COMPONENT_H
#define DOOR_COMPONENT_H

#include "components.h"

class RenderStage;

class DoorComponent : public TileComponent
{
private:
    RenderStage* destination;

public:
    DoorComponent(){}
    ~DoorComponent(){ destination = nullptr; }

    DoorComponent(int x, int y, RenderStage* destination)
    : TileComponent(x, y, Enums::TileType::DOOR)
    {
        this-> destination = destination;
    }

    RenderStage* getDestination()
    {
        return destination;
    }

};
#endif