

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "configuration/root_directory.h"
#include "model.h"
#include <ft2build.h>
#include "font.h"


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(vector<std::string> faces );
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


// other
float mix =0.0f;
bool wires = false;

int main(int argc, char** argv)
{


    std::string Path(logl_root);
    Path.append("/");
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    Shader* simpleShader = &ResourceManager::LoadShader("shaders/simple.vs.glsl","shaders/simple.fs.glsl", nullptr, "simple");//new Shader((Path + "shaders/simple.vs.glsl").c_str(),(Path + "shaders/simple.fs.glsl").c_str());
    Shader* lightingShader = &ResourceManager::LoadShader("shaders/colors.vs.glsl", "shaders/colors.fs.glsl", nullptr, "colors");
    Shader* lampShader = &ResourceManager::LoadShader("shaders/lamp.vs.glsl", "shaders/lamp.fs.glsl", nullptr, "lamp");
    Shader* plainShader = &ResourceManager::LoadShader("shaders/plain.vs.glsl", "shaders/plain.fs.glsl", nullptr,"plain");
    Shader* skyboxShader = &ResourceManager::LoadShader("shaders/skybox.vs.glsl", "shaders/skybox.fs.glsl", nullptr, "skybox");
     // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float plainVertices[] = {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f
    };

    float vertices[] = {
            // positions          // normals           // texture coords
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

    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    float points[] = {
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
            0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
    };

    unsigned int pointsVBO, pointsVAO;
    glGenVertexArrays(1, &pointsVAO);
    glGenBuffers(1, &pointsVBO);
    glBindVertexArray(pointsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);


    Model Model("resources/nanosuit/nanosuit.obj");


    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    vector<std::string> faces
            {
                    "resources/skybox/violentdays_rt.tga",
                    "resources/skybox/violentdays_lf.tga",
                    "resources/skybox/violentdays_up.tga",
                    "resources/skybox/violentdays_dn.tga",
                    "resources/skybox/violentdays_bk.tga",
                    "resources/skybox/violentdays_ft.tga"
            };
    unsigned int cubemapTexture = loadCubemap(faces);
    unsigned int cubemapVBO, cubemapVAO;
    glGenVertexArrays(1, &cubemapVAO);
    glGenBuffers(1, &cubemapVBO);
    glBindVertexArray(cubemapVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int plainVBO, plainVAO;
    glGenBuffers(1, &plainVBO);
    glGenVertexArrays(1, &plainVAO);
    glBindVertexArray(plainVAO);

    glBindBuffer(GL_ARRAY_BUFFER, plainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plainVertices), plainVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int VBO, boxVAO;
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(boxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the boxVAO afterwards so other boxVAO calls won't accidentally modify this boxVAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the correct data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attributes (only position data for our lamp)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int FBO;
    glGenFramebuffers(1,&FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    Font font("fonts/arial.ttf");
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4 model;
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


  // also draw the lamp object


        lampShader->use();

        lampShader->setMat4("projection", projection);
        lampShader->setMat4("view", view);



        glBindVertexArray(lightVAO);


        for(int i=0; i<4;i++){
            model = glm::mat4();
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
            lampShader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        glEnable(GL_STENCIL_TEST);
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader->use();

        // material properties
        lightingShader->setInt("material.diffuse", 0);
        lightingShader->setInt("material.specular", 1);
        lightingShader->setInt("material.reflect", 2);


        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        lightingShader->setInt("skybox",3);
        lightingShader->setFloat("material.shininess", 64.0f);

        lightingShader->setVec3("dirLight.ambient",  0.2f, 0.2f, 0.2f);
        lightingShader->setVec3("dirLight.diffuse",  0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
        lightingShader->setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);

        lightingShader->setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader->setVec3("pointLights[0].diffuse", 0.0f, 0.0f, 0.8f);
        lightingShader->setVec3("pointLights[0].specular", 0.0f, 0.0f, 1.0f);
        lightingShader->setFloat("pointLights[0].constant", 1.0f);
        lightingShader->setFloat("pointLights[0].linear", 0.09);
        lightingShader->setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        lightingShader->setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader->setVec3("pointLights[1].diffuse", 0.0f, 0.0f, 0.8f);
        lightingShader->setVec3("pointLights[1].specular", 0.0f, 0.0f, 1.0f);
        lightingShader->setFloat("pointLights[1].constant", 1.0f);
        lightingShader->setFloat("pointLights[1].linear", 0.09);
        lightingShader->setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        lightingShader->setVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader->setVec3("pointLights[2].diffuse", 0.0f, 0.0f, 0.8f);
        lightingShader->setVec3("pointLights[2].specular", 0.0f, 0.0f, 1.0f);
        lightingShader->setFloat("pointLights[2].constant", 1.0f);
        lightingShader->setFloat("pointLights[2].linear", 0.09);
        lightingShader->setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        lightingShader->setVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader->setVec3("pointLights[3].diffuse", 0.0f, 0.0f, 0.8f);
        lightingShader->setVec3("pointLights[3].specular", 0.0f, 0.0f, 1.0f);
        lightingShader->setFloat("pointLights[3].constant", 1.0f);
        lightingShader->setFloat("pointLights[3].linear", 0.09);
        lightingShader->setFloat("pointLights[3].quadratic", 0.032);

        lightingShader->setVec3("spotLight.position", camera.Position);
        lightingShader->setVec3("spotLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader->setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader->setFloat("spotLight.constant", 1.0f);
        lightingShader->setFloat("spotLight.linear", 0.09);
        lightingShader->setFloat("spotLight.quadratic", 0.032);
        lightingShader->setVec3("spotLight.direction", camera.Front);
        lightingShader->setFloat("spotLight.cutOff", cos(glm::radians(12.0f)));
        lightingShader->setFloat("spotLight.outerCutOff", cos(glm::radians(17.5f)));


        lightingShader->setVec3("viewPos", camera.Position);
        // view/projection transformations


        lightingShader->setMat4("projection", projection);
        lightingShader->setMat4("view", view);
        lightingShader->setFloat("time", currentFrame);

        model = glm::mat4(0.1);

        lightingShader->setMat4("model", model);
        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should update the stencil buffer
        glStencilMask(0xFF); // enable writing to the stencil buffer

        Model.Draw(*lightingShader);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00); // disable writing to the stencil buffer
        //glDisable(GL_DEPTH_TEST);
        simpleShader->use();
        model = glm::mat4(0.1);
        simpleShader->setMat4("model",model);
        simpleShader->setMat4("projection", projection);
        simpleShader->setMat4("view", view);

        //Model.Draw(*simpleShader);
        glEnable(GL_DEPTH_TEST);
        glStencilMask(0xFF);
        glDisable(GL_STENCIL_TEST);

        glDepthFunc(GL_LEQUAL);
        //glDepthMask(GL_FALSE);
        skyboxShader->use();
        skyboxShader->setInt("skybox", 0);
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        glBindVertexArray(cubemapVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        skyboxShader->setMat4("projection", projection);
        skyboxShader->setMat4("view", view);
        glDrawArrays(GL_TRIANGLES, 0 , 36);
        glDepthFunc(GL_LESS);
        //glDepthMask(GL_TRUE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        plainShader->use();
        glBindVertexArray(plainVAO);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glDrawArrays(GL_TRIANGLES,0,6);

        font.RenderText( "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        font.RenderText( "(Another text)", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &boxVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        mix +=0.005f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        mix -=0.005f;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        if (wires){
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        wires = !wires;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double Xoffset = xpos - lastX;
    double Yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(Xoffset, Yoffset);
}


unsigned int loadCubemap(vector<std::string> faces ){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for(unsigned int i=0; i<faces.size();i++){
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if(data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else{
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}