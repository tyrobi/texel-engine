#ifndef AI_H
#define AI_H

#include "components.h"
#include "../base/ECS.h"

class AiType
{
protected:
    Entity *host = nullptr;

public:
    virtual void update(int ticks)
    {}
};

class StaticAi : public AiType
{
public:
    StaticAi(Entity* obj)
    {
        host = obj;
    }

    void update(int ticks) override
    {

    }

    ~StaticAi()
    {

    }
};

class AiComponent : public Component
{
private:
    AiType* type = nullptr;
    bool isAiActive = false;

public:
    AiComponent() = default;
    AiComponent(AiType* type)
    {
        setAiType(type);
    }

    void setAiActive(bool active)
    {
        isAiActive = active;
    }

    void setAiType(AiType* type)
    {
        if (this-> type != nullptr)
            delete this-> type;
        this-> type = type;
    }

    void update(int ticks) override
    {
        if (type != nullptr && isAiActive)
            type-> update(ticks);
    }

    ~AiComponent(){}
};

#endif