
#include "MainViewController.h"
#include <iostream>
#include <vector>
#include "FallingObject.h"
#include  "SoundManager.h"

extern Player player;
extern Model appleModel;
extern Model maskModel;
extern Model ampouleModel;
extern Model mirrorModel;
extern int activeDistractionType;

enum GameState{ MENU, GAME, GAME_OVER_ANIMATION, INPUT_NAME, LEADERBOARD};
extern GameState currentState;

extern float game_z_speed;

extern SoundManager gameMusic;
extern SoundManager sfxMouse;
extern SoundManager goodApple;
extern SoundManager badApple;
extern SoundManager diamond;
extern SoundManager missed1;
extern SoundManager missed2;
extern SoundManager maskSpeech;
extern SoundManager maskVanish;
extern SoundManager glass;
extern SoundManager sfxMirror;

unsigned int diamondVAO = 0;
unsigned int diamondVBO = 0;

extern unsigned int overlayVAO, overlayVBO;
extern std::string currentInputName;
extern bool keyProcessed;
extern std::vector<ScoreEntry> highScoresList;
extern const std::string SCORE_FILE;

void renderMenu (GLFWwindow* window, Texture2D& startButtonTexture, Texture2D& startButtonTextureSelected, Texture2D& lbTexture, Texture2D& lbTextureSelected,Texture2D& bgTexture) {
    menuBGShader.use(); // 2D Image Shader to draw menu

    glm::mat4 projectionSprites = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
    menuBGShader.setMat4("projection", projectionSprites);

    // Background
    sprite2D.DrawSprite(bgTexture, glm::vec2(0.0f, 0.0f), glm::vec2(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT)));

    // Start Button: selected or not
    if (startButton.selected == false) {
        sprite2D.DrawSprite(startButtonTexture, glm::vec2(startButton.x, startButton.y), glm::vec2(startButton.width, startButton.height));
    }
    else {
        sprite2D.DrawSprite(startButtonTextureSelected, glm::vec2(startButton.x, startButton.y), glm::vec2(startButton.width, startButton.height));
    }

    // Leader Board Button: selected or not
    if (lbButton.selected == false) {
        sprite2D.DrawSprite(lbTexture, glm::vec2(lbButton.x, lbButton.y), glm::vec2(lbButton.width, lbButton.height));
    }
    else {
        sprite2D.DrawSprite(lbTextureSelected, glm::vec2(lbButton.x, lbButton.y), glm::vec2(lbButton.width, lbButton.height));
    }

};

void renderDiamond() {
    if (diamondVAO == 0) {
        float vertices[] = {
            // Position            // Normals (Flat Shading is used)
            // Front
             0.0f,  0.5f,  0.0f,   0.0f, 0.5f, 0.8f, // Top
            -0.5f,  0.0f,  0.5f,   0.0f, 0.5f, 0.8f, // Front Left
             0.5f,  0.0f,  0.5f,   0.0f, 0.5f, 0.8f, // Front Right

            // Right
             0.0f,  0.5f,  0.0f,   0.8f, 0.5f, 0.0f,
             0.5f,  0.0f,  0.5f,   0.8f, 0.5f, 0.0f,
             0.5f,  0.0f, -0.5f,   0.8f, 0.5f, 0.0f,

            // Back
             0.0f,  0.5f,  0.0f,   0.0f, 0.5f, -0.8f,
             0.5f,  0.0f, -0.5f,   0.0f, 0.5f, -0.8f,
            -0.5f,  0.0f, -0.5f,   0.0f, 0.5f, -0.8f,

            // Left
             0.0f,  0.5f,  0.0f,  -0.8f, 0.5f, 0.0f,
            -0.5f,  0.0f, -0.5f,  -0.8f, 0.5f, 0.0f,
            -0.5f,  0.0f,  0.5f,  -0.8f, 0.5f, 0.0f,

            // Bottom front
             0.0f, -0.5f,  0.0f,   0.0f, -0.5f, 0.8f, // Bottom
             0.5f,  0.0f,  0.5f,   0.0f, -0.5f, 0.8f,
            -0.5f,  0.0f,  0.5f,   0.0f, -0.5f, 0.8f,

            // Bottom right
             0.0f, -0.5f,  0.0f,   0.8f, -0.5f, 0.0f,
             0.5f,  0.0f, -0.5f,   0.8f, -0.5f, 0.0f,
             0.5f,  0.0f,  0.5f,   0.8f, -0.5f, 0.0f,

            // Bottom back
             0.0f, -0.5f,  0.0f,   0.0f, -0.5f, -0.8f,
            -0.5f,  0.0f, -0.5f,   0.0f, -0.5f, -0.8f,
             0.5f,  0.0f, -0.5f,   0.0f, -0.5f, -0.8f,

            // Bottom left
             0.0f, -0.5f,  0.0f,  -0.8f, -0.5f, 0.0f,
            -0.5f,  0.0f,  0.5f,  -0.8f, -0.5f, 0.0f,
            -0.5f,  0.0f, -0.5f,  -0.8f, -0.5f, 0.0f
        };

        glGenVertexArrays(1, &diamondVAO);
        glGenBuffers(1, &diamondVBO);
        glBindVertexArray(diamondVAO);
        glBindBuffer(GL_ARRAY_BUFFER, diamondVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    glBindVertexArray(diamondVAO);
    glDrawArrays(GL_TRIANGLES, 0, 24); // 3 vertices for 8 faces = 24
    glBindVertexArray(0);
}

void renderGame(GLFWwindow* window, const Player player, Texture2D& bgTexture,Texture2D& life3, Texture2D& life2, Texture2D& life1, float tiltAngle, float mirrorTimer) {
    glDisable(GL_DEPTH_TEST);
    menuBGShader.use(); // Background

    glm::mat4 projectionBG = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
    menuBGShader.setMat4("projection", projectionBG);
    sprite2D.DrawSprite(bgTexture, glm::vec2(0.0f, 0.0f), glm::vec2(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT)));
    glEnable(GL_DEPTH_TEST); // End Background

    // Camera position
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    gameShader.use();
    gameShader.setMat4("view", view);
    gameShader.setMat4("projection", projection);

    // Light
    gameShader.setVec3("lightPos", glm::vec3(0.0f, 10.0f, 1.0f));
    gameShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.2f)); // Moonlight
    gameShader.setVec3("viewPos", cameraPos);

    gameShader.setBool("useTexture", true);
    gameShader.setFloat("specularStrength", 0.1f);
    gameShader.setFloat("emissionStrength", 0.0f);

    // Viewing direction and position
    glm::mat4 modelPlayer = glm::mat4(1.0f);
    modelPlayer = glm::translate(modelPlayer, player.position);
    modelPlayer = glm::rotate(modelPlayer, glm::radians(tiltAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    modelPlayer = glm::rotate(modelPlayer, glm::radians(-105.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelPlayer = glm::rotate(modelPlayer, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    modelPlayer = glm::scale(modelPlayer, glm::vec3(0.025f));
    gameShader.setMat4("model", modelPlayer);
    player.playerModel.Draw(gameShader);

    gameShader.setBool("useTexture", false);

    for (const auto& obj : objects) {
        if (obj.active) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);

            // Apple's movement
            if (!obj.isLanded) {
                // It rotates on itself
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            } else {
                // It rotates on its center
                model = glm::rotate(model, glm::radians(obj.currentRollAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            }

            model = glm::scale(model, glm::vec3(0.075f));
            gameShader.setMat4("model", model);

            if (obj.type == GEM) {
                // Diamond
                gameShader.setBool("useTexture", false);

                // Colour
                gameShader.setVec3("objectColor", glm::vec3(0.1f, 0.9f, 1.0f));

                // Light
                gameShader.setFloat("specularStrength", 2.0f); // Riflessi massimi
                gameShader.setFloat("emissionStrength", 0.2f); // Brilla un po'

                // Diamond's movement
                model = glm::rotate(model, (float)glfwGetTime() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                gameShader.setMat4("model", model);

                renderDiamond();
            }
            else if (obj.type == AMPOULE_GOLD) {
                // Gold Ampoule
                gameShader.setBool("useTexture", false);

                // Colour
                gameShader.setVec3("objectColor", glm::vec3(1.0f, 0.84f, 0.0f));

                // Light
                gameShader.setFloat("specularStrength", 2.0f);
                gameShader.setFloat("emissionStrength", 0.3f);

                // To reuse it
                ampouleModel.Draw(gameShader);
            }
            else if (obj.type == AMPOULE) {
                // New ampoule
                gameShader.setBool("useTexture", false);

                // Colour
                gameShader.setVec3("objectColor", glm::vec3(0.2f, 1.0f, 0.2f));

                // Glass effect
                gameShader.setFloat("specularStrength", 1.5f);
                gameShader.setFloat("emissionStrength", 0.1f);

                ampouleModel.Draw(gameShader);
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

    // Score and lives text
    glm::mat4 projectionUI = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),static_cast<float>(SCR_HEIGHT),0.0f,-1.0f,1.0f);
    shaderText.setMat4("projection", projectionUI);

    float iconSize = 450.0f * scaleScreen;

    float aspectRatio = 1792.0f / 592.0f;
    float iconHeight = iconSize / aspectRatio;

    float textX = 25.0f * scaleScreen;
    float iconX = textX + (300.0f * scaleScreen);
    float iconY = 15.0f * scaleScreen;

    if (lives >= 3) {
        sprite2D.DrawSprite(life3, glm::vec2(iconX, iconY), glm::vec2(iconSize, iconHeight));
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
    // Score
    std::string scoreText = "Score: " + std::to_string(score);
    RenderText(shaderText, scoreText, SCR_WIDTH - 350.0f, textY, textScale, glm::vec3(1.0f, 1.0f, 1.0f));

    // Lives
    glm::vec3 lifeColor = (lives == 1) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);

    RenderText(shaderText, "Lives:", textX, textY, textScale, lifeColor);
    glEnable(GL_DEPTH_TEST);

    if (showMagicMask) {
        // Mask
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        gameShader.use();

        glm::mat4 projectionUI2 = glm::ortho(-1.0f * (float)SCR_WIDTH/(float)SCR_HEIGHT, 1.0f * (float)SCR_WIDTH/(float)SCR_HEIGHT, -1.0f, 1.0f, -10.f, 10.f);
        glm::mat4 viewUI = glm::mat4(1.0f);
        gameShader.setMat4("projection", projectionUI2);
        gameShader.setMat4("view", viewUI);
        gameShader.setVec3("lightPos", glm::vec3(2.0f, 5.0f, 5.0f));
        gameShader.setBool("useTexture", false);

        if (activeDistractionType==0)
        {
            // Mask's movement and light when it goes down, out of the scene
            glm::mat4 modelMask = glm::mat4(1.0f);
            modelMask = glm::translate(modelMask, glm::vec3(0.0f, maskOffsetY, -1.0f));
            modelMask = glm::rotate(modelMask, glm::radians((float)sin(glfwGetTime()*2.0f)*5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMask = glm::scale(modelMask, glm::vec3(0.35f));
            gameShader.setMat4("model", modelMask);
            gameShader.setVec3("objectColor", glm::vec3(0.9f, 0.9f, 0.9f)); // Bianco Maschera
            gameShader.setFloat("specularStrength", 0.3f);
            gameShader.setFloat("emissionStrength", 0.05f);

            maskModel.Draw(gameShader);
        } else
        {
            glm::mat4 modelMirror = glm::mat4(1.0f);

            // Fixed position when it is inside the screen
            modelMirror = glm::translate(modelMirror, glm::vec3(0.0f, 0.2f, 0.5f));

            // It rotates on itself, using glfwGetTime
            modelMirror = glm::rotate(modelMirror, (float)glfwGetTime() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            // It rotates also on x
            modelMirror = glm::rotate(modelMirror, (float)glfwGetTime() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

            // Dyanamic change of scale
            float scaleFactor = 0.30f * (1.0f - (mirrorTimer / 5.0f));
            if (scaleFactor < 0.0f) scaleFactor = 0.0f;

            modelMirror = glm::scale(modelMirror, glm::vec3(scaleFactor));

            gameShader.setMat4("model", modelMirror);

            gameShader.setVec3("objectColor", glm::vec3(0.7f, 0.5f, 0.1f));

            gameShader.setFloat("specularStrength", 2.0f);
            gameShader.setFloat("emissionStrength", 0.1f);

            mirrorModel.Draw(gameShader);
        }

        glEnable(GL_DEPTH_TEST);
    }
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

    std::string maskPath = std::string(getResource("Models/Sculpting.obj"));
    maskModel.directory = maskPath;
    maskModel.loadModel();

    std::string ampoulePath = std::string(getResource("Models/ampoule.obj"));
    ampouleModel.directory = ampoulePath;
    ampouleModel.loadModel();

    std::string mirrorPath = std::string(getResource("Models/Mirror.obj"));
    mirrorModel.directory = mirrorPath;
    mirrorModel.loadModel();

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
    Texture2D backGroundInputName = loadTextureFromFile(getResource("Backgrounds/Input_Name.png").c_str(),false);
    Texture2D startButtonTexture = loadTextureFromFile(getResource("Buttons/StartButton/unselected.png").c_str(), true);
    Texture2D startButtonTextureSelected = loadTextureFromFile(getResource("Buttons/StartButton/selected.png").c_str(), true);
    Texture2D lbTexture = loadTextureFromFile(getResource("Buttons/LBButton/unselected.png").c_str(), true);
    Texture2D lbTextureSelected = loadTextureFromFile(getResource("Buttons/LBButton/selected.png").c_str(), true);
    Texture2D backBtnUnselected = loadTextureFromFile(getResource("Buttons/BackButton/unselected.png").c_str(), true);
    Texture2D backBtnSelected = loadTextureFromFile(getResource("Buttons/BackButton/selected.png").c_str(), true);

    Texture2D texLives3 = loadTextureFromFile(getResource("Graphics/3lifes.png").c_str(),true);
    Texture2D texLives2 = loadTextureFromFile(getResource("Graphics/2lifes.png").c_str(),true);
    Texture2D texLives1 = loadTextureFromFile(getResource("Graphics/1life.png").c_str(),true);

    const float y = static_cast<float>(SCR_HEIGHT) - static_cast<float>(SCR_HEIGHT) * 0.3f;
    const float buttonWidth = static_cast<float>(SCR_WIDTH) / 5.0f;
    const float buttonHeight = buttonWidth / 3.0f;
    startButton.x = static_cast<float>(SCR_WIDTH) / 2 - buttonWidth*1.4f / 2;
    startButton.y = y - buttonHeight*0.4f/2;
    startButton.height = buttonHeight*1.4f;
    startButton.width = buttonWidth*1.4f;
    lbButton.width = startButton.width * 0.7f;
    lbButton.height = startButton.height * 0.7f;
    lbButton.x = (static_cast<float>(SCR_WIDTH) / 2.0f) - (lbButton.width / 2.0f);
    // Padding between the two buttons
    lbButton.y = startButton.y + startButton.height + (10.0f * scaleScreen);

    backButton.width = buttonWidth * 1.4f;
    backButton.height = buttonHeight * 1.4f;
    backButton.x = (static_cast<float>(SCR_WIDTH) / 2.0f) - (backButton.width / 2.0f);
    backButton.y = static_cast<float>(SCR_HEIGHT) - backButton.height - (100.0f * scaleScreen);

    player.position = glm::vec3(target_x_position, 0.5f, 0.0f);

    float currentSpawnInterval = 2.0f;
    float currentFallSpeed = 6.0f;
    int poisonThreshold = 30;
    bool alternateMiss=false;
    int level = 1;
    float playerVelocityX = 0.0f;

    bool isSpeedBonusActive = false;
    float speedBonusTimer = 0.0f;
    const float BONUS_DURATION = 8.0f;
    float totalGameTime = 0.0f;

    // Gameover apple falling
    std::vector<FallingObject> endApples; // Apples for final animation
    float gameOverTimer = 0.0f;           // Timer for animation
    float gameOverDuration = 4.0f;        // Total duration of animation

    while (!glfwWindowShouldClose(window))
    {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (deltaTime > 0.05f) deltaTime = 0.05f;

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Loading models and music
        if (loaded == false) {
            std::cout << "Load Models" << std::endl;
            loadModels();
            loaded = true;
            menuMusic.playSound();
        }

        if (currentState == MENU)
        {
            if (menuMusic.isFinished()) {
                menuMusic.playSound();
            }

            // Setting all variables
            totalGameTime = 0.0f;
            level = 1;
            currentSpawnInterval = 1.8f;
            currentFallSpeed = 6.0f;
            poisonThreshold = 30;
            alternateMiss = false;
            isSpeedBonusActive = false;
            speedBonusTimer = 0.0f;
            playerVelocityX = 0.0f;

            gameOverTimer = 0.0f;
            gameOverDuration = 4.0f;

            glDisable(GL_DEPTH_TEST);
            renderMenu(window, startButtonTexture, startButtonTextureSelected, lbTexture, lbTextureSelected, backGroundMenu);
        }
        else if (currentState == LEADERBOARD) {
            glDisable(GL_DEPTH_TEST);
            menuBGShader.use();
            glm::mat4 projectionBG = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
            menuBGShader.setMat4("projection", projectionBG);

            // Initially background
            sprite2D.DrawSprite(backGroundInputName, glm::vec2(0, 0), glm::vec2(SCR_WIDTH, SCR_HEIGHT));

            // "Press ESC to return"
            shaderText.use();
            glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
            shaderText.setMat4("projection", projectionText);

            // Title
            float titleBaseY = static_cast<float>(SCR_HEIGHT) * 0.85f;
            float titleActualY = titleBaseY - 200.0f;

            RenderText(shaderText, "HALL OF FAME", (SCR_WIDTH / 2.0f) - 250.0f, titleActualY, 1.5f, glm::vec3(1.0f, 0.84f, 0.0f));

            // Scores
            float startY = titleActualY - 75.0f;

            float stepY = 55.0f * scaleScreen;

            for (size_t i = 0; i < highScoresList.size(); ++i) {
                // 3 different colours for 3 positions on ranking
                glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f); // White default

                if (i == 0) color = glm::vec3(1.0f, 0.84f, 0.0f);      // 1° Gold
                else if (i == 1) color = glm::vec3(0.75f, 0.75f, 0.75f); // 2° Silver
                else if (i == 2) color = glm::vec3(0.8f, 0.5f, 0.2f);    // 3° Bronze

                // String like 1. AAA - 1500
                std::string line = std::to_string(i + 1) + ". " + highScoresList[i].name + " - " + std::to_string(highScoresList[i].score);

                float textX = (SCR_WIDTH / 2.0f) - 150.0f;
                float textY = startY - (i * stepY);

                RenderText(shaderText, line, textX, textY, 1.0f, color);
            }
            // Back button
            menuBGShader.use();

            glm::mat4 projectionButtons = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
            menuBGShader.setMat4("projection", projectionButtons);

            if (backButton.selected == false) {
                sprite2D.DrawSprite(backBtnUnselected, glm::vec2(backButton.x, backButton.y), glm::vec2(backButton.width, backButton.height));
            }
            else {
                sprite2D.DrawSprite(backBtnSelected, glm::vec2(backButton.x, backButton.y), glm::vec2(backButton.width, backButton.height));
            }
        }
        else if (currentState == GAME) {
            // Game state
            if (gameMusic.isFinished() && lives > 0) {
                gameMusic.playSound();
            }
            totalGameTime += deltaTime;

            glEnable(GL_DEPTH_TEST);
            if (totalGameTime < 30.0f) {
                level = 1;
            } else if (totalGameTime < 90.0f) {
                level = 2;
            } else {
                level = 3;
            }

            float baseFallSpeed = 6.0f;
            float baseSpawnInterval= 1.8f;

            if (level==1) {
                // Easy level: slow and just 20% of poisoned apples
                currentSpawnInterval = 1.8f;
                baseSpawnInterval= 1.8f;
                currentFallSpeed = 6.0f;
                baseFallSpeed= 6.0f;
                poisonThreshold = 30;
            }
            else if (level==2) {
                // Medium level: more frequently, fast and 40% of poisoned apples
                currentSpawnInterval = 1.2f;
                baseSpawnInterval= 1.2f;
                currentFallSpeed = 8.5f;
                baseFallSpeed = 8.5f;
                poisonThreshold = 50;
            }
            else {// Hard level: rain of objects and 60% of poisoned apples
                currentSpawnInterval = 0.9f;
                baseSpawnInterval = 0.9f;
                currentFallSpeed = 10.0f;
                baseFallSpeed = 10.0f;
                poisonThreshold = 70;
            }

            if (isSpeedBonusActive) {
                // Speed of level 1
                currentFallSpeed = 6.0f;
                currentSpawnInterval = 1.8f;
                speedBonusTimer += deltaTime;

                // After 8 seconds, bonus disappears
                if (speedBonusTimer >= BONUS_DURATION) {
                    isSpeedBonusActive = false;
                    speedBonusTimer = 0.0f;
                }
            } else {
                // Current level speed
                currentFallSpeed = baseFallSpeed;
                currentSpawnInterval = baseSpawnInterval;
            }

            // Calculating tilt based on velocity
            float stiffness = 110.0f;
            float damping = 9.0f;

            float displacement = target_x_position - player.position.x;
            float force = (displacement * stiffness) - (playerVelocityX * damping);

            playerVelocityX += force * deltaTime;
            player.position.x += playerVelocityX * deltaTime;

            float currentTilt = playerVelocityX * -3.5f;

            // Tilt clamping
            if (currentTilt > 30.0f) currentTilt = 30.0f;
            if (currentTilt < -30.0f) currentTilt = -30.0f;

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

                if (randomType < 5) {
                    newType = AMPOULE_GOLD; // 5%
                }
                else if (randomType < 13) {
                    newType = GEM; // 8%
                }
                else if (randomType < 25) {
                    newType = AMPOULE; // Bad Ampoule
                }
                else if (randomType < poisonThreshold) {
                    newType = APPLE_POISON;
                } else {
                    newType = APPLE_GOOD;
                }

                objects.push_back(FallingObject(glm::vec3(spawnX, 9.0f, 0.0f), newType));
            }

            for (auto& obj: objects) {
                if (obj.active) {
                    // Movement
                    if (!obj.isLanded) {
                        // Falling
                        obj.position.y -= currentFallSpeed * deltaTime;
                    }
                    else {
                        // Rotate on the floor
                        obj.landTimer += deltaTime;

                        // Moving right or left
                        obj.position.x += obj.rollSpeed * deltaTime;

                        // Rotate on itself depending on speed
                        obj.currentRollAngle -= obj.rollSpeed * deltaTime * 150.0f;


                        if (obj.landTimer > 2.0f) {
                            obj.active = false;
                        }
                    }
                    // If it falls inside the basket
                    if (!obj.isLanded) {
                        bool altezzaGiusta = (obj.position.y < -1.2f && obj.position.y > -2.2f);
                        bool corsiaGiusta = (abs(player.position.x - obj.position.x) < 0.5f);

                        if (altezzaGiusta && corsiaGiusta) {
                            if (obj.type == GEM) {
                                score += 10;
                                diamond.playSound();
                            }
                            else if (obj.type == AMPOULE_GOLD) {
                                isSpeedBonusActive = true;
                                speedBonusTimer = 0.0f; // If it is already active
                                score += 2;
                                glass.playSound();
                            }
                            else if (obj.type == APPLE_POISON) {
                                score -= 5;
                                badApple.playSound();
                            }
                            else if (obj.type == AMPOULE) {
                                lives--;
                                missed1.playSound();
                            }
                            else {
                                score += 1;
                                goodApple.playSound();
                            }
                            obj.active = false; // Catched!
                        }
                    }

                    // Apple on the floor
                    if (!obj.isLanded && obj.position.y < -2.5f) {
                        obj.isLanded = true;
                        obj.position.y = -2.5f; // Fixed to floor

                        // Random direction
                        float direction = (rand() % 2 == 0) ? 1.0f : -1.0f;
                        // Random speed
                        float speed = 1.0f + static_cast<float>(rand() % 150) / 100.0f;

                        obj.rollSpeed = direction * speed;

                        // Depth
                        obj.offsetZ = ((rand() % 60) / 100.0f) - 0.30f;
                        obj.position.z += obj.offsetZ;

                        // Malus
                        if (obj.type == APPLE_GOOD) {
                            lives--;
                            if (alternateMiss) missed2.playSound();
                            else missed1.playSound();
                            alternateMiss = !alternateMiss;
                        }
                    }
                }
            }

            if (lives <= 0) { // Gameover :(
                gameMusic.stopSound();

                currentState=GAME_OVER_ANIMATION;
                gameOverTimer=0.0f;
                endApples.clear();

                for (int i = 0; i < 50; i++) {
                    // Rain of poisoned apples

                    float randomX = ((rand() % 800) / 100.0f) - 4.0f;
                    float randomY = 5.0f + ((rand() % 1000) / 100.0f);
                    float randomZ = ((rand() % 100) / 100.0f) - 0.5f;

                    endApples.push_back(FallingObject(glm::vec3(randomX, randomY, randomZ), APPLE_POISON));
                }
                // Reset of the visual scene
                showMagicMask = false;
                maskEventTimer = 0.0f;
                maskOffsetY = 0.0f;
                isDraggingMask = false;
                controlsInverted = false;
                mirrorTimer = 0.0f;
            }

            if (!showMagicMask) {
                maskEventTimer += deltaTime;
                if (maskEventTimer >= timeBetweenMaskEvents) {
                    showMagicMask = true;
                    maskEventTimer = 0.0f;
                    maskOffsetY = 0.0f;
                    mirrorTimer = 0.0f;

                    activeDistractionType = rand() %2;
                    if (activeDistractionType == 0) {
                        maskSpeech.playSound(); // When mask appears
                    } else {
                        sfxMirror.playSound(); // When mirror appears
                    }

                }
            } else {
                if (activeDistractionType == 0)
                {
                    // Mouse using
                    double mouseX, mouseY;
                    glfwGetCursorPos(window, &mouseX, &mouseY);
                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                        if (!isDraggingMask) {
                            isDraggingMask = true;
                            dragStartY = mouseY;
                        } else {
                            float dragAmount = (float)(mouseY - dragStartY);
                            maskOffsetY = -dragAmount / (float)SCR_HEIGHT * 2.0f;

                            if (maskOffsetY > 0.0f) maskOffsetY = 0.0f;
                        }
                    } else {
                        isDraggingMask = false;

                        if (maskOffsetY < -1.2f) {
                            showMagicMask = false;
                            maskVanish.playSound();
                        } else {
                            maskOffsetY = 0.0f;
                        }
                    }
                }else{
                    // Mirror malus
                    controlsInverted = true;

                    mirrorTimer += deltaTime;

                    // For 5 seconds
                    if (mirrorTimer >= 5.0f) {
                        showMagicMask = false;     // Mirror is hidden
                        controlsInverted = false;  // Malus disactivated
                        mirrorTimer = 0.0f;
                        maskVanish.playSound();    // Vanishing of mirror
                    }
                }
            }

            renderGame(window,player, backGroundGame, texLives3, texLives2, texLives1, currentTilt, mirrorTimer);
        }
        else if (currentState == GAME_OVER_ANIMATION) {
            gameOverTimer += deltaTime;

            // GameOver Background
            glDisable(GL_DEPTH_TEST);
            menuBGShader.use();
            glm::mat4 projectionBG = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
            menuBGShader.setMat4("projection", projectionBG);
            sprite2D.DrawSprite(backGroundGame, glm::vec2(0.0f, 0.0f), glm::vec2(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT)));

            // Render of the rain of poisoned apples
            glEnable(GL_DEPTH_TEST);
            gameShader.use();

            // Setup Camera
            glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            gameShader.setMat4("view", view);
            gameShader.setMat4("projection", projection);

            gameShader.setVec3("lightPos", glm::vec3(0.0f, 10.0f, 5.0f));
            gameShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
            gameShader.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 8.0f));

            // Colour
            gameShader.setBool("useTexture", false);
            gameShader.setVec3("objectColor", glm::vec3(0.4f, 0.0f, 0.6f));
            gameShader.setFloat("specularStrength", 0.5f);

            for (auto& apple : endApples) {
                if (!apple.isLanded) {
                    // Falling
                    apple.position.y -= 12.0f * deltaTime;

                    // Caotic rotation
                    apple.currentRollAngle += 200.0f * deltaTime;

                    // If it is landed
                    if (apple.position.y <= -2.5f) {
                        apple.isLanded = true;
                        apple.position.y = -2.5f;

                        // Random direction and speed
                        float direction = (rand() % 2 == 0) ? 1.0f : -1.0f;
                        float speed = 2.0f + static_cast<float>(rand() % 300) / 100.0f;

                        apple.rollSpeed = direction * speed;
                    }
                }
                else {
                    // Horizontal rotation on the floor
                    apple.position.x += apple.rollSpeed * deltaTime;

                    apple.currentRollAngle -= apple.rollSpeed * 150.0f * deltaTime;
                }

                // Draw
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, apple.position);

                if (!apple.isLanded) {
                    // Rotation when it is falling
                    model = glm::rotate(model, glm::radians(apple.currentRollAngle), glm::vec3(1.0f, 1.0f, 0.0f));
                } else {
                    // Rotation on z axis
                    model = glm::rotate(model, glm::radians(apple.currentRollAngle), glm::vec3(0.0f, 0.0f, 1.0f));
                }
                model = glm::scale(model, glm::vec3(0.08f));
                gameShader.setMat4("model", model);

                appleModel.Draw(gameShader);
            }

            // GameOver render text
            glDisable(GL_DEPTH_TEST);
            shaderText.use();
            glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
            shaderText.setMat4("projection", projectionText);

            std::string text = "GAME OVER";
            float scale = 3.0f;

            float textX = (SCR_WIDTH / 2.0f) - 450.0f;
            float textY = (SCR_HEIGHT / 2.0f);

            // Colour
            RenderText(shaderText, text, textX, textY, scale, glm::vec3(0.8f, 0.0f, 0.0f));

            // After gameover animation
            if (gameOverTimer >= gameOverDuration) {
             if (HighScoreManager::isNewHighScore(score, SCORE_FILE)) {
                    currentState = INPUT_NAME;
                    currentInputName = "";
                    keyProcessed = true;
                } else {
                    currentState = MENU;
                    highScoresList = HighScoreManager::loadScores(SCORE_FILE);
                    menuMusic.playSound();
                    lbButton.clicked = false;
                    startButton.clicked = false;
                }
            }
        } else if (currentState == INPUT_NAME)
        {
            glDisable(GL_DEPTH_TEST);

            // Background
            menuBGShader.use();
            glm::mat4 projectionBG = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
            menuBGShader.setMat4("projection", projectionBG);
            sprite2D.DrawSprite(backGroundInputName, glm::vec2(0, 0), glm::vec2(SCR_WIDTH, SCR_HEIGHT));

            // Text
            shaderText.use();
            glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
            shaderText.setMat4("projection", projectionText);

            // Render Text
            RenderText(shaderText, "ENTER THE LEADERBOARD", SCR_WIDTH/2 - 400, SCR_HEIGHT/2 + 150, 1.2f, glm::vec3(1.0f, 0.0f, 0.0f));
            RenderText(shaderText, "Score: " + std::to_string(score), SCR_WIDTH/2 - 100, SCR_HEIGHT/2+ 50, 1.0f, glm::vec3(1.0f,0.0f, 0.0f));
            RenderText(shaderText, "Insert 3 letters: " + currentInputName, SCR_WIDTH/2 - 280, SCR_HEIGHT/2 - 50, 1.2f, glm::vec3(1.0f));

            if (currentInputName.length() >= 3) {
                RenderText(shaderText, "Press ENTER", SCR_WIDTH/2 - 150, SCR_HEIGHT/2 - 150, 1.0f, glm::vec3(1.0f));
            }
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