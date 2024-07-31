#pragma once

#include "board.h"

namespace fengine {
    struct Name {
        std::string name = "_";
    };

    struct Transform {
        glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
        glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        //glm::mat4 modelMatrix();
        //glm::mat3 normalMatrix();
    };

    struct RenderId {
        uint32_t id;
    };

    struct Pin {
        pin::Pin pin;
    };

    struct PhysicsSphere {
        float radius;
    };

    struct PhysicsCapsule {
        glm::vec3 offset;
        float radius;
    };
}