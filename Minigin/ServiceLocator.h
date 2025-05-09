#pragma once
#include "Audio.h"
#include <memory>

class ServiceLocator final
{
public:

	static Audio& GetAudioService() { return *audioService; }
	static void RegisterAudio(Audio* audio) { audioService = std::unique_ptr<Audio>(audio); }

private:
	static std::unique_ptr<Audio> audioService;
};

