#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void Renderer::Init(SDL_Window* window)
{
	m_window = window;
	SDL_GetWindowSize(window, &viewPort.width, &viewPort.height);
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	SceneManager::GetInstance().Render();
	
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame();
	//ImGui::NewFrame();
	//ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x + viewPort.width / 2);
	dst.y = static_cast<int>(y + viewPort.height / 2);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}
void Renderer::RenderTexture(const Texture2D& texture, Vector3 pos, Vector2 pivot, float angle, Vector3 scale, Vector2 size, SDL_RendererFlip flip, SDL_Color color) const
{
	if (flip & SDL_FLIP_HORIZONTAL) {
		pivot.x = 1 - pivot.x;
		
	}
	if (flip & SDL_FLIP_VERTICAL) {
		pivot.y = 1 - pivot.y;
	}
	SDL_Rect dst{};
	dst.x = static_cast<int>(pos.x + viewPort.width/2);
	dst.y = static_cast<int>(pos.y + viewPort.height/2);
	if (size.x == -1 || size.y == -1) SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	else {
		dst.w = static_cast<int>(size.x);
		dst.h = static_cast<int>(size.y);
	}
	dst.w = static_cast<int>(dst.w * scale.x);
	dst.h = static_cast<int>(dst.h * scale.y);
	const SDL_Point center{ static_cast<int>(pivot.x * dst.w), static_cast<int>(pivot.y * dst.h) };
	dst.x -= static_cast<int>(center.x);
	dst.y -= static_cast<int>(center.y);
	SDL_SetTextureColorMod(texture.GetSDLTexture(), color.r, color.g, color.b);
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, angle, &center, flip);
}

void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* Renderer::GetSDLRenderer() const { return m_renderer; }
