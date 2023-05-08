#pragma once

#include "../components/camera.hpp"
#include "../components/collision.hpp"
#include "../ecs/world.hpp"

#include "../application.hpp"

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

  public:
    int lives = 3;
    int score = 0;
    void enter(Application *app)
    {
        lives = 3;
        this->app = app;
    }

    // This should be called every frame to update player
    void update(World *world, float deltaTime)
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
        if (app->getKeyboard().isPressed(GLFW_KEY_SPACE))
        {
            laser->localTransform.scale = glm::vec3(0.2, 0.2, 10);
            Entity *fireEnemy = collisionSystem.detectFiring(world);
            if (fireEnemy)
            {
                fireEnemy->localTransform.scale = glm::vec3(0, 0, 0);
                fireEnemy->deleteComponent<CollisionComponent>();
                world->markForRemoval(fireEnemy);
                world->deleteMarkedEntities();
                score += 10;
            }
        }
        else
        {
            laser->localTransform.scale = glm::vec3(0, 0, 0);
        }
        // score += 1;
        Entity *enemy_collision = collisionSystem.detectCollision(world);
        if (enemy_collision)
        {
            enemy_collision->localTransform.scale = glm::vec3(0, 0, 0);
            enemy_collision->deleteComponent<CollisionComponent>();
            world->markForRemoval(enemy_collision);
            world->deleteMarkedEntities();
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
            }
        }
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
