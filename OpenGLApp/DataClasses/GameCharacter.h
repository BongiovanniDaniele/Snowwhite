#ifndef PLAYER_H
#define PLAYER_H

#include "model.h"
#include <glm/glm.hpp>
// #include "model.h" // Se usi una classe Model per caricare il 3D

class Player {
public:
    // ** Trasformazione Geometrica Essenziale **
    glm::vec3 position; // Posizione X, Y, Z del giocatore

    Model playerModel;


    // Dati per il rendering
    // Model characterModel; // Se usi la classe Model
    // Texture2D texture; // Se usi un semplice placeholder con texture

    // Metodi di rendering
    void Draw(Shader& shader) const {
        playerModel.Draw(shader);
    }

    // Costruttore
    Player() : position(0.0f, 0.5f, 0.0f) {}
};

#endif // PLAYER_H