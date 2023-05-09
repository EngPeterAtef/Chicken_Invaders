#pragma once

#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <random>
#include <iostream>
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
        void rendering(World *world, int zCounter)
        {

            Entity *newEntity = world->add();
            // newEntity->name = "enemy";
            // newEntity->name = "enemy";

            // newEntity->localTransform.position = glm::vec3(generateRandomNumber(-20, 20), -0.3, zCounter);
            // newEntity->localTransform.rotation = glm::vec3(0, 0, 0);
            // newEntity->localTransform.scale = glm::vec3(0.4, 0.4, 0.4);

            // MeshRendererComponent *meshRendererComp = newEntity->addComponent<MeshRendererComponent>();

            // meshRendererComp->deserialize({{"type", "Mesh Renderer"}, {"mesh", "chicken"}, {"material", "chicken"}});
            // MovementComponent *movementRendererComp = newEntity->addComponent<MovementComponent>();
            // // movementRendererComp->linearVelocity = glm::vec3(0, 0, 5);
            // movementRendererComp->angularVelocity = glm::vec3(0, 10, 0);
        }

        void delete_chickens(World *world)
        {

            //     for (auto entity : world->getEntities())
            //     {

            //         if (entity->name == "enemy")
            //         {
            //             // cout << "hereeeeeeeeeeeeee" << endl;

            //             glm::vec3 maxCollider = entity->getComponent<CollisionComponent>()->mesh->maxvertex;
            //             maxCollider *= entity->localTransform.scale[0];
            //             maxCollider += entity->localTransform.position;
            //             if (maxCollider.z > entity->parent->localTransform.position.z)
            //             {

            //                 // delete chicken
            //                 // std::cout << "Chicken out of bounds\n";
            //                 world->markForRemoval(entity);
            //                 world->deleteMarkedEntities();

            //                                         // cout << "helllllllllllllllllllllll" << endl;
            //             }
            //         }
            //     }
            //     world->deleteMarkedEntities();
        }
    };
}

// float zDistance =
//     glm::distance(player->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position, 1.0),
//                   enemy->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position.x,
//                                                              player->localTransform.position.y,
//                                                              enemy->local
