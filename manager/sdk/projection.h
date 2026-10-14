#pragma once
#include "entity.h"

namespace sdk {
    inline bool ProjectToScreen(const Vector3& world, Vector2& screen, const ViewMatrix& matrix, int screen_width, int screen_height) {
        float w = matrix.matrix[3][0] * world.x + matrix.matrix[3][1] * world.y +
            matrix.matrix[3][2] * world.z + matrix.matrix[3][3];
        if (!sdk::finite(world) || screen_width <= 0 || screen_height <= 0 || !std::isfinite(w) || w < 0.001f) return false;
        float x = matrix.matrix[0][0] * world.x + matrix.matrix[0][1] * world.y +
            matrix.matrix[0][2] * world.z + matrix.matrix[0][3];
        float y = matrix.matrix[1][0] * world.x + matrix.matrix[1][1] * world.y +
            matrix.matrix[1][2] * world.z + matrix.matrix[1][3];
        x /= w; y /= w; if (!std::isfinite(x) || !std::isfinite(y)) return false;
        screen.x = (screen_width / 2.0f) + (x * screen_width / 2.0f);
        screen.y = (screen_height / 2.0f) - (y * screen_height / 2.0f);
        return true;
    }

}
