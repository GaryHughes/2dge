#include "game.hpp"

int main()
{
    dge::game game;

    game.initialise();
    game.run();
    game.destroy();
    
    return 0;
}