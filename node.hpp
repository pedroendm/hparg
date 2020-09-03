#pragma once

#include <vector>

class Node {
    public:
    // Position and radius
    int x, y, r;

    // Label
    char l;

    // Adjacency nodes
    std::vector<Node*> adjacents;

    // Constructor
    Node(int x, int y, int r);

    // Add arc from this node to s
    void add_arc(Node* s);

    // Set the label of the node to 'l'
    void set_label(char l);
};