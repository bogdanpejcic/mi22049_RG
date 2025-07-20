//
// Created by bogi on 7/20/25.
//

#ifndef INSTANCINGCONTROLLER_HPP
#define INSTANCINGCONTROLLER_HPP
#include "engine/core/Controller.hpp"

namespace engine::graphics {
    class InstancingController final : public core::Controller {
    public:
        std::string_view name() const override;

    private:
        void initialize() override;
    };
} // engine

#endif //INSTANCINGCONTROLLER_HPP
