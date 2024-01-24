#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <camera.h>
#include <cube_mesh.h>
#include <debug_color.h>
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
bool renderWater = true;
bool wKeyPressed = false;


bool compareVec3(const glm::vec3 &v1, const glm::vec3 &v2) {
    return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

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
    const glm::vec3 &secondCamPos = *(cam.getPosition());
    

    bool xKeyDown = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
	if (xKeyDown && !xKeyPressed) {
		wireframeMode = !wireframeMode;
		if (wireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	xKeyPressed = xKeyDown;

    bool wKeyDown = glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS;
	if (wKeyDown && !wKeyPressed) {
		renderWater = !renderWater;
	}
	wKeyPressed = wKeyDown;

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

void createOcean(GLuint &waterVAO, GLuint &waterVBO, GLuint &waterEBO, float waterLevel) {
    GLfloat dim = 100000.0;
    GLfloat hgt = waterLevel;
    GLfloat vertices[] = {
        -dim, hgt, -dim,// 0.0, 0.0,
         dim, hgt, -dim,// 1.0, 0.0,
         dim, hgt,  dim,// 1.0, 1.0,
        -dim, hgt,  dim,// 0.0, 1.0,
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    glGenVertexArrays(1, &waterVAO);
    glGenBuffers(1, &waterVBO);
    glGenBuffers(1, &waterEBO);

    glBindVertexArray(waterVAO);

    glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}


int main(int argc, char **argv) {
    BuildMode buildMode = GREEDY;             // Default mode
    int world_width = -WORLD_WIDTH;           // Default value, indicates not set
    int world_height = -WORLD_HEIGHT;         // Default value, indicates not set

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        // Check for mode argument
        if (arg == "GREEDY") {
            buildMode = GREEDY;
        } else if (arg == "CLASSIC") {
            buildMode = CLASSIC;
        } else {
            // Check for numeric arguments
            try {
                int number = std::stoi(arg);
                if (world_width == -WORLD_WIDTH) {
                    // First number found, set world_width
                    number = abs(number);
                    world_width = number;
                } else if (world_height == -WORLD_HEIGHT) {
                    // Second number found, set world_height
                    number = abs(number);
                    world_height = number;
                }
            } catch (std::invalid_argument const &e) {
                std::cerr << Color::RED << "Invalid number: " << arg << Color::RESET << std::endl;
            } catch (std::out_of_range const &e) {
                std::cerr << Color::RED << "Number out of range: " << arg << Color::RESET << std::endl;
            }
        }
    }

    // Output for verification
    std::cout << Color::YELLOW << "Build mode: " << (buildMode == GREEDY ? Color::GREEN + "GREEDY" : Color::RED + "CLASSIC") << Color::RESET << std::endl;
    std::cout << Color::YELLOW << "World width: " << (world_width != -WORLD_WIDTH ? Color::BLUE + std::to_string(world_width) : "Not set") << Color::RESET << std::endl;
    std::cout << Color::YELLOW << "World height: " << (world_height != -WORLD_HEIGHT ? Color::BLUE + std::to_string(world_height) : "Not set") << Color::RESET << std::endl;

    world_width = abs(world_width);
    world_height = abs(world_height);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OM3D - 0 FPS", NULL, NULL);
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shaderProgram("..\\..\\shaders\\mesh.vtx.glsl", "..\\..\\shaders\\mesh.frg.glsl");
    Shader waterShader("..\\..\\shaders\\water.vtx.glsl", "..\\..\\shaders\\water.frg.glsl");

    int height, width;
    glfwGetWindowSize(window, &width, &height);
    Camera cam(height, width);
    cam.setPosition(glm::vec3(0, 320, 0));
    cam.setSpeed(50.0);

    glfwSetWindowUserPointer(window, &cam);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    World world(cam.getPosition(), world_width, world_height);
    world.build(buildMode);
    std::cout << Color::YELLOW << "Face count: " << Color::BLUE << world.facesCount << Color::RESET << std::endl;

    GLuint waterVAO, waterVBO, waterEBO;
    float waterLevel = ((float)(world.getHeight() - 1) * (float)CHUNK_DIM) / 2.0f + 0.5f;
    createOcean(waterVAO, waterVBO, waterEBO, waterLevel);

    Texture texture("..\\..\\textures\\atlas.png");
    texture.bind();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glClearColor(0.4f, 0.4f, 0.8f, 1.0f);

    double lastFrameTime = glfwGetTime();
    double lastFPSTime = lastFrameTime;
    int fpsCounter = 0;
    int fps = 0;

    while (!glfwWindowShouldClose(window)) {
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        fpsCounter++;
        if (currentFrameTime - lastFPSTime >= 1.0) {
            fps = fpsCounter;
            fpsCounter = 0;
            lastFPSTime = currentFrameTime;

            std::string windowTitle = "OM3D - " + std::to_string(fps) + " FPS";
            glfwSetWindowTitle(window, windowTitle.c_str());
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window, cam, (float)deltaTime);

        shaderProgram.use();
        shaderProgram.setMat4("view", cam.getViewMatrix());
        shaderProgram.setMat4("projection", cam.getProjectionMatrix());
        shaderProgram.setBool("underWater", (*cam.getPosition()).y < waterLevel && renderWater);
        world.render(shaderProgram);

        if (renderWater) {
            waterShader.use();
            waterShader.setMat4("view", cam.getViewMatrix());
            waterShader.setMat4("projection", cam.getProjectionMatrix());
            glBindVertexArray(waterVAO);
            glDrawElements(GL_TRIANGLES, (GLsizei)(6), GL_UNSIGNED_INT, (void*)0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
