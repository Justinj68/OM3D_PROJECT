# OM3D PROJECT

## Features

- **Cubical 3D Environment**: A world made up of cubes, generated using Perlin noise to create a natural, realistic landscape with caves.
- **Chunk System**: The world is divided into 32x32x32 voxel chunks to optimize rendering and memory management.
- **Greedy Meshing**: Rendering optimization by merging adjacent cube faces, significantly reducing the number of rendered triangles.
- **Transparent Water**: Water rendering with transparency.
- **Chunk Culling**: Chunks beyond a certain distance are not rendered, enhancing performance without sacrificing visual quality.
- **Backface Culling**: Non-rendering of back faces.

## Prerequisites

Before compiling and running the project, ensure that the following tools are installed on your Windows system:

- **CMake**: A cross-platform build system necessary for generating project files from build scripts.
- **Microsoft Visual C++ (MSVC) 2022**: The recommended compiler for compiling the project on Windows.

## Command Line Options

The build mode and world dimensions can be customized through command line arguments.  
You can choose between different build modes and set the width and height of the world at startup.

### Build Mode
Select the build mode to optimize voxel rendering:
- `GREEDY`: Uses 'Greedy Meshing', which merges adjacent faces to reduce the number of rendered polygons.
- `CLASSIC`: Uses the classic rendering mode where each voxel is processed individually.

Add `GREEDY` or `CLASSIC` to your command line arguments to set the mode.

### World Dimensions
You can specify the width and height of the world by adding numeric values after the build mode. The program will assign the first numeric value to the width and the second to the height.

**Syntax**:
```sh
./OM3DM [MODE] [WIDTH] [HEIGHT]
```

**Example** :
To launch the application in `GREEDY` mode with a world size of 20x10 chunks, enter:
```sh
./OM3DM GREEDY 20 10
```

The numeric values are interpreted as absolute values.
If an invalid input is detected, an error message will be displayed in the terminal.

By default, the parameters will be as follows:
- Build mode: `GREEDY`
- World width: 10
- World height: 10

### Error Handling
In case of entering an invalid or out-of-bounds number, the program will inform the user through a red error message in the terminal, indicating the problem with the provided argument.


## Commands

Navigate through the voxelized world using these keys (AZERTY):

| Key     | Action                              |
| ------- | ----------------------------------- |
| Z       | Move forward                        |
| S       | Move backward                       |
| Q       | Move left                           |
| D       | Move right                          |
| Space   | Ascend                              |
| A       | Descend                             |
| X       | Toggle between wireframe and solid polygon |
| W       | Enable/disable water rendering      |
| Escape  | Exit the program                    |

## Installation, Compilation, and Execution

To start using Voxel Engine, follow these steps:

1. **Extract the OM3D_PROJECT folder from the zip or clone the repository**:
    ```sh
    git clone git@github.com:Justinj68/OM3D_PROJECT.git
    ```
2. **Compile the project**:
    ```sh
    mkdir build
    cmake -B build
    cd build
    cmake --build .
    ```
3. **Run the program**:
    ```sh
    cd Debug
    .\OM3DM.exe
    ```

## Some Statistics

```
Build mode: GREEDY
World width: 100
World height: 10
Defining chunk data...
Chunk data definition completed:
        Total time: 12m 53.8957s
        Avg. time/chunk: 0m 0.00773896s
Building meshes...
Mesh building completed:
        Total time: 4m 24.6394s
        Avg. time/chunk: 0m 0.00264639s
Face count: 27410301
```
```
Build mode: GREEDY
World width: Not set
World height: Not set
Defining chunk data...
Chunk data definition completed:
        Total time: 0m 6.98893s
        Avg. time/chunk: 0m 0.00698893s
Building meshes...
Mesh building completed:
        Total time: 0m 2.32904s
        Avg. time/chunk: 0m 0.00232904s
Face count: 235914
```
```
Build mode: CLASSIC
World width: Not set
World height: Not set
Defining chunk data...
Chunk data definition completed:
        Total time: 0m 7.06783s
        Avg. time/chunk: 0m 0.00706783s
Building meshes...
Mesh building completed:
        Total time: 0m 1.28826s
        Avg. time/chunk: 0m 0.00128826s
Face count: 672414
```
