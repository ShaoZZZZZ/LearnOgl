#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup) {
	Position = position;
	Forward = glm::normalize(target - position);
	WorldUp = worldup;
	Right = glm::cross(Forward, WorldUp);
	Up = glm::normalize(glm::cross(  Forward, Right));
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup) {
	Position = position;
	WorldUp = worldup;
	Pitch = pitch;
	Yaw = yaw;
	Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(pitch) * glm::cos(Yaw);
	Right = glm::cross(Forward, WorldUp);
	Up = glm::normalize(glm::cross(Forward, Right));
}

Camera::~Camera() {

}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Forward, WorldUp);

}

void Camera::UpdateCameraVectors() {
	Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
	Right = glm::cross(Forward, WorldUp);
	Up = glm::normalize(glm::cross(Right, Forward));
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY) {
	Pitch -= deltaY * SenseY;
	Yaw -= deltaX * SenseX;
	UpdateCameraVectors();
}

void Camera::UpdateCameraPos() {
	Position += Forward *SpeedZ * 0.01f + Right * SpeedX * 0.01f+ Up * SpeedY * 0.01f;
}
