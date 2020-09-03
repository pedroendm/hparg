#pragma once

#include "graph.hpp"

class Graphics {
    public:
        static void setup(Graph* graph);
        static bool activity();
        static void close();
};