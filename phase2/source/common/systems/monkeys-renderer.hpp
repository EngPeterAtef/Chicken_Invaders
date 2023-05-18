#pragma once

#include "../components/collision.hpp"
#include "../components/light.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/world.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <random>
namespace our
{
// The movement system is responsible for moving every entity which contains a MovementComponent.
// This system is added as a simple example for how use the ECS framework to implement logic.
// For more information, see "common/components/movement.hpp"
class MonkeyRenderer
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
    void rendering(World *world)
    {

        Entity *newEntity = world->add();
        newEntity->name = "monkey";

        newEntity->localTransform.position = glm::vec3(generateRandomNumber(-30, 30), generateRandomNumber(-5, 5), -20);
        newEntity->localTransform.scale = glm::vec3(1, 1, 1);

        MeshRendererComponent *meshRendererComp = newEntity->addComponent<MeshRendererComponent>();
        meshRendererComp->deserialize({{"type", "Mesh Renderer"}, {"mesh", "suzanne"}, {"material", "rocky_monkey"}});

        CollisionComponent *collisionComp = newEntity->addComponent<CollisionComponent>();
        collisionComp->deserialize({{"type", "Collision"}, {"mesh", "suzanne"}});

        MovementComponent *movementRendererComp = newEntity->addComponent<MovementComponent>();
        movementRendererComp->linearVelocity = glm::vec3(0, 0, 20);
        movementRendererComp->angularVelocity = glm::vec3(0, 0, 0);

        // Entity *light = world->add();
        // light->parent = newEntity;
        // light->name = "monkey_light";
        // light->localTransform.position = glm::vec3(0, 0, 1);
        // light->localTransform.rotation = glm::vec3(0, 0, -1);
        // light->addComponent<LightComponent>()->deserialize({{"type", "Light"},
        //                                                     {"kind", 2},
        //                                                     {"color", {0.7, 0.7, 0.7}},
        //                                                     {"cone_angles", {10, 0}},
        //                                                     {"attenuation", {0.01, 0, 0}}});
    }
    void delete_monkey_light(World *world)
    {
        for (auto entity : world->getEntities())
        {

            if (entity->name == "monkey_light")
            {
                if (entity->parent == nullptr)
                {
                    world->markForRemoval(entity);
                    world->deleteMarkedEntities();
                    break;
                }
            }
        }
    }
    void delete_monkeys(World *world)
    {

        for (auto entity : world->getEntities())
        {

            if (entity->name == "monkey")
            {
                glm::vec3 maxCollider = entity->getComponent<CollisionComponent>()->mesh->maxvertex;
                maxCollider *= entity->localTransform.scale[0];
                maxCollider += entity->localTransform.position;
                if (entity->localTransform.position.z > 60)
                {

                    // delete monkey
                    // std::cout << "Monkeys out of bounds\n";
                    world->markForRemoval(entity);
                    world->deleteMarkedEntities();
                    break;
                }
            }
        }
        world->deleteMarkedEntities();
    }
};
} // namespace our
