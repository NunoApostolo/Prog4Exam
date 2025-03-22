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
		m_textTexture.clear();
		std::vector<std::string> strs{};

		std::string str = m_text;
		size_t end = str.find("\n");
		while (end != -1) {
			strs.emplace_back(str.substr(0, end));
			str.erase(str.begin(), str.begin() + end + 1);
			end = str.find("\n");
		}
		strs.emplace_back(str);
		for (auto& string : strs) {
			const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
			const auto surf = TTF_RenderText_Blended(m_font->GetFont(), string.c_str(), color);
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
			m_textTexture.emplace_back(std::make_shared<Texture2D>(texture));
			m_needsUpdate = false;
		}
	}
}

void TextObject::Render() const
{
	const glm::vec3& pos = gameObject->transform->position;
	int row{ 0 };
	float x{pos.x};
	for (auto& tex : m_textTexture) {
		Renderer::GetInstance().RenderTexture(*tex, x, pos.y + m_font->size * row);
		row++;
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


