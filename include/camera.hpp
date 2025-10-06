#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

class camera
{
    friend class world;
    friend class renderer;
private:
    float aspectRatio;
    float fieldOfView;
    float nearZ;
    float farZ;
    vec3 position;
    vec3 forward;
    vec3 right;
    vec3 up;
    mat4 translationMatrix;
    mat4 rotationMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
public:
    camera(float aspectRatio = 1, float fieldOfView = 90, float nearZ = 0.1, float farZ = 100, float x = 0, float y = 0, float z = 0);
    void setFieldOfView(float fieldOfView, bool degrees = true);
    void setNearClipPlane(float nearZ);
    void setFarClipPlane(float farZ);
    void moveTo(float x, float y, float z);
    void moveBy(float deltaX, float deltaY, float deltaZ);
    void moveForward(float value);
    void moveBackward(float value);
    void moveRight(float value);
    void moveLeft(float value);
    void moveUp(float value);
    void moveDown(float value);
    void pan(float degrees);
    void roll(float degrees);
    void pitch(float degrees);
    void yaw(float degrees);
    void update(void);
};
