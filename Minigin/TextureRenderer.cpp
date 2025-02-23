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
		gameObject->transform->GetPosition();
		for (int idx{}; idx < gameObject->childrenPtr.Size(); idx++) {
			gameObject->childrenPtr[idx]->transform->isDirty = true;
		}
	}

	Renderer::GetInstance().RenderTexture(*texture, gameObject->transform->position.x, gameObject->transform->position.y);

}
