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

    // Set the randomizer's seed.
    // TODO Make this unique each run.
    SetRandomSeed(1);

    // Set the trace log level to debug for testing.
    SetTraceLogLevel(LOG_DEBUG);

    // Set the target FPS to 120. 
    SetTargetFPS(120);

    InitWindow(0, 0, scn_title);

    // Load fonts.
    Font fnt_mono;
    fnt_mono = LoadFont("resources/fonts/JetBrainsMono-Regular.ttf");

    scene s;
    scene_create(&s, (Vector2) { GetMonitorWidth(0), GetMonitorHeight(0)} );

    const char *root_url = 
        "https://en.wikipedia.org/wiki/C_(programming_language)";
    scene_load(&s, root_url);

    // Enter render loop.
    while(!WindowShouldClose()) { 

        // Update the scene, i.e, move things.
        scene_update(&s, GetFrameTime());

        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw the scene to the window.
            scene_render(&s);
        EndDrawing();
    }

    UnloadFont(fnt_mono);
    CloseWindow();

    scene_destroy(&s);
    return 0;
}
