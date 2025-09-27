#include <engine/core/Engine.hpp>

#include "MyApp.hpp"

/**
 * Start here...
test commit......
 */
int main(int argc, char **argv) {
    return std::make_unique<app::MyApp>()->run(argc, argv);
}
