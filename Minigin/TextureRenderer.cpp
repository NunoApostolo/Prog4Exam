#include "TextureRenderer.h"
#include "GameObject.h"

void TextureRenderer::SetTexture(const std::string path)
{
	texture = ResourceManager::GetInstance().LoadTexture(path);
}

void TextureRenderer::Render() const
{
	Renderer::GetInstance().RenderTexture(*texture, gameObject->transform->position.x, gameObject->transform->position.y);
}
