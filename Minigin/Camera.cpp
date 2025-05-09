#include "Camera.h"
#include "GameObject.h"

Camera* Camera::mainCamera{nullptr};
//Rectf Camera::viewPort{};

//Point2f Camera::GetPosition()
//{
//	return Point2f(position.x - viewPort.width / 2, position.y - viewPort.height / 2);
//}

Camera& Camera::GetMainCamera()
{
	if (Camera::mainCamera == nullptr) GameObject::Create("Camera")->AddComponent<Camera>();
	return *Camera::mainCamera;
}

void Camera::SetMainCamera(Camera* cam)
{
	Camera::mainCamera = cam;
}
