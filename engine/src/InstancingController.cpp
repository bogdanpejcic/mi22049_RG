//
// Created by bogi on 7/20/25.
//
#include <random>
#include <cmath>
#include <engine/graphics/InstancingController.hpp>

#include "engine/graphics/GraphicsController.hpp"
#include "engine/resources/Shader.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"
#include "spdlog/spdlog.h"

namespace engine {
    std::string_view graphics::InstancingController::name() const {
        return "InstancingController";
    }

    inline float random_float() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(gen);
    }

    void graphics::InstancingController::generate_model_transformation_matrices() {
        amount = 1000;
        model_matrices = new glm::mat4[amount];
        srand(static_cast<unsigned int>(glfwGetTime()));

        float spread = 80.0f;
        float depth = 150.0f;
        float height = 20.0f;

        for (unsigned int i = 0; i < amount; i++) {
            glm::mat4 model = glm::mat4(1.0f);

            // Ellipsoidal distribution
            float x = (random_float() - 0.5f) * spread; // left-right
            float y = (random_float() - 0.5f) * height; // up-down
            float z = -random_float() * depth; // forward

            model = glm::translate(model, glm::vec3(x, y, z));

            // Random scale
            float scale = 0.08f + random_float() * 0.10f; // 0.08 - 0.18
            model = glm::scale(model, glm::vec3(scale));

            // Random rotation
            float rotAngle = random_float() * 360.0f;
            model = glm::rotate(model, glm::radians(rotAngle), glm::vec3(0.1f, 1.0f, 0.1f));

            model_matrices[i] = model;
        }
    }

    void graphics::InstancingController::configure_instanced_array() {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &model_matrices[0], GL_STATIC_DRAW);
    }

    void graphics::InstancingController::set_transformation_matrices(resources::Model *model) {
        for (unsigned int i = 0; i < model->meshes().size(); i++) {
            unsigned int VAO = model->meshes()[i].vao();
            glBindVertexArray(VAO);
            // set attribute pointers for matrix (4 times vec4)
            glEnableVertexAttribArray(8);
            glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) 0);
            glEnableVertexAttribArray(9);
            glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) (sizeof(glm::vec4)));
            glEnableVertexAttribArray(10);
            glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) (2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(11);
            glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) (3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(8, 1);
            glVertexAttribDivisor(9, 1);
            glVertexAttribDivisor(10, 1);
            glVertexAttribDivisor(11, 1);

            glBindVertexArray(0);
        }
    }

    void graphics::InstancingController::draw_instances(const resources::Model *model) {
        for (unsigned int i = 0; i < model->meshes().size(); i++) {
            auto &mesh = model->meshes()[i];

            // bind mesh texture if available
            if (!mesh.textures().empty()) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mesh.textures()[0]->id()); // safer
            }

            glBindVertexArray(mesh.vao());
            glDrawElementsInstanced(GL_TRIANGLES, mesh.index_count(), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }
    }
} // engine
