//
// Created by bogi on 7/5/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
    class MainController : public engine::core::Controller {
        void initialize() override;

    public:
        std::string_view name() const override {
            return "app::MainController";
        };

    private:
        void begin_draw() override; //happens before draw

        void draw_airplane();

        void update_camera();

        void draw() override;

        void update() override;

        void end_draw() override; //happens after draw

        bool loop() override;
    };
} // app

#endif //MAINCONTROLLER_HPP
