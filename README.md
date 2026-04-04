# Open-Render-X

## What is Open Render X?

Open Render X is a cross-platform OpenGL renderer which uses GLFW, GLAD, and ImGui. The goal of this project is for me to get a better understanding of graphics rendering, application development, and C++.

**Platforms currently supported:**
- MacOS
- Windows
- Linux

## Target Features

*List is a Work in Progress*

- OBJ loading ✅
- Lighting (Point Light, Directional Light, Spotlight, Multiple lights)
- Skybox Rendering
- Camera Movement ✅

## Controls

**WASD** - Movement

**Q/E** - Up & Down

**Right Mouse** - Look Around

**1** - Open Debug Window

**Left Shift + 1** - Close Debug Window

**2** - Open Scene List

**Left Shift + 2** - Close Scene List


## Building from Source

runnable versions of Open Render X should be available via

### On linux

**In order to build you must have gcc, g++ & gdb**

1) Clone the repository onto your PC

2) CD into or open a terminal from the build folder, then call:

```
make
```

3) Once it builds you should now have file called *OpenRenderX*. To open, double click on the file or type the following into the terminal:

```
./OpenRenderX
```

Open Render X should start

## Documentation 

**Work in Progress**

*All documentation can be found in the Documentation folder. Documentation has been generated using Doxygen*