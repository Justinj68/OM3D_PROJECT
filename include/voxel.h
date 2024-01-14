#pragma once

namespace Voxel {
    enum Type {
        AIR,
        STONE,
        DIRT,
        GRASS
    };

    bool isTransparent(Type type);
}