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
using namespace std;
namespace our
{
    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class MonkeyRenderer
    {

    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void rendering(World *world, int zCounter)
        {

            Entity *newEntity = world->add();
            newEntity->name = "monkey";
            
            newEntity->localTransform.position = glm::vec3(our::generateRandomNumber(-15, 15),our::generateRandomNumber(-5, 5), zCounter);
            newEntity->localTransform.scale = glm::vec3(1, 1, 1);

            MeshRendererComponent *meshRendererComp = newEntity->addComponent<MeshRendererComponent>();
            meshRendererComp->deserialize({{"type", "Mesh Renderer"}, {"mesh", "monkey"}, {"material", "rocky_monkey"}});

            CollisionComponent *collisionComp = newEntity->addComponent<CollisionComponent>();
            collisionComp->deserialize({{"type", "Collision"}, {"mesh", "monkey"}});

            MovementComponent *movementRendererComp = newEntity->addComponent<MovementComponent>();
            movementRendererComp->linearVelocity = glm::vec3(0, 0, 30);
            movementRendererComp->angularVelocity = glm::vec3(0, 0, 0);
        }

        void delete_monkeys(World *world)
        {

            for (auto entity : world->getEntities())
            {

                if (entity->name == "moneky")
                {
                    // cout << "hereeeeeeeeeeeeee" << endl;

                    glm::vec3 maxCollider = entity->getComponent<CollisionComponent>()->mesh->maxvertex;
                    maxCollider *= entity->localTransform.scale[0];
                    maxCollider += entity->localTransform.position;
                    if (maxCollider.z > entity->parent->localTransform.position.z)
                    {

                        // delete monkey
                        std::cout << "Monkeys out of bounds\n";
                        world->markForRemoval(entity);
                        world->deleteMarkedEntities();
                    }
                }
            }
            world->deleteMarkedEntities();
        }
    };
} // namespace our

