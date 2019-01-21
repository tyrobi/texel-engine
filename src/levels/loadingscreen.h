#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include "levels.h"

class LoadingScreen : public RenderStage
{
private:
    int count = 0;
    static constexpr int SPEED = 1;
    static constexpr int FADE_TIME = 2000 / SPEED;
    static constexpr int TITLE_FADE_TIME = 500 / SPEED;
    static constexpr int DESC_FADE_TIME = 1000 / SPEED;

    static float div(int x, int y)
    {
        return static_cast<float>(x) / static_cast<float>(y);
    }

public:
    LoadingScreen(RenderStage* nextStage)
    :RenderStage(nextStage-> title, nextStage-> description)
    {
        nextTarget = nextStage;
    }
    ~LoadingScreen()
    {

    }
    void update(int ticks) override
    {
        count+= ticks;
        if (!switchTarget && count > FADE_TIME)
            setRenderTarget(nextTarget);
    }

    void draw() override
    {
        TextureManager::renderText(title, Vec2(0, 10),
            (count > TITLE_FADE_TIME? 1 : div(count, TITLE_FADE_TIME)) * 255,
            Enums::AnchorPos::CENTER);
        if (count > TITLE_FADE_TIME)
            TextureManager::renderSmallText(description, Vec2(0, -20),
                (count - TITLE_FADE_TIME > DESC_FADE_TIME ?
                1 : div(count - TITLE_FADE_TIME, DESC_FADE_TIME)) * 255,
                Enums::AnchorPos::CENTER);
    }

};
#endif