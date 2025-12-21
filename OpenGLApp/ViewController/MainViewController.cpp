#include "MainViewController.h"
#include <iostream>
#include <vector>

#include "FallingObject.h"

extern Player player;
enum GameState{ MENU, GAME};
extern GameState currentState;
extern float game_z_speed;

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;

void renderMenu (GLFWwindow* window, Texture2D& backGroundTexture, Texture2D& startButtonTexture, Texture2D& startButtonTextureSelected) {
    menuBGShader.use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT), -1.0f, 1.0f);
    menuBGShader.setMat4("projection", projection);

    sprite2D.DrawSprite(backGroundTexture, glm::vec2(startButton.x, startButton.y), glm::vec2(startButton.width, startButton.height));

    if (startButton.selected == false) {
        sprite2D.DrawSprite(startButtonTexture, glm::vec2(startButton.x, startButton.y), glm::vec2(startButton.width, startButton.height));
    }
    else {
        sprite2D.DrawSprite(startButtonTextureSelected, glm::vec2(startButton.x, startButton.y), glm::vec2(startButton.width, startButton.height));
    }

    shaderText.use();
    shaderText.setMat4("projection", projection);

    float textScale = 2.0f * scaleScreen;
    RenderText(shaderText, gameName, static_cast<float>(SCR_WIDTH) / 2.0f - 150.f * scaleScreen, static_cast<float>(SCR_HEIGHT) * 0.2f, textScale, glm::vec3(1.0f, 0.5f, 0.0f));

    float buttonTextY = startButton.y + startButton.height * 0.5f - 20.f * scaleScreen;
    RenderText(shaderText, "SNOW WHITE", startButton.x + 10 * scaleScreen, buttonTextY, scaleScreen * 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
};

void renderCube() {
    if (cubeVAO == 0) {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindVertexArray(cubeVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void renderGame(GLFWwindow* window, const Player player) {
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    gameShader.use();
    gameShader.setMat4("view", view);
    gameShader.setMat4("projection", projection);

    glm::mat4 modelPlayer = glm::mat4(1.0f);
    modelPlayer = glm::translate(modelPlayer, player.position); // Usa la posizione aggiornata in update
    modelPlayer = glm::rotate(modelPlayer, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelPlayer = glm::rotate(modelPlayer, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    modelPlayer = glm::scale(modelPlayer, glm::vec3(0.01f)); // Scala se necessario
    gameShader.setMat4("model", modelPlayer);

    player.playerModel.Draw(gameShader);

    for (const auto& obj : objects) {
        if (obj.active) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);
            model = glm::scale(model, glm::vec3(0.5f));
            gameShader.setMat4("model", model);

            renderCube();
        }
    }

    glDisable(GL_DEPTH_TEST);
    shaderText.use();
    glm::mat4 projectionUI = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),0.0F,static_cast<float>(SCR_HEIGHT));
    shaderText.setMat4("projection", projectionUI);

    std::string scoreText = "Score: " + std::to_string(score);
    RenderText(shaderText, scoreText, SCR_WIDTH- 250.0f, SCR_HEIGHT - 50.0f, 1.0f, glm::vec3(1.0f, 1.0f,1.0f));

    std::string livesText = "Lives: " + std::to_string(lives);
    glm::vec3 lifeColor = (lives == 1) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
    RenderText(shaderText, livesText, 25.0f, SCR_HEIGHT - 50.0f, 1.0f, lifeColor);

    glEnable(GL_DEPTH_TEST);
}

void MainViewController::loadModels() {
    // You can use this function to preload the 3D models for your game, to use in another ViewController
    std::cout << "Loading Models" << std::endl;

    std::string directory = std::string(getResource("Models/10431_Wicker_Basket_v1_L3.obj"));
    player.playerModel.directory = directory;

    player.playerModel.loadModel();
}

int MainViewController::main(GLFWwindow* window)
{
    // Shader Loading
    shaderText = Shader(getResource("Shaders/shaderText.vs").c_str(), getResource("Shaders/shaderText.fs").c_str());
    gameShader = Shader(getResource("Shaders/shader.vs").c_str(), getResource("Shaders/shader.fs").c_str());
    menuBGShader = Shader(getResource("Shaders/2DImage.vs").c_str(), getResource("Shaders/2DImage.fs").c_str());

    sprite2D = SpriteRenderer(menuBGShader);

    Texture2D backGroundTexture = loadTextureFromFile(getResource("Backgrounds/templateBackground.png").c_str(),false);

    Texture2D startButtonTexture = loadTextureFromFile(getResource("Buttons/StartButton/unselected.png").c_str(), true);
    Texture2D startButtonTextureSelected = loadTextureFromFile(getResource("Buttons/StartButton/selected.png").c_str(), true);

    const float y = static_cast<float>(SCR_HEIGHT) - static_cast<float>(SCR_HEIGHT) * 0.25f;
    const float buttonWidth = static_cast<float>(SCR_WIDTH) / 5.0f;
    const float buttonHeight = buttonWidth / 4.0f;
    startButton.x = static_cast<float>(SCR_WIDTH) / 2 - buttonWidth*1.4f / 2;
    startButton.y = y - buttonHeight*0.4f/2;
    startButton.height = buttonHeight*1.4f;
    startButton.width = buttonWidth*1.4f;

    player.position = glm::vec3(target_x_position, 0.5f, 0.0f);

    menuMusic.playSound();

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


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // // Background
        // menuBGShader.use();
        // glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
        // menuBGShader.setMat4("projection", projection);
        //
        // sprite2D.DrawSprite(backGroundTexture, glm::vec2(0.0f, 0.0f), glm::vec2(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT)));
        //
        // //Start Button
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
            renderMenu(window, backGroundTexture, startButtonTexture, startButtonTextureSelected);
        } else if (currentState == GAME) {
            glEnable(GL_DEPTH_TEST);
            float interpolationFactor = player_speed * deltaTime;
            if (interpolationFactor > 1.0f) interpolationFactor = 1.0f;
            player.position.x = glm::mix(player.position.x, target_x_position, interpolationFactor);

            player.position.z -= game_z_speed*deltaTime;

            cameraPos.x = 0.0f;
            cameraPos.y = 2.0f;
            cameraPos.z = player.position.z + 5.0f;

            if (currentFrame - lastSpawnTime > spawnInterval) {
                lastSpawnTime = currentFrame;

                int randomLane = rand() % 3;
                float spawnX = 0.0f;
                if (randomLane == 0) spawnX = -lane_width;
                else if (randomLane == 1) spawnX = 0.0f;
                else if (randomLane == 2) spawnX = lane_width;

                objects.push_back(FallingObject(glm::vec3(spawnX, 5.0f, player.position.z - 30.0f)));
            }

            for (auto& obj: objects) {
                if (obj.active) {
                    obj.position.y -= 5.0f * deltaTime;
                    if (obj.position.y < 0.5f) obj.position.y = 0.5f;

                    if (abs(player.position.x - obj.position.x) < 1.0f &&
                        abs(player.position.z - obj.position.z) < 1.0f &&
                        abs(player.position.y - obj.position.y) < 1.0f) {
                        score++;
                        obj.active = false;
                    }
                    else if (obj.position.z > player.position.z + 2.0f) {
                        lives--;
                        obj.active = false;
                    }
                }
            }

            if (lives <= 0) {
                currentState = MENU;
                lives = 3;
                score = 0;
                objects.clear();
                player.position = glm::vec3(0.0f, 0.5f, 0.0f);
            }

            renderGame(window,player);
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
