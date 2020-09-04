#include "graph.hpp"
#include "graphics.hpp"

int main(void)
{

    Graph g;

    if(!Graphics::init(g))
        return 1;

    Graphics::activity();

    Graphics::close();

    return 0;
}

