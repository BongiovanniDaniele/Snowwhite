//
// Created by Alice Buccoleri on 10/12/25.
//

#ifndef YOUR_PROJECT_FALLINGOBJECT_H
#define YOUR_PROJECT_FALLINGOBJECT_H
#include <vector>

#include "glm/vec3.hpp"
#include <glm/glm.hpp>

enum ObjectType { APPLE_GOOD, APPLE_POISON, GEM, AMPOULE, AMPOULE_GOLD };

struct FallingObject {
    glm::vec3 position;
    bool active;
    ObjectType type;
    bool isLanded; // True when on floor
    float landTimer;
    // Rolling variables
    float rollSpeed;        // Horizontal speed
    float currentRollAngle; // Rotation angle
    float offsetZ;          // To avoid overlapping

    FallingObject(glm::vec3 pos, ObjectType t)
         : position(pos), active(true), type(t)
    {
        isLanded = false;
        landTimer = 0.0f;
        rollSpeed = 0.0f;
        currentRollAngle = 0.0f;
        offsetZ = 0.0f;
    }
};

#endif //YOUR_PROJECT_FALLINGOBJECT_H