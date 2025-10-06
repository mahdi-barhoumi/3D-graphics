#include <camera.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

camera::camera(float aspectRatio_, float fieldOfView_, float nearZ_, float farZ_, float x, float y, float z)
: aspectRatio(aspectRatio_), fieldOfView(radians(fieldOfView_)), nearZ(nearZ_), farZ(farZ_), position(vec3(x, y, z))
{
    forward = vec3(1, 0, 0);
    right = vec3(0, -1, 0);
    up = vec3(0, 0, 1);
    translationMatrix = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -position.x, -position.y, -position.z, 1);
    rotationMatrix = mat4(right.x, up.x, forward.x, 0, right.y, up.y, forward.y, 0, right.z, up.z, forward.z, 0, 0, 0, 0, 1);
    viewMatrix = rotationMatrix * translationMatrix;
    projectionMatrix = mat4(1 / aspectRatio / tan(radians(fieldOfView_) / 2), 0, 0, 0, 0, 1 / tan(radians(fieldOfView_) / 2), 0, 0, 0, 0, (nearZ + farZ) / (farZ - nearZ), 1, 0, 0, 2 * nearZ * farZ / (nearZ - farZ), 0);
}

void camera::setFieldOfView(float fieldOfView_, bool degrees)
{
    if (degrees) this->fieldOfView = radians(fieldOfView_);
    else this->fieldOfView = fieldOfView_;
}
void camera::setNearClipPlane(float nearZ_) { this->nearZ = nearZ_; }
void camera::setFarClipPlane(float farZ_) { this->farZ = farZ_; }
void camera::moveTo(float x, float y, float z) { position = vec3(x, y, z); }
void camera::moveBy(float deltaX, float deltaY, float deltaZ) { position += vec3(deltaX, deltaY, deltaZ); }
void camera::moveForward(float value) { position += value * vec3(forward.x, forward.y, 0); }
void camera::moveBackward(float value) { position += value * vec3(-forward.x, -forward.y, 0); }
void camera::moveRight(float value) { position += value * vec3(right.x, right.y, 0); }
void camera::moveLeft(float value) { position += vec3(-right.x, -right.y, 0); }
void camera::moveUp(float value) { position += vec3(0, 0, up.z); }
void camera::moveDown(float value) { position += vec3(0, 0, -up.z); }
void camera::pan(float degrees)
{
    quat pan = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * vec3(0, 0, 1));
    forward = pan * forward * conjugate(pan);
    right = pan * right * conjugate(pan);
    up = pan * up * conjugate(pan);
}
void camera::roll(float degrees)
{
    quat roll = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * forward);
    right = roll * right * conjugate(roll);
    up = roll * up * conjugate(roll);
}
void camera::pitch(float degrees)
{
    quat pitch = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * right);
    up = pitch * up * conjugate(pitch);
    forward = pitch * forward * conjugate(pitch);
}
void camera::yaw(float degrees)
{
    quat yaw = quat(cos(radians(degrees) / 2), sin(radians(degrees) / 2) * up);
    forward = yaw * forward * conjugate(yaw);
    right = yaw * right * conjugate(yaw);
}
void camera::update(void)
{
    translationMatrix = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -position.x, -position.y, -position.z, 1);
    rotationMatrix = mat4(right.x, up.x, forward.x, 0, right.y, up.y, forward.y, 0, right.z, up.z, forward.z, 0, 0, 0, 0, 1);
    viewMatrix = rotationMatrix * translationMatrix;
    projectionMatrix = mat4(1 / aspectRatio / tan(fieldOfView / 2), 0, 0, 0, 0, 1 / tan(fieldOfView / 2), 0, 0, 0, 0, (nearZ + farZ) / (farZ - nearZ), 1, 0, 0, 2 * nearZ * farZ / (nearZ - farZ), 0);
}
