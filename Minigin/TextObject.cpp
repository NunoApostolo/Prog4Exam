#include <stdexcept>
#include <SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

void TextObject::SetText(const std::string& text, std::shared_ptr<Font> font) 
{ 
	m_text = text;
	m_font = font;
}

void TextObject::Update()
{
	if (m_needsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void TextObject::Render() const
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = gameObject->transform->position;
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void TextObject::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void TextObject::SetPosition(const float x, const float y)
{
	gameObject->transform->position = glm::vec3(x, y, 0.f);
}


