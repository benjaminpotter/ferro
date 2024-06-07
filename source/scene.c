/// @file scene.c
/// @brief This file contains implementations related to rendering wikipedia 
/// pages as nodes in a graph.
///
/// @author Ben Potter
/// @date 27 May 2024

#include <stdlib.h>

#include "common.h"
#include "scene.h"

#define DEBUG 1


bool scene_create(scene *s, const Vector2 scene_size) { 

    s->scene_size = scene_size;

    s->size_nodes = 0;
    s->nodes = NULL;

    return true;
}


bool scene_load(scene *s, const char *root_url) {

    page *root = (page*) malloc(sizeof(page));

    page_create(root, root_url);
    page_load(root);
    page_load_children(root);

    const int scn_width = s->scene_size.x;
    const int scn_height = s->scene_size.y;

    // We require a page node for the root and all of its links.
    s->size_nodes = root->size_links + 1;

    // Allocate memory for the page nodes.
    s->nodes = (page_node*) malloc(sizeof(page_node) * s->size_nodes);

    // Temporarily simulate different nodes.
    // TODO Position nodes algorithmically.
    s->nodes[0].page = root;
    s->nodes[0].position = (Vector2) { scn_width / 2, scn_height / 2 };
    s->nodes[0].radius = 200;

    for(int i = 0; i < root->size_links; ++i) {
        page *child = root->links[i]; 

        // Position this child node.
        // Use i + 1 since the root node takes one space (i = 0).
        s->nodes[i + 1].page = child;
        s->nodes[i + 1].position = 
            (Vector2) { GetRandomValue(0, scn_width), GetRandomValue(0, scn_height) };
        s->nodes[i + 1].radius = 150;
    }
    
    return true;
}


bool scene_update(scene *s, float dt) {

    return true;
}


bool scene_render_debug_hints(const scene *s) {

    // The "root" position of the debug menu.
    Vector2 anchor = (Vector2) { 33, 100 };

    float fnt_size_h1 = 50;
    float fnt_size_p = 40;

    float col_width = 250;

    DrawText("Debug Menu", anchor.x, anchor.y, fnt_size_h1, GREEN);
    anchor.y += fnt_size_h1;

    DrawText(
        TextFormat("Ferro %s", FERRO_VER_STR), 
        anchor.x, anchor.y, 
        fnt_size_p, GREEN
    );
    anchor.y += fnt_size_p;

    // Skip a line.
    anchor.y += fnt_size_p;

    // Time since last frame.
    DrawText(
        "frame time",
        anchor.x, anchor.y, 
        fnt_size_p, GREEN
    );
    DrawText(
        TextFormat("%02.02f ms", GetFrameTime() * 1000), 
        anchor.x + col_width, anchor.y, 
        fnt_size_p, GREEN
    );
    anchor.y += fnt_size_p;

    // Number of nodes.
    DrawText(
        "node count",
        anchor.x, anchor.y, 
        fnt_size_p, GREEN
    );
    DrawText(
        TextFormat("%d", s->size_nodes), 
        anchor.x + col_width, anchor.y, 
        fnt_size_p, GREEN
    );
    anchor.y += fnt_size_p;

    // Root node title.
    DrawText(
        "root node",
        anchor.x, anchor.y, 
        fnt_size_p, GREEN
    );
    DrawText(
        TextFormat("%s", s->nodes[0].page->title), 
        anchor.x + col_width, anchor.y, 
        fnt_size_p, GREEN
    );
    anchor.y += fnt_size_p;

    return true;
}


bool scene_render(const scene *s) {
    for(int i = 0; i < s->size_nodes; ++i) {
        DrawCircle(
            s->nodes[i].position.x, 
            s->nodes[i].position.y,
            s->nodes[i].radius,
            BLACK
        );

        int title_width = 
            MeasureText(
                s->nodes[i].page->title, 
                s->nodes[i].radius * 0.25
            );

        // TODO Render text better.
        DrawText(
            s->nodes[i].page->title,
            s->nodes[i].position.x - (title_width / 2), 
            s->nodes[i].position.y,
            s->nodes[i].radius * 0.25,
            RED
        );
    }

    if(DEBUG)
        scene_render_debug_hints(s);

    return true;
}


bool scene_destroy(scene *s) {
    if(s == NULL)
        return false;

    // Deallocate root page.
    page_destroy(s->nodes[0].page);

    // Deallocate node list.
    free(s->nodes);

    return true;
}

