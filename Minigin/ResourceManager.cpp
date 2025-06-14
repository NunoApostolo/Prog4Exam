#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"


namespace fs = std::filesystem;

void ResourceManager::Init(const std::filesystem::path& dataPath)
{
	if (std::filesystem::exists(dataPath)) {
		m_dataPath = dataPath;
	}
	else {
		m_dataPath = dataPath.string().substr(3);
	}

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	if (m_loadedTextures.find(filename) == m_loadedTextures.end()) {
		Texture2D* tex = new Texture2D(fullPath.string());
		if (tex->GetSDLTexture() == nullptr) {
			delete tex;
			return LoadTexture("Missing.png");
		}
		m_loadedTextures.insert(std::pair(filename, std::shared_ptr<Texture2D>(tex)));
	}
	return m_loadedTextures.at(filename);
}

std::shared_ptr<Font> ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	const auto key = std::pair<std::string, uint8_t>(filename, size);
	if (m_loadedFonts.find(key) == m_loadedFonts.end()) {
		Font* font = new Font(fullPath.string(), size);
		if (font->GetFont() == nullptr) {
			delete font;
			return LoadFont("EncodeSansCompressed-700-Bold.ttf", size);
		}
		m_loadedFonts.insert(std::pair(key, std::shared_ptr<Font>(font)));
	}
	return m_loadedFonts.at(key);
}

void ResourceManager::UnloadUnusedResources()
{
	for (auto it = m_loadedTextures.begin(); it != m_loadedTextures.end();)
	{
		if (it->second.use_count() == 1)
			it = m_loadedTextures.erase(it);
		else
			++it;
	}

	for (auto it = m_loadedFonts.begin(); it != m_loadedFonts.end();)
	{
		if (it->second.use_count() == 1)
			it = m_loadedFonts.erase(it);
		else
			++it;
	}
}
std::string ResourceManager::GetPath(const std::string str)
{
	const auto fullPath = m_dataPath / str;
	const auto filename = fs::path(fullPath).filename().string();
	return filename;
}
std::string ResourceManager::GetPath(const char* str)
{
	const auto fullPath = m_dataPath / str;
	//const auto filename = fs::path(fullPath).filename().string();
	return fullPath.string();
}