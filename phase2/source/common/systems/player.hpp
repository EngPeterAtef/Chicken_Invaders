#pragma once

#include "../components/camera.hpp"
#include "../components/collision.hpp"
#include "../ecs/world.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

#include "./collision-system.hpp"

namespace our
{
class PlayerSystem
{
    Application *app;
    our::CollisionSystem collisionSystem;

  public:
    int lives = 3;
    int score = 0;
    void enter(Application *app)
    {
        lives = 3;
        this->app = app;
    }

    // This should be called every frame to update player
    void update(World *world, float deltaTime)
    {
        score += 1;
        Entity *enemy_collision = collisionSystem.detectCollision(world);
        if (enemy_collision)
        {
            enemy_collision->localTransform.scale = glm::vec3(0, 0, 0);
            enemy_collision->deleteComponent<CollisionComponent>();
            lives--;
            std::cout << "Lives: " << lives << " Score : " << score << std::endl;
            if (lives == 0)
            {
                app->changeState("menu");
            }
        }
    }

    // When the state exits, it should call this function to ensure the mouse is unlocked
    void exit()
    {
    }
};

} // namespace our
