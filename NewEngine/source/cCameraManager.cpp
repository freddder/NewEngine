#include "cCameraManager.h"
#include <glm/gtc/matrix_transform.hpp>

cCameraManager::cCameraManager()
{
	FOV = 30.f;//45.f;
	nearPlane = 0.5f;
	farPlane = 1'000'000.f;
	cameraSpeed = 10.f;
	yaw = 0.f;//-90.f;
	pitch = 0.f;
	mouseSensitivity = 0.1f;

	position = glm::vec3(-3.0f, 3.0f, 0.0f);

	usePlayerCamera = true;

	UpdateCameraVectors();
}

cCameraManager::~cCameraManager()
{
}

void cCameraManager::UpdateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 frontVec;
	frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec.y = sin(glm::radians(pitch));
	frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(frontVec);
	// also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));

	targetDistance =25.f; //17.f;
	targetAngle = 35.f;
}

void cCameraManager::MoveForward(float deltaTime)
{
	position += (cameraSpeed * deltaTime) * front;
}

void cCameraManager::MoveBackward(float deltaTime)
{
	position -= (cameraSpeed * deltaTime) * front;
}

void cCameraManager::MoveRight(float deltaTime)
{
	position += glm::normalize(glm::cross(front, up)) * (cameraSpeed * deltaTime);
}

void cCameraManager::MoveLeft(float deltaTime)
{
	position -= glm::normalize(glm::cross(front, up)) * (cameraSpeed * deltaTime);
}

void cCameraManager::MoveUp(float deltaTime)
{
	position += (cameraSpeed * deltaTime) * up;
}

void cCameraManager::MoveDown(float deltaTime)
{
	position -= (cameraSpeed * deltaTime) * up;
}

void cCameraManager::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

glm::mat4 cCameraManager::GetViewMatrix()
{
	if (usePlayerCamera)
	{
		glm::vec3 newPosition;
		newPosition.x = targetPosRef->x - (glm::cos(glm::radians(targetAngle)) * targetDistance);
		newPosition.y = targetPosRef->y + (glm::sin(glm::radians(targetAngle)) * targetDistance);
		newPosition.z = targetPosRef->z;

		position = newPosition;

		return glm::lookAt(newPosition,
			*targetPosRef,
			up);
	}
	else
	{
		return glm::lookAt(position,
			position + front,
			up);
	}
}

glm::mat4 cCameraManager::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, nearPlane, farPlane);
}
