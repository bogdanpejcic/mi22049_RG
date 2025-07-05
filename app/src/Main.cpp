#include <engine/core/Engine.hpp>

#include "MyApp.hpp"

/**
 * Start here...
test commit......
 */
int main(int argc, char **argv) {
    auto app = std::make_unique<app::MyApp>();
    return app->run(argc, argv);
}
