cmake_minimum_required (VERSION 3.1.0)

project (vkRend)
set (CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "./bin")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "./bin")

#vulkan---------------------------------------------
#find_package(Vulkan)
#include_directories( ${OPENGL_INCLUDE_DIRS} )
include_directories ("./external/Vulkan-Loader/external/Vulkan-Headers/build/install/include")
add_subdirectory("./external/Vulkan-Loader")
#GLM------------------------------------------------
include_directories ("./external/glm")

#SDL2-----------------------------------------------
add_subdirectory("./external/SDL2")


#main-----------------------------------------------
include_directories ("./src")
file(GLOB MAIN_SRC "./src/*.cpp")

add_executable(vkRend ${MAIN_SRC} )
target_link_libraries (vkRend SDL2-static vulkan)
