#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <iostream>

#include "../entityModule.h"
#include "../gameObject.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum Camera_Projection {
    ORTHO,
    PERSP
};

class Camera : public EntityModule {
public:
    Camera() = default;
    Camera(glm::vec3 worldUp, Camera_Projection projection, float fov, float aspect, float near, float far){
        this->worldUp = worldUp;
        this->projection = projection;
        this->projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
        this->front = glm::vec3(0.0f, 0.0f, -1.0f);
        this->up = worldUp;
    }
    Camera(glm::vec3 worldUp, Camera_Projection projection, float left, float right, float bottom, float top, float near, float far){
        this->worldUp = worldUp;
        this->projection = projection;
        this->projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
        this->front = glm::vec3(0.0f, 0.0f, -1.0f);
        this->up = worldUp;
    }
    ~Camera() = default;

    void OnRenderPass() override{
        
    }

    void OnUpdate() override{
        this->updateCameraVectors();
        this->updateViewMatrix();
    }

    void OnStart() override{
        
    }

    glm::mat4 getViewMatrix(){
        return this->viewMatrix;
    }

    glm::mat4 getProjectionMatrix(){
        return this->projectionMatrix;
    }

private:
    Camera_Projection projection;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    void updateCameraVectors(){
        glm::vec3 front = this->getParent()->getRotation() * glm::vec3(0.0f, 0.0f, -1.0f);
        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up    = glm::normalize(glm::cross(this->right, this->front));
    }

    void updateViewMatrix(){
        if(this->getParent() != nullptr){
            this->viewMatrix = glm::lookAt(this->getParent()->getPosition(), this->getParent()->getPosition() + this->front, this->up);
        }
    }

};

#endif // CAMERA_H
