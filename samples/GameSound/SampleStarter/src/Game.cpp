//------------------------------------------------------------------------
// Game.cpp
//------------------------------------------------------------------------

#include "AudioEngine.h"

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------

void Init()
{
	AudioEngine& audio = AudioEngine::Instance();

	// Init FMOD
	audio.Init();

	// Load the sound bank
	audio.LoadBank("Master");

	// Create an event instance
	AudioEvent* testEvent = AudioEngine::Instance().CreateEvent("Mii Channel Theme", "{b56cb9d2-1d47-4099-b80e-7d257b99a823}");

	// Play the event
	testEvent->Play();

	// Parameter change
	testEvent->SetParameter("Underwater", 1);



}

//------------------------------------------------------------------------
// Update your game here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	// Update fmod
	AudioEngine::Instance().Update();
	
}


//------------------------------------------------------------------------
// Add your display calls here (DrawLine, Print, DrawSprite.) 
//------------------------------------------------------------------------
void Render()
{

}


//------------------------------------------------------------------------
// Add your shutdown code here.
//------------------------------------------------------------------------
void Shutdown()
{
	AudioEngine::Instance().Shutdown();

}