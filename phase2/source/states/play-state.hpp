#pragma once

#include <application.hpp>
#include <asset-loader.hpp>
#include <ecs/world.hpp>
#include <imgui-utils/utils.hpp>
#include <systems/chicken-renderer.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/hearts-renderer.hpp>
#include <systems/light.hpp>
#include <systems/monkeys-renderer.hpp>
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
    our::ChickenRenderer chickenRenderer;
    our::MonkeyRenderer monkeyRenderer;
    our::HeartRenderer heartRenderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::PlayerSystem playerSystem;
    our::LightSystem lightSystem;
    int counter = 0;
    int heartsCounter = 0;
    int monkeysCounter = 0;
    void onInitialize() override
    {
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

        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        chickenRenderer.intialization();
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
        playerSystem.enter(&world, getApp(), &renderer);
    }

    void onDraw(double deltaTime) override
    {
        counter++;
        monkeysCounter++;
        heartsCounter++;
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        int bosses = playerSystem.update(&world, (float)deltaTime, chickenRenderer.boss_exists(&world));
        lightSystem.update(&world);

        // detele any entity that is out of the screen to save memory
        chickenRenderer.clearOutOfBoundEntities(&world);
        // monkeyRenderer.delete_monkey_light(&world); // this has a problem because the light is child to the monkey

        // generate hearts every 700 frames
        if (heartsCounter >= HEARTS_GEN_SPEED)
        {
            heartRenderer.rendering(&world);
            heartsCounter = 0;
        }
        // generate monkeys every 600 frames
        if (monkeysCounter >= MONKEY_GEN_SPEED)
        {
            monkeyRenderer.rendering(&world);
            monkeysCounter = 0;
        }
        // generate chicken every 30 frames
        if (counter >= CHICKENS_GEN_SPEED || bosses != 0)
        {

            chickenRenderer.rendering(&world, bosses);

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