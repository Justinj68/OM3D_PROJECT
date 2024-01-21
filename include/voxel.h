#pragma once

namespace Voxel {
    enum Type : unsigned short {
        AIR,
        STONE,
        DIRT,
        GRASS
    };

    bool isTransparent(Type type);
}