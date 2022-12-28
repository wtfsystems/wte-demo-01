############################################################
#
#  WTEngine Demo CMake
#
#  See LICENSE.md for copyright information.
#
#  This builds the engine example.
#
############################################################

########################################
#
#  Project Configuration
#
########################################
#  Configure build project
cmake_minimum_required(VERSION 3.11)
project(wte_demo VERSION 1.0 DESCRIPTION "WTEngine Demo Game")
enable_language(CXX)

#  If a build type is not set, set it to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#  Set compiler flags
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

########################################
#
#  Library Detection
#
########################################
#  Check for PhysFS
find_library(PHYSFS_LIB NAMES physfs libphysfs)
if(NOT PHYSFS_LIB)
    message("Error! PhysFS not found!  Aborting.")
    return()
endif()

#  pkg-config checks
find_package(PkgConfig REQUIRED)
    #  Check for Allegro and its required modules
    pkg_check_modules(ALLEGRO REQUIRED IMPORTED_TARGET allegro-5)
    pkg_check_modules(ALLEGRO_MAIN REQUIRED IMPORTED_TARGET allegro_main-5)
    pkg_check_modules(ALLEGRO_PHYSFS REQUIRED IMPORTED_TARGET allegro_physfs-5)
    pkg_check_modules(ALLEGRO_AUDIO REQUIRED IMPORTED_TARGET allegro_audio-5)
    pkg_check_modules(ALLEGRO_ACODEC REQUIRED IMPORTED_TARGET allegro_acodec-5)
    pkg_check_modules(ALLEGRO_FONT REQUIRED IMPORTED_TARGET allegro_font-5)
    pkg_check_modules(ALLEGRO_IMAGE REQUIRED IMPORTED_TARGET allegro_image-5)
    pkg_check_modules(ALLEGRO_PRIMV REQUIRED IMPORTED_TARGET allegro_primitives-5)
    #  Check for WTEngine
    pkg_check_modules(WTENGINE REQUIRED IMPORTED_TARGET wtengine)

#  Check for OpenGL
find_package(OpenGL REQUIRED)

########################################
#
#  Configure Sources
#
########################################
add_executable(wte_demo
    src/main.cpp)

target_compile_options(wte_demo PRIVATE)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(wte_demo PRIVATE "-DWTE_BUILD_DEBUG")
endif()

#  Set up flags for building with Emscripten
if(EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

########################################
#
#  Build Process
#
########################################
#  Set include folder for building
target_include_directories(wte_demo PRIVATE include
    ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

#  Link libraries
target_link_libraries(wte_demo PUBLIC
    ${PHYSFS_LIB}
    PkgConfig::WTENGINE
    PkgConfig::ALLEGRO
    PkgConfig::ALLEGRO_MAIN
    PkgConfig::ALLEGRO_PHYSFS
    PkgConfig::ALLEGRO_AUDIO
    PkgConfig::ALLEGRO_ACODEC
    PkgConfig::ALLEGRO_FONT
    PkgConfig::ALLEGRO_IMAGE
    PkgConfig::ALLEGRO_PRIMV
    ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

#  Set the build target
set_target_properties(wte_demo PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(wte_demo PROPERTIES LINK_SEARCH_END_STATIC 1)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set_target_properties(wte_demo PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED True)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

#  No install for the demo - Done!