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
    our::TexturedMaterial *menuMaterial;
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
        // First, we create a material for the menu's background
        menuMaterial = new our::TexturedMaterial();
        // Here, we load the shader that will be used to draw the background
        menuMaterial->shader = new our::ShaderProgram();
        menuMaterial->shader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
        menuMaterial->shader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
        menuMaterial->shader->link();
        // Then we load the menu texture
        menuMaterial->texture = our::texture_utils::loadImage("assets/textures/game_over.png");
        // Initially, the menu material will be black, then it will fade in
        menuMaterial->tint = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        if (!fontLoaded)
        {
            ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/Ruda-Bold.ttf", 17.0f);
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
        ImTextureID my_tex_id = (void *)(menuMaterial->texture); // use your own texture identifier here
        // ImGui::Image(my_tex_id, ImVec2(200, 200));
        ImVec2 window_pos = ImGui::GetWindowPos();
        ImVec2 window_size = ImGui::GetWindowSize();
        ImGui::GetBackgroundDrawList()->AddImage(my_tex_id, ImVec2(window_pos.x, window_pos.y),
                                                 ImVec2(window_pos.x + window_size.x, window_pos.y + window_size.y));
        ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetCursorPosY(300.0f);

        // set  Main menu title
        ImGui::SetWindowFontScale(2.0f);
        std::string message = "Game Over\n";
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Game Over").x - 10) / 2.0f);
        ImGui::TextUnformatted(message.c_str());

        // display score
        ImGui::SetWindowFontScale(0.8f);
        message = "Score : " + std::to_string(score) + "\nHighscore : " + std::to_string(highScore) + "\n";
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Game Over").x - 30) / 2.0f);
        ImGui::TextUnformatted(message.c_str());

        ImGui::SetWindowFontScale(1.2f);
        message = "\n";
        ImGui::TextUnformatted(message.c_str());

        // set New Game button
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Restart").x - 30) / 2.0f);
        if (ImGui::Button("Restart"))
        {
            game_over_sound.stop();
            getApp()->changeState("play");
        }
        // set Back to main menu
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Main Menu").x - 30) / 2.0f);
        if (ImGui::Button("Main Menu"))
        {
            game_over_sound.stop();
            getApp()->changeState("menu");
        }
        // set Exit button
        message = "\n";
        ImGui::TextUnformatted(message.c_str());
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Exit").x - 30) / 2.0f);
        if (ImGui::Button("Exit"))
        {
            game_over_sound.stop();
            glfwSetWindowShouldClose(getApp()->getWindow(), GLFW_TRUE);
        }
        ImGui::End();
    }
};