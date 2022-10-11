#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float cameraSpeed;
	float fov;
	float ratio;

public:
	Camera();
	void SetCameraSpeed(float speed);
	void SetCameraPos(glm::vec3 val) { cameraPos = val; };
	void SetCameraFov(float val){fov = val;};
	void SetCameraRatio(float val) { ratio = val; };
	void SetCameraFront(glm::vec3 front){cameraFront = front; };

	float GetCameraFov() { return fov;};
	glm::vec3 GetCameraPos() const;
	glm::vec3 GetCameraFace() const;
	glm::vec3 GetCameraUp() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectMatrix() const;

	void  MoveCameraUp(float deltaTime);
	void  MoveCameraDown(float deltaTime);
	void  MoveCameraForward(float deltaTime);
	void  MoveCameraBack(float deltaTime);
	void  MoveCameraLeft(float deltaTime);
	void  MoveCameraRight(float deltaTime);

	
};

