#pragma once

#include <application.hpp>
#include <asset-loader.hpp>
#include <ecs/world.hpp>
#include <imgui-utils/utils.hpp>
#include <systems/entity-renderer.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/light.hpp>
#include <systems/movement.hpp>
#include <systems/player.hpp>

#define HEARTS_GEN_SPEED 1000
#define MONKEY_GEN_SPEED 800
#define CHICKENS_GEN_SPEED 30
// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::EntityRenderer entityRenderer;               // this is the entity renderer that renders all random objects in space
    our::FreeCameraControllerSystem cameraController; // this is the camera controller that controls the camera and keyboard inputs
    our::MovementSystem movementSystem;
    our::PlayerSystem playerSystem; // this is the player system that controls the player logic (tracks score and health)
    our::LightSystem lightSystem;
    int counter = 0;
    int heartsCounter = 0;
    int monkeysCounter = 0;
    void onInitialize() override
    {
        counter = 30;
        heartsCounter = 0;
        monkeysCounter = 0;
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }
        auto size = getApp()->getFrameBufferSize();
        // Then we initialize the renderer
        renderer.initialize(size, config["renderer"]);

        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        playerSystem.enter(&world, getApp(), &renderer);
        entityRenderer.intialization(&world);
    }

    void onDraw(double deltaTime) override
    {
        counter++;
        monkeysCounter++;
        heartsCounter++;
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        int bosses = playerSystem.update(&world, (float)deltaTime, entityRenderer.boss_exists());
        lightSystem.update(&world);

        // detele any entity that is out of the screen to save memory
        entityRenderer.clearOutOfBoundEntities();
        // monkeyRenderer.delete_monkey_light(&world); // this has a problem because the light is child to the monkey

        // generate hearts every 1000 frames
        if (heartsCounter >= HEARTS_GEN_SPEED)
        {
            entityRenderer.renderHearts();
            heartsCounter = 0;
        }
        // generate monkeys every 800 frames
        if (monkeysCounter >= MONKEY_GEN_SPEED)
        {
            entityRenderer.renderMonkeys();
            monkeysCounter = 0;
        }
        // generate chicken every 30 frames and if there is no boss
        if (counter >= CHICKENS_GEN_SPEED || bosses != 0)
        {
            entityRenderer.renderChicken(bosses);
            counter = 0;
        }

        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto &keyboard = getApp()->getKeyboard();

        if (keyboard.justPressed(GLFW_KEY_ESCAPE))
        {
            // If the escape  key is pressed in this frame, go to the MENU state
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override
    {
        // playerSystem.exit();
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
    void onImmediateGui() override
    {
        //= gets called in application.cpp every frame
        //= Here, we just draw the player system's gui
        playerSystem.imgui();
    }
};