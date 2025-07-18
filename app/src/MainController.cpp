//
// Created by bogi on 7/5/25.
//

#include "../include/MainController.hpp"
#include "GuiController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bundled/compile.h"

namespace app {
    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
    public:
        void on_mouse_move(engine::platform::MousePosition position) override;
    };

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto gui_controller = engine::core::Controller::get<GuiController>();
        if (!gui_controller->is_enabled()) {
            auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
            camera->rotate_camera(position.dx, position.dy);
        }
    }

    bool day;

    void MainController::initialize() {
        spdlog::info("MainController initialized");
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();
        platform->set_enable_cursor(false); //cursor disappears
        day = true;
    }

    bool MainController::get_day() {
        return day;
    }

    void MainController::begin_draw() {
        //happens before draw
        //clear buffers (color, depth, stencil)
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::day_and_night(engine::resources::Shader *shader,
                                       engine::graphics::GraphicsController *graphics) {
        shader->use();
        shader->set_int("material.diffuse", 0);
        shader->set_int("material.specular", 1);

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.9f, -3.0f));
        model = glm::scale(model, glm::vec3(0.3f));
        shader->set_mat4("model", model);

        shader->set_vec3("viewPos", graphics->camera()->Position);
    }

    void MainController::draw_airplane() {
        //Model
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
        engine::resources::Model *airplane = resources->model("Airplane"); //taken from congig.json
        if (day) {
            //Shader
            engine::resources::Shader *shaderDir = resources->shader("directional_light");

            day_and_night(shaderDir, graphics);

            shaderDir->set_vec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

            // light properties
            shaderDir->set_vec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            shaderDir->set_vec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            shaderDir->set_vec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

            // material properties
            shaderDir->set_float("material.shininess", 32.0f);

            airplane->draw(shaderDir);
        } else {
            //Shader
            engine::resources::Shader *shaderSpot = resources->shader("spot_light");

            day_and_night(shaderSpot, graphics);

            shaderSpot->set_vec3("light.position", graphics->camera()->Position);
            shaderSpot->set_vec3("light.direction", graphics->camera()->Front);
            shaderSpot->set_float("light.cutOff", glm::cos(glm::radians(7.5f)));
            shaderSpot->set_float("light.outerCutOff", glm::cos(glm::radians(17.5f)));

            // light properties
            shaderSpot->set_vec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
            // each environment and lighting type requires some tweaking to get the best out of your environment.
            shaderSpot->set_vec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shaderSpot->set_vec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shaderSpot->set_float("light.constant", 1.0f);
            shaderSpot->set_float("light.linear", 0.09f);
            shaderSpot->set_float("light.quadratic", 0.032f);

            // material properties
            shaderSpot->set_float("material.shininess", 32.0f);

            airplane->draw(shaderSpot);
        }
    }

    void MainController::update_camera() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();
        float dt = platform->dt();
        if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_RIGHT).is_down()) {
            camera->rotate_camera(10.0f, 0.0f, true);
        }
        if (platform->key(engine::platform::KeyId::KEY_LEFT).is_down()) {
            camera->rotate_camera(-10.0f, 0.0f, true);
        }
        if (platform->key(engine::platform::KeyId::KEY_UP).is_down()) {
            camera->rotate_camera(0.0f, 10.0f, true);
        }
        if (platform->key(engine::platform::KeyId::KEY_DOWN).is_down()) {
            camera->rotate_camera(-0.0f, -10.0f, true);
        }
    }

    void MainController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_N).state() == engine::platform::Key::State::JustPressed) {
            day = !day;
        }
    }

    void MainController::draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox = resources->skybox("day");
        if (day) {
            skybox = resources->skybox("day");
        } else {
            skybox = resources->skybox("night");
        }
        auto shader = resources->shader("skybox");
        auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader, skybox);
    }

    void MainController::draw() {
        draw_airplane();
        draw_skybox();
    }

    void MainController::update() {
        update_camera();
    }

    void MainController::end_draw() {
        //happens after draw
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers(); //send frame to the screen
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }
} // app
