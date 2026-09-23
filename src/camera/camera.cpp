#include "camera.hpp"

Camera::Camera(const glm::vec3& origin, float fov) 
: fov(fov), pos(origin), forward(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f) {}

Camera::Camera(const glm::vec3& origin, const glm::vec3& forward, const glm::vec3& up, float fov) 
: fov(fov), pos(origin), forward(forward), up(up) {}

