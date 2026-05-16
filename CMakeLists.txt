#Set cmake minimum required version
cmake_minimum_required(VERSION 3.10.0)

#set project name version and languages
project(OpenRenderX VERSION 0.1.0 LANGUAGES C CXX)

#Set cmake standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

 set(OpenGL_GL_PREFERENCE GLVND)

# Find Packages
if(APPLE)
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)
    add_compile_definitions(Platform_Apple)

elseif(LINUX)
    add_compile_definitions(Platform_Linux)
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)

elseif(WIN32)
    add_compile_definitions(Platform_Windows)
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
       Content/Source/Camera.cpp
       Content/Source/Light.cpp

        Content/Additional/include/imgui/imgui.cpp
        Content/Additional/include/imgui/imgui_demo.cpp
        Content/Additional/include/imgui/imgui_draw.cpp
        Content/Additional/include/imgui/imgui_tables.cpp
        Content/Additional/include/imgui/imgui_widgets.cpp

       Content/Additional/include/imgui/backends/imgui_impl_glfw.cpp
       Content/Additional/include/imgui/backends/imgui_impl_opengl3.cpp
)

target_compile_definitions(OpenRenderX PRIVATE 
    PROJECT_DIR="${CMAKE_SOURCE_DIR}"
)

# Include Directories
target_include_directories(OpenRenderX PRIVATE
    "${CMAKE_SOURCE_DIR}/Content/Additional/include"
    #"${CMAKE_SOURCE_DIR}/Content/Additional/include/glad"
    "${CMAKE_SOURCE_DIR}/Content/Additional/include/GLFW"
    "${CMAKE_SOURCE_DIR}/Content/Additional/include/imgui"
    "${CMAKE_SOURCE_DIR}/Content/Additional/include/imgui/backends"
)

if(APPLE)
target_link_libraries(OpenRenderX PRIVATE glfw OpenGL::GL)
elseif(LINUX)

target_link_libraries(OpenRenderX PRIVATE glfw OpenGL::GL)

elseif (WIN32)
    target_link_libraries(OpenRenderX PRIVATE
        OpenGL::GL  # or opengl32
        "${CMAKE_SOURCE_DIR}/Content/Additional/library/glfw3.lib"      # or glfw3dll / glfw::glfw depending on your install
    )
endif()
