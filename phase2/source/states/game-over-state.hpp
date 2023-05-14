#pragma once

#include "menu-state.hpp"
#include <application.hpp>
#include <functional>
#include <imgui-utils/utils.hpp>
#include <material/material.hpp>
#include <mesh/mesh.hpp>
#include <shader/shader.hpp>
#include <texture/texture-utils.hpp>
#include <texture/texture2d.hpp>

// This state shows how to use some of the abstractions we created to make a menu.
class GameOverState : public our::State
{
    bool fontLoaded = true;
    int highScore = 0;
    int score = 0;

    Sound game_over_sound = Sound("assets/sounds/game_over.mp3", false);
    void onInitialize() override
    {
        game_over_sound.play();
        std::ifstream file_in("score.txt");
        if (!file_in)
        {
            std::cerr << "Couldn't open file: "
                      << "score.txt" << std::endl;
        }
        // Read the file into a json object then close the file
        std::string str;
        getline(file_in, str);
        highScore = std::stoi(str);
        getline(file_in, str);
        score = std::stoi(str);
        file_in.close();
        // Theme Source: https://github.com/ocornut/imgui/issues/707

        if (!fontLoaded)
        {
            ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/Ruda-Bold.ttf", 14.0f);
            ImGui::GetIO().Fonts->Build();
            fontLoaded = true;
        }
        auto &style = ImGui::GetStyle();
        style.FrameRounding = 4.0f;
        style.WindowBorderSize = 0.0f;
        style.PopupBorderSize = 0.0f;
        style.GrabRounding = 4.0f;

        ImVec4 *colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.94f);

        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.67f);

        colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.49f, 0.80f, 0.67f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.49f, 0.80f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.49f, 0.80f, 0.67f);

        colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);

        colors[ImGuiCol_Button] = ImVec4(0.00f, 0.49f, 0.80f, 0.65f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.51f, 0.87f, 1.00f, 0.65f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);

        colors[ImGuiCol_Header] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.65f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);

        colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.49f, 0.80f, 0.54f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.51f, 0.87f, 1.00f, 0.66f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.51f, 0.87f, 1.00f, 0.66f);
    }

    void onDraw(double deltaTime,int speed,bool level_state) override
    {
        glClearColor(0.1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void onDestroy() override
    {
    }
    void onImmediateGui() override
    {

        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoScrollbar;
        bool *open_ptr = (bool *)true;
        ImGui::Begin("Game Over", open_ptr, window_flags);
        ImGui::SetWindowSize(ImVec2((float)550, (float)400));
        ImGui::SetWindowPos(ImVec2(400.0f, 250.0f));

        // set  Main menu title
        ImGui::SetWindowFontScale(3.5f);
        std::string message = "Game Over\n";
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Game Over").x - 10) / 2.0f);
        ImGui::TextUnformatted(message.c_str());

        // display score
        ImGui::SetWindowFontScale(1.5f);
        message = "Score : " + std::to_string(score) + "\nHighscore : " + std::to_string(highScore) + "\n";
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Game Over").x - 30) / 2.0f);
        ImGui::TextUnformatted(message.c_str());

        ImGui::SetWindowFontScale(1.0f);
        message = "\n";
        ImGui::TextUnformatted(message.c_str());

        // set New Game button
        ImGui::SetWindowFontScale(2.0f);
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Restart").x - 30) / 2.0f);
        if (ImGui::Button("Restart"))
        {
            game_over_sound.stop();
            getApp()->changeState("play");
        }
        // set Back to main menu
        ImGui::SetWindowFontScale(2.0f);
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Main Menu").x - 30) / 2.0f);
        if (ImGui::Button("Main Menu"))
        {
            game_over_sound.stop();
            getApp()->changeState("menu");
        }
        // set Exit button
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Exit").x - 30) / 2.0f);
        if (ImGui::Button("Exit"))
        {
            game_over_sound.stop();
            glfwSetWindowShouldClose(getApp()->getWindow(), GLFW_TRUE);
        }
        ImGui::End();
    }
    // // A meterial holding the menu shader and the menu texture to draw
    // our::TexturedMaterial *menuMaterial;
    // // A material to be used to highlight hovered buttons (we will use blending to create a negative effect).
    // our::TintedMaterial *highlightMaterial;
    // // A rectangle mesh on which the menu material will be drawn
    // our::Mesh *rectangle;
    // // A variable to record the time since the state is entered (it will be used for the fading effect).
    // float time;
    // // An array of the button that we can interact with
    // Button buttons;

    // void onInitialize() override
    // {
    //     // First, we create a material for the menu's background
    //     menuMaterial = new our::TexturedMaterial();
    //     // Here, we load the shader that will be used to draw the background
    //     menuMaterial->shader = new our::ShaderProgram();
    //     menuMaterial->shader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
    //     menuMaterial->shader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
    //     menuMaterial->shader->link();
    //     // Then we load the menu texture
    //     menuMaterial->texture = our::texture_utils::loadImage("assets/textures/game_over.jpeg");
    //     // Initially, the menu material will be black, then it will fade in
    //     menuMaterial->tint = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    //     // Second, we create a material to highlight the hovered buttons
    //     highlightMaterial = new our::TintedMaterial();
    //     // Since the highlight is not textured, we used the tinted material shaders
    //     highlightMaterial->shader = new our::ShaderProgram();
    //     highlightMaterial->shader->attach("assets/shaders/tinted.vert", GL_VERTEX_SHADER);
    //     highlightMaterial->shader->attach("assets/shaders/tinted.frag", GL_FRAGMENT_SHADER);
    //     highlightMaterial->shader->link();
    //     // The tint is white since we will subtract the background color from it to create a negative effect.
    //     highlightMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //     // To create a negative effect, we enable blending, set the equation to be subtract,
    //     // and set the factors to be one for both the source and the destination.
    //     highlightMaterial->pipelineState.blending.enabled = true;
    //     highlightMaterial->pipelineState.blending.equation = GL_FUNC_SUBTRACT;
    //     highlightMaterial->pipelineState.blending.sourceFactor = GL_ONE;
    //     highlightMaterial->pipelineState.blending.destinationFactor = GL_ONE;

    //     // Then we create a rectangle whose top-left corner is at the origin and its size is 1x1.
    //     // Note that the texture coordinates at the origin is (0.0, 1.0) since we will use the
    //     // projection matrix to make the origin at the the top-left corner of the screen.
    //     rectangle = new our::Mesh(
    //         {
    //             {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
    //             {{1.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
    //             {{1.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    //             {{0.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    //         },
    //         {
    //             0,
    //             1,
    //             2,
    //             2,
    //             3,
    //             0,
    //         });

    //     // Reset the time elapsed since the state is entered.
    //     time = 0;

    //     // Fill the positions, sizes and actions for the menu buttons
    //     // Note that we use lambda expressions to set the actions of the buttons.
    //     // A lambda expression consists of 3 parts:
    //     // - The capture list [] which is the variables that the lambda should remember because it will use them
    //     during
    //     // execution.
    //     //      We store [this] in the capture list since we will use it in the action.
    //     // - The argument list () which is the arguments that the lambda should receive when it is called.
    //     //      We leave it empty since button actions receive no input.
    //     // - The body {} which contains the code to be executed.
    //     buttons.position = {930.0f, 603.0f};
    //     buttons.size = {300.0f, 43.0f};
    //     buttons.action = [this]() { this->getApp()->changeState("menu"); };
    // }

    // void onDraw(double deltaTime) override
    // {
    //     // Get a reference to the keyboard object
    //     auto &keyboard = getApp()->getKeyboard();

    //     if (keyboard.justPressed(GLFW_KEY_SPACE))
    //     {
    //         // If the space key is pressed in this frame, go to the play state
    //         getApp()->changeState("menu");
    //     }
    //     else if (keyboard.justPressed(GLFW_KEY_ESCAPE))
    //     {
    //         // If the escape key is pressed in this frame, exit the game
    //         getApp()->close();
    //     }

    //     // Get a reference to the mouse object and get the current mouse position
    //     auto &mouse = getApp()->getMouse();
    //     glm::vec2 mousePosition = mouse.getMousePosition();

    //     // If the mouse left-button is just pressed, check if the mouse was inside
    //     // any menu button. If it was inside a menu button, run the action of the button.
    //     if (mouse.justPressed(0))
    //     {
    //         if (buttons.isInside(mousePosition))
    //             buttons.action();
    //     }

    //     // Get the framebuffer size to set the viewport and the create the projection matrix.
    //     glm::ivec2 size = getApp()->getFrameBufferSize();
    //     // Make sure the viewport covers the whole size of the framebuffer.
    //     glViewport(0, 0, size.x, size.y);

    //     // The view matrix is an identity (there is no camera that moves around).
    //     // The projection matrix applys an orthographic projection whose size is the framebuffer size in pixels
    //     // so that the we can define our object locations and sizes in pixels.
    //     // Note that the top is at 0.0 and the bottom is at the framebuffer height. This allows us to consider the
    //     // top-left corner of the window to be the origin which makes dealing with the mouse input easier.
    //     glm::mat4 VP = glm::ortho(0.0f, (float)size.x, (float)size.y, 0.0f, 1.0f, -1.0f);
    //     // The local to world (model) matrix of the background which is just a scaling matrix to make the menu cover
    //     the
    //     // whole window. Note that we defind the scale in pixels.
    //     glm::mat4 M = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

    //     // First, we apply the fading effect.
    //     time += (float)deltaTime;
    //     menuMaterial->tint = glm::vec4(glm::smoothstep(0.00f, 2.00f, time));
    //     // Then we render the menu background
    //     // Notice that I don't clear the screen first, since I assume that the menu rectangle will draw over the
    //     whole
    //     // window anyway.
    //     menuMaterial->setup();
    //     menuMaterial->shader->set("transform", VP * M);
    //     rectangle->draw();

    //     // For every button, check if the mouse is inside it. If the mouse is inside, we draw the highlight rectangle
    //     // over it.
    //     if (buttons.isInside(mousePosition))
    //     {
    //         highlightMaterial->setup();
    //         highlightMaterial->shader->set("transform", VP * buttons.getLocalToWorld());
    //         rectangle->draw();
    //     }
    // }

    // void onDestroy() override
    // {
    //     // Delete all the allocated resources
    //     delete rectangle;
    //     delete menuMaterial->texture;
    //     delete menuMaterial->shader;
    //     delete menuMaterial;
    //     delete highlightMaterial->shader;
    //     delete highlightMaterial;
    // }
};