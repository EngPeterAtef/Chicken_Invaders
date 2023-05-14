#pragma once
#include <application.hpp>
#include <array>
#include <functional>
#include <imgui-utils/utils.hpp>
#include <material/material.hpp>
#include <mesh/mesh.hpp>
#include <shader/shader.hpp>
#include <texture/texture-utils.hpp>
#include <texture/texture2d.hpp>
// include sound inside systems
#include "systems/sound.hpp"
// This state shows how to use some of the abstractions we created to make a menu.
class Menustate : public our::State
{
    bool fontLoaded = false;
    bool isMute = false;
    Sound background_sound = Sound("assets/sounds/intro.mp3", true);
    void onInitialize() override
    {
        // Theme Source: https://github.com/ocornut/imgui/issues/707
        // ---------------INIT SOUND----------------

        background_sound.changeVolume(30);
        background_sound.play();
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
        ImGui::Begin("Main Menu", open_ptr, window_flags);
        ImGui::SetWindowSize(ImVec2((float)550, (float)400));
        ImGui::SetWindowPos(ImVec2(400.0f, 150.0f));

        // set title
        ImGui::SetWindowFontScale(3.0f);
        std::string message = "Welcome to Chicken Invaders!\n\n";
        ImGui::TextUnformatted(message.c_str());

        // set  Main menu title
        ImGui::SetWindowFontScale(3.5f);
        message = "Main Menu\n";
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Main Menu").x - 30) / 2.0f);
        ImGui::TextUnformatted(message.c_str());

        ImGui::SetWindowFontScale(1.0f);
        message = "\n";
        ImGui::TextUnformatted(message.c_str());

        // set New Game button
        ImGui::SetWindowFontScale(2.0f);
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("New Game").x - 30) / 2.0f);
        if (ImGui::Button("New Game"))
        {
            background_sound.stop();
            getApp()->changeState("play");
        }
        if (isMute)
        {
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Unmute").x - 30) / 2.0f);
            if (ImGui::Button("Unmute"))
            {
                background_sound.changeVolume(30);
                isMute = false;
            }
        }
        else
        {
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Mute").x - 30) / 2.0f);
            if (ImGui::Button("Mute"))
            {
                background_sound.changeVolume(0);
                isMute = true;
            }
        }

        // set Exit button
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Exit").x - 30) / 2.0f);
        if (ImGui::Button("Exit"))
        {
            glfwSetWindowShouldClose(getApp()->getWindow(), GLFW_TRUE);
        }

        ImGui::End();
    }
};
