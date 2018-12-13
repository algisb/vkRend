cmake_minimum_required (VERSION 3.1.0)

project (vkRend)
set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "./bin")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "./bin")

#vulkan----------------------------------------------------
#find_package(Vulkan)
#include_directories( ${OPENGL_INCLUDE_DIRS} )


#glfw------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")

include_directories ("./external/glfw/include")
add_subdirectory("./external/glfw")
add_definitions(-DGLFW_STATIC)


#main-----------------------------------------------
include_directories ("./src")
file(GLOB MAIN_SRC "./src/*.cpp")

add_executable(vkRend ${MAIN_SRC} )
target_link_libraries (vkRend glfw)
