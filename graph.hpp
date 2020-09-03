#pragma once

#include <vector>
#include "node.hpp"

class Graph {
    private:
        // Used to check if the new node intersects with others.
        bool no_intersection(Node* np);
        
    public:
        // Set of nodes of the graph
        std::vector<Node*> nodes;

        // Add a node to the graph
        bool add_node(int x, int y, int d);
        
        // Return the node in the position (x,y), or nullptr if there isn't a node. 
        Node* get_node(int x, int y);

        // Remove a node from the graph.
        void remove_node(Node* np);

        // Add a arc to the graph from the pointed from f to s.
        void add_arc(Node* f, Node* s);

        // Set the label of node pointed by np to label.
        void set_label(Node* np, char label);
};