/// @file main.c
/// @brief This file contains the application entry point.
///
/// @author Ben Potter
/// @date 19 May 2024

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#include "scene.h"


int main(int argc, char* argv[]) {
    
    const char* scn_title = "ferro";
    const int scn_width = 1920;
    const int scn_height = 1080;


    // Set the randomizer's seed.
    // TODO Make this unique each run.
    SetRandomSeed(1);

    // Set the trace log level to debug for testing.
    SetTraceLogLevel(LOG_DEBUG);

    scene s;
    scene_create(&s, (Vector2) { scn_width, scn_height} );

    const char *root_url = 
        "https://en.wikipedia.org/wiki/C_(programming_language)";
    scene_load(&s, root_url);

    InitWindow(scn_width, scn_height, scn_title);

    // Enter render loop.
    while(!WindowShouldClose()) { 

        // Update the scene, i.e, move things.
        scene_update(&s, GetFrameTime());

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(scn_width - 100, 0);
            DrawText("Welcome to Ferro", 0, 0, 40, BLACK);

            // Draw the scene to the window.
            scene_render(&s);
        EndDrawing();
    }

    CloseWindow();

    scene_destroy(&s);
    return 0;
}
