//
// Created by Alice Buccoleri on 10/12/25.
//

#ifndef YOUR_PROJECT_FALLINGOBJECT_H
#define YOUR_PROJECT_FALLINGOBJECT_H
#include <vector>

#include "glm/vec3.hpp"
#include <glm/glm.hpp>


struct FallingObject {
    glm::vec3 position;
    bool active;

    FallingObject(glm::vec3 startPos): position(startPos), active(true)
    {
    }
};

#endif //YOUR_PROJECT_FALLINGOBJECT_H