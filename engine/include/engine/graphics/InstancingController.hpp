//
// Created by bogi on 7/20/25.
//

#ifndef INSTANCINGCONTROLLER_HPP
#define INSTANCINGCONTROLLER_HPP
#include "engine/core/Controller.hpp"
#include "engine/resources/Model.hpp"
#include "glm/fwd.hpp"

namespace engine::graphics {
    class InstancingController final : public core::Controller {
    public:
        std::string_view name() const override;

        unsigned int amount;
        glm::mat4 *model_matrices;
        float radius;
        float offset;
        unsigned int buffer;

        void generate_model_transformation_matrices();

        void configure_instanced_array();

        void set_transformation_matrices(resources::Model *model);

        void configure_transformation_matrices(resources::Shader *shader);

        void draw_instances(const resources::Shader *shader, const resources::Model *model);
    }; // engine
}
#endif //INSTANCINGCONTROLLER_HPP
