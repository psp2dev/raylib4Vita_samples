/*******************************************************************************************
*
*   raylib example - procedural mesh generation
*
*   This example has been created using raylib 1.8 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2017 Ramon Santamaria (Ray San)
*	Copyright (c) 2020 Antonio Jose Ramos Marquez (@psxdev) adapt sample to liborbis
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



bool l1flag=false;
bool r1flag=false;

#define NUM_MODELS  8      // Parametric 3d shapes to generate

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
  
  if (enter_button == SCE_SYSTEM_PARAM_ENTER_BUTTON_ASSIGN_CIRCLE) {
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


void printModel(Model mod)
{
  TraceLog(LOG_INFO,"Model 0");
  TraceLog(LOG_INFO,"Matrix transform");
  TraceLog(LOG_INFO,"%.2f %.2f %.2f %.2f",mod.transform.m0,mod.transform.m4,mod.transform.m8,mod.transform.m12);
  TraceLog(LOG_INFO,"%.2f %.2f %.2f %.2f",mod.transform.m1,mod.transform.m5,mod.transform.m9,mod.transform.m13);
  TraceLog(LOG_INFO,"%.2f %.2f %.2f %.2f",mod.transform.m2,mod.transform.m6,mod.transform.m10,mod.transform.m14);
  TraceLog(LOG_INFO,"%.2f %.2f %.2f %.2f",mod.transform.m3,mod.transform.m7,mod.transform.m11,mod.transform.m15);
  TraceLog(LOG_INFO,"meshCount=%d",mod.meshCount);
  TraceLog(LOG_INFO,"materialCount=%d",mod.materialCount);
  TraceLog(LOG_INFO,"Mesh meshes");
  TraceLog(LOG_INFO,"vertexCount=%d",mod.meshes->vertexCount);
  TraceLog(LOG_INFO,"triangleCount=%d",mod.meshes->triangleCount);
  int i=0;
  int flagcolor=0;
  int flagtex2=0;
  int flagtangent=0;
  for(i=0;i<mod.meshes->vertexCount;i++)
  {
      TraceLog(LOG_INFO,"vertices[%d]= %.2f %.2f %.2f",i,mod.meshes->vertices[3*i],mod.meshes->vertices[3*i+1],mod.meshes->vertices[3*i+2]);
      TraceLog(LOG_INFO,"texcoords[%d]= %.2f %.2f",i,mod.meshes->texcoords[2*i],mod.meshes->texcoords[2*i+1]);
      if(mod.meshes->texcoords2!=NULL && !flagtex2)
      {
        TraceLog(LOG_INFO,"texcoords2[%d]= %.2f %.2f",i,mod.meshes->texcoords2[2*i],mod.meshes->texcoords2[2*i+1]);
      }
      else
      {
        flagtex2=1;
        TraceLog(LOG_INFO,"no texcoords2");
      }
      TraceLog(LOG_INFO,"normals[%d]= %.2f %.2f %.2f",i,mod.meshes->normals[3*i],mod.meshes->normals[3*i+1],mod.meshes->normals[3*i+2]);
      if(mod.meshes->tangents!=NULL && !flagtangent)
      {
        flagtangent=1;
        TraceLog(LOG_INFO,"tangents[%d]= %.2f %.2f %.2f %.2f",i,mod.meshes->tangents[4*i],mod.meshes->tangents[4*i+1],mod.meshes->tangents[4*i+2],mod.meshes->tangents[4*i+3]);
      }
      else
      {
        TraceLog(LOG_INFO,"no tangents");
      }
      if(mod.meshes->colors!=NULL && !flagcolor)
      {
        TraceLog(LOG_INFO,"colors[%d]= %d %d %d %d",i,mod.meshes->colors[4*i],mod.meshes->colors[4*i+1],mod.meshes->colors[4*i+2],mod.meshes->colors[4*i+3]);
      }
      else
      { flagcolor=1;
        TraceLog(LOG_INFO,"no colors");
      }
      sleep(1);
  }
  if(mod.meshes->indices!=NULL)
  {
    i=0;
    for(i=0;i<mod.meshes->triangleCount;i++)
    {
      sleep(1);

      TraceLog(LOG_INFO,"indices[%d]= %d %d %d",i,mod.meshes->indices[3*i],mod.meshes->indices[3*i+1],mod.meshes->indices[3*i+2]);
     
    }
  }
  else
  {
    TraceLog(LOG_INFO,"no indices");

  }
  TraceLog(LOG_INFO,"vboId=%d %d %d %d %d %d %d",mod.meshes->vboId[0],mod.meshes->vboId[1],mod.meshes->vboId[2],mod.meshes->vboId[3],mod.meshes->vboId[4],mod.meshes->vboId[5],mod.meshes->vboId[6]);
  TraceLog(LOG_INFO,"Material materials");
  TraceLog(LOG_INFO,"shader.id=%d",(mod.materials)->shader.id);
  TraceLog(LOG_INFO,"meshMaterial=%d",*(mod.meshMaterial));





}
Mesh GenMeshPlaneAux(float width, float length, int resX, int resZ);
    Model models[NUM_MODELS] = { 0 };


int main(int argc, char *argv[])
{
	
	

	//init liborbis libraries
	pibInit(PIB_SHACCCG |  PIB_GET_PROC_ADDR_CORE | PIB_ENABLE_MSAA);

    //initEGL();
    debugNetInit("192.168.1.47",18194,3);
    sleep(1);
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
		
	InitWindow(screenWidth, screenHeight,"raylib [models] example - mesh generation");
	
	// We generate a checked image for texturing
   Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);

  //    Image checked = GenImageColor(2, 2, RED);

    //Texture2D texture = LoadTexture("ux0:/data/assets/wabbit_alpha.png");
Texture2D texture =LoadTextureFromImage(checked);
    UnloadImage(checked);

  //Mesh myMesh=GenMeshCube(1, 1, 1);
 //models[0] = LoadModelFromMesh(myMesh);
//models[0].materials[0].maps[MAP_DIFFUSE].texture = texture;

   // printModel(models[0]);
       models[0] = LoadModelFromMesh(GenMeshPlane(2, 2, 5, 5));

    models[1] = LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 2.0f));
    models[2] = LoadModelFromMesh(GenMeshSphere(2, 32, 32));
    models[3] = LoadModelFromMesh(GenMeshHemiSphere(2, 16, 16));
    models[4] = LoadModelFromMesh(GenMeshCylinder(1, 2, 16));
    models[5] = LoadModelFromMesh(GenMeshTorus(0.25f, 4.0f, 16, 32));
    models[6] = LoadModelFromMesh(GenMeshKnot(1.0f, 2.0f, 16, 128));
    models[7] = LoadModelFromMesh(GenMeshPoly(5, 2.0f));

    // Set checked texture as default diffuse component for all models material
   for (int i = 0; i < NUM_MODELS; i++) models[i].materials[0].maps[MAP_DIFFUSE].texture = texture;

 /*Camera camera = { 0 };
    camera.position = (Vector3){ 5.0f, 5.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.type = CAMERA_PERSPECTIVE;
    // Model drawing position
    Vector3 position = { 0.0f, 0.0f, 0.0f };

    int currentModel = 0;*/

    // Define the camera to look into our 3d world
        Camera camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    //Camera camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
//Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    // Model drawing position
    Vector3 position = { 0.0f, 0.0f, 0.0f };

    int currentModel = 0;

             SetCameraMode(camera, CAMERA_ORBITAL);  // Set a orbital camera mode*/

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (1)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
     UpdateCamera(&camera);      // Update internal camera and our camera
    	readPad();
    	/*if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            currentModel = (currentModel + 1)%NUM_MODELS; // Cycle between the textures
        }*/
        if(pressed_pad[PAD_CROSS])
        {
             SetCameraMode(camera, CAMERA_ORBITAL);  // Set a orbital camera mode*/

        }
        if(pressed_pad[PAD_CIRCLE])
        {
             SetCameraMode(camera, CAMERA_PERSPECTIVE);  // Set a orbital camera mode*/
            
        }
        if (pressed_pad[PAD_RIGHT])
        {
            currentModel++;
            if (currentModel >= NUM_MODELS) currentModel = 0;
        }
        else if (pressed_pad[PAD_LEFT])
        {
            currentModel--;
            if (currentModel < 0) currentModel = NUM_MODELS - 1;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

           BeginMode3D(camera);

                //DrawCubeTexture(GetTextureDefault(),(Vector3){7.0f, 0.0f, 0.0f},1.0f,1.0f,1.0f,RED);
                DrawModel(models[currentModel], position, 1.0f, WHITE);
               // DrawSphere((Vector3){0.0f, 0.0f, 0.0f}, 1.0f, GREEN);
                DrawGrid(10, 1.0);

            EndMode3D();

            DrawRectangle(30, screenHeight-50, 310, 30, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(30, screenHeight-50, 310, 30, Fade(DARKBLUE, 0.5f));
            DrawText("MOUSE LEFT BUTTON to CYCLE PROCEDURAL MODELS", 40, screenHeight-50+10, 10, BLUE);

            switch(currentModel)
            {
                case 0: DrawText("PLANE", screenWidth-140, 10, 20, DARKBLUE); break;
                case 1: DrawText("CUBE", screenWidth-140, 10, 20, DARKBLUE); break;
                case 2: DrawText("SPHERE", screenWidth-140, 10, 20, DARKBLUE); break;
                case 3: DrawText("HEMISPHERE", screenWidth-140, 10, 20, DARKBLUE); break;
                case 4: DrawText("CYLINDER", screenWidth-140, 10, 20, DARKBLUE); break;
                case 5: DrawText("TORUS", screenWidth-140, 10, 20, DARKBLUE); break;
                case 6: DrawText("KNOT", screenWidth-140, 10, 20, DARKBLUE); break;
                case 7: DrawText("POLY", screenWidth-140, 10, 20, DARKBLUE); break;
                default: break;
            }
		DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
   // UnloadTexture(texture); // Unload texture

    // Unload models data (GPU VRAM)
	sceKernelExitProcess(0);
    return 0;
}
