#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <queue>

// Static classes to access game components from other locations
class GameWindow
{
private:
    static SDL_Window* window;
public:
    GameWindow();
    GameWindow(const char* name, int w, int h);
    static SDL_Window* get();
    static void destroy();
};

class GameRenderer
{
private:
    static SDL_Renderer* renderer;
public:
    GameRenderer();
    static SDL_Renderer* get();
    static void destroy();

};

class EventQueue
{
private:
    static std::queue<SDL_Event> events;
    static bool shouldQuit;
public:
    static bool query(SDL_Event& e)
    {
        if (events.empty())
            return false;
        e = events.front();
        events.pop();
        return true;
    }

    static bool checkEvents()
    {
        if (shouldQuit)
        {
            printf("previous quit command issued, forcing quit...");
            std::exit(0);
            return true;
        }
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            SDL_Event e(event);
            switch(event.type)
            {
                case SDL_QUIT:
                    shouldQuit = true;
                    printf("Quit detected!!\n");
                    return true;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    events.push(e);
                    break;
            }
        }
        return false;
    }
};
bool EventQueue::shouldQuit = false;

class Entity;
class Vec2;
class GameCamera
{
private:
    static Vec2 position;
    static Vec2 offset;
    static SDL_Rect viewport;
    static float scaling;
    static Entity* following;
    static constexpr float MAX_SCALE = 4.0f;
    static constexpr float MIN_SCALE = 2.0f;

public:
    static void setScale(float scale)
    {
        scaling = scale;
    }
    static float getScale()
    {
        return scaling;
    }

    static void addScale(float val)
    {
        scaling += (val / (32 * scaling));
        if (scaling > MAX_SCALE) scaling = MAX_SCALE;
        if (scaling < MIN_SCALE) scaling = MIN_SCALE;
    }
    static void setSize(int w, int h);
    static void setSize();
    static void setPosition(Vec2& pos);
    static Vec2& getOffset();
    static Vec2 getPosition();
    static void setFollowing(Entity& e);
    static void update();
    static SDL_Rect* getViewport()
    {
        return &viewport;
    }
};
Entity* GameCamera::following = nullptr;
std::queue<SDL_Event> EventQueue::events;

#include "gamewindow.cpp"
#include "gamerenderer.cpp"

#include "engine/datatypes/vec.h"
#include "misc\enums.h"
#include "engine\profile.h"
#include "player\character.h"
#include "engine/visuals/textureManager.h"
#include "engine/visuals/map.h"
#include "engine/base/ECS.h"
#include "engine/components/components.h"
#include "engine/physics/collision.h"
#include "engine/base/tilemanager.h"
#include "engine/datatypes/level.h"

#include "gamecamera.cpp"

#include "engine/base/renderstage.h"
#include "levels/levels.h"

class Game
{
private:

    Enums::GameStatus status;
    Profile *profile;
    RenderStage* r = new TestStage();
    RenderStage* t = new LoadingScreen(r);
    RenderManager manager = RenderManager(t);

    int cnt = 0;

    void enterGameLoop()
    {
        int ticks = 0;
        int start = SDL_GetTicks();
        int end  = start;
        while (status != Enums::GameStatus::STOP_REQUESTED
            && status != Enums::GameStatus::STOPPED)
        {
            checkEvents();
            update(ticks);
            render();
            end = SDL_GetTicks();
            ticks = end - start;
            start = end;
        }
        status = Enums::GameStatus::STOPPED;
    };

    void checkEvents()
    {
        if (EventQueue::checkEvents())
            status = Enums::GameStatus::STOP_REQUESTED;
    }

    void render()
    {
        SDL_RenderClear(GameRenderer::get());
        manager.draw();
        SDL_RenderPresent(GameRenderer::get());
    }

    void update(int ticks)
    {
        GameCamera::update();
        cnt += ticks;

        manager.update(ticks);
    }

public:
    static SDL_Event event;

    Game()
    {
        status = Enums::GameStatus::UNINITIALIZED;
    };

    void start()
    {
        status = Enums::GameStatus::INITIALIZING;
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cout << "SDL did not initialize: error - "
                << std::endl << SDL_GetError() << std::endl;
            return;
        }
        GameWindow("Advent", 320, 240);
        GameRenderer();
        GameCamera::setSize();


        status = Enums::GameStatus::READY;

        enterGameLoop();
    };

    void close()
    {
        SDL_Quit();
    };

    ~Game()
    {
        GameWindow::destroy();
        GameRenderer::destroy();
        TextureManager::destroy();
        close();
        delete profile;
    };
};
#endif