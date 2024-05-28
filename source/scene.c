/// @file scene.c
/// @brief This file contains implementations related to rendering wikipedia 
/// pages as nodes in a graph.
///
/// @author Ben Potter
/// @date 27 May 2024

#include <stdlib.h>

#include "scene.h"


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
    s->nodes[0].radius = 100;

    for(int i = 0; i < root->size_links; ++i) {
        page *child = root->links[i]; 

        // Position this child node.
        // Use i + 1 since the root node takes one space (i = 0).
        s->nodes[i + 1].page = child;
        s->nodes[i + 1].position = 
            (Vector2) { GetRandomValue(0, scn_width), GetRandomValue(0, scn_height) };
        s->nodes[i + 1].radius = 50;
    }
    
    return true;
}


bool scene_update(scene *s, float dt) {

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
            WHITE
        );
    }

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

