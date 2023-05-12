#pragma once

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
using namespace std;
namespace our
{

    double generateRandomNumber(double minX, double maxX)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(minX, maxX);
        return dis(gen);
    }
    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class ChickenRenderer
    {

    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void rendering(World *world, int speedIncrease)
        {

            Entity *newEntity = world->add();
            newEntity->name = "enemy";
            // cout <<world->
            newEntity->localTransform.position = glm::vec3(generateRandomNumber(-30, 30), 0, -20);
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
    };
} // namespace our

// float zDistance =
//     glm::distance(player->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position, 1.0),
//                   enemy->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position.x,
//                                                              player->localTransform.position.y,
//                                                              enemy->local
