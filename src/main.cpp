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

double lastX = WINDOW_WITDTH / 2.0;
double lastY = WINDOW_HEIGHT / 2.0;
bool firstMouse = true;
bool xKeyPressed = false;
bool wireframeMode = false;

bool compareVec3(const glm::vec3 &v1, const glm::vec3 &v2) {
    return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

void processInput(GLFWwindow* window, Camera& cam, float deltaTime) {
    // const glm::vec3 &firstCamPos = *(cam.getPosition());
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
    const glm::vec3 &secondCamPos = *(cam.getPosition());
    // if (!(compareVec3(firstCamPos, secondCamPos)))
    // std::cout << "Position: (" << secondCamPos.x << ", " << secondCamPos.y << ", " << secondCamPos.z << ")" << std::endl;
    

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

    float xoffset = (float)(xpos - lastX);
    float yoffset = (float)(lastY - ypos); // Inversé puisque y-coordonnées vont de bas en haut
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Camera* camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->rotate(xoffset, yoffset);
}


int main(int argc, char **argv) {
    BuildMode buildMode = GREEDY;
    if (argc >= 2 && strcmp(argv[1], "CLASSIC") == 0)
        buildMode = CLASSIC;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Meinkraft - 0 FPS", NULL, NULL);
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

    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    Shader shaderProgram("..\\..\\shaders\\mesh.vtx.glsl", "..\\..\\shaders\\mesh.frg.glsl");

    int height, width;
    glfwGetWindowSize(window, &width, &height);
    Camera cam(height, width);
    cam.setPosition(glm::vec3(0, 320, 0));
    cam.setSpeed(50.0);

    glfwSetWindowUserPointer(window, &cam);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    World world(cam.getPosition());
    world.build(buildMode);
    std::cout << "Faces count: " << world.facesCount << std::endl;

    Texture texture("..\\..\\textures\\atlas.png");
    texture.bind();

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CW);

    glClearColor(0.4f, 0.4f, 0.8f, 1.0f);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window, cam, (float)deltaTime);

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
