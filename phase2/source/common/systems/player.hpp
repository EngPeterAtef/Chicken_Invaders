#pragma once

#include "../components/camera.hpp"
#include "../components/collision.hpp"
#include "../ecs/world.hpp"
#include <ctime>

#include "../application.hpp"

#include "../systems/sound.hpp"
#include "./collision-system.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

namespace our
{
    class PlayerSystem
    {
        Application *app;
        our::CollisionSystem collisionSystem;
        Entity *laser;
        Entity *laser_green;
        Entity *laser_left;
        Entity *laser_right;
        Entity *player;
        Sound chicken_kaaaak_sound = Sound("assets/sounds/chicken_kill.mp3", false);
        Sound bomb_sound = Sound("assets/sounds/bomb-explosion.mp3", false);
        Sound laser_sound = Sound("assets/sounds/laser.mp3", false);
        Sound chicken_leg_sound = Sound("assets/sounds/chicken_leg.mp3", false);
        Sound heart_sound = Sound("assets/sounds/monkey.mp3", false);
        Sound background_sound = Sound("assets/sounds/in_game.mp3", true);
        Sound rocket_sound = Sound("assets/sounds/rocket.mp3", true);
        bool is_rotating = false;

    public:
        int lives = 3;
        int score = 0;
        int weapon_level = 0;
        int level_counter = 0;
        double level_start_time = 0;
        void enter(World *world, Application *app)
        {
            background_sound.play();
            rocket_sound.play();
            weapon_level = 0;
            score = 0;
            lives = 3;
            this->app = app;
            level_start_time = glfwGetTime(); //////////start time of level
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
            for (auto entity1 : world->getEntities())
            {
                // Look for the right laser
                if (entity1->name == "laser_green")
                {
                    laser_green = entity1;
                    break;
                }
            }
            for (auto entity1 : world->getEntities())
            {
                // Look for the right laser
                if (entity1->name == "laser_right")
                {
                    laser_right = entity1;
                    break;
                }
            }
            for (auto entity1 : world->getEntities())
            {
                // Look for the left laser
                if (entity1->name == "laser_left")
                {
                    laser_left = entity1;
                    break;
                }
            }
        }

        // This should be called every frame to update player
        void update(World *world, float deltaTime)
        {
            double currentFrameTime = glfwGetTime();
            if (currentFrameTime - level_start_time >= 15) // each level 15 seconds
            {
                level_counter = 0;

                level_start_time = currentFrameTime;
            }
            if (weapon_level == 1)
                weapon_level1_controll();
            if (app->getKeyboard().isPressed(GLFW_KEY_SPACE))
            {
                laser_sound.play();
                if (weapon_level == 1)
                {

                    laser_green->localTransform.scale = glm::vec3(0.2, 0.2, 10);
                    laser_left->localTransform.scale = glm::vec3(1, 1, 1);
                    laser_right->localTransform.scale = glm::vec3(1, 1, 1);
                    Entity *fireEnemy1 = collisionSystem.detectFiring(world, laser_green);
                    Entity *fireEnemy2 = collisionSystem.detectFiring(world, laser_left);
                    Entity *fireEnemy3 = collisionSystem.detectFiring(world, laser_right);
                    if (fireEnemy1 && fireEnemy1->getComponent<CollisionComponent>())
                        hurt_enemy(world, fireEnemy1);

                    if (fireEnemy2 && fireEnemy2->getComponent<CollisionComponent>())
                        hurt_enemy(world, fireEnemy2);

                    if (fireEnemy3 && fireEnemy3->getComponent<CollisionComponent>())
                        hurt_enemy(world, fireEnemy3);
                }
                else
                {

                    laser->localTransform.scale = glm::vec3(0.2, 0.2, 10);
                    Entity *fireEnemy = collisionSystem.detectFiring(world, laser);
                    if (fireEnemy)
                        hurt_enemy(world, fireEnemy);
                }
            }
            else
            {
                laser->localTransform.scale = glm::vec3(0, 0, 0);
                laser_green->localTransform.scale = glm::vec3(0, 0, 0);
                laser_left->localTransform.scale = glm::vec3(0, 0, 0);
                laser_right->localTransform.scale = glm::vec3(0, 0, 0);
            }
            // score += 1;
            Entity *enemy_collision = collisionSystem.detectCollision(world, player);
            if (enemy_collision)
            {
                chicken_kaaaak_sound.play();
                enemy_collision->localTransform.scale = glm::vec3(0, 0, 0);
                enemy_collision->deleteComponent<CollisionComponent>();
                world->markForRemoval(enemy_collision);

                lives--;
                // std::cout << "Lives: " << lives << " Score : " << score << std::endl;

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
                    rocket_sound.stop();
                    background_sound.stop();
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
                    outfile << highScore << '\n'
                            << score;
                    outfile.close();
                }
            }
            Entity *monkey_collision = collisionSystem.detectMonkeyCollision(world, player);
            if (monkey_collision)
            {
                monkey_collision->localTransform.scale = glm::vec3(0, 0, 0);
                monkey_collision->deleteComponent<CollisionComponent>();
                // world->markForRemoval(monkey_collision); // if we delete monkey an error occurs in monkey light
                /* kill al chickens when you take a monkey */
                for (auto entity : world->getEntities())
                {
                    bomb_sound.play();
                    if ((entity->name == "enemy" || entity->name == "boss") && entity->getComponent<CollisionComponent>())
                    {
                        entity->getComponent<CollisionComponent>()->health -= 100;
                        hurt_enemy(world, entity);
                    }
                }
            }

            Entity *heart_collision = collisionSystem.detectHeartCollision(world, player);
            if (heart_collision)
            {
                heart_collision->localTransform.scale = glm::vec3(0, 0, 0);
                heart_collision->deleteComponent<CollisionComponent>();
                world->markForRemoval(heart_collision);
                heart_sound.play();
                lives++;
                // std::cout << "Lives: " << lives << " Score : " << score << std::endl;
                if (lives > 3)
                {
                    // std::cout << "Lives are more than 3" << std::endl;
                    lives = 3;
                    score += 100;
                }
                else
                {
                    // std::cout << "Lives are less than 3" << std::endl;
                    for (auto entity1 : world->getEntities())
                    {
                        // Look for the lives
                        if (lives == 3 && entity1->name == "lives1")
                        {
                            // std::cout << "Lives are 3" << std::endl;
                            entity1->localTransform.scale = glm::vec3(0.008, 0.008, 0.008);
                            break;
                        }
                        if (lives == 2 && entity1->name == "lives2")
                        {
                            // std::cout << "Lives are 2" << std::endl;
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
                chicken_leg_sound.play();
                score += 50;
            }
            if (score > 500 && weapon_level != 1)
            {
                weapon_level = 1;
                laser->localTransform.scale = glm::vec3(0, 0, 0);
            }
            world->deleteMarkedEntities();
            if (app->getKeyboard().isPressed(GLFW_KEY_ESCAPE))
            {
                rocket_sound.stop();
            }
        }

        void weapon_level1_controll()
        {
            glm::vec3 &rocket_rotation = player->localTransform.rotation;
            laser_green->localTransform.rotation.z = rocket_rotation.x;
        }

        void hurt_enemy(World *world, Entity *firedEnemy)
        {
            chicken_kaaaak_sound.play();
            int enenmy_health = firedEnemy->getComponent<CollisionComponent>()->health--;
            // std::cout << "Enemy health: " << enenmy_health << std::endl;

            if (enenmy_health <= 0)
            {

                generate_chicken_leg(world, firedEnemy->localTransform.position);
                score += firedEnemy->getComponent<CollisionComponent>()->bonus;
                firedEnemy->deleteComponent<CollisionComponent>();
                world->markForRemoval(firedEnemy);
            }
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
                ImGui::SetWindowSize(ImVec2((float)550, (float)200));
                ImGui::SetWindowPos(ImVec2(app->getWindowSize().x - 200.0f, 0.0f)); // Set position to top right
                std::string player_score = "score: " + std::to_string((int)score);  // Set score
                ImGui::SetWindowFontScale(1.0f);
                ImGui::TextUnformatted(player_score.c_str());
                // ImGui::TextUnformatted(hearts.c_str());
                ImGui::End();
            }
        }

        void exit()
        {
            delete player;
            delete laser;
            delete laser_green;
            delete laser_left;
            delete laser_right;
        }
    };

} // namespace our
