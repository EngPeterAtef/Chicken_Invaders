#pragma once

#include "../components/camera.hpp"
#include "../components/collision.hpp"
#include "../ecs/world.hpp"
#include <ctime>

#include "../application.hpp"

#include "../systems/forward-renderer.hpp"
#include "../systems/sound.hpp"
#include "./collision-system.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
// #include "./chicken-renderer.hpp"
#define TIME_LEVEL_1 30
#define TIME_LEVEL_2 60
#define TIME_LEVEL_3 90
#define TIME_LEVEL_4 120
#define TIME_LEVEL_5 160
#define postProcessingDuration 20

namespace our
{
class PlayerSystem
{
    Application *app;
    // our::ChickenRenderer chickenRenderer;
    our::CollisionSystem collisionSystem;
    Entity *laser;
    Entity *laser_green;
    Entity *laser_left;
    Entity *laser_right;
    Entity *player;
    Entity *shield;
    Sound chicken_kaaaak_sound = Sound("assets/sounds/chicken_kill.mp3", false);
    Sound chicken_hit_sound = Sound("assets/sounds/chicken_hit.mp3", false);
    Sound bomb_sound = Sound("assets/sounds/bomb-explosion.mp3", false);
    Sound laser_sound = Sound("assets/sounds/laser.mp3", false);
    Sound chicken_leg_sound = Sound("assets/sounds/chicken_leg.mp3", false);
    Sound heart_sound = Sound("assets/sounds/monkey.mp3", false);
    Sound background_sound = Sound("assets/sounds/in_game.mp3", true);
    Sound rocket_sound = Sound("assets/sounds/rocket.mp3", true);
    bool is_rotating = false;
    our::ForwardRenderer *forwardRenderer;
    bool applyingPostProcess = false;
    int postProcessingCurrentCounter = 0;
    int shieldCounter = 0;
    bool shieldEnabled = false;

  public:
    int lives = 3;
    int score = 0;
    int weapon_level = 0;
    bool bossExisted = false;
    // int level_counter = 0;
    double level_start_time = 0;
    int currentLevel = 0;
    void enter(World *world, Application *app, our::ForwardRenderer *forwardRenderer)
    {
        // chickenRenderer.intialization();
        this->forwardRenderer = forwardRenderer;
        bossExisted = false;
        currentLevel = 0;
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
            if (entity1->name == "player")
            {
                player = entity1;
                break;
            }
        }
        for (auto entity1 : world->getEntities())
        {
            // Look for the shield
            if (entity1->name == "shield")
            {
                shield = entity1;
                break;
            }
        }
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
            // Look for the middle green laser
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
    int update(World *world, float deltaTime, bool bossExists)
    {
        // checks if there is a postprocessing effect now and if yes it will terminate it after some number of frames
        checkForPostProcessing();
        // generates the bosses for levels 1, 2, 3, 4 & 5.
        int create_boss = boss(bossExists);
        // if we are in weapon level 1 this will control it
        if (weapon_level == 1)
            weapon_level1_controll();
        // pressing space will activate the laser else it will be deactivated
        if (app->getKeyboard().isPressed(GLFW_KEY_SPACE))
            fire(world);
        else
        {
            laser->localTransform.scale = glm::vec3(0, 0, 0);
            laser_green->localTransform.scale = glm::vec3(0, 0, 0);
            laser_left->localTransform.scale = glm::vec3(0, 0, 0);
            laser_right->localTransform.scale = glm::vec3(0, 0, 0);
        }
        // check for collision with chickens
        Entity *enemy_collision = collisionSystem.detectCollision(world, player, "enemy");
        if (enemy_collision)
            chickenHit(world, enemy_collision);
        // check for collision with monkey
        Entity *monkey_collision = collisionSystem.detectCollision(world, player, "monkey");
        if (monkey_collision)
            monkey(world, monkey_collision);
        // check for collision with heart
        Entity *heart_collision = collisionSystem.detectCollision(world, player, "heart");
        if (heart_collision)
            heart(world, heart_collision);
        // check for collision with egg
        Entity *egg_collision = collisionSystem.detectCollision(world, player, "egg");
        if (egg_collision)
            egg(world, egg_collision);
        // check for collision with chicken leg
        Entity *chicken_leg = collisionSystem.detectCollision(world, player, "chicken_leg");
        if (chicken_leg)
        {

            chicken_leg->deleteComponent<CollisionComponent>();
            world->markForRemoval(chicken_leg);
            chicken_leg_sound.play();
            score += 50;
        }
        // check for score to increase weapon level
        if (score > 2000 && weapon_level != 1)
        {
            weapon_level = 1;
            laser->localTransform.scale = glm::vec3(0, 0, 0);
        }
        // delete all marked entities
        world->deleteMarkedEntities();
        // stops all sounds if escape is pressed and returns to main menu
        if (app->getKeyboard().isPressed(GLFW_KEY_ESCAPE))
        {
            rocket_sound.stop();
            background_sound.stop();
        }

        return create_boss;
    }

    // post processing logic
    void checkForPostProcessing()
    {
        if (applyingPostProcess)
        {
            if (postProcessingCurrentCounter == postProcessingDuration)
            {
                forwardRenderer->changePostprocessShader(0);
                applyingPostProcess = false;
                postProcessingCurrentCounter = 0;
            }
            else
            {
                postProcessingCurrentCounter++;
            }
        }
    }
    // switching to weapon level 1
    void weapon_level1_controll()
    {
        glm::vec3 &rocket_rotation = player->localTransform.rotation;
        laser_green->localTransform.rotation.z = rocket_rotation.x;
    }

    // responsible for fire on enemy logic
    void hurt_enemy(World *world, Entity *firedEnemy)
    {
        chicken_kaaaak_sound.play();
        int enenmy_health = firedEnemy->getComponent<CollisionComponent>()->health--;

        if (enenmy_health <= 0)
        {

            generate_chicken_leg(world, firedEnemy->localTransform.position);
            score += firedEnemy->getComponent<CollisionComponent>()->bonus;
            firedEnemy->deleteComponent<CollisionComponent>();
            world->markForRemoval(firedEnemy);
        }
    }

    // responsible for boss generation logic
    int boss(bool bossExists)
    {
        int create_boss = 0;
        if (!bossExists && !bossExisted)
        {
            int time = glfwGetTime() - level_start_time;
            if (time == TIME_LEVEL_1 && currentLevel == 0)
            {
                currentLevel = 1;
                create_boss = 1;
                // std::cout << "create 1 bosses!!!!!!!!!!!!!!!!!";
            }
            else if (time == TIME_LEVEL_2 && currentLevel == 1)
            {
                create_boss = 2;

                currentLevel = 2;
            }
            else if (time == TIME_LEVEL_3 && currentLevel == 2)
            {
                create_boss = 3;

                currentLevel = 3;
            }
            else if (time == TIME_LEVEL_4 && currentLevel == 3)
            {
                create_boss = 4;

                currentLevel = 4;
            }
            else if (time >= TIME_LEVEL_5 && currentLevel == 4)
            {
                create_boss = 5;

                currentLevel = 5;
            }
        }
        else if (!bossExists && bossExisted) // el boss kan mawgoud w meshy
        {
            level_start_time = glfwGetTime();
            bossExisted = false;
        }
        if (bossExists && !bossExisted) // law el boss lessa gai
        {
            bossExisted = true;
        }
        return create_boss;
    }
    // responsible for firing logic
    void fire(World *world)
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

    // responsible for collision with monkey logic
    void egg(World *world, Entity *egg_collision)
    {
        changePostprocessing(3);
        egg_collision->localTransform.scale = glm::vec3(0, 0, 0);
        egg_collision->deleteComponent<CollisionComponent>();
        world->markForRemoval(egg_collision);
        if (!shieldEnabled)
            lives--;
        else
        {
            shield->localTransform.scale = glm::vec3(0, 0, 0);
            shieldEnabled = false;
        }
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
            std::string str;
            getline(file_in, str);
            file_in.close();
            int highScore = std::stoi(str);

            if (score > highScore)
                highScore = score;

            std::ofstream outfile;
            outfile.open("score.txt");
            outfile << highScore << '\n' << score;
            outfile.close();
        }
    }
    // responsible for collision with monkey logic
    void monkey(World *world, Entity *monkey_collision)
    {
        changePostprocessing(2);
        monkey_collision->localTransform.scale = glm::vec3(0, 0, 0);
        monkey_collision->deleteComponent<CollisionComponent>();
        world->markForRemoval(monkey_collision);
        if (shieldCounter < 1 || shieldEnabled)
        {
            shieldCounter++;
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
        else
        {
            shieldCounter = 0;
            shield->localTransform.scale = glm::vec3(2, 2, 2);
            shieldEnabled = true;
        }
    }
    // responsible for collision with heart logic
    void heart(World *world, Entity *heart_collision)
    {
        heart_collision->localTransform.scale = glm::vec3(0, 0, 0);
        heart_collision->deleteComponent<CollisionComponent>();
        world->markForRemoval(heart_collision);
        heart_sound.play();
        lives++;
        if (lives > 3)
        {
            lives = 3;
            score += 100;
        }
        else
        {
            for (auto entity1 : world->getEntities())
            {
                // Look for the lives
                if (lives == 3 && entity1->name == "lives1")
                {
                    entity1->localTransform.scale = glm::vec3(0.008, 0.008, 0.008);
                    break;
                }
                if (lives == 2 && entity1->name == "lives2")
                {
                    entity1->localTransform.scale = glm::vec3(0.008, 0.008, 0.008);
                    break;
                }
            }
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
    void chickenHit(World *world, Entity *enemy_collision)
    {
        changePostprocessing(3);

        chicken_hit_sound.play();
        enemy_collision->localTransform.scale = glm::vec3(0, 0, 0);
        enemy_collision->deleteComponent<CollisionComponent>();
        world->markForRemoval(enemy_collision);
        if (!shieldEnabled)
            lives--;
        else
        {
            shield->localTransform.scale = glm::vec3(0, 0, 0);
            shieldEnabled = false;
        }
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
            std::string str;
            getline(file_in, str);
            file_in.close();
            int highScore = std::stoi(str);

            if (score > highScore)
                highScore = score;

            std::ofstream outfile;
            outfile.open("score.txt");
            outfile << highScore << '\n' << score;
            outfile.close();
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
            ImGui::SetWindowSize(ImVec2((float)550, (float)200));
            ImGui::SetWindowPos(ImVec2(app->getWindowSize().x - 200.0f, 0.0f)); // Set position to top right
            std::string score__and_level =
                "Score: " + std::to_string((int)score) +
                "\nLevel: " + std::to_string((int)currentLevel + 1); // Set score ImGui::SetWindowFontScale(1.0f);
            ImGui::TextUnformatted(score__and_level.c_str());
            ImGui::End();
        }
    }
    void changePostprocessing(int shaderIndex)
    {
        postProcessingCurrentCounter = 0;
        applyingPostProcess = true;
        forwardRenderer->changePostprocessShader(shaderIndex);
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
