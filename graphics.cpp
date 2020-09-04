#include "graphics.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <algorithm>

#include <iostream>

// SDL
static SDL_Window *window;
static SDL_Renderer *renderer;

// Window size
static int window_width = 720, window_height = 720;

// The graph that we are currently working on
static Graph *g;

// For moving nodes, in the case that the final position isn't valid,
// then prev_position_node is used to reset the position of the node.
static std::pair<int, int> prev_position_node;

// Node in which the mouse was over when clicked down.
// Note that can be nullptr, if the mouse wasn't over a node.
// Used to passed info from event SDL_MOUSEBUTTONDOWN to SDL_MOUSEBUTTONUP
static Node *clicked;

// 0 for nothing, 'l' for left, 'r' for right
static char button_pressed = 0;

// The set of selected nodes
static std::vector<Node *> selected;

// SELECTED SET

static inline void add_to_selected(Node *np)
{
    if (std::find(selected.begin(), selected.end(), np) == selected.end())
        selected.push_back(np);
}

// DRAWING

static inline void draw_background()
{
    SDL_SetRenderDrawColor(renderer, 230, 230, 240, 97);
    SDL_RenderClear(renderer);
}

static inline void draw_node(Node *np, int r, int g, int b, int a)
{
    filledCircleRGBA(renderer, np->x, np->y, np->r, r, g, b, a);
}

static inline void draw_arc(Node *f, Node *t, int r, int g, int b, int a)
{
    // Close point in the frontier of the node f to the center of the node t
    double vXf = t->x - f->x;
    double vYf = t->y - f->y;

    double magVf = sqrt(vXf * vXf + vYf * vYf);

    double pf_x = f->x + ((vXf / magVf) * f->r);
    double pf_y = f->y + ((vYf / magVf) * f->r);

    // Close point in the frontier of the node t to the center of the node f
    double vXt = f->x - t->x;
    double vYt = f->y - t->y;
    double magVt = sqrt(vXt * vXt + vYt * vYt);

    double pt_x = t->x + ((vXt / magVt) * t->r);
    double pt_y = t->y + ((vYt / magVt) * t->r);

    // Draw line
    lineRGBA(renderer, pf_x, pf_y, pt_x, pt_y, r, g, b, a);

    // TODO: Finish arrow shape
}

static inline void draw_graph()
{
    for (auto np : g->nodes)
    {
        draw_node(np, 108, 159, 206, 255);

        characterRGBA(renderer, np->x, np->y, np->l, 0, 0, 0, 255);

        for (auto t : np->adjacents)
            draw_arc(np, t, 108, 159, 206, 255);
    }
}

static inline void draw_selected()
{
    for (auto np : selected)
        filledCircleRGBA(renderer, np->x, np->y, np->r, 255, 219, 88, 200);
}

static inline void draw_all()
{
    draw_background();

    draw_graph();

    draw_selected();
}

static inline void refresh()
{
    SDL_RenderPresent(renderer);
}

bool valid_move(Node *np)
{
    for (auto p : g->nodes)
    {
        if (p != np)
        {
            int lb = (np->r - p->r) * (np->r - p->r);
            int ub = (np->r + p->r) * (np->r + p->r);
            int it = ((np->x - p->x) * (np->x - p->x)) + ((np->y - p->y) * (np->y - p->y));
            if (lb <= it && ub >= it)
                return false;
        }
    }
    return true;
}

// Interface functions

bool Graphics::init(Graph &graph)
{
    g = &graph;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    if (SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return false;
    }

    SDL_SetWindowTitle(window, "hparg");

    draw_all();

    refresh();

    return true;
}

void Graphics::activity()
{
    bool running = true;

    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                running = false;
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {

                int x, y;
                SDL_GetMouseState(&x, &y);

                clicked = g->get_node(x, y);

                // Left click
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    button_pressed = 'l';

                    // Clicked on a node
                    if (clicked)
                    {
                        prev_position_node.first = clicked->x;
                        prev_position_node.second = clicked->y;
                    }
                }
                // Right click
                else
                    button_pressed = 'r';

                break;
            }

            case SDL_MOUSEMOTION:
            {
                // Pressed the left button ...
                if (button_pressed == 'l')
                {
                    // ... on a node
                    if (clicked)
                    {
                        SDL_GetMouseState(&clicked->x, &clicked->y);

                        draw_all();

                        if (valid_move(clicked))
                            draw_node(clicked, 189, 236, 182, 255);
                        else
                            draw_node(clicked, 180, 50, 30, 255);

                        refresh();
                    }
                }

                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                // If the click was a left click...
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    // on a node...
                    if (clicked)
                    {
                        clicked->x = x;
                        clicked->y = y;

                        // See if the final position (after moving) is valid
                        // if not, reset initial position
                        if (!valid_move(clicked))
                        {
                            clicked->x = prev_position_node.first;
                            clicked->y = prev_position_node.second;
                        }
                    }
                    // on the background
                    else
                    {
                        // If there's no node selected, just make a new node where was clicked
                        if (selected.empty())
                            g->add_node(x, y, 40);
                        // Otherwise, clean the selected set
                        else
                            selected.clear();
                    }

                    draw_all();

                    refresh();
                }
                // If the click was a right click...
                else
                {
                    Node *up = g->get_node(x, y);

                    // ... and was a node
                    if (clicked)
                    {
                        // Released right-button on a node
                        if (up)
                        {
                            // Click node == Release node, then add it to the selected set
                            if (clicked == up)
                                add_to_selected(up);
                            // Click node != Release node, then add an arc from click to release node
                            else
                                g->add_arc(clicked, up);

                            draw_all();

                            refresh();
                        }
                        // Released right-button on the background (CARE can be an arc, latter on)
                        // do nothing
                    }
                    // If clicked on the background, do nothing. (Maybe a missclick)
                }

                // Reset
                clicked = nullptr;

                break;
            }

            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_BACKSPACE:
                {
                    if (!selected.empty())
                    {
                        for (auto np : selected)
                            g->remove_node(np);

                        selected.clear();

                        draw_all();

                        refresh();
                    }
                    break;
                }

                default:
                {
                    if (selected.size() == 1 && isalpha(event.key.keysym.sym))
                        g->set_label(selected[0], event.key.keysym.sym);

                    selected.clear();

                    draw_all();

                    refresh();

                    break;
                }
                }

                break;
            }

            case SDL_WINDOWEVENT:
            {
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    SDL_GetWindowSize(window, &window_width, &window_height);

                    draw_all();

                    refresh();

                    break;
                }
                }
                break;
            }
            }
        }
    }
}

void Graphics::close()
{
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();
}