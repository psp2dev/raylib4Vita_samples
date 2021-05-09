/*******************************************************************************************
*
*   raylib [shapes] example - collision area
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2019 Ramon Santamaria (@raysan5)
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
bool flag=true;
extern uint32_t sceLibcHeapSize = 2194304;
#define ALIGN_CENTER(a, b) (((a) - (b)) / 2)
#define ALIGN_RIGHT(x, w) ((x) - (w))

#define ANALOG_CENTER 128
#define ANALOG_THRESHOLD 64
#define ANALOG_SENSITIVITY 16
int enter_button=0;
enum PadButtons {
  PAD_UP,
  PAD_DOWN,
  PAD_LEFT,
  PAD_RIGHT,
  PAD_LTRIGGER,
  PAD_RTRIGGER,
  PAD_TRIANGLE,
  PAD_CIRCLE,
  PAD_CROSS,
  PAD_SQUARE,
  PAD_START,
  PAD_SELECT,
  PAD_ENTER,
  PAD_CANCEL,
  PAD_LEFT_ANALOG_UP,
  PAD_LEFT_ANALOG_DOWN,
  PAD_LEFT_ANALOG_LEFT,
  PAD_LEFT_ANALOG_RIGHT,
  PAD_RIGHT_ANALOG_UP,
  PAD_RIGHT_ANALOG_DOWN,
  PAD_RIGHT_ANALOG_LEFT,
  PAD_RIGHT_ANALOG_RIGHT,
  PAD_N_BUTTONS
};

typedef uint8_t Pad[PAD_N_BUTTONS];

SceCtrlData pad;
Pad old_pad, current_pad, pressed_pad, released_pad, hold_pad, hold2_pad;
Pad hold_count, hold2_count;





void readPad() {
  memset(&pad, 0, sizeof(SceCtrlData));
  sceCtrlPeekBufferPositive(0, &pad, 1);

  memcpy(&old_pad, current_pad, sizeof(Pad));
  memset(&current_pad, 0, sizeof(Pad));

  if (pad.buttons & SCE_CTRL_UP)
    current_pad[PAD_UP] = 1;
  if (pad.buttons & SCE_CTRL_DOWN)
    current_pad[PAD_DOWN] = 1;
  if (pad.buttons & SCE_CTRL_LEFT)
    current_pad[PAD_LEFT] = 1;
  if (pad.buttons & SCE_CTRL_RIGHT)
    current_pad[PAD_RIGHT] = 1;
  if (pad.buttons & SCE_CTRL_LTRIGGER)
    current_pad[PAD_LTRIGGER] = 1;
  if (pad.buttons & SCE_CTRL_RTRIGGER)
    current_pad[PAD_RTRIGGER] = 1;
  if (pad.buttons & SCE_CTRL_TRIANGLE)
    current_pad[PAD_TRIANGLE] = 1;
  if (pad.buttons & SCE_CTRL_CIRCLE)
    current_pad[PAD_CIRCLE] = 1;
  if (pad.buttons & SCE_CTRL_CROSS)
    current_pad[PAD_CROSS] = 1;
  if (pad.buttons & SCE_CTRL_SQUARE)
    current_pad[PAD_SQUARE] = 1;
  if (pad.buttons & SCE_CTRL_START)
    current_pad[PAD_START] = 1;
  if (pad.buttons & SCE_CTRL_SELECT)
    current_pad[PAD_SELECT] = 1;

  if (pad.ly < ANALOG_CENTER - ANALOG_THRESHOLD) {
    current_pad[PAD_LEFT_ANALOG_UP] = 1;
  } else if (pad.ly > ANALOG_CENTER + ANALOG_THRESHOLD) {
    current_pad[PAD_LEFT_ANALOG_DOWN] = 1;
  }

  if (pad.lx < ANALOG_CENTER - ANALOG_THRESHOLD) {
    current_pad[PAD_LEFT_ANALOG_LEFT] = 1;
  } else if (pad.lx > ANALOG_CENTER + ANALOG_THRESHOLD) {
    current_pad[PAD_LEFT_ANALOG_RIGHT] = 1;
  }

  if (pad.ry < ANALOG_CENTER - ANALOG_THRESHOLD) {
    current_pad[PAD_RIGHT_ANALOG_UP] = 1;
  } else if (pad.ry > ANALOG_CENTER + ANALOG_THRESHOLD) {
    current_pad[PAD_RIGHT_ANALOG_DOWN] = 1;
  }

  if (pad.rx < ANALOG_CENTER - ANALOG_THRESHOLD) {
    current_pad[PAD_RIGHT_ANALOG_LEFT] = 1;
  } else if (pad.rx > ANALOG_CENTER + ANALOG_THRESHOLD) {
    current_pad[PAD_RIGHT_ANALOG_RIGHT] = 1;
  }

  int i;
  for (i = 0; i < PAD_N_BUTTONS; i++) {
    pressed_pad[i] = current_pad[i] & ~old_pad[i];
    released_pad[i] = ~current_pad[i] & old_pad[i];

    hold_pad[i] = pressed_pad[i];
    hold2_pad[i] = pressed_pad[i];

    if (current_pad[i]) {
      if (hold_count[i] >= 10) {
        hold_pad[i] = 1;
        hold_count[i] = 6;
      }

      if (hold2_count[i] >= 10) {
        hold2_pad[i] = 1;
        hold2_count[i] = 10;
      }

      hold_count[i]++;
      hold2_count[i]++;
    } else {
      hold_count[i] = 0;
      hold2_count[i] = 0;
    }
  }

  if (enter_button == SCE_SYSTEM_PARAM_ENTER_BUTTON_CIRCLE) {
    old_pad[PAD_ENTER] = old_pad[PAD_CIRCLE];
    current_pad[PAD_ENTER] = current_pad[PAD_CIRCLE];
    pressed_pad[PAD_ENTER] = pressed_pad[PAD_CIRCLE];
    released_pad[PAD_ENTER] = released_pad[PAD_CIRCLE];
    hold_pad[PAD_ENTER] = hold_pad[PAD_CIRCLE];
    hold2_pad[PAD_ENTER] = hold2_pad[PAD_CIRCLE];

    old_pad[PAD_CANCEL] = old_pad[PAD_CROSS];
    current_pad[PAD_CANCEL] = current_pad[PAD_CROSS];
    pressed_pad[PAD_CANCEL] = pressed_pad[PAD_CROSS];
    released_pad[PAD_CANCEL] = released_pad[PAD_CROSS];
    hold_pad[PAD_CANCEL] = hold_pad[PAD_CROSS];
    hold2_pad[PAD_CANCEL] = hold2_pad[PAD_CROSS];
  } else {
    old_pad[PAD_ENTER] = old_pad[PAD_CROSS];
    current_pad[PAD_ENTER] = current_pad[PAD_CROSS];
    pressed_pad[PAD_ENTER] = pressed_pad[PAD_CROSS];
    released_pad[PAD_ENTER] = released_pad[PAD_CROSS];
    hold_pad[PAD_ENTER] = hold_pad[PAD_CROSS];
    hold2_pad[PAD_ENTER] = hold2_pad[PAD_CROSS];

    old_pad[PAD_CANCEL] = old_pad[PAD_CIRCLE];
    current_pad[PAD_CANCEL] = current_pad[PAD_CIRCLE];
    pressed_pad[PAD_CANCEL] = pressed_pad[PAD_CIRCLE];
    released_pad[PAD_CANCEL] = released_pad[PAD_CIRCLE];
    hold_pad[PAD_CANCEL] = hold_pad[PAD_CIRCLE];
    hold2_pad[PAD_CANCEL] = hold2_pad[PAD_CIRCLE];
  }
}

int holdButtons(SceCtrlData *pad, uint32_t buttons, uint64_t time) {
  if ((pad->buttons & buttons) == buttons) {
    uint64_t time_start = sceKernelGetProcessTimeWide();

    while ((pad->buttons & buttons) == buttons) {
      sceKernelDelayThread(10 * 1000);
      sceCtrlPeekBufferPositive(0, pad, 1);

      if ((sceKernelGetProcessTimeWide() - time_start) >= time) {
        return 1;
      }
    }
  }

  return 0;
}



const int screenWidth = 960;
const int screenHeight = 544;

int main(void)
{
    // Initialization
    //---------------------------------------------------------
    pibInit(PIB_SHACCCG|PIB_GET_PROC_ADDR_CORE);

    debugNetInit("192.168.1.47",18194,3);
    sleep(1);
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - collision area");

    // Box A: Moving box
    Rectangle boxA = { 10, GetScreenHeight()/2 - 50, 200, 100 };
    int boxASpeedX = 4;
    int xflag=0;
    // Box B: Mouse moved box
    Rectangle boxB = { GetScreenWidth()/2 - 30, GetScreenHeight()/2 - 30, 60, 60 };
    Vector2 playerPosition = {GetScreenWidth()/2 - 30, GetScreenHeight()/2 - 30};
    Rectangle boxCollision = { 0 }; // Collision rectangle

    int screenUpperLimit = 40;      // Top menu limits

    bool pause = false;             // Movement pause
    bool collision = false;         // Collision detection

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //----------------------------------------------------------

    // Main game loop
    while (1)    // Detect window close button or ESC key
    {
	readPad();
	if (pressed_pad[PAD_RIGHT] || hold_pad[PAD_RIGHT]) playerPosition.x += 10.0f;
        else if (pressed_pad[PAD_LEFT] || hold_pad[PAD_LEFT]) playerPosition.x -= 10.0f;
        else if (pressed_pad[PAD_DOWN] || hold_pad[PAD_DOWN]) playerPosition.y += 10.0f;
        else if (pressed_pad[PAD_UP] || hold_pad[PAD_UP]) playerPosition.y -= 10.0f;
        else if (pressed_pad[PAD_CROSS]) xflag=1;
	else xflag=0;

        // Update
        //-----------------------------------------------------
        // Move box if not paused
        if (!pause) boxA.x += boxASpeedX;

        // Bounce box on x screen limits
        if (((boxA.x + boxA.width) >= GetScreenWidth()) || (boxA.x <= 0)) boxASpeedX *= -1;

        // Update player-controlled-box (box02)
        boxB.x = playerPosition.x - boxB.width/2;
        boxB.y = playerPosition.y - boxB.height/2;

        // Make sure Box B does not go out of move area limits
        if ((boxB.x + boxB.width) >= GetScreenWidth()) boxB.x = GetScreenWidth() - boxB.width;
        else if (boxB.x <= 0) boxB.x = 0;

        if ((boxB.y + boxB.height) >= GetScreenHeight()) boxB.y = GetScreenHeight() - boxB.height;
        else if (boxB.y <= screenUpperLimit) boxB.y = screenUpperLimit;

        // Check boxes collision
        collision = CheckCollisionRecs(boxA, boxB);

        // Get collision rectangle (only on collision)
        if (collision) boxCollision = GetCollisionRec(boxA, boxB);

        // Pause Box A movement
        if (xflag) pause = !pause;
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawRectangle(0, 0, screenWidth, screenUpperLimit, collision? RED : BLACK);

            DrawRectangleRec(boxA, GOLD);
            DrawRectangleRec(boxB, BLUE);

            if (collision)
            {
                // Draw collision area
                DrawRectangleRec(boxCollision, LIME);

                // Draw collision message
                DrawText("COLLISION!", GetScreenWidth()/2 - MeasureText("COLLISION!", 20)/2, screenUpperLimit/2 - 10, 20, BLACK);

                // Draw collision area
                DrawText(TextFormat("Collision Area: %i", (int)boxCollision.width*(int)boxCollision.height), GetScreenWidth()/2 - 100, screenUpperLimit + 10, 20, BLACK);
            }

            DrawFPS(10, 10);

        EndDrawing();
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //----------------------------------------------------------
        sceKernelExitProcess(0);
    return 0;
}
