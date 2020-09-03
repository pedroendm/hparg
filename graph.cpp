#include "graph.hpp"

#include <algorithm>

bool Graph::no_intersection(Node* np)
{
    for(auto n : nodes) {
        int lb = (n->r - np->r) * (n->r - np->r);
        int ub = (n->r + np->r) * (n->r + np->r);
        int it = ((n->x - np->x) * (n->x - np->x)) + ((n->y - np->y) * (n->y - np->y));
        if(lb <= it && ub >= it)
            return false;
    }
    return true;
}

bool Graph::add_node(int x, int y, int d)
{
    Node* np = new Node(x, y, d);
    if(no_intersection(np)) {
        nodes.push_back(np);
        return true;
    } 
    delete np;
    return false;
}

Node* Graph::get_node(int x, int y)
{
    for(auto np : nodes) {
        int sq_dist = ((np->x - x) * (np->x - x)) + ((np->y - y) * (np->y - y));
        if(sq_dist <= np->r * np->r)
            return np;
    }
    return nullptr;
}

void Graph::remove_node(Node* np)
{
    // Remove the links directed to the node
    for(int i = 0; i < nodes.size(); i++) {
        if(nodes[i] != np) {
            auto r = std::find(nodes[i]->adjacents.begin(), nodes[i]->adjacents.end(), np);
            if(r != nodes[i]->adjacents.end())
                nodes[i]->adjacents.erase(r);
        }
    }

    // Remove the node (and, implicitly, the links starting from the node)
    nodes.erase(std::find(nodes.begin(), nodes.end(), np));
}

void Graph::add_arc(Node* f, Node* s)
{
    if(std::find(f->adjacents.begin(), f->adjacents.end(), s) == f->adjacents.end())
       f->add_arc(s);
}

void Graph::set_label(Node* np, char l)
{
    np->set_label(l);
}
