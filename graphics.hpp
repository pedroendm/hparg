#pragma once

#include "graph.hpp"

class Graphics {
    public:
        static bool init(Graph& graph);
        static void activity();
        static void close();
};