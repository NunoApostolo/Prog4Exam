#include "Audio.h"
#include <SDL_mixer.h>
#include "ResourceManager.h"
#include <thread>
#include <mutex>
#include "Console.h"

struct AudioQueue {
	
};

class Audio::AudioImpl {
public:
	AudioImpl() {
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
			Console::GetInstance().Log("MIX error");
		}
		Mix_AllocateChannels(16);
		//thread = std::jthread(&AudioImpl::ThreadLoop);
		ThreadLoop();
	}
	~AudioImpl()
	{
		running = false;
		thread.join();
		SDL_CloseAudio();
	}

	void AddAudioQueue(const uint16_t , float ) {

	}
	void AddAudioQueue(const char* str, float volume) {
		if (volume > 1) volume = 1;
		Mix_Volume(-1, static_cast<int>(volume * globalVolume * MIX_MAX_VOLUME));

		std::lock_guard<std::mutex> lock(mutex);
		queue.emplace_back(str);
	}

	void SetVolume(float volume) {
		globalVolume = volume;
		if (globalVolume > 1) globalVolume = 1;
		if (globalVolume < 0) globalVolume = 0;
	}
	float GetVolume() {
		return globalVolume;
	}
private:
	
	bool PullQueue(std::string_view& str) {
		if (queue.size() == 0) return false;
		std::lock_guard<std::mutex> lock(mutex);
		str = queue[0];
		queue.erase(queue.begin());
		return true;
	}

	void PlayMusic(const char* str) {
		Mix_Chunk* sample = Mix_LoadWAV(ResourceManager::GetInstance().GetPath(str).c_str()); // 
		Mix_PlayChannel(-1, sample, 0);

	}

	void ThreadLoop() {
		thread = std::jthread{
			[&]() {
				while (running) {
					std::string_view str;
					if (PullQueue(str)) {
						PlayMusic(str.data());
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			}
		};

	}


	//Mix_Music musicChunk;
	std::jthread thread{};
	std::vector<std::string_view> queue{};
	std::mutex mutex{};
	bool running{ true };
	float globalVolume{ 1 };
};

SDLAudio::SDLAudio() : Audio()
{
}

void SDLAudio::PlayAudio(const uint16_t , const float )
{

}

void SDLAudio::PlayAudio(const char* file, const float volume)
{
	file; volume;
	audioImpl->AddAudioQueue(file, volume);
}

void SDLAudio::SetVolume(const float volume)
{
	audioImpl->SetVolume(volume);
}

float SDLAudio::GetVolume()
{
	return audioImpl->GetVolume();
}

Audio::Audio()
{
	audioImpl = std::make_unique<AudioImpl>();
}
