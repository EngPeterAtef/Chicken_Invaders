#pragma once
#include "../components/collision.hpp"
#include "../components/movement.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/world.hpp"
#include <GLFW/glfw3.h>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <math.h>
#include <random>
namespace our
{

    // The EntityRenderer class is responsible for generating all random entities in the world
    class EntityRenderer
    {
        World *world;
        double scaling;
        double startTime;
        double speedIncrease;
        // this function is used to generate random numbers between a range
        double generateRandomNumber(double minX, double maxX)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(minX, maxX);
            return dis(gen);
        }

        // this function is used to generate new entity to the world
        void generate_entity(std::string name, std::string mesh, std::string material, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation, glm::vec3 linearVelocity, glm::vec3 angularVelocity, int health, int bonus)
        {
            Entity *newEntity = world->add();
            newEntity->name = name;

            newEntity->localTransform.position = position;
            newEntity->localTransform.scale = scale;
            newEntity->localTransform.rotation = rotation;

            MeshRendererComponent *meshRendererComp = newEntity->addComponent<MeshRendererComponent>();
            meshRendererComp->deserialize({{"type", "Mesh Renderer"}, {"mesh", mesh}, {"material", material}});

            CollisionComponent *collisionComp = newEntity->addComponent<CollisionComponent>();
            collisionComp->deserialize({{"type", "Collision"}, {"mesh", mesh}, {"health", health}, {"bonus", bonus}});

            MovementComponent *movementRendererComp = newEntity->addComponent<MovementComponent>();
            movementRendererComp->linearVelocity = linearVelocity;
            movementRendererComp->angularVelocity = angularVelocity;
        }

    public:
        void intialization(World *world)
        {
            scaling = 0.4;
            startTime = 0;
            speedIncrease = 0;
            this->world = world;
        }

        // this function is responsible for generating hearts in the world
        void renderHearts()
        {

            glm::vec3 scale = glm::vec3(0.1, 0.1, 0.1);
            glm::vec3 position = glm::vec3(generateRandomNumber(-30, 30), generateRandomNumber(-5, 5), -20);
            glm::vec3 rotation = glm::vec3(80, -10, 0);
            glm::vec3 lineVel = glm::vec3(0, 0, 20);
            glm::vec3 angVel = glm::vec3(0, 5, 0);
            generate_entity("heart", "heart", "heart", scale, position, rotation, lineVel, angVel, 0, 0);
        }

        // this function is responsible for generating monkeys in the world
        void renderMonkeys()
        {
            glm::vec3 scale = glm::vec3(1.1, 1.1, 1.1);
            glm::vec3 position = glm::vec3(generateRandomNumber(-30, 30), generateRandomNumber(-5, 5), -20);
            glm::vec3 rotation = glm::vec3(0, 0, 0);
            glm::vec3 lineVel = glm::vec3(0, 0, 20);
            glm::vec3 angVel = glm::vec3(0, 0, 0);
            generate_entity("monkey", "suzanne", "rocky_monkey", scale, position, rotation, lineVel, angVel, 1, 10);
        }

        // this function is responsible for generating chickens and bosses in the world
        void renderChicken(int bossesToCreate)
        {
            // if there is no boss in the world and the time to generate boss hasn't come yet we keep generating chickens
            if (bossesToCreate == 0 && !boss_exists())
            {
                speedIncrease += 0.1; // increase the speed of the chickens to make it harder through time
                glm::vec3 scale = glm::vec3(scaling + speedIncrease / 80.0, scaling + speedIncrease / 80.0, scaling + speedIncrease / 80.0);
                glm::vec3 position = glm::vec3(generateRandomNumber(-30, 30), generateRandomNumber(-5, 5), -20);
                glm::vec3 rotation = glm::vec3(0, 0, 0);
                glm::vec3 lineVel = glm::vec3(0, 0, (int)fmin(40, 4 + speedIncrease));
                glm::vec3 angVel = glm::vec3(0, 3, 0);
                generate_entity("enemy", "chicken", "chicken", scale, position, rotation, lineVel, angVel, 1, 10);
            }
            else if (bossesToCreate != 0) // now we should generate N boss (depending on the level), this if condition will be entered only one time each level
            {
                for (int i = 1; i <= bossesToCreate; i++)
                {
                    // shift the boss to the right or left depending on the number of the boss
                    int shift = 0;
                    if (i % 2 == 0)
                        shift = 10;

                    else if (i % 2 != 0 && i != 1)
                        shift = -10;

                    generate_entity("boss", "chicken", "chicken", glm::vec3(1, 1, 1), glm::vec3(shift, 0, 30), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 200, 1000 * i);
                }
            }

            // if the boss exists we generate eggs that come out of it
            if (boss_exists())
            {
                for (auto entity1 : world->getEntities())
                {
                    // Look for the boss to get its position
                    if (entity1->name == "boss")
                    {
                        glm::vec3 pos = entity1->localTransform.position;
                        glm::vec3 scale = glm::vec3(0.1, 0.1, 0.1);
                        glm::vec3 position = glm::vec3(generateRandomNumber(pos.x - 5, pos.x + 5), generateRandomNumber(pos.y, pos.y + 7), pos.z);
                        glm::vec3 rotation = glm::vec3(0, 0, 0);
                        glm::vec3 lineVel = glm::vec3(0, 0, 5);
                        glm::vec3 angVel = glm::vec3(0, 0, 0);
                        generate_entity("egg", "egg", "egg", scale, position, rotation, lineVel, angVel, 0, 0);
                    }
                }
            }
        }

        // check if there is a boss in the world
        bool boss_exists()
        {
            for (auto entity : world->getEntities())
                if (entity->name == "boss")
                    return true;

            return false;
        }

        // clear all entities that are behind the camera
        void clearOutOfBoundEntities()
        {

            for (auto entity : world->getEntities())
                if (entity->localTransform.position.z > 100)
                    world->markForRemoval(entity);

            world->deleteMarkedEntities();
        }
    };
}; // namespace our