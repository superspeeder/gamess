
#include <iostream>
#include "game/game.hpp"


int main() {
    using namespace game;
    glfwInit();
    {
        Game game;
        game.run();
    }
    glfwTerminate();
    return 0;
}
