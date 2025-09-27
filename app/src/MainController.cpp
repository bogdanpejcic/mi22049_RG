//
// Created by bogi on 7/5/25.
//

#include <MainController.hpp>
#include "GuiController.hpp"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/InstancingController.hpp"
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

    bool is_panorama_active = false;
    engine::platform::FrameTime panorama_start_time;
    float panorama_duration = 3.0f;

    bool MainController::get_is_panorama_active() {
        return is_panorama_active;
    }

    void MainController::initialize() {
        spdlog::info("MainController initialized");
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();
        platform->set_enable_cursor(false); //cursor disappears
        day = true;
        auto instances = engine::graphics::GraphicsController::get<engine::graphics::InstancingController>();

        instances->generate_model_transformation_matrices();

        instances->configure_instanced_array();
    }

    bool MainController::get_day() {
        return day;
    }

    void MainController::begin_draw() {
        //happens before draw
        //clear buffers (color, depth, stencil)
        engine::graphics::OpenGL::clear_buffers();
    }

    // Shared calls for both
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

        // material properties
        shader->set_float("material.shininess", 32.0f);
    }

    void MainController::draw_airplane() {
        //Model
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
        engine::resources::Model *airplane = resources->model("Airplane"); //taken from congig.json
        if (day) {
            //Shader
            engine::resources::Shader *shader_dir = resources->shader("directional_light");

            day_and_night(shader_dir, graphics);

            shader_dir->set_vec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

            // light properties
            shader_dir->set_vec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            shader_dir->set_vec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            shader_dir->set_vec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

            airplane->draw(shader_dir);
        } else {
            //Shader
            engine::resources::Shader *shader_spot = resources->shader("spot_light");

            day_and_night(shader_spot, graphics);

            shader_spot->set_vec3("light.position", graphics->camera()->Position);
            shader_spot->set_vec3("light.direction", graphics->camera()->Front);
            shader_spot->set_float("light.cutOff", glm::cos(glm::radians(7.5f)));
            shader_spot->set_float("light.outerCutOff", glm::cos(glm::radians(17.5f)));

            // light properties
            shader_spot->set_vec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
            // each environment and lighting type requires some tweaking to get the best out of your environment.
            shader_spot->set_vec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader_spot->set_vec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader_spot->set_float("light.constant", 1.0f);
            shader_spot->set_float("light.linear", 0.09f);
            shader_spot->set_float("light.quadratic", 0.032f);

            airplane->draw(shader_spot);
        }
    }

    void MainController::update_camera() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto camera = get_camera();
        float dt = platform->dt();
        if (!is_panorama_active) {
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
    }

    void MainController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_N).state() == engine::platform::Key::State::JustPressed) {
            day = !day;
        }
        if (platform->key(engine::platform::KEY_M).state() == engine::platform::Key::State::JustPressed) {
            auto camera = get_camera();
            is_panorama_active = true;
            panorama_start_time = engine::core::Controller::get<engine::platform::PlatformController>()->
                    frame_time();
            panorama_start_pos = camera->Position;
            engine::core::Controller::get<engine::platform::PlatformController>()->first_mouse = true;
        }
    }

    void MainController::draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        engine::resources::Skybox *skybox;
        if (day) {
            skybox = resources->skybox("day");
        } else {
            skybox = resources->skybox("night");
        }
        auto shader = resources->shader("skybox");
        auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader, skybox);
    }

    void MainController::draw_bird_instances() {
        auto instances = engine::graphics::GraphicsController::get<engine::graphics::InstancingController>();

        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        engine::resources::Model *bird = resources->model("bird");
        instances->set_transformation_matrices(bird);

        engine::resources::Shader *instancing_shader = resources->shader("instancing");
        auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
        instancing_shader->use();
        instancing_shader->set_mat4("projection", graphics->projection_matrix());
        instancing_shader->set_mat4("view", graphics->camera()->view_matrix());
        instancing_shader->set_int("texture_diffuse1", 0);
        instances->draw_instances(bird);
    }

    void MainController::draw() {
        draw_airplane();
        draw_skybox();
        draw_bird_instances();
    }

    void MainController::update() {
        update_camera();
        if (is_panorama_active) {
            engine::platform::FrameTime now = engine::core::Controller::get<engine::platform::PlatformController>()->
                    frame_time();
            float t = (now.current - panorama_start_time.current) / panorama_duration;
            auto camera = get_camera();
            if (t >= 1.0f) {
                camera->Position = panorama_target_pos;
                camera->Front = glm::normalize(model_target - camera->Position);
                camera->Right = glm::normalize(glm::cross(camera->Front, glm::vec3(0.0f, 1.0f, 0.0f)));
                camera->Up = glm::normalize(glm::cross(camera->Right, camera->Front));
                is_panorama_active = false;
                engine::core::Controller::get<engine::platform::PlatformController>()->first_mouse = true;
            } else {
                glm::vec3 newPos = glm::mix(panorama_start_pos, panorama_target_pos, t);
                camera->Position = newPos;
                camera->Front = glm::normalize(model_target - camera->Position);
                camera->Right = glm::normalize(glm::cross(camera->Front, glm::vec3(0.0f, 1.0f, 0.0f)));
                camera->Up = glm::normalize(glm::cross(camera->Right, camera->Front));
            }
        }
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

    engine::graphics::Camera *MainController::get_camera() {
        auto graphics = engine::graphics::GraphicsController::get<engine::graphics::GraphicsController>();
        return graphics->camera();
    }
} // app
