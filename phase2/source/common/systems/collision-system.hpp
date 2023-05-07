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
            if (entity1->name == "player")
            {
                player = entity1;
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

                // // gets the min and max vertices using the mesh class
                // glm::vec3 minPlayerVertex = player->getComponent<CollisionComponent>()->mesh->minvertex;
                // glm::vec3 maxPlayerVertex = player->getComponent<CollisionComponent>()->mesh->maxvertex;

                // // transforms the min and max vertices to the wold space
                // minPlayerVertex *= player->localTransform.scale[0];
                // maxPlayerVertex *= player->localTransform.scale[0];
                // minPlayerVertex += player->parent->localTransform.position + player->localTransform.position;
                // maxPlayerVertex += player->parent->localTransform.position + player->localTransform.position;

                // // gets the min and max vertices using the mesh class
                // glm::vec3 minCollider = entity1->getComponent<CollisionComponent>()->mesh->minvertex;
                // glm::vec3 maxCollider = entity1->getComponent<CollisionComponent>()->mesh->maxvertex;

                // // transforms the min and max vertices to the wold space
                // minCollider *= entity1->localTransform.scale[0];
                // maxCollider *= entity1->localTransform.scale[0];
                // minCollider += entity1->localTransform.position;
                // maxCollider += entity1->localTransform.position;

                // collision between AABBs check
                // if ((minPlayerVertex.x <= maxCollider.x && maxPlayerVertex.x >= minCollider.x) &&
                //     (minPlayerVertex.y <= maxCollider.y && maxPlayerVertex.y >= minCollider.y) &&
                //     (minPlayerVertex.z <= maxCollider.z && maxPlayerVertex.z >= minCollider.z))

                // calculates the distance between the player and the enemy
                float distance =
                    glm::distance(player->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position, 1.0),
                                  enemy->getLocalToWorldMatrix() * glm::vec4(enemy->localTransform.position, 1.0));
                // calculates the distance between the player and the enemy in each direction seperately
                float xDistance =
                    glm::distance(player->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position, 1.0),
                                  enemy->getLocalToWorldMatrix() * glm::vec4(enemy->localTransform.position.x,
                                                                             player->localTransform.position.y,
                                                                             player->localTransform.position.z, 1.0));
                float yDistance =
                    glm::distance(player->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position, 1.0),
                                  enemy->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position.x,
                                                                             enemy->localTransform.position.y,
                                                                             player->localTransform.position.z, 1.0));

                float zDistance =
                    glm::distance(player->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position, 1.0),
                                  enemy->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position.x,
                                                                             player->localTransform.position.y,
                                                                             enemy->localTransform.position.z, 1.0));
                if (zDistance < 4.0f && xDistance < 3.0) // 4.0 and 3.0 are calculated by trial and error

                {

                    std::cout << "collision detected distance : (" << xDistance << "," << zDistance << ")" << '\n';
                    return enemy;
                }
            }
        }
        return nullptr;
    }
};

} // namespace our