#pragma once

#ifndef CAMERACLASS_H_
#define CAMERACLASS_H_

#include <DirectXMath.h>
#include <fstream>
#include <string>
#include "Input.h"
#include "Transform.h"
#include "MouseInput.h"

class CameraClass : public Transform
{
private:

	Matrix viewMatrix;												
	Matrix projectionMatrix;
	Vector3 forwardVec = { 0,0,1 };
	Vector3 up = { 0, 1, 0 };

	float ToDegrees(float radians) { return radians * 180 / DirectX::XM_PI; }
	void UpdateViewMatrix();
	void UpdateOrthoViewMatrix();
	float Lerp(float a, float b, float t);

public:

	CameraClass();

	void SetProjection(float angle, float aspectRatio, float nearZ, float farZ);
	void SetOrthographicProjection(float width, float height, float nearZ, float farZ);
	void OtherInput(const float& time);
	void UpdateInput(const float& time);
	void SetRotation(float time, MouseInput& mouse);

	bool Frame(float time, MouseInput& mouse);
	bool orthoFrame(float time, MouseInput& mouse);

	Matrix GetViewMatrix() { return viewMatrix; }
	Matrix GetProjectionMatrix() { return projectionMatrix; }
};

#endif // !CAMERACLASS_H_