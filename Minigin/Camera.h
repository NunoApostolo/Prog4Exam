#pragma once
#include "BaseComponent.h"
#include "Renderer.h"

class Camera : public BaseComponent
{
public:
	Camera() { Camera::SetMainCamera(this); }
	static Camera& GetMainCamera();
	static void SetMainCamera(Camera* cam);

	void SetViewPort(Rect view) { viewPort = view; }
	Rect GetViewPort() { return viewPort; }

private:
	Rect viewPort;
	static Camera* mainCamera;
};

