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
    bool isLanded; // Diventa true quando tocca il pavimento
    float landTimer; // Conta i secondi passati da quando è atterrato
    // Variabili per il rotolamento
    float rollSpeed;        // Velocità orizzontale (negativa=sinistra, positiva=destra)
    float currentRollAngle; // Angolo di rotazione (per l'animazione del rotolamento)
    float offsetZ;          // Piccola variazione di profondità per non farle sovrapporre

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