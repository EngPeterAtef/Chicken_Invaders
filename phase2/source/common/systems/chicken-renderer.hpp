#pragma once
#include <GLFW/glfw3.h>
#include "../components/collision.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/world.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <random>
#include <math.h>
#include <ctime>

using namespace std;
namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class ChickenRenderer
    {
        double scaling = 0.4;
        double startTime = 0;
        int speedIncrease = 0;
        double generateRandomNumber(double minX, double maxX)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(minX, maxX);
            return dis(gen);
        }

    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void rendering(World *world)
        {
            double currentTime = glfwGetTime();
            if (currentTime - startTime > 5)
            { // each 5 senconds increase the speed
                // start time is of the current speed slot which is 5 seconds 1/3 the level period

                speedIncrease += 4;
                startTime = currentTime;
            }
            // cout <<world->
            // 2 3 4  5 6 7  8 9 10  11 12 13
            //  8 12 16   20 24 28    32 36 40      44 48 52

            if (speedIncrease == 16 || speedIncrease == 28 || speedIncrease == 40 || speedIncrease == 52)
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
                if (exists == true)
                    return;
                delete_chickens(world);
                scaling = 1;
                Entity *newEntity = world->add();
                newEntity->name = "boss";

                newEntity->localTransform.scale = glm::vec3(scaling, scaling, scaling);
                newEntity->localTransform.position = glm::vec3(0, 0, 40);

                MeshRendererComponent *meshRendererComp = newEntity->addComponent<MeshRendererComponent>();
                meshRendererComp->deserialize({{"type", "Mesh Renderer"}, {"mesh", "chicken"}, {"material", "chicken"}});

                CollisionComponent *collisionComp = newEntity->addComponent<CollisionComponent>();
                collisionComp->deserialize({{"type", "Collision"}, {"mesh", "chicken"}});

                MovementComponent *movementRendererComp = newEntity->addComponent<MovementComponent>();
                movementRendererComp->linearVelocity = glm::vec3(0, 0, 0);
                // cout << (int)fmin(40, 4 + speedIncrease) << endl;
                movementRendererComp->angularVelocity = glm::vec3(0, 0, 0);
                newEntity->name = "boss";
            }
            else
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
                if (exists == true)
                    return;
                Entity *newEntity = world->add();
                newEntity->name = "enemy";

                scaling = 0.4;
                newEntity->localTransform.scale = glm::vec3(scaling + speedIncrease / 80.0, scaling + speedIncrease / 80.0, scaling + speedIncrease / 80.0);

                newEntity->localTransform.position = glm::vec3(generateRandomNumber(-30, 30), generateRandomNumber(-5, 5), -20);
                newEntity->localTransform.scale = glm::vec3(0.4, 0.4, 0.4);

                MeshRendererComponent *meshRendererComp = newEntity->addComponent<MeshRendererComponent>();
                meshRendererComp->deserialize({{"type", "Mesh Renderer"}, {"mesh", "chicken"}, {"material", "chicken"}});

                CollisionComponent *collisionComp = newEntity->addComponent<CollisionComponent>();
                collisionComp->deserialize({{"type", "Collision"}, {"mesh", "chicken"}});

                MovementComponent *movementRendererComp = newEntity->addComponent<MovementComponent>();
                movementRendererComp->linearVelocity = glm::vec3(0, 0, (int)fmin(40, 4 + speedIncrease));
                cout << 6 + speedIncrease << endl;
                movementRendererComp->angularVelocity = glm::vec3(0, 3, 0);
            }
            // if (speedIncrease == 12 || speedIncrease = 24 || speedIncrease == 36 || speedIncrease == 48)
            // {
            //     scaling = 4;
            // }
            // if (speedIncrease == 8 || speedIncrease = 20 || speedIncrease == 32 || speedIncrease == 40)
            // {
            //     scaling = 4;
            // }
        }

        void delete_chickens(World *world)
        {

            for (auto entity : world->getEntities())
            {

                if (entity->name == "enemy")
                {
                    // cout << "hereeeeeeeeeeeeee" << endl;

                    // cout << (maxCollider.z) << "   " << entity->localTransform.position.z << endl;
                    if (entity->localTransform.position.z > 100)
                    {

                        // // delete chicken
                        // std::cout << "Chicken out of bounds\n";
                        world->markForRemoval(entity);
                        // world->deleteMarkedEntities();

                        // std::cout << "helllllllllllllllllllllll" << endl;
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

                    world->markForRemoval(entity);
                }
            }
            world->deleteMarkedEntities();
        }
    };
} // namespace our

// float zDistance =
//     glm::distance(player->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position, 1.0),
//                   enemy->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position.x,
//                                                              player->localTransform.position.y,
//                                                              enemy->local
