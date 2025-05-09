#include <stdexcept>
#include <SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Texture2D.h"
#include <math.h>
#include "Font.h"

#include "Console.h"

void TextObject::SetText(std::string text, std::shared_ptr<Font> font, int renderOrder, TextAllingmentType allignType, SDL_Color col)
{ 
	m_text = text;
	m_font = font;
	pivot = TextAllingment.at(allignType);
	SetOrder(renderOrder);
	SetColor(col);
	m_needsUpdate = true;
	Update();
}

void TextObject::SetOrder(int renderOrder)
{
	gameObject->SetOrder(renderOrder);
}

void TextObject::SetColor(SDL_Color col)
{
	color = col;
	m_needsUpdate = true;
}

void TextObject::SetBounds(const Vector2& bound)
{
	bounds = bound;
	m_needsUpdate = true;
}

void TextObject::Update()
{
	if (m_needsUpdate)
	{
		m_textTexture.clear();
		if (m_text == "") return;

		std::replace(m_text.begin(), m_text.end(), '\n', '\0');

		CreateTexture(m_font->GetFont(), m_text);

		if (bounds == Vector2(0, 0)) bounds = GetSize() + Vector2(10, 10);
		Vector2 size{ GetSize() };
		if (size.x > bounds.x) {
			// adds sepparators depending on bounds /only x is implemented

			float charPerc{ 1.f / m_text.size() };
			float perc{ static_cast<float>(bounds.x / size.x) };
			float totalPerc{ perc };
			int prevIndex{ 1 };
			int index{ 0 };
			while (totalPerc < 1) {
				index = static_cast<int>(std::ceill((totalPerc) * m_text.size())) - 1;

				while (m_text[index] != ' ') {
					--index;
					perc -= charPerc;
					if (index < 0) {
						bounds = size;
						m_needsUpdate = false;
						return;
					}
				}
				totalPerc += perc;
				if (m_text.size() > static_cast<size_t>(index)) {
					prevIndex++;
					//m_text.insert(index, "\n");
					//m_text.replace(m_text.begin() + index, 1, '\n');
					m_text[index] = '\n';
				}
				else {
					bounds = size;
					//m_needsUpdate = false;
					return;
				}

			}

			std::vector<std::string> strs{};
			std::string str = m_text;
			size_t end = str.find("\n");
			while (end != -1) {
				strs.emplace_back(str.substr(0, end));
				str.erase(str.begin(), str.begin() + end + 1);
				end = str.find("\n");
			}
			strs.emplace_back(str);

			m_textTexture.clear();
			for (auto& string : strs) {
				//const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
				//if (string == "") continue;

				CreateTexture(m_font->GetFont(), string);

				if (GetSize().x > bounds.x) {

					//Update();
				}
				else m_needsUpdate = false;
			}
		}
	}
}

void TextObject::CreateTexture(TTF_Font* font, const std::string str)
{
	const auto surf = TTF_RenderText_Blended(font, str.c_str(), color);
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
	m_textTexture.emplace_back(std::make_pair<std::shared_ptr<Texture2D>, int>(std::make_shared<Texture2D>(texture), static_cast<int>(str.size())));
}

void TextObject::Render() const
{
	//if (gameObject->transform->isDirty) {
	//	gameObject->transform->GetPosition();
	//	gameObject->transform->GetScale();
	//	for (int idx{}; idx < gameObject->childrenPtr.size(); idx++) {
	//		gameObject->childrenPtr[idx]->transform->isDirty = true;
	//	}
	//}

	if (m_textTexture.size() == 0) return;
	Vector3 pos = gameObject->transform->GetPosition();
	if (pivot.y == 0) {
		pos = pos;
	}
	pos.y -= GetScaledSize().y/2 * pivot.y * (m_textTexture.size() - 1);
	//int row{ 0 };
	//float x{pos.x};
	for (auto& tex : m_textTexture) {
		int width{}, height{};
		//SDL_QueryTexture(tex.first->GetSDLTexture(), nullptr, nullptr, &width, nullptr);
		//Renderer::GetInstance().RenderTexture(*tex.first, x - (width * pivot.x),
		//	pos.y - (m_font->size * pivot.y) + m_font->size * row);
		SDL_QueryTexture(tex.first->GetSDLTexture(), nullptr, nullptr, &width, &height);
		Vector2 size{(float)width, (float)height};
		Renderer::GetInstance().RenderTexture(*tex.first.get(),
			pos,
			pivot,
			gameObject->transform->localRotation,
			gameObject->transform->GetScale(),
			size);
		//row++;
		pos.y += ((m_font->size) * gameObject->transform->GetScale().y );
	}
}

// This implementation uses the "dirty flag" pattern
void TextObject::SetText(const std::string text)
{
	m_text = text;
	m_needsUpdate = true;
}
SDL_Color& TextObject::GetColor()
{
	m_needsUpdate = true;
	return color;
}
Vector2 TextObject::GetSize() const {
	Vector2 size{};
	if (m_textTexture.size() == 0) return size;
	for (auto& tex : m_textTexture) {
		int width{}, height{};

		SDL_QueryTexture(tex.first->GetSDLTexture(), nullptr, nullptr, &width, &height);
		if (size.x < width) {
			size.x = static_cast<float>(width);
		}
		if (size.y < height) {
			//size.y = static_cast<float>(height);
		}
		size.y += static_cast<float>(height);
	}
	return size;
}
Vector2 TextObject::GetScaledSize() const{
	Vector2 size{GetSize() * gameObject->transform->GetScale()};
	return size;
}

