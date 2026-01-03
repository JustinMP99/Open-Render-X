
cmake_minimum_required(VERSION 3.10.0)
project(OpenRenderX VERSION 0.1.0 LANGUAGES C CXX)

# Find Packages
if(APPLE)
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)
endif()

# Add Source Files
add_executable(
        OpenRenderX
Content/Source/glad.c
Content/Source/Engine.cpp
Content/Source/Graphics.cpp
Content/Source/Vertex.cpp
Content/main.cpp
)

# Set Path Variables
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Additional/include/glad")
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Additional/include/GLFW")
set(KHR_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Additional/include/KHR")
set(GLM_INCLUDE_DIR "Content/Additional/include/glm")
#set(FRAG_DIR "${CMAKE_SOURCE_DIR}/Source/Shaders/Fragment Shaders")

# set(IMGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Additional/include/imgui")
set(GLFW_LIBRARY "${CMAKE_SOURCE_DIR}/Additional/Library/glfw3.lib")
# set(STB_IMAGE_DIR "${PROJECT_SOURCE_DIR}/Additional/include/stb_image")
# set(IMAGE_DIR "${PROJECT_SOURCE_DIR}/Additional/Images")

# Include Directories
target_include_directories(OpenRenderX PRIVATE
    "${CMAKE_SOURCE_DIR}/Content/Additional/include"
)

target_link_libraries(OpenRenderX PRIVATE glfw OpenGL::GL)