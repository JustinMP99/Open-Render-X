#Set cmake minimum required version
cmake_minimum_required(VERSION 3.10.0)

#set project name version and languages
project(OpenRenderX VERSION 0.1.0 LANGUAGES C CXX)

#Set cmake standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Packages
if(APPLE)
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)
elseif(WIN32)
    find_package(OpenGL REQUIRED)
endif()

# Add Source Files
add_executable(OpenRenderX
       Content/Source/glad.c
       Content/Source/Vertex.cpp
       Content/Source/Engine.cpp
       Content/Source/Graphics.cpp
       Content/main.cpp
       Content/Source/SceneObject.cpp
       Content/Source/Mesh.cpp
       Content/Source/Material.cpp
)

# Include Directories
target_include_directories(OpenRenderX PRIVATE
    "${CMAKE_SOURCE_DIR}/Content/Additional/include"
        "${CMAKE_SOURCE_DIR}/Content/Additional/include/GLFW"
)

if(APPLE)
    target_link_libraries(OpenRenderX PRIVATE glfw OpenGL::GL)
elseif (WIN32)
    target_link_libraries(OpenRenderX PRIVATE
            OpenGL::GL  # or opengl32
            "${CMAKE_SOURCE_DIR}/Content/Additional/library/glfw3.lib"      # or glfw3dll / glfw::glfw depending on your install
    )
endif()