#ifndef COLLIDER_H
#define COLLIDER_H

#include <SDL.h>
#include <string>
#include "../base/ECS.h"

class ColliderComponent : public Component
{
private:
    SDL_Rect collider;
    std::string id;
    TransformComponent* transform;

public:
    ColliderComponent(){}
    ColliderComponent(const char* tag)
    {
        id = tag;
    }

    ~ColliderComponent(){}

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();

    }

    SDL_Rect& getCollider()
    {
        return collider;
    }

    void update(int ticks) override
    {
        collider.x = transform->getPosition().x();
        collider.y = transform->getPosition().y();
        collider.w = transform->getSize().x() * GameCamera::getScale();
        collider.h = transform->getSize().y() * GameCamera::getScale();
    }

};
#endif