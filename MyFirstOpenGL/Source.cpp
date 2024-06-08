#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <stb_image.h>
#include "Mesh.h"
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "ProgramManager.h"
#include "Model.h"

#define WINDOW_WIDTH_DEFAULT 640
#define WINDOW_HEIGHT_DEFAULT 480

int windowWidth = WINDOW_WIDTH_DEFAULT;
int windowHeight = WINDOW_HEIGHT_DEFAULT;

std::vector <Model> models;

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {
    glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
    windowWidth = iFrameBufferWidth;
    windowHeight = iFrameBufferHeight;
}

int main() {
    // Crear cámara
    Camera mainCamera;

    //crear Sol
    GameObject sunPointLight;

    //crear Luna
    GameObject moonPointLight;

    //bool para enviar posicion sol o Luna
    bool sunActive;

    // Definir semillas del rand según el tiempo
    srand(static_cast<unsigned int>(time(NULL)));

    // Inicializamos GLFW para gestionar ventanas e inputs
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    // Configuramos la ventana
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Inicializamos la ventana
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "My Engine", NULL, NULL);
    if (!window) {
        std::cerr << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Iniciar Input Manager
    IM.Init(window);

    // Asignamos función de callback para cuando el frame buffer es modificado
    glfwSetFramebufferSizeCallback(window, Resize_Window);

    // Definimos espacio de trabajo
    glfwMakeContextCurrent(window);

    // Permitimos a GLEW usar funcionalidades experimentales
    glewExperimental = GL_TRUE;

    // Inicializamos GLEW y controlamos errores
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Activamos cull face
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Leer textura troll
    int width, height, nrChannels;
    unsigned char* textureInfo = stbi_load("Assets/Textures/troll.png", &width, &height, &nrChannels, 0);
    if (!textureInfo) {
        std::cerr << "Error al cargar la textura troll" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Leer textura piedra
    int widthRock, heightRock, nrChannelsRock;
    unsigned char* textureInfoRock = stbi_load("Assets/Textures/rock.png", &widthRock, &heightRock, &nrChannelsRock, 0);
    if (!textureInfoRock) {
        std::cerr << "Error al cargar la textura piedra" << std::endl;
        stbi_image_free(textureInfo);
        glfwTerminate();
        return -1;
    }

    // Crear y configurar texturas
    GLuint textureID, textureID2;
    glGenTextures(1, &textureID);
    glGenTextures(1, &textureID2);

    // Configurar primera textura
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureInfo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(textureInfo);

    // Configurar segunda textura
    glBindTexture(GL_TEXTURE_2D, textureID2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthRock, heightRock, 0, GL_RGB, GL_UNSIGNED_BYTE, textureInfoRock);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(textureInfoRock);

    // Definimos color para limpiar el buffer de color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Definimos modo de dibujo para cada cara
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Compilar shaders y cargar modelos
    models.push_back(Model("MyFirstFragmentShader.glsl", "GeometryOfModels.glsl", "MyFirstVertexShader.glsl", "Assets/Models/troll.obj", "Assets/Materials/troll.mtl"));
    

    for (int i = 0; i < models.size(); i++) {
        glUseProgram(models[i].GetProgram());
        glUniform2f(glGetUniformLocation(models[i].GetProgram(), "windowSize"), windowWidth, windowHeight);
    }

    // Asignar valor variable textura a usar
    glUseProgram(models[0].GetProgram());
    glUniform1i(glGetUniformLocation(models[0].GetProgram(), "textureSampler"), 0);

    // Matrices de transformación de los modelos
    models[0].position = glm::vec3(0.f, -0.4f, 0.6f);
    models[0].rotation = glm::vec3(0.f, 180.f, 0.f);
    models[0].scale = glm::vec3(0.4f);

    sunPointLight.position = glm::vec3(0.f, 1.0f, 0.f);
    sunPointLight.radius = 1.f;
    sunPointLight.velocity = 18.f;

    moonPointLight.position = glm::vec3(0.f, -1.f, 0.f);
    moonPointLight.radius = 1.f;
    moonPointLight.velocity = 18.f;


    for (int i = 0; i < models.size(); i++) {
        models[i].GenerateAllMatrixs();
    }

    //variable para medir el tiempo
    double lastTime = glfwGetTime();
    double currentTime;
    float deltaTime;

    // Generamos el game loop
    while (!glfwWindowShouldClose(window)) {
        // Pulleamos los eventos (botones, teclas, mouse...)
        glfwPollEvents();

        // Input Manager Update
        IM.Update();

        // Camara state
        mainCamera.Update();

        // Depth test
        glEnable(GL_DEPTH_TEST);

        // Limpiamos los buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Matrices de la camara
        glm::mat4 viewMatrix = glm::lookAt(mainCamera.position, mainCamera.position + glm::vec3(0.f, 0.f, 1.f), mainCamera.localVectorUp);
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(mainCamera.fFov), (float)windowWidth / (float)windowHeight, mainCamera.fNear, mainCamera.fFar);

        // Pasar Uniforms
        for (int i = 0; i < models.size(); i++) {
            glUseProgram(models[i].GetProgram());
            glUniformMatrix4fv(glGetUniformLocation(models[i].GetProgram(), "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
            glUniformMatrix4fv(glGetUniformLocation(models[i].GetProgram(), "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        }

        // Actualizar primer troll
        models[0].Update();

        //calcular deltaTime
        currentTime = glfwGetTime();
        deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        //Update de Sun
        sunPointLight.Update(deltaTime);
        moonPointLight.Update(deltaTime);

        std::cout << sunPointLight.position.x << " " << sunPointLight.position.y << " " << sunPointLight.position.z << std::endl;

        //pasar la sourceLight
        if (sunPointLight.position.y < 0)
            sunActive = false;
        else
            sunActive = true;
        if (sunActive)
        {
            for (int i = 0; i < models.size(); i++)
            {
                glUniform3fv(glGetUniformLocation(models[i].GetProgram(), "sourceLight"), 1, glm::value_ptr(sunPointLight.position));
                glUniform1i(glGetUniformLocation(models[i].GetProgram(), "moonActive"), false);
            }
        }
        else if (!sunActive)
        {
            for (int i = 0; i < models.size(); i++)
            {
                glUniform3fv(glGetUniformLocation(models[i].GetProgram(), "sourceLight"), 1, glm::value_ptr(moonPointLight.position));
                glUniform1i(glGetUniformLocation(models[i].GetProgram(), "moonActive"), true);
            }

        }


        // Cambiar buffers
        glfwSwapBuffers(window);
    }

    // Liberar recursos de cada modelo
    for (int i = 0; i < models.size(); i++) {
        glUseProgram(0);
        glDeleteProgram(models[i].GetProgram());
    }

    // Eliminar texturas
    glDeleteTextures(1, &textureID);
    glDeleteTextures(1, &textureID2);

    // Finalizar GLFW
    glfwTerminate();

    return 0;
}






