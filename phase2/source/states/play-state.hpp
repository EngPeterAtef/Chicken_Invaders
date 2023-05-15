#pragma once

#include <application.hpp>
#include <asset-loader.hpp>
#include <ecs/world.hpp>
#include <imgui-utils/utils.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/light.hpp>
#include <systems/movement.hpp>
#include <systems/player.hpp>
#include <systems/chicken-renderer.hpp>
// #include <systems/chicken-renderer.hpp>

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::ChickenRenderer chickenRenderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::PlayerSystem playerSystem;
    our::LightSystem lightSystem;
    int counter = 0;

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
        playerSystem.enter(&world, getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override
    {
        counter++;
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        playerSystem.update(&world, (float)deltaTime);

        lightSystem.update(&world);
        // And finally we use the renderer system to draw the scene
        chickenRenderer.delete_chickens(&world);
        if (counter >= 30)
        {
            chickenRenderer.rendering(&world);
            counter = 0;
        }
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto &keyboard = getApp()->getKeyboard();

        if (keyboard.justPressed(GLFW_KEY_ESCAPE))
        {
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override
    {
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
    { //= gets called in application.cpp every frame
        //= Here, we just draw the camera controller system's gui
        playerSystem.imgui();
    }
};