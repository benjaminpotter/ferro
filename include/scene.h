/// @file scene.h
/// @brief This file contains definitions related to rendering wikipedia pages
/// as nodes in a graph.
///
/// @author Ben Potter
/// @date 27 May 2024

#pragma once

#include "raylib.h"
#include "page.h"


/// @struct page_node
/// @brief Stores rendering information about a wikipedia page.
typedef struct page_node {
    
    /// @brief Reference to the page data for this node.
    page *page;

    /// @brief Position of this node in world space.
    Vector2 position;

    /// @brief Radius of this node in world space.
    float radius;

} page_node;


/// @struct scene
/// @brief Stores all the state about a scene.
typedef struct scene {

    /// @brief The size of the scene in world space.
    Vector2 scene_size;

    /// @brief The number of nodes in the scene.
    size_t size_nodes;

    /// @brief A list of all nodes in the scene.
    page_node *nodes;

} scene;


/// @brief Initializes all members of a new scene.
///
/// @param s A reference to the scene to be initialized.
/// @param scene_size The size of the scene in world space.
/// @return True on success, otherwise false.
bool scene_create(scene *s, const Vector2 scene_size);


/// @brief Loads a new scene starting at a root URL.
///
/// @param s A reference to the scene to be loaded.
/// @param root_url A URL to load a page for the root node.
/// @return True on success, otherwise false.
bool scene_load(scene *s, const char *root_url);


/// @brief Steps the scene simulation forward by one time interval.
///
/// @param s A reference to the scene to be updated.
/// @param dt The time since the last call.
/// @return True on success, otherwise false.
bool scene_update(scene *s, float dt);


/// @brief Renders the scene.
///
/// @param s A reference to the scene to be rendered.
/// @return True on success, otherwise false.
bool scene_render(const scene *s);


/// @brief Destroys a scene structure.
///
/// @param s A reference to the scene to be destroyed.
/// @return True on success, otherwise false.
bool scene_destroy(scene *s);

