#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"

class TextureRenderer : public BaseComponent
{
public:
	TextureRenderer(GameObject* objPtr, std::string type) : BaseComponent(objPtr, type) {}
	
	void SetTexture(const std::string path);
	void SetPivot(Vector2 pivot);

	void Render() const override;

private:
	std::shared_ptr<Texture2D> texture;
	Vector2 pivot{ 0,0 };
};

