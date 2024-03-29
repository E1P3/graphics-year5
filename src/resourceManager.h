#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "material.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "model.h"
#include "gameObject.h"
#include "lights.h"
#include "camera.h"
#include "bone.h"

class Model;
class Bone;
class Ray;

struct keyData{
    float pressDuration;
    bool isPressed;
};

class ResourceManager {
public:
    static GLFWwindow* createWindow(int width = 800, int height = 600, const char* title = "Graphics");
    static GLFWwindow* getWindow();

    //Resource management
    static Shader* addShader(Shader* shader);
    static Shader* getShader(unsigned int ID);
    static Texture* loadTexture( TextureType type, const char* textureFile, bool useMipmaps = true, GLenum interpolation = GL_LINEAR);
    static Texture* getTexture(unsigned int ID);
    static Mesh* loadMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures);
    static Mesh* getMesh(unsigned int ID);
    static Model* loadModel(const char* modelFile);
    static Model* getModel(unsigned int ID);
    static GameObject* loadGameObject();
    static GameObject* getGameObject(unsigned int ID);
    static DirectionalLight* loadDirectionalLight(float strength, glm::vec3 rotation);
    static PointLight* loadPointLight(float strength ,glm::vec3 position, float constant, float linear, float quadratic);

    //IO events
    static float getDeltaTime();
    static void updateDeltaTime();
    static void updateKeysPressed();
    static bool isKeyPressed(int key);
    static void updateMousePosition();
    static void updateMousePressed();
    static bool isMousePressed(int button);
    static keyData getKeyData(int key);
    static keyData getMouseData(int button);
    static void setMouseEnabled(bool enabled);
    static bool getIsMouseEnabled();
    static double getMouseX();
    static double getMouseY();
    static double getMouseDeltaX();
    static double getMouseDeltaY();
    static void setActiveCamera(Camera* camera);
    static Camera* getActiveCamera();
    static int getScreenWidth() { return screenWidth; };
    static int getScreenHeight() { return screenHeight; };
    static glm::vec3 getMouseRayHit();
    static glm::vec3 getMouseRayOrigin();
    static void addGeometryInfo(GameObject* gameObject, std::vector<glm::vec3> vertexPositions);
    static GameObject* checkMouseVertexPick(glm::vec3& vertex, int& vertexIndex);
    static GameObject* getCurrentlySelected();
    static void setCurrentlySelected(GameObject* selected);

    //glfw loop events
    static void initialize();
    static void runGameLoop();

private:
    static bool isDebug;
    static bool isMouseEnabled;
    static GLFWwindow* window;
    static int screenWidth, screenHeight;
    static float deltaTime, previousTime;
    static double mouseX, mouseY, lastMouseX, lastMouseY;
    static Camera* activeCamera;
    static std::unordered_map<int, keyData> keyStates;
    static std::unordered_map<int, keyData> mouseStates;
    static std::vector<Shader*> shaders;
    static std::vector<Texture*> textures;
    static std::vector<Mesh*> meshes;
    static std::vector<Model*> models;
    static std::vector<GameObject*> gameObjects;
    static std::vector<PointLight*> pointLights;
    static std::vector<DirectionalLight*> directionalLights;
    static std::map<GameObject*, std::vector<glm::vec3>> pickableVerticies;
    static GameObject* currentlySelected;
};

#endif
