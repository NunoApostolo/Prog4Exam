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

	Renderer::GetInstance().RenderTexture(*texture, gameObject->transform->GetPosition().x, gameObject->transform->GetPosition().y);

}
