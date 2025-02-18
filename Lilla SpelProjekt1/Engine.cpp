#include "Engine.h"

void Engine::shutdown()
{
	gfx.Shutdown();
}

bool Engine::Initialize(HINSTANCE instance, string windowName, string windowClass, int width, int height)
{
	if (!this->renderWindow.initialize(instance, windowName, windowClass, width, height))
	{
		return false;
	}
	if (!mouse.Initalize(instance, renderWindow.GetHWND(), width, height)) {

		return false;
	}
	if (!gfx.InitializeDirectX(renderWindow.GetHWND(), width, height))
	{
		return false;
	}
	if (!gfx.Initialize(width, height))
	{
		return false;
	}
	if (!sceneManager.initializeSceneManagerObjects(gfx.GetDevice(), gfx.GetDeviceContext(), gfx.getcbPlayer())) 
	{
		ErrorLogger::Log("Failed to Start scene manager");
		return false;
	}
	sceneManager.Initialize(renderWindow.GetHWND(), width, height, gfx.GetRTD());

	// initialize the correct values for Camera 1
	camera.SetProjection(80, static_cast<float>(width /2.f) / static_cast<float>(height), 0.01f, 100.f);
	camera.Rotate(-45.f, 0.f, 0.f);

	// initialize the correct values for Camera 2
	otherCamera.Rotate(-45.f, 0.f, 0.f);
	otherCamera.SetProjection(80, static_cast<float>(width / 2.f) / static_cast<float>(height), 0.01f, 100.f);
	otherCamera.SetPos(0, 1, 0);

	// initialize the correct values for the light Camera
	lightCamera.SetPos(4.0f, 50.0f, 4.0f);
	lightCamera.SetOrthographicProjection(210, 210, 0.001f, 100.f);
	lightCamera.orthoFrame(1, mouse);

	return true;
}

void Engine::update(const float deltaTime)
{
	camera.Frame(deltaTime, mouse);

	//camera.UpdateInput(deltaTime);
	//camera.SetRotation(deltaTime, mouse);

	if (sceneManager.GetScene() == SCN::LEVEL1)
	{
		//Input::SetMousePos(GetSystemMetrics(SM_CXFULLSCREEN) / 2, GetSystemMetrics(SM_CYFULLSCREEN) / 2);
	}

	otherCamera.Frame(deltaTime, mouse);
	//otherCamera.OtherInput(deltaTime);
	sceneManager.update(gfx.GetDevice(), gfx.GetDeviceContext(), gfx.getcbPlayer(), deltaTime);
	sceneManager.SetCam(camera, otherCamera);
}


bool Engine::processMessages()
{
	return this->renderWindow.processMessages();
}

void Engine::RenderFrame()
{
	// call render to depth buffer here before the main render pass
	gfx.RenderToDepthBuffer(lightCamera, sceneManager);  

	gfx.Render(camera, otherCamera, lightCamera, this->sceneManager);
}
