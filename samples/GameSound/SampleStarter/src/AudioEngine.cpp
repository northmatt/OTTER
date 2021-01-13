
#include "AudioEngine.h"



////////////////////////////////////
//			 Event				  //
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

	if (m_EventInstance == nullptr)
	{
		return;
	}

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

	ErrorCheck(m_StudioSystem->getCoreSystem(&m_System));
}

void AudioEngine::Update()
{
	// Update FMOD system
	ErrorCheck(m_StudioSystem->update());

}


void AudioEngine::Shutdown()
{
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

AudioEvent* AudioEngine::CreateEvent(const std::string& eventName, const std::string& eventNumber)
{
	// Get find event in file
	FMOD::Studio::EventDescription* eventDescription = NULL;
	ErrorCheck(m_StudioSystem->getEvent(eventNumber.c_str(), &eventDescription));

	// If the event exists
	if (eventDescription)
	{
		// Use the event description to make and return an instance
		FMOD::Studio::EventInstance* newFMODEvent = nullptr;
		eventDescription->createInstance(&newFMODEvent);

		// Create an audio event
		AudioEvent* newAudioEvent = new AudioEvent(newFMODEvent); // TODO: Delete these in shutdown

		// Add the audio event to our map
		m_EventMap[eventName] = newAudioEvent;

		return newAudioEvent;
	}

	return nullptr;
}

AudioEvent* AudioEngine::GetEvent(const std::string& strEventName)
{
	// If the event exists
	if (m_EventMap.find(strEventName) != m_EventMap.end())
	{
		// Return it
		return m_EventMap.at(strEventName);
	}

	// No event found
	return nullptr;
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
		std::cout << "FMOD ERROR: " << FMOD_ErrorString(result) << std::endl;
		
		__debugbreak();

		return 1;
	}

	// All good
	return 0;
}




