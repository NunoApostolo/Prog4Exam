#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"

class Font;
class Texture2D;
class TextObject : public BaseComponent
{
public:
	void Update() override;
	void Render() const override;

	void SetText(const std::string& text);
	void SetPosition(float x, float y);

	TextObject(GameObject* objPtr, std::string type) 
		: m_needsUpdate{true}, BaseComponent(objPtr, type) {}
	virtual ~TextObject() = default;
	TextObject(const TextObject& other) = delete;
	TextObject(TextObject&& other) = delete;
	TextObject& operator=(const TextObject& other) = delete;
	TextObject& operator=(TextObject&& other) = delete;

	void SetText(const std::string& text, std::shared_ptr<Font> font);

private:
	bool m_needsUpdate;
	std::string m_text;
	//Transform m_transform{};
	std::shared_ptr<Font> m_font;
	std::vector<std::shared_ptr<Texture2D>> m_textTexture{};
};

