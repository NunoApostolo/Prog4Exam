#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "Vector.h"

class Texture2D;
/**
 * Simple RAII wrapper for the SDL renderer
 */
class Renderer final : public Singleton<Renderer>
{
	SDL_Renderer* m_renderer{};
	SDL_Window* m_window{};
	SDL_Color m_clearColor{};
	Rect viewPort{};
public:
	void Init(SDL_Window* window);
	void Render() const;
	void Destroy();

	Rect GetViewPort() { return viewPort; }

	void RenderTexture(const Texture2D& texture, float x, float y) const;
	void RenderTexture(const Texture2D& texture, Vector3 pos, const Vector2 pivot, float angle, Vector3 scale, Vector2 size, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Color color = SDL_Color(255,255,255,255)) const;
	void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

	SDL_Renderer* GetSDLRenderer() const;

	const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
	void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
};


