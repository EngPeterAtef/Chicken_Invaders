#pragma once

#include "../application.hpp"
#include "../components/camera.hpp"
#include "../components/collision.hpp"
#include "../components/mesh-renderer.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>

#include "../asset-loader.hpp"

#include <iostream>
namespace our
{
class CollisionSystem
{
    // the player we will be checking for collisions
    Entity *player;
    // will store the entities that we will be checking for collisions
    Entity *enemy;

  public:
    // This should be called every frame to detect if the player and any other entity collide.
    Entity *detectCollision(World *world)
    {
        CollisionComponent *collider = nullptr;
        for (auto entity1 : world->getEntities())
        {
            // Look for the player
            collider = entity1->getComponent<CollisionComponent>();
            if (collider != nullptr && entity1->name == "player")
            {
                player = collider->getOwner();
                // std::cout << "player collider detected" << '\n';
                break;
            }
        }

        CollisionComponent *col2 = nullptr;
        for (auto entity1 : world->getEntities())
        {
            // Look for the player
            col2 = entity1->getComponent<CollisionComponent>();
            if (col2 != nullptr && entity1->name == "enemy")
            {
                enemy = col2->getOwner();
                std::string name = entity1->name;

                // gets the min and max vertices using the mesh class
                glm::vec3 minPlayerVertex = player->getComponent<CollisionComponent>()->mesh->minvertex;
                glm::vec3 maxPlayerVertex = player->getComponent<CollisionComponent>()->mesh->maxvertex;

                // transforms the min and max vertices to the wold space
                minPlayerVertex *= player->localTransform.scale[0];
                maxPlayerVertex *= player->localTransform.scale[0];
                minPlayerVertex += player->localTransform.position;
                maxPlayerVertex += player->localTransform.position;

                // gets the min and max vertices using the mesh class
                glm::vec3 minCollider = entity1->getComponent<CollisionComponent>()->mesh->minvertex;
                glm::vec3 maxCollider = entity1->getComponent<CollisionComponent>()->mesh->maxvertex;

                // transforms the min and max vertices to the wold space
                minCollider *= entity1->localTransform.scale[0];
                maxCollider *= entity1->localTransform.scale[0];
                minCollider += entity1->localTransform.position;
                maxCollider += entity1->localTransform.position;

                // std::cout << "X : " << maxCollider.x - minPlayerVertex.x
                //           << "  Y:  " << maxCollider.y - minPlayerVertex.y << '\n';
                // collision between AABBs check
                if ((minPlayerVertex.x <= maxCollider.x && maxPlayerVertex.x >= minCollider.x) &&
                    (minPlayerVertex.y <= maxCollider.y && maxPlayerVertex.y >= minCollider.y) &&
                    (minPlayerVertex.z <= maxCollider.z && maxPlayerVertex.z >= minCollider.z))
                {
                    // i++;
                    std::cout << "collision detected" << '\n';
                    return enemy;
                }
            }
        }
        return nullptr;
    }
};

} // namespace our
