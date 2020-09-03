#include "node.hpp"

#include <algorithm>

Node::Node(int x, int y, int r) 
    : x{x}, y{y}, r{r}, l{0}
{}

void Node::add_arc(Node* s)
{
    if(std::find(adjacents.begin(), adjacents.end(), s) == adjacents.end())
        adjacents.push_back(s);
}

void Node::set_label(char l)
{
    this->l = l;
}
