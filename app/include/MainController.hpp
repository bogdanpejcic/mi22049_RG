//
// Created by bogi on 7/5/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

#include "engine/graphics/GraphicsController.hpp"
#include "engine/resources/Shader.hpp"

namespace app {
    class MainController : public engine::core::Controller {
        void initialize() override;

    public:
        std::string_view name() const override {
            return "app::MainController";
        }

        static bool get_is_panorama_active();

        static bool get_day();;

    private:
        void begin_draw() override; //happens before draw

        void day_and_night(engine::resources::Shader *shader, engine::graphics::GraphicsController *graphics);

        void draw_airplane();

        void update_camera();

        void poll_events() override;

        void draw_skybox();

        void draw_bird_instances();

        void draw() override;

        void update() override;

        void end_draw() override; //happens after draw

        bool loop() override;

        engine::graphics::Camera *get_camera();

        glm::vec3 m_panorama_start_pos;
        glm::vec3 m_panorama_target_pos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_model_target = glm::vec3(0.0f, -0.9f, -3.0f);
    };
} // app

#endif //MAINCONTROLLER_HPP
