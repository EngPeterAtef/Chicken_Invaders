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

    // will store the entities that we will be checking for collisions
    Entity *enemy;

  public:
    Entity *detectFiring(World *world, Entity *laser)
    {
        for (auto entity1 : world->getEntities())
        {
            // Look for the player
            if (entity1->name == "enemy" && entity1->getComponent<CollisionComponent>())
            {
                enemy = entity1;
                // gets the min and max vertices using the mesh class
                glm::vec3 minLaserVertex = laser->getComponent<CollisionComponent>()->mesh->minvertex;
                glm::vec3 maxLaserVertex = laser->getComponent<CollisionComponent>()->mesh->maxvertex;

                // transforms the min and max vertices to the wold space
                minLaserVertex *= laser->localTransform.scale[2];
                maxLaserVertex *= laser->localTransform.scale[2];
                minLaserVertex.x *= laser->localTransform.scale[0] * 0.005;
                maxLaserVertex.x *= laser->localTransform.scale[0] * 0.005;
                minLaserVertex.y *= laser->localTransform.scale[0];
                maxLaserVertex.y *= laser->localTransform.scale[0];
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

                // std::cout << "minLaserVertex: " << minLaserVertex.x << " " << minLaserVertex.y << " "
                //           << minLaserVertex.z << std::endl;
                // std::cout << "maxLaserVertex: " << maxLaserVertex.x << " " << maxLaserVertex.y << " "
                //           << maxLaserVertex.z << std::endl;
                // std::cout << "minCollider: " << minCollider.x << " " << minCollider.y << " " << minCollider.z
                //           << std::endl;
                // std::cout << "maxCollider: " << maxCollider.x << " " << maxCollider.y << " " << maxCollider.z
                //           << std::endl
                //           << std::endl
                //           << std::endl;

                // collision between AABBs check
                if ((minLaserVertex.x <= maxCollider.x && maxLaserVertex.x >= minCollider.x) &&
                    (minLaserVertex.y <= maxCollider.y && maxLaserVertex.y >= minCollider.y) &&
                    (minLaserVertex.z <= maxCollider.z && maxLaserVertex.z >= minCollider.z))
                {
                    std::cout << "collision detected laser ----------------------------------" << '\n';
                    return enemy;
                }
            }
        }
        return nullptr;
    }
    // return nullptr;

    Entity *detectMonkeyCollision(World *world, Entity *player)
    {

        for (auto entity : world->getEntities())
        {
            // Look for the player
            if (entity->name == "monkey" && entity->getComponent<CollisionComponent>())
            {
                // gets the min and max vertices using the mesh class
                glm::vec3 minPlayerVertex = player->getComponent<CollisionComponent>()->mesh->minvertex;
                glm::vec3 maxPlayerVertex = player->getComponent<CollisionComponent>()->mesh->maxvertex;

                // transforms the min and max vertices to the wold space
                minPlayerVertex *= player->localTransform.scale[0];
                maxPlayerVertex *= player->localTransform.scale[0];
                minPlayerVertex += player->localTransform.position + player->parent->localTransform.position;
                maxPlayerVertex += player->localTransform.position + player->parent->localTransform.position;

                // gets the min and max vertices using the mesh class
                glm::vec3 minCollider = entity->getComponent<CollisionComponent>()->mesh->minvertex;
                glm::vec3 maxCollider = entity->getComponent<CollisionComponent>()->mesh->maxvertex;

                // transforms the min and max vertices to the wold space
                minCollider *= entity->localTransform.scale[0];
                maxCollider *= entity->localTransform.scale[0];
                minCollider += entity->localTransform.position;
                maxCollider += entity->localTransform.position;

                // collision between AABBs check
                if ((minPlayerVertex.x <= maxCollider.x && maxPlayerVertex.x >= minCollider.x) &&
                    (minPlayerVertex.y <= maxCollider.y && maxPlayerVertex.y >= minCollider.y) &&
                    (minPlayerVertex.z <= maxCollider.z && maxPlayerVertex.z >= minCollider.z))
                {
                    std::cout << "Monkey collision" << '\n';
                    return entity;
                }
            }
        }
        return nullptr;
    }
    Entity *detectCollision(World *world, Entity *player)
    {

        for (auto entity1 : world->getEntities())
        {
            // Look for the player
            if (entity1->name == "enemy" && entity1->getComponent<CollisionComponent>())
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

                // collision between AABBs check
                if ((minPlayerVertex.x <= maxCollider.x && maxPlayerVertex.x >= minCollider.x) &&
                    (minPlayerVertex.y <= maxCollider.y && maxPlayerVertex.y >= minCollider.y) &&
                    (minPlayerVertex.z <= maxCollider.z && maxPlayerVertex.z >= minCollider.z))
                {

                    std::cout << "collision detected distance : (" << '\n';
                    return enemy;
                }
            }
        }
        return nullptr;
    }
    Entity *detectChickenLeg(World *world, Entity *player)
    {

        for (auto entity1 : world->getEntities())
        {
            // Look for the player
            if (entity1->name == "chicken_leg" && entity1->getComponent<CollisionComponent>())
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

                // collision between AABBs check
                if ((minPlayerVertex.x <= maxCollider.x && maxPlayerVertex.x >= minCollider.x) &&
                    (minPlayerVertex.y <= maxCollider.y && maxPlayerVertex.y >= minCollider.y) &&
                    (minPlayerVertex.z <= maxCollider.z && maxPlayerVertex.z >= minCollider.z))
                {
                    std::cout << "collision  with chicken legs detected - bonus +100" << '\n';
                    return enemy;
                }
            }
        }
        return nullptr;
    }
};

} // namespace our
