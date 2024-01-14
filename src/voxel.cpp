#include <voxel.h>

namespace Voxel {
    bool isTransparent(Type type) {
        return type == AIR;
    }
}