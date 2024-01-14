#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <camera.h>
#include <cube_mesh.h>
#include <graphics/shader.h>
#include <graphics/texture.h>

#include <chunk.h>
#include <world.h>

#define WINDOW_HEIGHT 900
#define WINDOW_WITDTH 1440

float lastX = WINDOW_WITDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
bool xKeyPressed = false;
bool wireframeMode = false;

void processInput(GLFWwindow* window, Camera& cam, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.moveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.moveRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.moveDown(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.moveUp(deltaTime);

    bool xKeyDown = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
	if (xKeyDown && !xKeyPressed) {
		wireframeMode = !wireframeMode;
		if (wireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	xKeyPressed = xKeyDown;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose (window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    Camera* camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Inversé puisque y-coordonnées vont de bas en haut
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Camera* camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->rotate(xoffset, yoffset);
}


int createAndBindMesh(GLuint VAO) {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    buildCube(-0.5, -0.5, -0.5, vertices, indices);

    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    return indices.size();
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Meinkraft - 10000 FPS", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and compile our shader program
    Shader shaderProgram("C:\\Users\\justi\\Desktop\\Meinkraft2\\OM3D_PROJECT\\shaders\\mesh.vtx.glsl", "C:\\Users\\justi\\Desktop\\Meinkraft2\\OM3D_PROJECT\\shaders\\mesh.frg.glsl");

    int height, width;
    glfwGetWindowSize(window, &width, &height);
    Camera cam(height, width);
    cam.setPosition(glm::vec3(16.0, 33.0, 16.0));

    glfwSetWindowUserPointer(window, &cam);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    World world;
    world.build();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW); 

    double lastFrameTime = glfwGetTime();
    double lastFPSTime = lastFrameTime;
    int fpsCounter = 0;
    int fps = 0;

    while (!glfwWindowShouldClose(window)) {
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Compteur de FPS
        fpsCounter++;
        if (currentFrameTime - lastFPSTime >= 1.0) { // Mise à jour chaque seconde
            fps = fpsCounter;
            fpsCounter = 0;
            lastFPSTime = currentFrameTime;

            // Afficher les FPS (par exemple, dans le titre de la fenêtre)
            std::string windowTitle = "Meinkraft - " + std::to_string(fps) + " FPS";
            glfwSetWindowTitle(window, windowTitle.c_str());
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window, cam, deltaTime);

        shaderProgram.use();
        shaderProgram.setMat4("view", cam.getViewMatrix());
        shaderProgram.setMat4("projection", cam.getProjectionMatrix());
        world.render(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
