#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "components.h"
#include "../base/ECS.h"
#include "../../game.h"
#include "../../misc/enums.h"

class InputManager : public Component
{
private:
    bool zoomInPressed = false;
    bool zoomOutPressed = false;
    bool quitPressed = false;
    int quitTimer = 0;
    static constexpr int QUIT_TIME = 1700; // ticks

public:
    TransformComponent* transform;
    PlayerComponent* player = nullptr;

    InputManager(){}

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        if (entity-> hasComponent<PlayerComponent>())
        {
            player = &entity-> getComponent<PlayerComponent>();
        }
    }

    void update(int ticks) override
    {
        SDL_Event event;

        Vec2& vel = transform->getVelocity();
        float speed = 0.0f;
        float xVel, yVel = 0.0f;
        bool keydown = true;
        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_W]) yVel -= 1;
        if (state[SDL_SCANCODE_S]) yVel += 1;
        if (state[SDL_SCANCODE_A]) xVel -= 1;
        if (state[SDL_SCANCODE_D]) xVel += 1;

        vel.x(xVel);
        vel.y(yVel);
        while (EventQueue::query(event))
        {
            // process potential mouse move events,
            // happening faster than the poll rate
            if (event.key.repeat > 0)
                continue;
            if (event.type == SDL_KEYUP)
                keydown = false;
            switch(event.key.keysym.sym)
            {
                case SDLK_n:
                    zoomOutPressed = keydown;
                    break;
                case SDLK_m:
                    zoomInPressed = keydown;
                    break;
                case SDLK_q:
                    quitPressed = keydown;
                    break;
            }
        }
        if (quitPressed)
        {
            quitTimer += ticks;
            if (quitTimer >= QUIT_TIME)
            {
                SDL_Event e;
                e.type = SDL_QUIT;
                SDL_PushEvent(&e);
            }
        }
        else
            quitTimer = 0.0f;
        if (zoomOutPressed) GameCamera::addScale(1);
        if (zoomInPressed) GameCamera::addScale(-1);
        if (player != nullptr)
        {
            Enums::Direction dir = Enums::Direction::NONE;
            float min = 0.5f;
            if (vel.magnitude() > 0)
            {
                if (vel.x() >= min) dir = Enums::Direction::EAST;
                else if (vel.x() <= -min) dir = Enums::Direction::WEST;
                if (vel.y() >= min) dir = Enums::Direction::NORTH;
                else if (vel.y() <= -min) dir = Enums::Direction::SOUTH;
            }
            player-> setDirection(dir);
        }
        // vel.normalize();
    }

    bool isQuitting()
    {
        return quitPressed;
    }

    float getQuitPercent()
    {
        float f = static_cast<float>(quitTimer) / static_cast<float>(QUIT_TIME);
        return (f * 1.4f > 1.0f)? 1.0f : f * 1.4f;
    }

    ~InputManager()
    {

    }

};
#endif