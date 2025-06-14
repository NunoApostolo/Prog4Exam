#pragma once
#include <memory>


class Audio
{
public:
	Audio();
	virtual ~Audio() = default;
	virtual void PlayAudio(const uint16_t id, const float volume = 1) = 0;
	virtual void PlayAudio(const char* file, const float volume = 1) = 0;
	virtual void SetVolume(const float volume) = 0;
	virtual float GetVolume() = 0;
protected:
	class AudioImpl;
	std::unique_ptr<AudioImpl> audioImpl;
private:

};

class SDLAudio final : public Audio {
public:
	SDLAudio();
	void PlayAudio(const uint16_t id, const float volume = 1) override;
	void PlayAudio(const char* file, const float volume = 1) override;
	void SetVolume(const float volume) override;
	float GetVolume() override;

};