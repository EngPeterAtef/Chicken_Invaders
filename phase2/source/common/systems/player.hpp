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
            Entity *fireEnemy = collisionSystem.detectFiring(world);
            if (fireEnemy)
            {
                fireEnemy->localTransform.scale = glm::vec3(0, 0, 0);
                fireEnemy->deleteComponent<CollisionComponent>();
                world->markForRemoval(fireEnemy);
                world->deleteMarkedEntities();
                score += 10;
            }
            score += 1;
            Entity *enemy_collision = collisionSystem.detectCollision(world);
            if (enemy_collision)
            {
                enemy_collision->localTransform.scale = glm::vec3(0, 0, 0);
                enemy_collision->deleteComponent<CollisionComponent>();
                world->markForRemoval(enemy_collision);
                world->deleteMarkedEntities();
                lives--;
                std::cout << "Lives: " << lives << " Score : " << score << std::endl;

                for (auto entity1 : world->getEntities())
                {
                    // Look for the lives
                    if (lives == 2 && entity1->name == "lives1")
                    {
                        entity1->localTransform.scale = glm::vec3(0, 0, 0);
                        break;
                    }
                    else if (lives == 1 && entity1->name == "lives2")
                    {
                        entity1->localTransform.scale = glm::vec3(0, 0, 0);
                        break;
                    }
                }
                if (lives == 0)
                {
                    app->changeState("game-over");
                }
            }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
        }
    };

} // namespace our
