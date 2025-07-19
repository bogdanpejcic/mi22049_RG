//
// Created by bogi on 7/5/25.
//
#include "../include/MyApp.hpp"
#include <spdlog/spdlog.h>

#include "GuiController.hpp"
#include "MainController.hpp"

namespace app {
    void MyApp::app_setup() {
        auto main_controller = register_controller<app::MainController>();
        auto gui_controller = register_controller<app::GuiController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        main_controller->before(gui_controller);
        spdlog::info("App setup completed!");
    }
} // app
