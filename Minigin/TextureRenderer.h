#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"

class TextureRenderer : public BaseComponent
{
public:
	void SetTexture(std::shared_ptr<Texture2D> tex);
	void SetTexture(std::shared_ptr<Texture2D> tex, Vector2 pivot, int renderOrder = 0, Vector2 size = Vector2(-1, -1));
	void SetTexture(const std::string path);
	void SetTexture(const std::string path, Vector2 pivot, int renderOrder = 0, Vector2 size = Vector2(-1, -1));
	void SetPivot(Vector2 pivot);
	void SetOrder(int renderOrder);
	void SetOrderChildren(int offset);
	void SetSize(const Vector2& newSize);
	void SetColor(SDL_Color color) { col = color; }
	//int GetOrder() { return order; }
	Vector2 GetPivot() { return pivot; }
	Texture2D* GetTexture() { return texture.get(); }
	Vector2 GetSize();
	Vector2 GetScaledSize();
	Vector2 GetLocalScaledSize();

	void Render() const override;

	bool flipHort{ false };
	bool flipVert{ false };
private:
	std::shared_ptr<Texture2D> texture{};
	SDL_Color col{255,255,255,255};
	Vector2 pivot{ 0,0 };
	Vector2 size{ -1,-1 };
	Vector2 scaledSize{ -1,-1 };
	//int order{};
};

