/*******************************************************************************************
*
*   raylib [shapes] example - Draw raylib logo using basic shapes
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pib.h>
#include <psp2/kernel/processmgr.h>
#include <raylib.h>
#include <psp2/gxm.h>
#include <psp2/ctrl.h>
#include <psp2/system_param.h>
extern uint32_t sceLibcHeapSize = 2194304;
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 960;
    const int screenHeight = 544;
    pibInit(PIB_SHACCCG );
    debugNetInit("192.168.1.47",18194,3);
    sleep(1);

    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - raylib logo using shapes");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (1)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawRectangle(screenWidth/2 - 128, screenHeight/2 - 128, 256, 256, BLACK);
            DrawRectangle(screenWidth/2 - 112, screenHeight/2 - 112, 224, 224, RAYWHITE);
            DrawText("raylib", screenWidth/2 - 44, screenHeight/2 + 48, 50, BLACK);

            DrawText("this is NOT a texture!", screenWidth/2 - 50, 464, 10, GRAY);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    sceKernelExitProcess(0);
    return 0;
}
