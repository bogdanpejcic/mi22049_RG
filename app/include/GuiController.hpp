//
// Created by bogi on 7/12/25.
//

#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
    class GuiController : public engine::core::Controller {
        void initialize() override;

        std::string_view name() const override;

        void poll_events() override;

        void draw() override;
    };
} // app

#endif //GUICONTROLLER_HPP
