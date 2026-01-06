#include "MainViewController.h"
#include <iostream>
#include <vector>
#include "FallingObject.h"
#include  "SoundManager.h"

extern Player player;
extern Model appleModel;
enum GameState{ MENU, GAME};
extern GameState currentState;
extern float game_z_speed;

extern SoundManager menuMusic;
extern SoundManager gameMusic;
extern SoundManager sfxMouse;
extern SoundManager goodApple;
extern SoundManager badApple;
extern SoundManager diamond;
extern SoundManager missed1;
extern SoundManager missed2;

unsigned int diamondVAO = 0;
unsigned int diamondVBO = 0;

void renderMenu (GLFWwindow* window, Texture2D& startButtonTexture, Texture2D& startButtonTextureSelected, Texture2D& bgTexture) {
    menuBGShader.use();

    glm::mat4 projectionSprites = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
    menuBGShader.setMat4("projection", projectionSprites);

    sprite2D.DrawSprite(bgTexture, glm::vec2(0.0f, 0.0f), glm::vec2(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT)));

    if (startButton.selected == false) {
        sprite2D.DrawSprite(startButtonTexture, glm::vec2(startButton.x, startButton.y), glm::vec2(startButton.width, startButton.height));
    }
    else {
        sprite2D.DrawSprite(startButtonTextureSelected, glm::vec2(startButton.x, startButton.y), glm::vec2(startButton.width, startButton.height));
    }
};

void renderDiamond() {
    if (diamondVAO == 0) {
        // Un diamante semplice (Ottaedro allungato)
        // Posizioni (X, Y, Z) e Normali (NX, NY, NZ) per la luce
        float vertices[] = {
            // -- PARTE SUPERIORE (Piramide a punta) --
            // Pos              // Normali (Approssimate per flat shading)
            // Triangolo 1 (Fronte)
             0.0f,  0.5f,  0.0f,   0.0f, 0.5f, 0.8f, // Top
            -0.5f,  0.0f,  0.5f,   0.0f, 0.5f, 0.8f, // Front Left
             0.5f,  0.0f,  0.5f,   0.0f, 0.5f, 0.8f, // Front Right

            // Triangolo 2 (Destra)
             0.0f,  0.5f,  0.0f,   0.8f, 0.5f, 0.0f,
             0.5f,  0.0f,  0.5f,   0.8f, 0.5f, 0.0f,
             0.5f,  0.0f, -0.5f,   0.8f, 0.5f, 0.0f,

            // Triangolo 3 (Dietro)
             0.0f,  0.5f,  0.0f,   0.0f, 0.5f, -0.8f,
             0.5f,  0.0f, -0.5f,   0.0f, 0.5f, -0.8f,
            -0.5f,  0.0f, -0.5f,   0.0f, 0.5f, -0.8f,

            // Triangolo 4 (Sinistra)
             0.0f,  0.5f,  0.0f,  -0.8f, 0.5f, 0.0f,
            -0.5f,  0.0f, -0.5f,  -0.8f, 0.5f, 0.0f,
            -0.5f,  0.0f,  0.5f,  -0.8f, 0.5f, 0.0f,

            // -- PARTE INFERIORE (Piramide rovesciata più corta) --
            // Triangolo 5 (Fronte)
             0.0f, -0.5f,  0.0f,   0.0f, -0.5f, 0.8f, // Bottom
             0.5f,  0.0f,  0.5f,   0.0f, -0.5f, 0.8f,
            -0.5f,  0.0f,  0.5f,   0.0f, -0.5f, 0.8f,

            // Triangolo 6 (Destra)
             0.0f, -0.5f,  0.0f,   0.8f, -0.5f, 0.0f,
             0.5f,  0.0f, -0.5f,   0.8f, -0.5f, 0.0f,
             0.5f,  0.0f,  0.5f,   0.8f, -0.5f, 0.0f,

            // Triangolo 7 (Dietro)
             0.0f, -0.5f,  0.0f,   0.0f, -0.5f, -0.8f,
            -0.5f,  0.0f, -0.5f,   0.0f, -0.5f, -0.8f,
             0.5f,  0.0f, -0.5f,   0.0f, -0.5f, -0.8f,

            // Triangolo 8 (Sinistra)
             0.0f, -0.5f,  0.0f,  -0.8f, -0.5f, 0.0f,
            -0.5f,  0.0f,  0.5f,  -0.8f, -0.5f, 0.0f,
            -0.5f,  0.0f, -0.5f,  -0.8f, -0.5f, 0.0f
        };

        glGenVertexArrays(1, &diamondVAO);
        glGenBuffers(1, &diamondVBO);
        glBindVertexArray(diamondVAO);
        glBindBuffer(GL_ARRAY_BUFFER, diamondVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Attributo Posizione
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Attributo Normale (Serve per la luce!)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    glBindVertexArray(diamondVAO);
    glDrawArrays(GL_TRIANGLES, 0, 24); // 8 facce * 3 vertici
    glBindVertexArray(0);
}

void renderGame(GLFWwindow* window, const Player player, Texture2D& bgTexture,Texture2D& life3, Texture2D& life2, Texture2D& life1) {
    glDisable(GL_DEPTH_TEST);
    menuBGShader.use();

    glm::mat4 projectionBG = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
    menuBGShader.setMat4("projection", projectionBG);
    sprite2D.DrawSprite(bgTexture, glm::vec2(0.0f, 0.0f), glm::vec2(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT)));
    glEnable(GL_DEPTH_TEST); // Riabilita per gli oggetti 3D

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    gameShader.use();
    gameShader.setMat4("view", view);
    gameShader.setMat4("projection", projection);

    gameShader.setVec3("lightPos", glm::vec3(0.0f, 10.0f, 1.0f));

    // Colore Luna: Bianco freddo / Azzurrino
    gameShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.2f));

    // Posizione camera per i riflessi
    gameShader.setVec3("viewPos", cameraPos);

    gameShader.setBool("useTexture", true);
    gameShader.setFloat("specularStrength", 0.1f);
    gameShader.setFloat("emissionStrength", 0.0f);

    glm::mat4 modelPlayer = glm::mat4(1.0f);
    modelPlayer = glm::translate(modelPlayer, player.position); // Usa la posizione aggiornata in update
    modelPlayer = glm::rotate(modelPlayer, glm::radians(-105.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelPlayer = glm::rotate(modelPlayer, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    modelPlayer = glm::scale(modelPlayer, glm::vec3(0.025f)); // Scala se necessario
    gameShader.setMat4("model", modelPlayer);
    player.playerModel.Draw(gameShader);

    gameShader.setBool("useTexture", false);

    for (const auto& obj : objects) {
        if (obj.active) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.075f));
            gameShader.setMat4("model", model);

            if (obj.type == GEM) {
                // --- DIAMANTE ---
                gameShader.setBool("useTexture", false);

                // Colore Ciano brillante
                gameShader.setVec3("objectColor", glm::vec3(0.1f, 0.9f, 1.0f));

                // Parametri LUCE per farlo sembrare cristallo
                gameShader.setFloat("specularStrength", 2.0f); // Riflessi massimi
                gameShader.setFloat("emissionStrength", 0.2f); // Brilla un po'

                // Rotazione veloce su due assi per mostrare le facce
                model = glm::rotate(model, (float)glfwGetTime() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                gameShader.setMat4("model", model);

                renderDiamond(); // <--- Disegna il tuo oggetto OpenGL!
            }
            else if (obj.type == APPLE_GOOD) {
                // RED
                gameShader.setBool("useTexture", false);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, 0);

                gameShader.setVec3("objectColor", glm::vec3(0.8f, 0.05f, 0.05f));
                gameShader.setFloat("specularStrength", 0.5f);
                gameShader.setFloat("emissionStrength", 0.0f);

                appleModel.Draw(gameShader);
            }
            else {
                // PURPLE
                gameShader.setBool("useTexture", false);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, 0);

                gameShader.setVec3("objectColor", glm::vec3(0.4f, 0.0f, 0.6f));
                gameShader.setFloat("specularStrength", 0.8f);
                gameShader.setFloat("emissionStrength", 0.5f);

                appleModel.Draw(gameShader);
            }
        }
    }

    glDisable(GL_DEPTH_TEST);
    menuBGShader.use();

    glm::mat4 projectionUI = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),static_cast<float>(SCR_HEIGHT),0.0f,-1.0f,1.0f);
    shaderText.setMat4("projection", projectionUI);

    float iconSize = 450.0f * scaleScreen;

    float aspectRatio = 1792.0f / 592.0f;
    float iconHeight = iconSize / aspectRatio;

    float textX = 25.0f * scaleScreen;
    float iconX = textX + (300.0f * scaleScreen);
    float iconY = 15.0f * scaleScreen;

    if (lives >= 3) {
        sprite2D.DrawSprite(life3, glm::vec2(iconX, iconY), glm::vec2(iconSize, iconHeight)); // Adatta /2.5f in base alla forma della tua immagine
    }
    else if (lives == 2) {
        sprite2D.DrawSprite(life2, glm::vec2(iconX, iconY), glm::vec2(iconSize, iconHeight));
    }
    else {
        sprite2D.DrawSprite(life1, glm::vec2(iconX, iconY), glm::vec2(iconSize, iconHeight));
    }

    shaderText.use();
    glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    shaderText.setMat4("projection", projectionText);

    float textScale = 1.5f;
    float textY = static_cast<float>(SCR_HEIGHT) - (125.0f * scaleScreen);
    // Score (a destra)
    std::string scoreText = "Score: " + std::to_string(score);
    RenderText(shaderText, scoreText, SCR_WIDTH - 350.0f, textY, textScale, glm::vec3(1.0f, 1.0f, 1.0f));

    // Lives (a sinistra, spostato un po' a destra per non coprire l'immagine)
    glm::vec3 lifeColor = (lives == 1) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);

    // NOTA: Ho cambiato la X da 25.0f a (iconX + iconSize + 10) per metterlo ACCANTO all'immagine
    RenderText(shaderText, "Lives:", textX, textY, textScale, lifeColor);
    glEnable(GL_DEPTH_TEST);
}

void MainViewController::loadModels() {
    // You can use this function to preload the 3D models for your game, to use in another ViewController
    std::cout << "Loading Models" << std::endl;

    std::string directory = std::string(getResource("Models/10431_Wicker_Basket_v1_L3.obj"));
    player.playerModel.directory = directory;
    player.playerModel.loadModel();

    std::string applePath = std::string(getResource("Models/apple.obj"));
    appleModel.directory = applePath;
    appleModel.loadModel();
}

int MainViewController::main(GLFWwindow* window)
{
    // Shader Loading
    shaderText = Shader(getResource("Shaders/shaderText.vs").c_str(), getResource("Shaders/shaderText.fs").c_str());
    gameShader = Shader(getResource("Shaders/shader.vs").c_str(), getResource("Shaders/shader.fs").c_str());
    menuBGShader = Shader(getResource("Shaders/2DImage.vs").c_str(), getResource("Shaders/2DImage.fs").c_str());

    sprite2D = SpriteRenderer(menuBGShader);

    Texture2D backGroundMenu = loadTextureFromFile(getResource("Backgrounds/Menu_Background_Logo.png").c_str(),false);
    Texture2D backGroundGame = loadTextureFromFile(getResource("Backgrounds/bosco.png").c_str(),false);

    Texture2D startButtonTexture = loadTextureFromFile(getResource("Buttons/StartButton/unselected.png").c_str(), true);
    Texture2D startButtonTextureSelected = loadTextureFromFile(getResource("Buttons/StartButton/selected.png").c_str(), true);

    Texture2D texLives3 = loadTextureFromFile(getResource("Graphics/3lifes.png").c_str(),true);
    Texture2D texLives2 = loadTextureFromFile(getResource("Graphics/2lifes.png").c_str(),true);
    Texture2D texLives1 = loadTextureFromFile(getResource("Graphics/1life.png").c_str(),true);

    const float y = static_cast<float>(SCR_HEIGHT) - static_cast<float>(SCR_HEIGHT) * 0.25f;
    const float buttonWidth = static_cast<float>(SCR_WIDTH) / 5.0f;
    const float buttonHeight = buttonWidth / 3.0f;
    startButton.x = static_cast<float>(SCR_WIDTH) / 2 - buttonWidth*1.4f / 2;
    startButton.y = y - buttonHeight*0.4f/2;
    startButton.height = buttonHeight*1.4f;
    startButton.width = buttonWidth*1.4f;

    player.position = glm::vec3(target_x_position, 0.5f, 0.0f);

    menuMusic.playSound();

    float currentSpawnInterval = 2.0f;
    float currentFallSpeed = 6.0f;
    int poisonThreshold = 30;
    bool alternateMiss=false;

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        // render
        // ------


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Start Button
        // const float y = static_cast<float>(SCR_HEIGHT) - static_cast<float>(SCR_HEIGHT) * 0.25f;
        // const float buttonWidth = static_cast<float>(SCR_WIDTH) / 5.0f;
        // const float buttonHeight = buttonWidth / 4.0f;
        // startButton.x = static_cast<float>(SCR_WIDTH) / 2 - buttonWidth*1.4f / 2;
        // startButton.y = y - buttonHeight*0.4f/2;
        // startButton.height = buttonHeight*1.4f;
        // startButton.width = buttonWidth*1.4f;
        // if (startButton.selected == false) {
        //     sprite2D.DrawSprite(startButtonTexture, glm::vec2(startButton.x, startButton.y), glm::vec2(startButton.width, startButton.height));
        // }
        // else {
        //     sprite2D.DrawSprite(startButtonTextureSelected, glm::vec2(startButton.x, startButton.y), glm::vec2(startButton.width, startButton.height));
        // }

        if (loaded == false) {
            std::cout << "Load Models" << std::endl;
            loadModels();
            loaded = true;
        }

        if (currentState == MENU) {
            glDisable(GL_DEPTH_TEST);
            renderMenu(window, startButtonTexture, startButtonTextureSelected, backGroundMenu);
        } else if (currentState == GAME) {
            glEnable(GL_DEPTH_TEST);

            if (score < 10) {
                // LIVELLO FACILE
                currentSpawnInterval = 1.8f; // Lento
                currentFallSpeed = 6.0f;     // Caduta tranquilla
                poisonThreshold = 30;        // ~20% Veleno (considerando 10% gemme)
            }
            else if (score >= 10 && score < 30) {
                // LIVELLO MEDIO
                currentSpawnInterval = 1.2f; // Più frequente
                currentFallSpeed = 8.5f;     // Più veloce
                poisonThreshold = 50;        // ~40% Veleno
            }
            else {
                // LIVELLO DIFFICILE (INCUBO)
                currentSpawnInterval = 0.7f; // Pioggia di oggetti!
                currentFallSpeed = 11.0f;    // Velocissimi
                poisonThreshold = 70;        // ~60% Veleno
            }

            float interpolationFactor = player_speed * deltaTime;
            if (interpolationFactor > 1.0f) interpolationFactor = 1.0f;

            player.position.x = glm::mix(player.position.x, target_x_position, interpolationFactor);
            player.position.z = 0.0f;
            player.position.y = -1.7f;

            cameraPos = glm::vec3(0.0f, 4.0f, 8.0f);
            cameraFront = glm::vec3(0.0f, -0.4f, -1.0f);

            if (currentFrame - lastSpawnTime > currentSpawnInterval) {
                lastSpawnTime = currentFrame;

                int randomLane = rand() % 3;
                float spawnX = 0.0f;
                if (randomLane == 0) spawnX = -lane_width;
                else if (randomLane == 1) spawnX = 0.0f;
                else if (randomLane == 2) spawnX = lane_width;

                int randomType = rand() % 100;
                ObjectType newType;
                if (randomType < 10) {
                    newType = GEM; // 10% probabilità: Gemma
                } else if (randomType < poisonThreshold) {
                    newType = APPLE_POISON; // 30% probabilità: Veleno
                } else {
                    newType = APPLE_GOOD; // 60% probabilità: Mela normale
                }

                objects.push_back(FallingObject(glm::vec3(spawnX, 9.0f, 0.0f), newType));
            }

            for (auto& obj: objects) {
                if (obj.active) {
                    obj.position.y -= currentFallSpeed * deltaTime;
                    bool altezzaGiusta = (obj.position.y < -1.2f && obj.position.y > -2.2f);

                    // Controlliamo se è nella stessa corsia orizzontale (X)
                    bool corsiaGiusta = (abs(player.position.x - obj.position.x) < 0.5f);

                    if (altezzaGiusta && corsiaGiusta) {
                        if (obj.type == GEM) {
                            score += 10; // Bonus grande!
                            diamond.playSound();
                        }
                        else if (obj.type == APPLE_POISON) {
                            score -= 5;  // Penalità punti
                            badApple.playSound();
                        }
                        else { // APPLE_GOOD
                            score += 1;  // Punti normali
                            goodApple.playSound();
                        }
                        obj.active = false; // Mela raccolta, sparisce
                    }

                    // 3. CONTROLLO FINE CORSA (Caduta nel vuoto)
                    // Se la mela arriva a Y = -5.0 (ben oltre il fondo dello schermo), perdi una vita
                    else if (obj.position.y < -5.0f) {
                        if (obj.type == APPLE_GOOD) {
                            lives--;
                            if (alternateMiss){
                                missed2.playSound();
                            }else{
                                missed1.playSound();
                            }
                            alternateMiss = !alternateMiss;
                        }
                        obj.active = false; // Mela persa, sparisce
                    }
                }
            }

            if (lives <= 0) {
                currentState = MENU;
                lives = 3;
                score = 0;
                objects.clear();
                player.position = glm::vec3(0.0f, 0.5f, 0.0f);
                gameMusic.stopSound();
                menuMusic.playSound();
            }

            renderGame(window,player, backGroundGame, texLives3, texLives2, texLives1);
        }
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------

    glfwTerminate();
	return 0;
}