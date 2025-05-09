#include "TextureRenderer.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include <iostream>

void TextureRenderer::SetTexture(std::shared_ptr<Texture2D> tex)
{
	texture = tex;
}

void TextureRenderer::SetTexture(std::shared_ptr<Texture2D> tex, Vector2 pivotPercent, int renderOrder, Vector2 texSize)
{
	SetTexture(tex);
	SetPivot(pivotPercent);
	SetOrder(renderOrder);
	size = texSize;
}

void TextureRenderer::SetTexture(const std::string path)
{
	texture = ResourceManager::GetInstance().LoadTexture(path);
	GetScaledSize();
}

void TextureRenderer::SetTexture(const std::string path, Vector2 pivotPercent, int renderOrder, Vector2 texSize)
{
	SetTexture(path);
	SetPivot(pivotPercent);
	SetOrder(renderOrder);
	size = texSize;
}

void TextureRenderer::Render() const
{
	//Renderer::GetInstance().RenderTexture(*texture, gameObject->transform->GetPosition().x, gameObject->transform->GetPosition().y);
	SDL_RendererFlip flip{};
	flip = SDL_RendererFlip((flipHort ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (flipVert ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));

	if (texture == nullptr) return;
	Renderer::GetInstance().RenderTexture(*texture, 
		gameObject->transform->GetPosition(),
		pivot,
		gameObject->transform->localRotation,
		gameObject->transform->GetScale(),
		size,
		flip,
		col);
}

// pivot between 0 and 1
void TextureRenderer::SetPivot(Vector2 pivotPercent) {
	if (pivotPercent.x > 1 || pivotPercent.y > 1 || pivotPercent.x < 0 || pivotPercent.y < 0) return;
	pivot = pivotPercent;
}

void TextureRenderer::SetOrder(int renderOrder)
{
	gameObject->SetOrder(renderOrder);
	SceneManager::GetInstance().curScene->SetOrderFlag();
}
Vector2 TextureRenderer::GetSize()
{
	if (size.x == -1 || size.y == -1) {
		if (texture.get() == nullptr) Update();
		int w, h;
		SDL_QueryTexture(texture->GetSDLTexture(), nullptr, nullptr, &w, &h);
		size = Vector2((float)w, (float)h);
	}
	return size;
}
Vector2 TextureRenderer::GetScaledSize() {
	if (size.x == -1 || size.y == -1) {
		int w, h;
		SDL_QueryTexture(texture->GetSDLTexture(), nullptr, nullptr, &w, &h);
		size = Vector2((float)w, (float)h);
	}
	scaledSize = Vector2(size.x * gameObject->transform->GetScale().x, size.y * gameObject->transform->GetScale().y);
	//scaledSize = Vector2(size.x, size.y) * gameObject->transform->localScale;
	return scaledSize;
}

Vector2 TextureRenderer::GetLocalScaledSize()
{
	if (size.x == -1 || size.y == -1) {
		int w, h;
		SDL_QueryTexture(texture->GetSDLTexture(), nullptr, nullptr, &w, &h);
		size = Vector2((float)w, (float)h);
	}
	//scaledSize = Vector2(size.x * gameObject->transform->GetScale().x, size.y * gameObject->transform->GetScale().y);
	scaledSize = Vector2(size.x, size.y) * gameObject->transform->localScale;
	return scaledSize;
}
