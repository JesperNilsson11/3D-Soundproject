#include "pch.h"
#include "soundsystem.h"
#include "input.h"
#include "AudioFile.h"

float Distance(dx::XMFLOAT3 p1, dx::XMFLOAT3 p2) {
    dx::XMFLOAT3 line = { p1.x - p2.x, p1.y - p2.y , p1.z - p2.z };
    return std::sqrt(line.x * line.x + line.y * line.y + line.z * line.z);
}

SoundSystem::SoundSystem() {
    m_sounds.reserve(100);
    HRESULT hr;
    if (FAILED(hr = XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
        return;
    
    if (FAILED(hr = m_pXAudio2->CreateMasteringVoice(&m_pMasterVoice)))
        return;
    m_samplerate = 44100;
    int channels = 2;
    m_numsamples = m_samplerate * 5;
    int SIZE = channels * m_numsamples;

    WAVEFORMATEXTENSIBLE wfx = { 0 };
    wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
    wfx.Format.nChannels = 2;
    wfx.Format.nSamplesPerSec = m_samplerate;
    wfx.Format.wBitsPerSample = 32;
    wfx.Format.nBlockAlign = wfx.Format.nChannels * wfx.Format.wBitsPerSample / 8;
    wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;
    wfx.Format.cbSize = 22;
    wfx.dwChannelMask = 3;
    wfx.Samples.wValidBitsPerSample = 32;
    wfx.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

    if (FAILED(hr = m_pXAudio2->CreateSourceVoice(&m_pSourceVoice, (WAVEFORMATEX*)&wfx))) return;

    m_soundsamples = new float[SIZE];

    for (int i = 0; i < m_numsamples - 1; i += 2) {
        m_soundsamples[i] = 0;
        m_soundsamples[i + 1] = 0;
    }

    XAUDIO2_BUFFER buffer = { 0 };
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = SIZE * 4;
    buffer.pAudioData = reinterpret_cast<BYTE*>(m_soundsamples);
    buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

    if (FAILED(hr = m_pSourceVoice->SubmitSourceBuffer(&buffer)))return;
    m_currentsample = m_samplerate * 0.1;
}

void SoundSystem::start() {
    HRESULT hr;
    if (FAILED(hr = m_pSourceVoice->Start(0)))
        return;
}

SoundSystem::~SoundSystem() {
    m_pSourceVoice->DestroyVoice();
    m_pMasterVoice->DestroyVoice();
    m_pXAudio2->Release();
    delete[] m_soundsamples;
}

void SoundSystem::update(double dt, dx::XMFLOAT3 position, dx::XMFLOAT2 dir) {
    m_listener = position;
    m_timer += dt;

    int loopfactor = 0;
    while (m_timer > 0.01) {
        m_timer -= 0.01;
        loopfactor++;
    }
    int loopcount = m_samplerate * 0.01 * loopfactor;
    for (int i = 0; i < loopcount; i++) {
        float sample1 = 0;
        float sample2 = 0;
        for (auto& sound : m_sounds) {
            if (sound.playing) {
                dx::XMFLOAT2 soundtoear = { position.x - sound.position.x , position.z - sound.position.z };
                dx::XMVECTOR v1 = dx::XMLoadFloat2(&soundtoear);
                v1 = dx::XMVector2Normalize(v1);
                dx::XMStoreFloat2(&soundtoear, v1);
                dx::XMVECTOR v2 = dx::XMLoadFloat2(&dir);
                v2 = dx::XMVector2Normalize(v2);
                dx::XMStoreFloat2(&dir, v2);
                dx::XMVECTOR v3 = dx::XMVector3Cross({ 0.f, soundtoear.x, soundtoear.y }, { 0.f, dir.x, dir.y });
                float leftorright;
                dx::XMStoreFloat(&leftorright, v3);
                v3 = dx::XMVector2Dot(v1, v2);
                float dot;
                dx::XMStoreFloat(&dot, v3);
                float angle = std::acos(dot);
                if (angle > dx::XM_PIDIV2)
                    angle = dx::XM_PI - angle;
                angle = angle / 2 + dx::XM_PIDIV4;
                if (leftorright > 0) {
                    angle = dx::XM_PIDIV2 - angle;
                }
                float distance = Distance(sound.position, m_listener);
                float factorR = std::sin(angle) * std::sin(angle);
                float factorL = std::cos(angle) * std::cos(angle);
                float factor = 1.f;
                if (distance > 1.f) {
                    factor = 1.f / distance;
                }

                sample1 += sound.data[sound.currentsample * 2] * factorL * factor;
                sample2 += sound.data[sound.currentsample * 2] * factorR * factor;

                sound.currentsample++;
                if (sound.currentsample >= sound.numsamples) {
                    sound.playing = false;
                    sound.currentsample = 0;
                }
            }
        }
        for (auto& sound : m_background) {
            sample1 += sound.data[sound.currentsample * 2] * 0.05;
            sample2 += sound.data[sound.currentsample * 2 + 1] * 0.05;

            sound.currentsample++;
            if (sound.currentsample >= sound.numsamples) {
                sound.currentsample = 0;
            }
        }
        m_soundsamples[(m_currentsample) * 2] = sample1;
        m_soundsamples[(m_currentsample) * 2 + 1] = sample2;
        m_currentsample++;
        m_currentsample %= m_numsamples;
    }
}

Sound SoundSystem::createSound(std::string filename) {
    Sound sound;
    sound.handle = m_currentHandle;
    m_currentHandle++;

    AudioFile<float> audioFile;
    audioFile.load(assets + filename);
    int sampleRate = audioFile.getSampleRate();
    int bitDepth = audioFile.getBitDepth();

    float factor = m_samplerate / sampleRate;
    
    int numSamples = audioFile.getNumSamplesPerChannel()*factor;
    double lengthInSeconds = audioFile.getLengthInSeconds();

    int numChannels = audioFile.getNumChannels();
    bool isMono = audioFile.isMono();
    bool isStereo = audioFile.isStereo();

    float* data = new float[numSamples * 2];
    for (int i = 0; i < numSamples; i++) {
        data[i * 2] = audioFile.samples[0][i / factor];
        if (isStereo) {
            data[i * 2 + 1] = audioFile.samples[1][i / factor];
        }
        else {
            data[i * 2 + 1] = audioFile.samples[0][i / factor];
        }
    }

    m_sounds.emplace_back();
    auto& soundImpl = m_sounds.back();
    soundImpl.data = data;
    soundImpl.numsamples = numSamples;
    soundImpl.handle = sound.handle;
    soundImpl.position.x = 0;
    soundImpl.position.y = 0;
    soundImpl.position.z = 0;

    return sound;
}

void SoundSystem::createBackgroundSound(std::string filename) {
    AudioFile<float> audioFile;
    audioFile.load(assets + filename);
    int sampleRate = audioFile.getSampleRate();
    int bitDepth = audioFile.getBitDepth();

    float factor = m_samplerate / sampleRate;
    
    int numSamples = audioFile.getNumSamplesPerChannel() * factor;
    double lengthInSeconds = audioFile.getLengthInSeconds();

    int numChannels = audioFile.getNumChannels();
    bool isMono = audioFile.isMono();
    bool isStereo = audioFile.isStereo();

    float* data = new float[numSamples * 2];
    for (int i = 0; i < numSamples; i++) {
        data[i * 2] = audioFile.samples[0][i / factor];
        if (isStereo) {
            data[i * 2 + 1] = audioFile.samples[1][i / factor];
        }
        else {
            data[i * 2 + 1] = audioFile.samples[0][i / factor];
        }
    }

    m_background.emplace_back();
    auto& soundImpl = m_background.back();
    soundImpl.data = data;
    soundImpl.numsamples = numSamples;
}

void SoundSystem::play(Sound sound) {
    for (auto& s : m_sounds) {
        if (s.handle == sound.handle) {
            s.playing = true;
            break;
        }
    }
}

void SoundSystem::setPosition(Sound sound, DirectX::XMFLOAT3 position) {
    for (auto& s : m_sounds) {
        if (s.handle == sound.handle) {
            s.position = position;
            break;
        }
    }
}