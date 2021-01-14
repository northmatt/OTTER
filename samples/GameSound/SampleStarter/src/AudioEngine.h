#pragma once

// FMOD
#include "fmod_studio.hpp"
#include "fmod_errors.h"

// Standard Library
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

#include <glm/glm.hpp>


class AudioEvent
{
	friend class AudioEngine;

public:

	~AudioEvent();

	// Will only play if event is not currently playing
	void Play();

	// Restarts the event
	void Restart();

	// Allows AHDSR modulators to complete their release, and DSP effect tails to play out.
	void Stop();

	// Stops the event instance immediately.
	void StopImmediately();

	// Checks if event is playing
	bool isPlaying();

	// Parameters
	void SetParameter(const char* name, const float& value, const bool& ignoreSeekSpeed = false);
	float GetParameterValue(const char* name);


private:

	// AudioEngine class uses this to create Event objects
	AudioEvent(FMOD::Studio::EventInstance* eventInstance);

	// Don't want copies, should only grab refs from audio engine
	AudioEvent(AudioEvent const&) = delete;

	int ErrorCheck(FMOD_RESULT result);

private:

	FMOD::Studio::EventInstance* m_EventInstance;
};


class AudioEngine
{
	friend class AudioEvent;

public:

	//// Singleton ///////////////////

	static AudioEngine& Instance();

	AudioEngine(AudioEngine const&) = delete;
	void operator=(AudioEngine const&) = delete;

	//////////////////////////////////

	void Init();
	void Update();
	void Shutdown();



	//// Banks ////
	void LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMOD_STUDIO_LOAD_BANK_NORMAL);

	//// Events ////
	AudioEvent& CreateEvent(const std::string& strEventName, const std::string& strEventNumber);
	AudioEvent& GetEvent(const std::string& strEventName);

	//// Global Parameters ////
	void SetGlobalParameter(const char* name, const float& value, const bool& ignoreSeekSpeed = false);
	float GetGlobalParameterValue(const char* name);

	//// Helpers ////
	float dbToVolume(float db);
	float VolumeTodb(float volume);
	FMOD_VECTOR VectorToFmod(const glm::vec3& vPosition);


private:

	AudioEngine() {}

	int ErrorCheck(FMOD_RESULT result);

private:
	FMOD::Studio::System* m_StudioSystem;
	FMOD::System* m_System;


	int m_ChannelIDProvider;
	std::map<int, FMOD::Channel*> m_ChannelMap;
	std::map<std::string, FMOD::Studio::Bank*> m_BankMap;

	// Holds our event class
	std::map<std::string, AudioEvent*> m_EventMap;

};




