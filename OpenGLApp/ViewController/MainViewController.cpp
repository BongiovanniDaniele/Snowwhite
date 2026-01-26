
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

    if (lbButton.selected == false) {
        sprite2D.DrawSprite(lbTexture, glm::vec2(lbButton.x, lbButton.y), glm::vec2(lbButton.width, lbButton.height));
    }
    else {
        sprite2D.DrawSprite(lbTextureSelected, glm::vec2(lbButton.x, lbButton.y), glm::vec2(lbButton.width, lbButton.height));
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

void renderGame(GLFWwindow* window, const Player player, Texture2D& bgTexture,Texture2D& life3, Texture2D& life2, Texture2D& life1, float tiltAngle, float mirrorTimer) {
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
    modelPlayer = glm::rotate(modelPlayer, glm::radians(tiltAngle), glm::vec3(0.0f, 0.0f, 1.0f));
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

            if (!obj.isLanded) {
                // IN ARIA: Ruota su se stessa
                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            } else {
                // 2. Ora ruotiamo (la mela girerà attorno al suo vero centro)
                model = glm::rotate(model, glm::radians(obj.currentRollAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            }

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
            else if (obj.type == AMPOULE_GOLD) {
                // --- RENDERING AMPOLLA D'ORO ---
                gameShader.setBool("useTexture", false);

                // Colore ORO (Gold): R=1.0, G=0.84, B=0.0
                gameShader.setVec3("objectColor", glm::vec3(1.0f, 0.84f, 0.0f));

                // Parametri per effetto metallico lucente
                gameShader.setFloat("specularStrength", 2.0f); // Molto riflettente
                gameShader.setFloat("emissionStrength", 0.3f); // Brilla di luce propria

                // Riutilizziamo il modello dell'ampolla
                ampouleModel.Draw(gameShader);
            }
            else if (obj.type == AMPOULE) {
                // --- NUOVO BLOCCO AMPOLLA ---
                gameShader.setBool("useTexture", false);

                // Colore Azzurro Vetro / Ciano Chiaro
                gameShader.setVec3("objectColor", glm::vec3(0.2f, 1.0f, 0.2f));

                // Parametri per effetto "Vetro"
                gameShader.setFloat("specularStrength", 1.5f); // Molto lucido
                gameShader.setFloat("emissionStrength", 0.1f); // Leggera luminosità interna

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
            glm::mat4 modelMask = glm::mat4(1.0f);
            modelMask = glm::translate(modelMask, glm::vec3(0.0f, maskOffsetY, -1.0f));
            modelMask = glm::rotate(modelMask, glm::radians((float)sin(glfwGetTime()*2.0f)*5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMask = glm::scale(modelMask, glm::vec3(0.35f));
            gameShader.setMat4("model", modelMask);
            gameShader.setVec3("objectColor", glm::vec3(0.9f, 0.9f, 0.9f)); // Bianco Maschera
            gameShader.setFloat("specularStrength", 0.3f);
            gameShader.setFloat("emissionStrength", 0.05f);

            maskModel.Draw(gameShader);
        }else
        {
            glm::mat4 modelMirror = glm::mat4(1.0f);

            // POSIZIONE: Fisso al centro (0.0), un po' in alto (es. 0.0f o 0.2f su Y) e avanti (0.5f)
            modelMirror = glm::translate(modelMirror, glm::vec3(0.0f, 0.2f, 0.5f));

            // ROTAZIONE: Ruota su se stesso continuamente
            // Usiamo glfwGetTime() * velocità (es. 2.0f)
            modelMirror = glm::rotate(modelMirror, (float)glfwGetTime() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            // Aggiungiamo una leggera rotazione anche su X per dinamismo
            modelMirror = glm::rotate(modelMirror, (float)glfwGetTime() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

            // SCALA: Parte da 0.30f e diventa 0.0f in 5 secondi
            // Formula: BaseScale * (1.0 - (TempoPassato / DurataTotale))
            float scaleFactor = 0.30f * (1.0f - (mirrorTimer / 5.0f));
            if (scaleFactor < 0.0f) scaleFactor = 0.0f; // Evita valori negativi

            modelMirror = glm::scale(modelMirror, glm::vec3(scaleFactor));

            gameShader.setMat4("model", modelMirror);

            // COLORE: Oro Scuro / Antico
            // R=0.7, G=0.5, B=0.1
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
    // Centrato orizzontalmente
    lbButton.x = (static_cast<float>(SCR_WIDTH) / 2.0f) - (lbButton.width / 2.0f);
    // Posizionato SOTTO il tasto play con un po' di padding (es. +20 pixel o relativo allo schermo)
    lbButton.y = startButton.y + startButton.height + (10.0f * scaleScreen);

    // 1. Grandezza: Uguagliamo le dimensioni del tasto Start (che usa il moltiplicatore 1.4f)
    backButton.width = buttonWidth * 1.4f;
    backButton.height = buttonHeight * 1.4f;

    // 2. Centratura Orizzontale Corretta
    // Formula: (Centro Schermo) - (Metà Larghezza Pulsante)
    backButton.x = (static_cast<float>(SCR_WIDTH) / 2.0f) - (backButton.width / 2.0f);
    // 3. Posizione Verticale
    // Lo alziamo un po' (es. 100px dal fondo invece di 50px)
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


    std::vector<FallingObject> endApples; // Mele per l'animazione finale
    float gameOverTimer = 0.0f;           // Timer per durare qualche secondo
    float gameOverDuration = 4.0f;        // Durata totale animazione

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (deltaTime > 0.05f) deltaTime = 0.05f;

        // input
        // -----
        processInput(window);
        // render
        // ------


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
            // --- RESET VARIABILI LOCALI ---
            // Le forziamo ai valori iniziali costantemente mentre siamo nel menu
            totalGameTime = 0.0f;
            level = 1;
            currentSpawnInterval = 1.8f; // Valore del livello 1
            currentFallSpeed = 6.0f;     // Valore del livello 1
            poisonThreshold = 30;
            alternateMiss = false;
            isSpeedBonusActive = false;
            speedBonusTimer = 0.0f;
            playerVelocityX = 0.0f;
            // ------------------------------

            gameOverTimer = 0.0f;
            gameOverDuration = 4.0f;        // Durata totale animazione

            glDisable(GL_DEPTH_TEST);
            renderMenu(window, startButtonTexture, startButtonTextureSelected, lbTexture, lbTextureSelected, backGroundMenu);
        }else if (currentState == LEADERBOARD) { // <--- NUOVO STATO
            glDisable(GL_DEPTH_TEST);
            menuBGShader.use();
            glm::mat4 projectionBG = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
            menuBGShader.setMat4("projection", projectionBG);

            // Disegna solo lo sfondo Input_Name come richiesto (temporaneamente)
            sprite2D.DrawSprite(backGroundInputName, glm::vec2(0, 0), glm::vec2(SCR_WIDTH, SCR_HEIGHT));

            // Opzionale: Aggiungi una scritta "Press ESC to return"
            shaderText.use();
            glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
            shaderText.setMat4("projection", projectionText);
            // --- TITOLO ---
            // Hai spostato il titolo giù di 200 pixel
            float titleBaseY = static_cast<float>(SCR_HEIGHT) * 0.85f;
            float titleActualY = titleBaseY - 200.0f; // Salviamo la posizione vera del titolo

            RenderText(shaderText, "HALL OF FAME", (SCR_WIDTH / 2.0f) - 250.0f, titleActualY, 1.5f, glm::vec3(1.0f, 0.84f, 0.0f));

            // --- CICLO SCORE ---
            // startY deve partire SOTTO il titolo.
            // Usiamo la posizione del titolo e scendiamo di altri 80-100 pixel.
            float startY = titleActualY - 75.0f;

            float stepY = 55.0f * scaleScreen;

            for (size_t i = 0; i < highScoresList.size(); ++i) {
                // Assegna colori diversi per i primi 3 posti
                glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f); // Bianco di default

                if (i == 0) color = glm::vec3(1.0f, 0.84f, 0.0f);      // 1° Oro
                else if (i == 1) color = glm::vec3(0.75f, 0.75f, 0.75f); // 2° Argento
                else if (i == 2) color = glm::vec3(0.8f, 0.5f, 0.2f);    // 3° Bronzo

                // Formatta la stringa: "1. AAA - 1500"
                std::string line = std::to_string(i + 1) + ". " + highScoresList[i].name + " - " + std::to_string(highScoresList[i].score);

                // Calcola posizione X centrata approssimativamente
                // Modifica il "150.0f" per spostare tutto il blocco a destra o sinistra
                float textX = (SCR_WIDTH / 2.0f) - 150.0f;
                float textY = startY - (i * stepY);

                RenderText(shaderText, line, textX, textY, 1.0f, color);
            }
            // --- AGGIUNGI DISEGNO PULSANTE BACK ---
            // Usiamo lo shader delle immagini 2D (menuBGShader)
            menuBGShader.use();
            // Reimpostiamo la projection per le immagini (se necessario, ma dovrebbe essere già settata all'inizio del blocco)
            glm::mat4 projectionButtons = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
            menuBGShader.setMat4("projection", projectionButtons);

            if (backButton.selected == false) {
                sprite2D.DrawSprite(backBtnUnselected, glm::vec2(backButton.x, backButton.y), glm::vec2(backButton.width, backButton.height));
            }
            else {
                sprite2D.DrawSprite(backBtnSelected, glm::vec2(backButton.x, backButton.y), glm::vec2(backButton.width, backButton.height));
            }
        } else if (currentState == GAME) {
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
                // LIVELLO FACILE
                currentSpawnInterval = 1.8f; // Lento
                baseSpawnInterval= 1.8f;
                currentFallSpeed = 6.0f;     // Caduta tranquilla
                baseFallSpeed= 6.0f;
                poisonThreshold = 30;        // ~20% Veleno (considerando 10% gemme)
            }
            else if (level==2) {
                // LIVELLO MEDIO
                currentSpawnInterval = 1.2f; // Più frequente
                baseSpawnInterval= 1.2f;
                currentFallSpeed = 8.5f;     // Più veloce
                baseFallSpeed = 8.5f;
                poisonThreshold = 50;        // ~40% Veleno
            }
            else {// LIVELLO DIFFICILE (INCUBO)
                currentSpawnInterval = 0.9f; // Pioggia di oggetti!
                baseSpawnInterval = 0.9f;
                currentFallSpeed = 10.0f;    // Velocissimi
                baseFallSpeed = 10.0f;
                poisonThreshold = 70;        // ~60% Veleno
            }

            if (isSpeedBonusActive) {
                // Se il bonus è attivo, forziamo la velocità del livello 1
                currentFallSpeed = 6.0f;
                currentSpawnInterval = 1.8f;
                // Aggiorniamo il timer
                speedBonusTimer += deltaTime;

                // Se sono passati 8 secondi, disattiviamo il bonus
                if (speedBonusTimer >= BONUS_DURATION) {
                    isSpeedBonusActive = false;
                    speedBonusTimer = 0.0f;
                    // Al prossimo frame tornerà a baseFallSpeed automaticamente
                }
            } else {
                // Se non c'è bonus, usiamo la velocità del livello corrente
                currentFallSpeed = baseFallSpeed;
                currentSpawnInterval = baseSpawnInterval;
            }

            //float interpolationFactor = player_speed * deltaTime;
            float stiffness = 110.0f; // Rigidità della molla (più alto = scatto più veloce)
            float damping = 9.0f;     // Smorzamento (più basso = più rimbalzi/overshoot)

            // 1. Calcola la distanza dal bersaglio
            float displacement = target_x_position - player.position.x;

            // 2. Calcola la forza della molla (Hooke's Law + attrito)
            float force = (displacement * stiffness) - (playerVelocityX * damping);

            // 3. Applica fisica
            playerVelocityX += force * deltaTime;
            player.position.x += playerVelocityX * deltaTime;

            // 4. Calcola INCLINAZIONE BASATA SULLA VELOCITÀ
            // Se va veloce a destra, si inclina indietro (effetto cartone animato)
            // Moltiplica per -3.0f o -5.0f per aumentare l'inclinazione
            float currentTilt = playerVelocityX * -3.5f;

            // Clamp per evitare che si cappotti (massimo 30 gradi)
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
                    newType = AMPOULE_GOLD; // 5% Probabilità Ampolla D'ORO (Molto rara)
                }
                else if (randomType < 13) {
                    newType = GEM; // 8% Probabilità Gemma (spostato range)
                }
                else if (randomType < 25) {
                    newType = AMPOULE; // Ampolla cattiva
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
                    // --- FASE 1: GESTIONE MOVIMENTO (Caduta vs Rotolamento) ---
                    if (!obj.isLanded) {
                        // Cade verso il basso
                        obj.position.y -= currentFallSpeed * deltaTime;
                    }
                    else {
                        // E' a terra: ROTOLA!
                        obj.landTimer += deltaTime;

                        // Muovi la mela a destra o sinistra
                        obj.position.x += obj.rollSpeed * deltaTime;

                        // Ruota la mela in base alla velocità (effetto ruota)
                        // Moltiplichiamo per 150.0f per farla girare velocemente
                        obj.currentRollAngle -= obj.rollSpeed * deltaTime * 150.0f;


                        if (obj.landTimer > 2.0f) {
                            obj.active = false;
                        }
                    }
                    // --- FASE 2: COLLISIONE COL GIOCATORE (Solo mentre cade) ---
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
                                speedBonusTimer = 0.0f; // Resetta il timer se ne prendi un'altra mentre è già attiva
                                score += 2;
                                glass.playSound(); // O usa un suono specifico se ne hai uno
                            }
                            else if (obj.type == APPLE_POISON) {
                                score -= 5;
                                badApple.playSound();
                            }
                            else if (obj.type == AMPOULE) { // <--- NUOVO CASO
                                // Toglie una vita
                                lives--;
                                // Usiamo il suono del veleno o della risata
                                missed1.playSound();
                            }
                            else { // APPLE_GOOD
                                score += 1;
                                goodApple.playSound();
                            }
                            obj.active = false; // Presa!
                        }
                    }

                    // --- FASE 3: TOCCO TERRA (Inizio Rotolamento) ---
                    if (!obj.isLanded && obj.position.y < -2.5f) {
                        obj.isLanded = true;
                        obj.position.y = -2.5f; // Fissa al pavimento

                        // Calcola una direzione casuale: -1 (Sinistra) o 1 (Destra)
                        float direction = (rand() % 2 == 0) ? 1.0f : -1.0f;

                        // Assegna velocità casuale tra 1.0 e 2.5
                        float speed = 1.0f + static_cast<float>(rand() % 150) / 100.0f;

                        obj.rollSpeed = direction * speed;

                        // Piccolo offset Z per evitare "sfarfallii" se due mele si incrociano
                        obj.offsetZ = ((rand() % 60) / 100.0f) - 0.30f;
                        obj.position.z += obj.offsetZ;

                        // Applica penalità
                        if (obj.type == APPLE_GOOD) {
                            lives--;
                            if (alternateMiss) missed2.playSound();
                            else missed1.playSound();
                            alternateMiss = !alternateMiss;
                        }
                    }
                }
            }

            if (lives <= 0) {
                gameMusic.stopSound();

                currentState=GAME_OVER_ANIMATION;
                gameOverTimer=0.0f;
                endApples.clear();

                for (int i = 0; i < 50; i++) {
                    // Posizione X casuale ma centrata (per sembrare che escano dalla scritta)
                    // Range tra -1.5 e 1.5
                    float randomX = ((rand() % 800) / 100.0f) - 4.0f;

                    // Posizione Y sfalsata (così non cadono tutte insieme come un muro)
                    // Partono dall'alto (da 5.0f a 15.0f)
                    float randomY = 5.0f + ((rand() % 1000) / 100.0f);

                    // Posizione Z un po' random per dare profondità
                    float randomZ = ((rand() % 100) / 100.0f) - 0.5f;

                    // Creiamo l'oggetto (usiamo APPLE_POISON per farle viola o APPLE_GOOD per rosse)
                    // Qui uso APPLE_POISON per dare l'idea di "Game Over" cupo
                    endApples.push_back(FallingObject(glm::vec3(randomX, randomY, randomZ), APPLE_POISON));
                }
                // Fermiamo e puliamo la scena "visiva" ma NON resettiamo score/vite
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
                        maskSpeech.playSound(); // Suona SOLO all'inizio della Maschera
                    } else {
                        sfxMirror.playSound();
                    }

                }
            } else {
                if (activeDistractionType == 0)
                {
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
                    // Attiva Malus
                    controlsInverted = true;

                    // Avanza Timer
                    mirrorTimer += deltaTime;

                    // Durata 5 Secondi
                    if (mirrorTimer >= 5.0f) {
                        showMagicMask = false;     // Nascondi specchio
                        controlsInverted = false;  // Disattiva malus
                        mirrorTimer = 0.0f;
                        maskVanish.playSound();    // Suono scomparsa
                    }
                }
            }

            renderGame(window,player, backGroundGame, texLives3, texLives2, texLives1, currentTilt, mirrorTimer);
        } else if (currentState == GAME_OVER_ANIMATION) {
            // 1. Avanza Timer
            gameOverTimer += deltaTime;

            // 2. Disegna Sfondo Gioco (come richiesto)
            glDisable(GL_DEPTH_TEST);
            menuBGShader.use();
            glm::mat4 projectionBG = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
            menuBGShader.setMat4("projection", projectionBG);
            sprite2D.DrawSprite(backGroundGame, glm::vec2(0.0f, 0.0f), glm::vec2(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT)));

            // 3. Disegna le Mele che cadono (3D)
            glEnable(GL_DEPTH_TEST);
            gameShader.use();

            // Setup Camera (fissa)
            glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            gameShader.setMat4("view", view);
            gameShader.setMat4("projection", projection);

            gameShader.setVec3("lightPos", glm::vec3(0.0f, 10.0f, 5.0f));
            gameShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
            gameShader.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 8.0f));

            // Colore per le mele finali (Viola o Rosso)
            gameShader.setBool("useTexture", false);
            gameShader.setVec3("objectColor", glm::vec3(0.4f, 0.0f, 0.6f)); // Viola scuro
            gameShader.setFloat("specularStrength", 0.5f);

            for (auto& apple : endApples) {
                if (!apple.isLanded) {
                    // FASE 1: CADUTA
                    apple.position.y -= 12.0f * deltaTime; // Caduta veloce

                    // Rotazione "caotica" mentre cade
                    apple.currentRollAngle += 200.0f * deltaTime;

                    // CONTROLLO IMPATTO COL SUOLO (Usiamo -2.5f come pavimento)
                    if (apple.position.y <= -2.5f) {
                        apple.isLanded = true;
                        apple.position.y = -2.5f; // Fissa al pavimento

                        // Genera direzione casuale: -1 (Sinistra) o 1 (Destra)
                        float direction = (rand() % 2 == 0) ? 1.0f : -1.0f;

                        // Genera velocità casuale
                        float speed = 2.0f + static_cast<float>(rand() % 300) / 100.0f;

                        apple.rollSpeed = direction * speed;
                    }
                }
                else {
                    // FASE 2: ROTOLAMENTO ORIZZONTALE
                    apple.position.x += apple.rollSpeed * deltaTime;

                    // Rotazione "da ruota" (segno meno per farla girare nel verso giusto rispetto al movimento)
                    // Moltiplico per 100 per renderla visibile
                    apple.currentRollAngle -= apple.rollSpeed * 150.0f * deltaTime;
                }

                // Disegna
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, apple.position);

                if (!apple.isLanded) {
                    // Mentre cade ruota in modo disordinato (su asse obliquo)
                    model = glm::rotate(model, glm::radians(apple.currentRollAngle), glm::vec3(1.0f, 1.0f, 0.0f));
                } else {
                    // Quando rotola, ruota SOLO sull'asse Z (come una ruota di un'auto vista di lato)
                    model = glm::rotate(model, glm::radians(apple.currentRollAngle), glm::vec3(0.0f, 0.0f, 1.0f));
                }
                model = glm::scale(model, glm::vec3(0.08f)); // Un po' più grandi magari
                gameShader.setMat4("model", model);

                appleModel.Draw(gameShader);
            }

            // 4. Disegna la scritta "GAME OVER" sopra tutto (2D)
            glDisable(GL_DEPTH_TEST);
            shaderText.use();
            glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
            shaderText.setMat4("projection", projectionText);

            std::string text = "GAME OVER";
            float scale = 3.0f; // Scritta grande
            // Centriamo la scritta (approssimativo)
            float textX = (SCR_WIDTH / 2.0f) - 450.0f;
            float textY = (SCR_HEIGHT / 2.0f);

            // Scritta Rossa Sangue
            RenderText(shaderText, text, textX, textY, scale, glm::vec3(0.8f, 0.0f, 0.0f));

            // 5. Fine Animazione -> Vai al prossimo stato
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
            // 1. Disabilitiamo la profondità per disegnare in 2D
            glDisable(GL_DEPTH_TEST);

            // 2. Setup Sfondo
            menuBGShader.use();
            glm::mat4 projectionBG = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
            menuBGShader.setMat4("projection", projectionBG);
            sprite2D.DrawSprite(backGroundInputName, glm::vec2(0, 0), glm::vec2(SCR_WIDTH, SCR_HEIGHT));

            // 3. Setup Testo (QUESTO MANCAVA!)
            shaderText.use();
            // La proiezione per il testo è diversa (0,0 in basso a sinistra) rispetto agli sprite (0,0 in alto a sinistra)
            glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
            shaderText.setMat4("projection", projectionText); // <--- FONDAMENTALE

            // 4. Disegno Testo
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