#pragma once

#include <xaudio2.h>
#include <DirectXMath.h>

class Sound {
private:
	int handle = 0;

	friend class SoundSystem;
};

class SoundSystem {
public:
	SoundSystem();
	~SoundSystem();

	void start();
	void update(double dt, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 dir);

	Sound createSound(std::string filename);
	void createBackgroundSound(std::string filename);
	void play(Sound sound);
	void setPosition(Sound sound, DirectX::XMFLOAT3 position);
private:
	class SoundImpl {
	public:
		SoundImpl() = default;
		SoundImpl(const SoundImpl&) = delete;
		SoundImpl(SoundImpl&& other) {
			data = other.data;
			currentsample = other.currentsample;
			numsamples = other.numsamples;
			playing = other.playing;
			handle = other.handle;

			other.data = nullptr;
		}
		~SoundImpl() {
			delete[] data;
		}

		DirectX::XMFLOAT3 position = {};
		float* data = nullptr;
		int currentsample = 0;
		int numsamples = 0;
		bool playing = false;
		int handle = 0;
	};
	class BackgroundImpl {
	public:
		BackgroundImpl() = default;
		BackgroundImpl(const BackgroundImpl&) = delete;
		BackgroundImpl(BackgroundImpl&& other) {
			data = other.data;
			currentsample = other.currentsample;
			numsamples = other.numsamples;

			other.data = nullptr;
		}
		float* data = nullptr;
		int currentsample = 0;
		int numsamples = 0;
	};
private:
	IXAudio2* m_pXAudio2 = nullptr;
	IXAudio2MasteringVoice* m_pMasterVoice = nullptr;
	IXAudio2SourceVoice* m_pSourceVoice = nullptr;
	float* m_soundsamples = nullptr;
	int m_currentsample = 0;
	double m_timer = 0.0;
	int m_samplerate = 48000;
	int m_numsamples = 0;
	int m_currentHandle = 1;
	std::vector<SoundImpl> m_sounds;
	std::vector<BackgroundImpl> m_background;
	DirectX::XMFLOAT3 m_listener;
};