#ifndef LEVELS_H
#define LEVELS_H

#include "../engine/base/renderstage.h"
#include "../engine/base/ECS.h"
#include "../engine/base/tilemanager.h"
#include "../engine/visuals/textureManager.h"


class TestStage : public RenderStage
{
private:
    Manager manager;
    TileManager tileManager;
    Entity& player = manager.addEntity();
    Entity& enemy = manager.addEntity();

public:
    TestStage();
    ~TestStage();
    void draw() override;
    void update(int ticks) override;
    void init() override;
};

#include "loadingscreen.h"
#include "teststage_2.h"
#include "teststage.cpp"

#endif