#ifndef PLAYER_H
#define PLAYER_H

#include "model.h"
#include <glm/glm.hpp>

class Player {
public:
    glm::vec3 position;
    Model playerModel;


    void Draw(Shader& shader) const {
        playerModel.Draw(shader);
    }
    Player() : position(0.0f, 0.5f, 0.0f) {}
};

#endif // PLAYER_H