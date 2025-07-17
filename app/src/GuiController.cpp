//
// Created by bogi on 7/12/25.
//

#include "../include/GuiController.hpp"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "MainController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"

namespace app {
    ImFont *g_LargeFont = nullptr;

    void GuiController::initialize() {
        set_enable(false);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ///ImGui::SetNextWindowSize(ImVec2(x, y));
        ImGuiIO &io = ImGui::GetIO();

        // Load font
        g_LargeFont = io.Fonts->AddFontFromFileTTF("resources/fonts/JetBrainsMono/JetBrainsMonoNL-SemiBoldItalic.ttf",
                                                   26.0f);
        if (!g_LargeFont) {
            engine::util::EngineError::type_string(engine::util::EngineError::Type::FileNotFound);
        }
        ImGui_ImplOpenGL3_CreateFontsTexture();
    }

    std::string_view GuiController::name() const {
        return "app::GUIController";
    }

    void GuiController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_F2).state() == engine::platform::Key::State::JustPressed) {
            set_enable(!is_enabled());
            platform->set_enable_cursor(is_enabled()); //cursor appears or dissapears
        }
    }

    void GuiController::draw() {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();
        graphics->begin_gui();
        ImGui::Begin("Info");
        if (g_LargeFont)
            ImGui::PushFont(g_LargeFont);

        ImGui::Begin("Camera info");
        ImGui::Text("Camera position: (%f, %f, %f)", camera->Position.x, camera->Position.y, camera->Position.z);
        ImGui::Text("(Yaw, Pitch): (%f, %f)", camera->Yaw, camera->Pitch);
        ImGui::Text("Camera front: (%f, %f, %f)", camera->Front.x, camera->Front.y, camera->Front.z);

        auto airplane = engine::core::Controller::get<engine::resources::ResourcesController>()->model("Airplane");
        engine::core::Controller::get<engine::resources::ResourcesController>()->skybox("day");
        ImGui::Text("Airplane model loaded from: %s", airplane->path().c_str());

        if (app::MainController::get_day()) {
            ImGui::Text("Day skybox loaded from: resources/skyboxes/day");
            ImGui::Text("It is daytime. Press F3 to change to nighttime.");
        } else {
            ImGui::Text("Night skybox loaded from: resources/skyboxes/night");
            ImGui::Text("It is nighttime. Press F3 to change to daytime.");
        }
        if (g_LargeFont)
            ImGui::PopFont();
        ImGui::End();
        graphics->end_gui();
    }
} // app
