#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui-plot-master/include/imgui_plot.h"
#include "BaseComponent.h"
#include <chrono>

using namespace std::chrono;

struct transform
{
	float matrix[16]{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};
class GameObject3D {
public:
	transform local;
	int id;
};
class GameObject3DAlt {
public:
	transform* local;
	int id{ 1 };
};
#pragma once
class GUI : public BaseComponent
{
public:
	GUI(GameObject* objPtr, std::string type) : BaseComponent(objPtr, type) {}

	void Start() override;
	void Update() override;
	void RenderUI() override;

private:

};

