#include "Camera.h"

Camera::Camera()
{
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraSpeed = 2.5f;
	fov = 50.0f;
	ratio = 800.0f / 600.0f;
}

void Camera::SetCameraSpeed(float speed)
{
	cameraSpeed = speed;
}

glm::vec3 Camera::GetCameraPos() const
{
	return cameraPos;
}

glm::vec3 Camera::GetCameraFace() const
{
	return cameraPos + cameraFront;
}

glm::vec3 Camera::GetCameraUp() const
{
	return cameraUp;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(cameraPos, GetCameraFace(), cameraUp);
}

glm::mat4 Camera::GetProjectMatrix() const
{
	return glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f);
}

void Camera::MoveCameraUp(float deltaTime) {
	float speed = cameraSpeed;
	if (deltaTime > 0) {
		speed = cameraSpeed * deltaTime;
	}
	cameraPos += speed * cameraUp;
}
void Camera::MoveCameraDown(float deltaTime) {
	float speed = cameraSpeed;
	if (deltaTime > 0) {
		speed = cameraSpeed * deltaTime;
	}
	cameraPos -= speed * cameraUp;
}
void Camera::MoveCameraForward(float deltaTime) {
	float speed = cameraSpeed;
	if (deltaTime > 0) {
		speed = cameraSpeed * deltaTime;
	}
	cameraPos += speed * cameraFront;
}
void Camera::MoveCameraBack(float deltaTime) {
	float speed = cameraSpeed;
	if (deltaTime > 0) {
		speed = cameraSpeed * deltaTime;
	}
	cameraPos -= speed * cameraFront;
}
void Camera::MoveCameraLeft(float deltaTime) {
	float speed = cameraSpeed;
	if (deltaTime > 0) {
		speed = cameraSpeed * deltaTime;
	}
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
		speed;
}
void Camera::MoveCameraRight(float deltaTime) {
	float speed = cameraSpeed;
	if (deltaTime > 0) {
		speed = cameraSpeed * deltaTime;
	}
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
		speed;
}
