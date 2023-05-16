#pragma once
#include "../components/collision.hpp"
#include "../components/movement.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/world.hpp"
#include <GLFW/glfw3.h>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
// #include <iostream>
#include <math.h>
#include <random>
// using namespace std;
namespace our
{

// The movement system is responsible for moving every entity which contains a MovementComponent.
// This system is added as a simple example for how use the ECS framework to implement logic.
// For more information, see "common/components/movement.hpp"
class ChickenRenderer
{

    double generateRandomNumber(double minX, double maxX)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(minX, maxX);
        return dis(gen);
    }

  public:
    // This should be called every frame to update all entities containing a MovementComponent.
    double scaling = 0.4;
    double startTime = 0;
    double speedIncrease = 0;
    void intialization()
    {
        startTime = 0;
        speedIncrease = 0;
    }
    void rendering(World *world, int bossesToCreate)
    {

        if (bossesToCreate == 0 && !boss_exists(world))
        {
            speedIncrease += 0.1;
            Entity *newEntity = world->add();
            newEntity->name = "enemy";

            scaling = 0.4;
            newEntity->localTransform.scale = glm::vec3(scaling + speedIncrease / 80.0, scaling + speedIncrease / 80.0,
                                                        scaling + speedIncrease / 80.0);

            newEntity->localTransform.position =
                glm::vec3(generateRandomNumber(-30, 30), generateRandomNumber(-5, 5), -20);
            newEntity->localTransform.scale = glm::vec3(0.4, 0.4, 0.4);

            MeshRendererComponent *meshRendererComp = newEntity->addComponent<MeshRendererComponent>();
            meshRendererComp->deserialize({{"type", "Mesh Renderer"}, {"mesh", "chicken"}, {"material", "chicken"}});

            CollisionComponent *collisionComp = newEntity->addComponent<CollisionComponent>();
            collisionComp->deserialize({{"type", "Collision"}, {"mesh", "chicken"}, {"health", 1}, {"bonus", 10}});

            MovementComponent *movementRendererComp = newEntity->addComponent<MovementComponent>();
            movementRendererComp->linearVelocity = glm::vec3(0, 0, (int)fmin(40, 4 + speedIncrease));
            movementRendererComp->angularVelocity = glm::vec3(0, 3, 0);
        }
        else if (bossesToCreate != 0)
        {
            delete_chickens(world);
            for (int i = 1; i <= bossesToCreate; i++)
            {
                scaling = 1;
                Entity *newEntity = world->add();
                newEntity->name = "boss";
                int shift = 0;
                if (i % 2 == 0)
                {
                    shift = 10;
                }
                else if (i % 2 != 0 && i != 1)
                {
                    shift = -10;
                }

                newEntity->localTransform.scale = glm::vec3(scaling, scaling, scaling);
                newEntity->localTransform.position = glm::vec3(shift, 0, 40);

                MeshRendererComponent *meshRendererComp = newEntity->addComponent<MeshRendererComponent>();
                meshRendererComp->deserialize(
                    {{"type", "Mesh Renderer"}, {"mesh", "chicken"}, {"material", "chicken"}});

                CollisionComponent *collisionComp = newEntity->addComponent<CollisionComponent>();
                collisionComp->deserialize(
                    {{"type", "Collision"}, {"mesh", "chicken"}, {"health", 100}, {"bonus", 1000}});

                MovementComponent *movementRendererComp = newEntity->addComponent<MovementComponent>();
                movementRendererComp->linearVelocity = glm::vec3(0, 0, 0);
                movementRendererComp->angularVelocity = glm::vec3(0, 0, 0);
                newEntity->name = "boss";
            }
        }
    }

    bool boss_exists(World *world)
    {
        bool exists = false;
        for (auto entity : world->getEntities())
        {

            if (entity->name == "boss")
            {

                exists = true;
                break;
            }
        }
        return exists;
    }
    void delete_chickens(World *world)
    {

        for (auto entity : world->getEntities())
        {

            if (entity->name == "enemy")
            {
                if (entity->localTransform.position.z > 100)
                {
                    // // delete chicken
                    world->markForRemoval(entity);
                }
            }
        }
        world->deleteMarkedEntities();
    }

    void delete_all_chickens(World *world)
    {

        for (auto entity : world->getEntities())
        {

            if (entity->name == "enemy")
            {
                // delete chicken
                world->markForRemoval(entity);
            }
        }
        world->deleteMarkedEntities();
    }
};
}; // namespace our