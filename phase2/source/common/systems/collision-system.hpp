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
        // Laser entity
        Entity *laser;
        // will store the entities that we will be checking for collisions
        Entity *enemy;

    public:
        Entity *detectFiring(World *world)
        {
            for (auto entity1 : world->getEntities())
            {
                // Look for the player
                if (entity1->name == "laser")
                {
                    laser = entity1;
                    break;
                }
            }
            for (auto entity1 : world->getEntities())
            {
                // Look for the player
                if (entity1->name == "enemy")
                {
                    enemy = entity1;
                    // gets the min and max vertices using the mesh class
                    glm::vec3 minLaserVertex = laser->getComponent<CollisionComponent>()->mesh->minvertex;
                    glm::vec3 maxLaserVertex = laser->getComponent<CollisionComponent>()->mesh->maxvertex;

                    // transforms the min and max vertices to the wold space
                    minLaserVertex *= laser->localTransform.scale[0];
                    maxLaserVertex *= laser->localTransform.scale[0];
                    minLaserVertex += laser->localTransform.position + laser->parent->localTransform.position;
                    maxLaserVertex += laser->localTransform.position + laser->parent->localTransform.position;

                    // gets the min and max vertices using the mesh class
                    glm::vec3 minCollider = enemy->getComponent<CollisionComponent>()->mesh->minvertex;
                    glm::vec3 maxCollider = enemy->getComponent<CollisionComponent>()->mesh->maxvertex;

                    // transforms the min and max vertices to the wold space
                    minCollider *= enemy->localTransform.scale[0];
                    maxCollider *= enemy->localTransform.scale[0];
                    minCollider += enemy->localTransform.position;
                    maxCollider += enemy->localTransform.position;

                    std::cout << "minLaserVertex " << minLaserVertex.x << " " << minLaserVertex.y << " " << minLaserVertex.z << std::endl;
                    std::cout << "maxLaserVertex " << maxLaserVertex.x << " " << maxLaserVertex.y << " " << maxLaserVertex.z << std::endl;
                    std::cout << "minCollider " << minCollider.x << " " << minCollider.y << " " << minCollider.z << std::endl;
                    std::cout << "maxCollider " << maxCollider.x << " " << maxCollider.y << " " << maxCollider.z << std::endl;
                    // collision between AABBs check
                    if ((minLaserVertex.x <= maxCollider.x && maxLaserVertex.x >= minCollider.x) &&
                        (minLaserVertex.y <= maxCollider.y && maxLaserVertex.y >= minCollider.y) &&
                        (minLaserVertex.z <= maxCollider.z && maxLaserVertex.z >= minCollider.z))
                    {
                        std::cout << "collision detected laser : (" << '\n';
                        return enemy;
                    }
                }
            }
            return nullptr;
        }
        // This should be called every frame to detect if the player and any other entity collide.
        Entity *detectCollision(World *world)
        {
            for (auto entity1 : world->getEntities())
            {
                // Look for the player
                if (entity1->name == "player")
                {
                    player = entity1;
                    break;
                }
            }

            for (auto entity1 : world->getEntities())
            {
                // Look for the player
                if (entity1->name == "enemy")
                {
                    enemy = entity1;
                    // gets the min and max vertices using the mesh class
                    glm::vec3 minPlayerVertex = player->getComponent<CollisionComponent>()->mesh->minvertex;
                    glm::vec3 maxPlayerVertex = player->getComponent<CollisionComponent>()->mesh->maxvertex;

                    // transforms the min and max vertices to the wold space
                    minPlayerVertex *= player->localTransform.scale[0];
                    maxPlayerVertex *= player->localTransform.scale[0];
                    minPlayerVertex += player->localTransform.position + player->parent->localTransform.position;
                    maxPlayerVertex += player->localTransform.position + player->parent->localTransform.position;

                    // gets the min and max vertices using the mesh class
                    glm::vec3 minCollider = enemy->getComponent<CollisionComponent>()->mesh->minvertex;
                    glm::vec3 maxCollider = enemy->getComponent<CollisionComponent>()->mesh->maxvertex;

                    // transforms the min and max vertices to the wold space
                    minCollider *= enemy->localTransform.scale[0];
                    maxCollider *= enemy->localTransform.scale[0];
                    minCollider += enemy->localTransform.position;
                    maxCollider += enemy->localTransform.position;

                    std::cout << "minPlayerVertex " << minPlayerVertex.x << " " << minPlayerVertex.y << " " << minPlayerVertex.z << std::endl;
                    std::cout << "maxPlayerVertex " << maxPlayerVertex.x << " " << maxPlayerVertex.y << " " << maxPlayerVertex.z << std::endl;
                    std::cout << "minCollider " << minCollider.x << " " << minCollider.y << " " << minCollider.z << std::endl;
                    std::cout << "maxCollider " << maxCollider.x << " " << maxCollider.y << " " << maxCollider.z << std::endl;
                    // collision between AABBs check
                    if ((minPlayerVertex.x <= maxCollider.x && maxPlayerVertex.x >= minCollider.x) &&
                        (minPlayerVertex.y <= maxCollider.y && maxPlayerVertex.y >= minCollider.y) &&
                        (minPlayerVertex.z <= maxCollider.z && maxPlayerVertex.z >= minCollider.z))
                    {
                        std::cout << "collision detected distance : (" << '\n';
                        return enemy;
                    }

                    // calculates the distance between the player and the enemy
                    // float distance =
                    //     glm::distance(player->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position, 1.0),
                    //                   enemy->getLocalToWorldMatrix() * glm::vec4(enemy->localTransform.position, 1.0));
                    // // calculates the distance between the player and the enemy in each direction seperately
                    // float xDistance =
                    //     glm::distance(player->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position, 1.0),
                    //                   enemy->getLocalToWorldMatrix() * glm::vec4(enemy->localTransform.position.x,
                    //                                                              player->localTransform.position.y,
                    //                                                              player->localTransform.position.z, 1.0));
                    // float yDistance =
                    //     glm::distance(player->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position, 1.0),
                    //                   enemy->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position.x,
                    //                                                              enemy->localTransform.position.y,
                    //                                                              player->localTransform.position.z, 1.0));

                    // float zDistance =
                    //     glm::distance(player->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position, 1.0),
                    //                   enemy->getLocalToWorldMatrix() * glm::vec4(player->localTransform.position.x,
                    //                                                              player->localTransform.position.y,
                    //                                                              enemy->localTransform.position.z, 1.0));

                    // std::cout << "distance : (" <<distance<<","<<xDistance<< "," <<yDistance << "," << zDistance << ")" << '\n';
                    // if the distance between the player and the enemy is less than 0.5f then we have a collision
                    // if (zDistance < 4.0f && xDistance < 4.0) // 4.0 and 3.0 are calculated by trial and error
                    // if ((zDistance < 4.0f && xDistance < 4.0) || (yDistance < 2.0 && xDistance < 3.0))
                    // {

                    //     std::cout << "collision detected distance : (" << distance << "," << xDistance << "," << yDistance << "," << zDistance << ")" << '\n';
                    //     return enemy;
                    // }
                }
            }
            return nullptr;
        }
    };

} // namespace our
