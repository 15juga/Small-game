#include "Camera.h"

void CameraClass::SetRotation(float time, MouseInput& mouse)
{
	//Yaw -= X * cameraSensitivity * time;
	//Pitch += Y * cameraSensitivity * time;

	LONG X, Y;
	mouse.GetDelta(X, Y);
	//OutputDebugStringA(("Mouse X: " + std::to_string(X) + "	Mouse Y: " + std::to_string(Y) + "\n").c_str());

	Vector2 mouseLook = Vector2(Lerp((float)X, 0, 0.99f), Lerp((float)Y, 0, 0.99f));

	if (mouseLook.y > 0.015 || mouseLook.y < -0.015) {

		Rotate(mouseLook.y * 20 * time, 0, 0);
	}

	if (mouseLook.x > 0.015 || mouseLook.x < -0.015) {

		Rotate(0, mouseLook.x * -20 * time, 0);
	}
}

float CameraClass::Lerp(float a, float b, float t)
{
	return (t - 1.f) * a + t * b;
}

void CameraClass::UpdateViewMatrix()
{
	Matrix camRotationMatrix = XMMatrixRotationRollPitchYawFromVector(rot);
	Vector3 camTarget = Vector3::Transform(forwardVec, camRotationMatrix);
	camTarget += pos;
	Vector3 upDirection = Vector3::Transform(up, XMMatrixRotationRollPitchYawFromVector(rot));
	viewMatrix = Matrix::CreateLookAt(pos, camTarget, upDirection);		
}

void CameraClass::UpdateOrthoViewMatrix()
{
	// 75 degrees
	rot = Vector3(-1, 0.7f, 0.9f);
	Matrix camRotationMatrix = XMMatrixRotationRollPitchYawFromVector(rot);
	Vector3 camTarget = Vector3::Transform(forwardVec, camRotationMatrix);
	camTarget += pos;
	Vector3 upDirection = Vector3::Transform(up, XMMatrixRotationRollPitchYawFromVector(rot));
	viewMatrix = Matrix::CreateLookAt(pos, camTarget, upDirection);
}

void CameraClass::UpdateInput(const float& time)
{
	Vector3 axis;
	axis.x = Input::KeyPress('A') - Input::KeyPress('D');
	axis.z = Input::KeyPress('S') - Input::KeyPress('W');
	axis.y = Input::KeyPress('E') - Input::KeyPress('Q');

	MoveRelative(axis.x * 10 * time, 0, axis.z * 10 * time);
	Move(0, axis.y * 10 * time, 0);
}

void CameraClass::OtherInput(const float& time)
{
	//pos = Vector3(0, 10, 0);

	Vector3 axis;
	axis.x = Input::KeyPress(VK_LEFT) - Input::KeyPress(VK_RIGHT);
	axis.z = Input::KeyPress(VK_DOWN) - Input::KeyPress(VK_UP);
	axis.y = Input::KeyPress(VK_SPACE) - Input::KeyPress(VK_TAB);

	MoveRelative(axis.x * 10 * time, 0, axis.z * 10 * time);
	Move(0, axis.y * 10 * time, 0);
}

CameraClass::CameraClass()
{
	//pos = Vector3(0, 10, 0);
}

void CameraClass::SetProjection(float angle, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (angle / 360.0f) * XM_2PI;
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);

}

void CameraClass::SetOrthographicProjection(float width, float height, float nearZ, float farZ)
{
	this->projectionMatrix = XMMatrixOrthographicLH(width, height, nearZ, farZ);
}

bool CameraClass::Frame(float time, MouseInput& mouse)
{
	mouse.Frame();
	UpdateViewMatrix();

	return true;
}

bool CameraClass::orthoFrame(float time, MouseInput& mouse)
{
	SetRotation(time, mouse);
	UpdateOrthoViewMatrix();
	return false;
}
