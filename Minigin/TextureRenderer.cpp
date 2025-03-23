#include "TextureRenderer.h"
#include "GameObject.h"
#include <iostream>

void TextureRenderer::SetTexture(const std::string path)
{
	texture = ResourceManager::GetInstance().LoadTexture(path);
}

void TextureRenderer::Render() const
{
	if (gameObject->transform->isDirty) {
		for (int idx{}; idx < gameObject->childrenPtr.size(); idx++) {
			gameObject->childrenPtr[idx]->transform->isDirty = true;
		}
		gameObject->transform->GetPosition();
	}

	//Renderer::GetInstance().RenderTexture(*texture, gameObject->transform->GetPosition().x, gameObject->transform->GetPosition().y);
	Renderer::GetInstance().RenderTexture(*texture, gameObject->transform->GetPosition().x, 
		gameObject->transform->GetPosition().y,
		pivot,
		gameObject->transform->localRotation,
		gameObject->transform->scale.x,
		gameObject->transform->scale.y);

}

// pivot between 0 and 1
void TextureRenderer::SetPivot(Vector2 pivotPercent) {
	if (pivotPercent.x > 1 || pivotPercent.y > 1 || pivotPercent.x < 0 || pivotPercent.y < 0) return;
	pivot = pivotPercent;
}

