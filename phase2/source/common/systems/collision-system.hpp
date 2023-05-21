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

namespace our
{
    // this class is reponsible for detecting collisions between entities
    // it uses the AABBs collision detection algorithm from the collision component on each entity
    class CollisionSystem
    {

    public:
        // Detect collision between laser and chickens
        // returns the chicken laser that the laser collided with
        Entity *detectFiring(World *world, Entity *laser)
        {
            for (auto entity : world->getEntities())
            {
                // Look for the player
                if ((entity->name == "enemy" || entity->name == "boss") && entity->getComponent<CollisionComponent>())
                {
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
                    if (laser->parent->parent)
                    {
                        minLaserVertex.x *= laser->localTransform.scale[0] * 1000;
                        maxLaserVertex.x *= laser->localTransform.scale[0] * 1000;
                        minLaserVertex += laser->parent->parent->localTransform.position;
                        maxLaserVertex += laser->parent->parent->localTransform.position;
                    }
                    minLaserVertex += laser->localTransform.position + laser->parent->localTransform.position;
                    maxLaserVertex += laser->localTransform.position + laser->parent->localTransform.position;

                    // gets the min and max vertices using the mesh class
                    glm::vec3 minCollider = entity->getComponent<CollisionComponent>()->mesh->minvertex;
                    glm::vec3 maxCollider = entity->getComponent<CollisionComponent>()->mesh->maxvertex;

                    // transforms the min and max vertices to the wold space
                    minCollider *= entity->localTransform.scale[0];
                    maxCollider *= entity->localTransform.scale[0];
                    minCollider += entity->localTransform.position;
                    maxCollider += entity->localTransform.position;

                    // collision between AABBs check
                    if ((minLaserVertex.x <= maxCollider.x && maxLaserVertex.x >= minCollider.x) &&
                        (minLaserVertex.y <= maxCollider.y && maxLaserVertex.y >= minCollider.y) &&
                        (minLaserVertex.z <= maxCollider.z && maxLaserVertex.z >= minCollider.z))
                    {
                        return entity;
                    }
                }
            }
            // returns nullptr if no collision is detected
            return nullptr;
        }

        // Detect collision between player and any other collider
        // return the entity that the player collided with
        Entity *detectCollision(World *world, Entity *player, std::string name)
        {

            for (auto entity : world->getEntities())
            {
                if (entity->name == name && entity->getComponent<CollisionComponent>())
                {
                    Entity *result = checkCollision(player, entity);
                    if (result)
                        return result;
                }
            }
            return nullptr;
        }

        Entity *checkCollision(Entity *player, Entity *enemy)
        {
            // gets the min and max vertices using the mesh class
            glm::vec3 minPlayerVertex = player->getComponent<CollisionComponent>()->mesh->minvertex;
            glm::vec3 maxPlayerVertex = player->getComponent<CollisionComponent>()->mesh->maxvertex;

            // transforms the min and max vertices to the wold space
            minPlayerVertex *= player->localTransform.scale[0];
            maxPlayerVertex *= player->localTransform.scale[0];
            if (enemy->name == "enemy") // special case for chickens because its collider is too long in y direction
            {
                minPlayerVertex.y *= 0.1;
                maxPlayerVertex.y *= 0.1;
            }
            if (enemy->name == "egg") // special case for eggs because its collider is too long in y direction
            {
                minPlayerVertex.y *= 0.3;
                maxPlayerVertex.y *= 0.3;
            }
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
            // special case for heart because it's y min/max colliders are shifted up for some reason
            if (enemy->name == "heart")
            {
                minCollider.y += 5;
                maxCollider.y += 5;
            }
            // collision between AABBs check
            if ((minPlayerVertex.x <= maxCollider.x && maxPlayerVertex.x >= minCollider.x) &&
                (minPlayerVertex.y <= maxCollider.y && maxPlayerVertex.y >= minCollider.y) &&
                (minPlayerVertex.z <= maxCollider.z && maxPlayerVertex.z >= minCollider.z))
            {
                return enemy;
            }
            // returns nullptr if no collision is detected
            return nullptr;
        }
    };

} // namespace our
