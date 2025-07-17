//
// Created by bogi on 7/12/25.
//

#include "../include/GuiController.hpp"

#include "imgui.h"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/platform/PlatformController.hpp"

namespace app {
    void GuiController::initialize() {
        set_enable(false);
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

        ImGui::Begin("Camera info");
        ImGui::Text("Camera position: (%f, %f, %f)", camera->Position.x, camera->Position.y, camera->Position.z);

        ImGui::End();;

        graphics->end_gui();
    }
} // app
