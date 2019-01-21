#ifndef RENDERSTAGE_H
#define RENDERSTAGE_H

#include <thread>

class RenderStage
{
protected:
    bool initialized = false;
    RenderStage* nextTarget = nullptr;
    bool switchTarget = false;
    static constexpr int FADE_FRAMES = 30;
    int fadeInTimer = FADE_FRAMES;

    void setRenderTarget(RenderStage* stage)
    {
        nextTarget = stage;
        switchTarget = true;
    }

    void fadeIn()
    {
        if (fadeInTimer > 0)
        {
            fadeInTimer--;
            SDL_Rect r;
            r.w = GameCamera::getViewport()->w;
            r.h = GameCamera::getViewport()->h;
            SDL_SetRenderDrawColor(GameRenderer::get(), 0, 0, 0,
                ((float)fadeInTimer / (float)FADE_FRAMES) * 255);
            SDL_RenderFillRect(GameRenderer::get(), &r);
        }
    }

public:
    const char* title;
    const char* description;

    RenderStage()
    {
        title = "<No name provided>";
        description = "<No description provided>";
    }
    RenderStage(const char* title, const char* desc)
    {
        this-> title = title;
        description = desc;
    }

    virtual void init()
    {
        initialized = true;
    }

    virtual void uninitialize()
    {
        initialized = false;
    }

    virtual void update(int ticks){}
    virtual void draw(){}

    bool isSwitchRequested()
    {
        return switchTarget;
    }

    RenderStage* getNext()
    {
        return nextTarget;
    }

    bool isInitialized()
    {
        return initialized;
    }

    virtual ~RenderStage()
    {
        uninitialize();
        delete title;
        delete description;
    }

};

class RenderManager
{
private:
    RenderStage* stage;
    RenderStage* nextStage;
    bool loadingNextStage = false;

    void loadNext()
    {
        if (!loadingNextStage)
        {
            loadingNextStage = true;
            nextStage = stage-> getNext();
            Vec2 z;
            GameCamera::setPosition(z);
            nextStage-> init();
        }
        else if (nextStage-> isInitialized())
        {
            RenderStage* temp = stage;
            stage = nextStage;
            nextStage = nullptr;
            delete temp;
            loadingNextStage = false;
        }
    }

public:
    RenderManager(RenderStage* firstStage){
        stage = firstStage;
    };

    ~RenderManager()
    {
        delete stage;
    };

    void update(int ticks)
    {
        stage-> update(ticks);
        if (stage-> isSwitchRequested())
            loadNext();
    }

    void draw()
    {
        stage-> draw();
    }
};

#include "../../levels/levels.h"

#endif