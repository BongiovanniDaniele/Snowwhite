//
// Created by Alice Buccoleri on 10/12/25.
//

#ifndef YOUR_PROJECT_FALLINGOBJECT_H
#define YOUR_PROJECT_FALLINGOBJECT_H
#include <vector>

#include "glm/vec3.hpp"
#include <glm/glm.hpp>

enum ObjectType { APPLE_GOOD, APPLE_POISON, GEM };

struct FallingObject {
    glm::vec3 position;
    bool active;
    ObjectType type;

    FallingObject(glm::vec3 pos, ObjectType t)
         : position(pos), active(true), type(t) {}
};

#endif //YOUR_PROJECT_FALLINGOBJECT_H