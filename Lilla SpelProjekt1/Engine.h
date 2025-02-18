#pragma once
#include "WindowContainer.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphic.h"
#include "Mesh.h"
#include "GameObject.h"

class Engine : WindowContainer
{
public:
	void shutdown();
	bool Initialize(HINSTANCE instance, string windowName, string windowClass, int width, int height);

	void update(const float deltaTime);
	bool processMessages();
	void RenderFrame();
protected:
	Graphic gfx;
private:
	CameraClass camera;
	CameraClass otherCamera;
	CameraClass lightCamera;
	MouseInput mouse;
	SceneManager sceneManager;
};