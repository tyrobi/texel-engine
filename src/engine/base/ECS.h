#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <typeinfo>

class Component;
class Entity;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
    static ComponentID lastID = 0u;
    return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 16;
constexpr std::size_t maxGroups = 16;

using ComponentBitset = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
    Entity* entity;

    virtual void init()
    {}

    virtual void update(int ticks)
    {}

    virtual void draw()
    {}

    virtual ~Component()
    {}
};
class Manager;

class Entity
{
private:
    Manager& manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitset componentBitset;
    GroupBitset groupBitset;

public:

    Entity(Manager& memberManager)
    :  manager(memberManager)
    {}

    bool hasGroup(Group mGroup)
    {
        return groupBitset[mGroup];
    }

    void addGroup(Group g);

    void removeGroup(Group g)
    {
        groupBitset[g] = false;
    }

    void update(int ticks)
    {
        for (auto& c : components) c->update(ticks);
    }

    void draw()
    {
        for (auto& c : components) c->draw();
    }

    bool isActive() const
    {
        return active;
    }

    void destroy()
    {
        active = false;
    }

    template <typename T>
    bool hasComponent() const
    {
        return componentBitset[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        if (this-> hasComponent<T>())
            return this-> getComponent<T>();

        T* c(new T(std::forward<TArgs>(mArgs)...));
        c-> entity = this;
        std::unique_ptr<Component> unPtr{ c };

        components.emplace_back(std::move(unPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitset[getComponentTypeID<T>()] = true;
        c -> init();
        return *c;
    }

    template <typename T> T& getComponent() const
    {

        auto& ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }

};

class Manager
{
protected:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;
public:

    Manager(){}

    ~Manager()
    {
        reset();
    }

    void update(int ticks)
    {
        for (auto& e : entities)
            e-> update(ticks);
    }

    void draw()
    {
        for (auto& e : entities)
            e-> draw();
    }

    void draw(Enums::GroupLabels group)
    {
        auto groupSprites = getGroup(group);
        for (auto& e : groupSprites)
            e-> draw();
    }

    void refresh()
    {
        for (auto i = 0u; i < maxGroups; i++)
        {
            auto& v(groupedEntities[i]);
            v.erase(std::remove_if(std::begin(v), std::end(v),
                [i](Entity* mEntity)
                {
                    return !mEntity-> isActive() || mEntity-> hasGroup(i);
                }),
                std::end(v));
        }

        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
            [](const std::unique_ptr<Entity> &mEntity)
        {
            return !mEntity-> isActive();
        }),
        std::end(entities));
    }

    void addToGroup(Entity* e, Group g)
    {
        groupedEntities[g].emplace_back(e);
    }

    std::vector<Entity*>& getGroup(Group g)
    {
        return groupedEntities[g];
    }

    Entity& addEntity()
    {
        Entity *e = new Entity(*this);
        std::unique_ptr<Entity> uPtr {e};
        entities.emplace_back(std::move(uPtr));
        return *e;
    }

    virtual void reset()
    {
        entities.clear();
        for (int i = 0; i < groupedEntities.size(); i++)
            groupedEntities[i].clear();
    }
};

void Entity::addGroup(Group g)
{
    groupBitset[g] = true;
    manager.addToGroup(this, g);
}

#endif