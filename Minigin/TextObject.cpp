#pragma once
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

void TextObject::SetOrderChildren(int offset)
{
	gameObject->SetOrder(gameObject->GetOrder() + offset);

	for (auto& c : gameObject->childrenPtr) {
		TextureRenderer* t = c->GetComponent<TextureRenderer>();
		if (t != nullptr) {
			t->SetOrderChildren(offset);
		}
		TextObject* text = c->GetComponent<TextObject>();
		if (text != nullptr) {
			text->SetOrderChildren(offset);
		}
	}

	//SceneManager::GetInstance().curScene->SetOrderFlag();
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

void TextObject::SetFontSize(uint8_t size)
{
	m_font = ResourceManager::GetInstance().LoadFont("", size);
	m_needsUpdate = true;
}

void TextObject::SetAllingment(TextAllingmentType allignType)
{
	pivot = TextAllingment.at(allignType);
	m_needsUpdate = true;
}

void TextObject::Update()
{
	if (m_text == "[On Defend] Increase max roll for the last action by 3") {
		Console::GetInstance().Log("test");
	}
	if (m_needsUpdate)
	{
		m_textTexture.clear();
		if (m_text == "") return;

		std::replace(m_text.begin(), m_text.end(), '\n', ' ');

		if (m_font == nullptr) SetFontSize(60);
		CreateTexture(m_font->GetFont(), m_text);

		if (bounds == Vector2(0, 0)) bounds = GetSize() + Vector2(10, 10);
		Vector2 size{ GetSize() };
		if (size.x > std::abs(bounds.x - (bounds.x * pivot.x))) {
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
		if (GetSize().y > bounds.y) {
			if (m_font->size > 5) m_font = ResourceManager::GetInstance().LoadFont("", static_cast<uint8_t>(m_font->size - 5));
			else m_font = ResourceManager::GetInstance().LoadFont("", 5);
			
			//Update();
			m_needsUpdate = true;
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
	if (m_textTexture.size() != 1) pos.y -= GetScaledSize().y / 2 * pivot.y;
	
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
	if (m_font == nullptr) ResourceManager::GetInstance().LoadFont("", 30);
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
	if (m_textTexture.size() == 0) {
		return size;
	}
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

