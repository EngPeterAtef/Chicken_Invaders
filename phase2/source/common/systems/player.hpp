#pragma once

#include "../components/camera.hpp"
#include "../components/collision.hpp"
#include "../ecs/world.hpp"

#include "../application.hpp"

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

#include "./collision-system.hpp"

namespace our
{
class PlayerSystem
{
    Application *app;
    our::CollisionSystem collisionSystem;
    Entity *laser;
    Entity *player;

  public:
    int lives = 3;
    int score = 0;
    void enter(World *world, Application *app)
    {
        score = 0;
        lives = 3;
        this->app = app;
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
            if (entity1->name == "player")
            {
                player = entity1;
                break;
            }
        }
    }

    // This should be called every frame to update player
    void update(World *world, float deltaTime)
    {

        if (app->getKeyboard().isPressed(GLFW_KEY_SPACE))
        {
            laser->localTransform.scale = glm::vec3(0.2, 0.2, 10);
            Entity *fireEnemy = collisionSystem.detectFiring(world, laser);
            if (fireEnemy)
            {
                generate_chicken_leg(world, fireEnemy->localTransform.position);
                fireEnemy->localTransform.scale = glm::vec3(0, 0, 0);
                fireEnemy->deleteComponent<CollisionComponent>();
                world->markForRemoval(fireEnemy);

                score += 10;
            }
        }
        else
        {
            laser->localTransform.scale = glm::vec3(0, 0, 0);
        }
        // score += 1;
        Entity *enemy_collision = collisionSystem.detectCollision(world, player);
        if (enemy_collision)
        {
            enemy_collision->localTransform.scale = glm::vec3(0, 0, 0);
            enemy_collision->deleteComponent<CollisionComponent>();
            world->markForRemoval(enemy_collision);

            lives--;
            std::cout << "Lives: " << lives << " Score : " << score << std::endl;

            for (auto entity1 : world->getEntities())
            {
                // Look for the lives
                if (lives == 2 && entity1->name == "lives1")
                {
                    entity1->localTransform.scale = glm::vec3(0, 0, 0);
                    break;
                }
                else if (lives == 1 && entity1->name == "lives2")
                {
                    entity1->localTransform.scale = glm::vec3(0, 0, 0);
                    break;
                }
            }
            if (lives == 0)
            {
                app->changeState("game-over");
                std::ifstream file_in("score.txt");
                if (!file_in)
                {
                    std::cerr << "Couldn't open file: "
                              << "score.txt" << std::endl;
                }
                // Read the file into a json object then close the file
                std::string str;
                getline(file_in, str);
                file_in.close();
                int highScore = std::stoi(str);

                if (score > highScore)
                    highScore = score; // Compare json highscore with current score

                std::ofstream outfile;
                outfile.open("score.txt");
                outfile << highScore << '\n' << score;
                outfile.close();
            }
        }
        Entity *monkey_collision = collisionSystem.detectMonkeyCollision(world, player);
        if (monkey_collision)
        {
            monkey_collision->localTransform.scale = glm::vec3(0, 0, 0);
            monkey_collision->deleteComponent<CollisionComponent>();
            world->markForRemoval(monkey_collision);

            lives++;
            std::cout << "Lives: " << lives << " Score : " << score << std::endl;
            if (lives > 3)
            {
                std::cout << "Lives are more than 3" << std::endl;
                lives = 3;
                score += 100;
            }
            else
            {
                std::cout << "Lives are less than 3" << std::endl;
                for (auto entity1 : world->getEntities())
                {
                    // Look for the lives
                    if (lives == 3 && entity1->name == "lives3")
                    {
                        std::cout << "Lives are 3" << std::endl;
                        entity1->localTransform.scale = glm::vec3(0.008, 0.008, 0.008);
                        break;
                    }
                    if (lives == 2 && entity1->name == "lives2")
                    {
                        std::cout << "Lives are 2" << std::endl;
                        entity1->localTransform.scale = glm::vec3(0.008, 0.008, 0.008);
                        break;
                    }
                }
            }
        }

        Entity *chicken_leg = collisionSystem.detectChickenLeg(world, player);
        if (chicken_leg)
        {
            chicken_leg->deleteComponent<CollisionComponent>();
            world->markForRemoval(chicken_leg);

            score += 50;
        }
        world->deleteMarkedEntities();
    }

    void generate_chicken_leg(World *world, glm::vec3 position)
    {
        Entity *childEntity = world->add();
        childEntity->name = "chicken_leg";
        childEntity->localTransform.position = position;
        childEntity->localTransform.scale = glm::vec3(0.2, 0.2, 0.2);
        childEntity->localTransform.rotation = glm::vec3(0, 0, -90);
        MeshRendererComponent *childMeshRendererComp = childEntity->addComponent<MeshRendererComponent>();
        childMeshRendererComp->deserialize(
            {{"type", "Mesh Renderer"}, {"mesh", "chicken_leg"}, {"material", "chicken_leg"}});
        CollisionComponent *childCollisionComp = childEntity->addComponent<CollisionComponent>();
        childCollisionComp->deserialize({{"type", "Collision"}, {"mesh", "chicken_leg"}});
        MovementComponent *childMovementRendererComp = childEntity->addComponent<MovementComponent>();
        childMovementRendererComp->linearVelocity = glm::vec3(0, 0, 15);
        childMovementRendererComp->angularVelocity = glm::vec3(0, 10, 0);
    }

    void imgui()
    {
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoScrollbar;
        bool *open_ptr = (bool *)true;
        if (ImGui::Begin("Chicken", open_ptr, window_flags))
        {
            ImGui::SetWindowPos(ImVec2(app->getWindowSize().x - 200.0f, 0.0f)); // Set position to top right
            std::string player_score = "score: " + std::to_string((int)score);  // Set score
            // std::string hearts = "Lives: " + std::to_string(lives);            // Set lives
            ImGui::SetWindowFontScale(2.0f);
            ImGui::TextUnformatted(player_score.c_str());
            // ImGui::TextUnformatted(hearts.c_str());
            ImGui::End();
        }
    }

    // When the state exits, it should call this function to ensure the mouse is unlocked
    void exit()
    {
    }
};

} // namespace our
