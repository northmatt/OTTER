#include "AudioEngine.h"



////////////////////////////////////
//			 AudioEvent			  //
////////////////////////////////////

AudioEvent::AudioEvent(FMOD::Studio::EventInstance* eventInstance)
	: m_EventInstance(eventInstance)
{
}

AudioEvent::~AudioEvent()
{
}

void AudioEvent::Play()
{

	// Check if already playing
	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;

	if (m_EventInstance->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING)
	{
		return;
	}

	// Start the event
	m_EventInstance->start();
}


void AudioEvent::Restart()
{
	// Start the event
	m_EventInstance->start();
}

void AudioEvent::Stop()
{
	// Check if already playing
	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (m_EventInstance->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING)
	{
		// Stop the event
		m_EventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
	}
}

void AudioEvent::StopImmediately()
{
	// Check if already playing
	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (m_EventInstance->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING)
	{
		// Stop the event
		m_EventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
	}
}

bool AudioEvent::isPlaying()
{
	// Check if already playing
	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (m_EventInstance->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING)
	{
		return true;
	}

	return false;
}

void AudioEvent::SetParameter(const char* name, const float& value, const bool& ignoreSeekSpeed)
{
	ErrorCheck(m_EventInstance->setParameterByName(name, value, ignoreSeekSpeed));
}

float AudioEvent::GetParameterValue(const char* name)
{
	// Make float for output
	float value;

	// Put value into float
	ErrorCheck(m_EventInstance->getParameterByName(name, &value));

	// Return float
	return value;
}


int AudioEvent::ErrorCheck(FMOD_RESULT result)
{

	// Outputs FMOD error message
	if (result != FMOD_OK)
	{
		std::cout << "FMOD EVENT ERROR: " << FMOD_ErrorString(result) << std::endl;

		__debugbreak();

		return 1;
	}

	// All good
	return 0;
}


////////////////////////////////////
//			 Audio Engine		  //
////////////////////////////////////

AudioEngine& AudioEngine::Instance()
{
	static AudioEngine instance;

	return instance;
}

void AudioEngine::Init()
{
	// Initilizs the audio systems in FMOD
	ErrorCheck(FMOD::Studio::System::create(&m_StudioSystem));
	ErrorCheck(m_StudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	// Save ref to core system
	ErrorCheck(m_StudioSystem->getCoreSystem(&m_System));
}

void AudioEngine::Update()
{
	// Update FMOD system
	ErrorCheck(m_StudioSystem->update());

}

void AudioEngine::Shutdown()
{

	// Delete events
	for (auto event : m_EventMap)
	{
		delete event.second;
	}
	m_EventMap.clear();

	AudioEngine::ErrorCheck(m_StudioSystem->unloadAll());
	AudioEngine::ErrorCheck(m_StudioSystem->release());
}

void AudioEngine::LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
{

	// Make sure bank isn't already loaded
	auto bank = m_BankMap.find(bankName);
	if (bank != m_BankMap.end())
		return;

	// Create new bank
	FMOD::Studio::Bank* newBank;

	// Load the bank with FMOD
	ErrorCheck(m_StudioSystem->loadBankFile((bankName + ".bank").c_str(), flags, &newBank));


	// Store the bank in our map so we can use it later
	if (newBank)
	{
		m_BankMap[bankName] = newBank;
	}
}

AudioEvent& AudioEngine::CreateEvent(const std::string& eventName, const std::string& eventNumber)
{
	// Get find event in file
	FMOD::Studio::EventDescription* eventDescription = NULL;
	ErrorCheck(m_StudioSystem->getEvent(eventNumber.c_str(), &eventDescription));

	// If the event exists
	if (eventDescription)
	{
		// Use the event description to make and return an instance
		FMOD::Studio::EventInstance* newFMODEvent = nullptr;
		ErrorCheck(eventDescription->createInstance(&newFMODEvent));

		// Create an audio event
		AudioEvent* newAudioEvent = new AudioEvent(newFMODEvent);
		
		// Make sure multiple events with the same name aren't created
		if (m_EventMap.find(eventName) != m_EventMap.end())
		{
			__debugbreak();
		}

		// Add the audio event to our map
		m_EventMap[eventName] = newAudioEvent;

		return *newAudioEvent;
	}
	
}

AudioEvent& AudioEngine::GetEvent(const std::string& strEventName)
{
	// If the event exists
	if (m_EventMap.find(strEventName) != m_EventMap.end())
	{
		// Return it
		return *m_EventMap.at(strEventName);
	}
	else
	{
		// Event not found
		__debugbreak;
	}

}

/////// Global Parameters //////////

void AudioEngine::SetGlobalParameter(const char* name, const float& value, const bool& ignoreSeekSpeed)
{
	ErrorCheck(m_StudioSystem->setParameterByName(name, value, ignoreSeekSpeed));
}

float AudioEngine::GetGlobalParameterValue(const char* name)
{
	// Make float for output
	float value;

	// Put value into float
	ErrorCheck(m_StudioSystem->getParameterByName(name, &value));

	// Return float
	return value;
}




/////// Helpers //////////

FMOD_VECTOR AudioEngine::VectorToFmod(const glm::vec3& vPosition)
{
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

float AudioEngine::dbToVolume(float db)
{
	return powf(10.0f, 0.05f * db);
}

float AudioEngine::VolumeTodb(float volume)
{
	return 20.0f * log10f(volume);
}


int AudioEngine::ErrorCheck(FMOD_RESULT result)
{

	// Outputs FMOD error message
	if (result != FMOD_OK)
	{
		std::cout << "FMOD ENGINE ERROR: " << FMOD_ErrorString(result) << std::endl;
		
		__debugbreak();

		return 1;
	}

	// All good
	return 0;
}




