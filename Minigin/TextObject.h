#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"
#include <map>

//could add more
enum class TextAllingmentType {
	Center = 0,
	Left = 1,
	Right = 2,
	TopLeft = 3
};
static std::map<TextAllingmentType, Vector2> TextAllingment{ {
	std::make_pair(TextAllingmentType::Center, Vector2(0.5f,0.5f)),
	std::make_pair(TextAllingmentType::Left, Vector2(0,0.5f)),
	std::make_pair(TextAllingmentType::Right, Vector2(1,0.5f)),
	std::make_pair(TextAllingmentType::TopLeft, Vector2(0,0))
}
};

class Font;
class Texture2D;
struct _TTF_Font;
class TextObject : public BaseComponent
{
public:
	void Update() override;
	void CreateTexture(_TTF_Font* font, const std::string str);
	void Render() const override;

	void SetText(const std::string text);

	TextObject() : m_needsUpdate{true}{}
	~TextObject() = default;
	TextObject(const TextObject& other) = delete;
	TextObject(TextObject&& other) = delete;
	TextObject& operator=(const TextObject& other) = delete;
	TextObject& operator=(TextObject&& other) = delete;

	void SetText(std::string text, std::shared_ptr<Font> font, int order = 0, TextAllingmentType allignType = TextAllingmentType::Center, SDL_Color col = SDL_Color(255, 255, 255, 255));
	void SetOrder(int renderOrder);
	void SetOrderChildren(int offset);
	void SetColor(SDL_Color col);
	void SetBounds(const Vector2& bound);
	void SetFontSize(uint8_t size);
	void SetAllingment(TextAllingmentType allignType);

	SDL_Color& GetColor();
	Vector2 GetSize() const;
	Vector2 GetScaledSize() const;

	//int GetOrder() { return order; }
private:
	bool m_needsUpdate;
	SDL_Color color{ 255,255,255,255 };
	std::string m_text;
	Vector2 pivot;
	Vector2 bounds{};
	//int order{ 0 };
	//Transform m_transform{};
	std::shared_ptr<Font> m_font;
	std::vector<std::pair<std::shared_ptr<Texture2D>, int>> m_textTexture{};
};

