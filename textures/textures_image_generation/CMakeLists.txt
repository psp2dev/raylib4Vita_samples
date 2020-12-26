cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{DOLCESDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{DOLCESDK}/share/dolce.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define DOLCESDK to point to your SDK path!")
  endif()
endif()

project(raylib_textures_image_generation)
include("${DOLCESDK}/share/dolce.cmake" REQUIRED)

set(DOLCE_APP_NAME "RAYLIB 6")
set(DOLCE_TITLEID  "RAYL00006")
set(DOLCE_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVITA -Wall -fno-lto -fno-pic -fno-plt -DRLGL_IMPLEMENTATION -DGRAPHICS_API_OPENGL_ES2 -DSUPPORT_MESH_GENERATION -DPLATFORM_VITA")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(DOLCE_MKSFOEX_FLAGS "${DOLCE_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

add_executable(${PROJECT_NAME}
  main.c
)

target_link_libraries(${PROJECT_NAME}
  raylib
  pib
  SceLibKernel_stub
  SceThreadmgr_stub
  SceModulemgr_stub
  SceSysmodule_stub
  SceIofilemgr_stub
  SceNetCtl_stub 
  SceNet_stub
  SceGxm_stub
  SceDisplay_stub
  SceCtrl_stub
  SceTouch_stub
  pthread
  m
)

set(DOLCE_ELF_CREATE_FLAGS "${DOLCE_ELF_CREATE_FLAGS} -h 2194304")

dolce_create_self(eboot.bin ${PROJECT_NAME} UNSAFE NOSTRIP)

dolce_create_vpk(${PROJECT_NAME}.vpk ${DOLCE_TITLEID} eboot.bin
  VERSION ${DOLCE_VERSION}
  NAME ${DOLCE_APP_NAME}
)
