#include "graph.hpp"
#include "graphics.hpp"

int main(void)
{

    Graph g;

    Graphics::setup(&g);

    while (Graphics::activity());

    Graphics::close();

    return 0;
}

