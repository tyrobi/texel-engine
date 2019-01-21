#ifndef TEST_STAGE_2_H
#define TEST_STAGE_2_H

#include "levels.h"

class TestStage2 : public RenderStage
{
private:
    Manager manager;
    TileManager tileManager;
    Entity& player = manager.addEntity();
    Entity& enemy = manager.addEntity();
public:
    TestStage2(): RenderStage("Test Stage 2", "Another test stage")
    {

    }
    ~TestStage2()
    {

    }

    void draw() override
    {
        tileManager.render(Enums::GroupLabels::SUB_MAP);
        tileManager.render(Enums::GroupLabels::MAP);
        manager.draw(Enums::GroupLabels::PLAYER);
        manager.draw(Enums::GroupLabels::UI);

        auto& in = player.getComponent<InputManager>();
        if (in.isQuitting())
            TextureManager::renderText("Quitting...", Vec2(3, 3),
                in.getQuitPercent() * 255, Enums::AnchorPos::BOT_RIGHT);
        fadeIn();
    }

    void init() override
    {
        Level::loadLevel("res/maps/test_map.png", tileManager);
        player.addComponent<TransformComponent>(10,10);
        player.addComponent<PlayerComponent>("res/mobs/player.png");
        player.addComponent<ColliderComponent>();
        player.addComponent<InputManager>();
        player.addGroup(Enums::GroupLabels::PLAYER);

        enemy.addComponent<TransformComponent>(70,60);
        enemy.addComponent<HumanoidComponent>("res/mobs/player.png");
        enemy.addComponent<AiComponent>(new StaticAi(&enemy));
        enemy.addGroup(Enums::GroupLabels::PLAYER);

        GameCamera::setFollowing(player);
        RenderStage::init();
    }

    void update(int ticks) override
    {
        manager.update(ticks);
        tileManager.update(ticks);
        TransformComponent* t = &player.getComponent<TransformComponent>();
        if (Level::getTile(t) == Enums::TileType::WATER && !switchTarget)
        {
            RenderStage* r = new TestStage();
            setRenderTarget(r);
        }
    }

};
#endif