#include "Audio.h"
#if 0//AK_ENABLE_PROFILING
#pragma comment(lib,"CommunicationCentral.lib") 
#include <AK/Comm/AkCommunication.h>	// Communication between Wwise designer and the game
#else
#define AK_OPTIMIZED
#endif //AK_USE_PROFILER

// load the libraries required, this alternatively could be done in the project properties but having it defined in code makes it more portable and reusable
#pragma comment(lib,"dxguid.lib")						//needed for 3d sounds
#pragma comment(lib,"Ws2_32.lib")						//wwise uses this for communication
#include <AK/SoundEngine/Common/AkSoundEngine.h>		// Sound Engine
#pragma comment(lib,"AkSoundEngine.lib")
#include <AK/MusicEngine/Common/AkMusicEngine.h>		// Music Engine
#pragma comment(lib,"AkMusicEngine.lib")
#include <AK/SoundEngine/Common/IAkStreamMgr.h>			// Streaming Manager
#pragma comment(lib,"AkStreamMgr.lib")
#include <AK/SoundEngine/Common/AkMemoryMgr.h>			// Memory Manager
#pragma comment(lib,"AkMemoryMgr.lib")
#include <AK/Plugin/AkVorbisFactory.h>					// Needed for the Ogg Vorbis codec plugin
#pragma comment(lib,"AkVorbisDecoder.lib")

#include <AK/Tools/Common/AkPlatformFuncs.h>			// Thread defines
#include "AKIncludes/AkFilePackageLowLevelIOBlocking.h" // Sample low-level I/O implementation
#include <AK/SoundEngine/Common/AkModule.h>				// Default memory and stream managers
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include <AK/SoundEngine/Common/AkDynamicSequence.h>
#include <AK/IBytes.h>

// Bank file names
#define BANKNAME_INIT		L"Init.bnk"
#define BANKNAME_SOUND_BANK	L"SoundBank.bnk"


// We're using the default Low-Level I/O implementation that's part
// of the SDK's sample code, with the file package extension
CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

// Defining the hooks below is required for every project that links to the
// Wwise sound engine.
// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{
	// This is the default implementation of the Memory Manager.
	void* AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void* in_ptr)
	{
		free(in_ptr);
	}

	// Note: VirtualAllocHook(() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	void* VirtualAllocHook(void*  in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(void* in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
};

namespace WwiseNS
{
	// For the Sonic object array
	enum SoundIndex
	{
		INDEX_SOUND_DOOR = 0,
		INDEX_SOUND_PISTOL,
		INDEX_SOUND_EXPLOSION,
		INDEX_SOUND_PUNCHES,
		INDEX_SOUND_AMT
	};



	// Register and Unregister of Wwise
	// game object.
	void RegisterGameObjects();
	void UnregisterGameObjects();

	// Loading and Unloading of Wwise
	// sound banks.
	void LoadSoundBanks(char* szFilePath);
	void UnloadSoundBanks();

	// Toggling, handling, processing and
	// playing of events/music.
	
	void ToggleEventsOn(SoundEvents soundEvent);
	void HandleEvents();
	int ProcessAudio();

	

	// 'Set' functions for Wwise game object
	// positions.
	void SetSoundObjectPositions();
	void SetGameObjPos(AkGameObjectID gameObjectID, AkSoundPosition soundPosition);
	

	//speed at which the player moves
	AkSoundPosition		soundPos;
	AkSoundPosition		objPositions[INDEX_SOUND_AMT];

	AkListenerPosition	listener;
	AkTimeMs			uPosition = 0;

	int		triggerSoundEvents;	// sound event bit toggles
};

int WwiseNS::InitSoundEngine(char* szFilePath, float fX, float fY, float fZ, float fForwardX, float fForwardY, float fForwardZ)
{
	// Creating and initializing an instance of the default memory manager.
	// It is highly recommended you don't use the default settings for your game.
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;
	if(AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return 0;
	}

	// Create and initialize an instance of the default streaming manager. Note
	// that you can override the dafault streaming manager with your own. Refer
	// to the SDK documentation for more information.
	// It is highly recommended you don't use the default settings for your game.
	AkStreamMgrSettings stmSettings;
	// 	stmSettings.uMemorySize = 32 * 1024*1024; // 32 MB
	AK::StreamMgr::GetDefaultSettings(stmSettings);
	if(!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		return 0;
	}

	// Create a streaming device with blocking low-level I/O handshaking.
	// Note that you can override the default low-level I/O module with your own. Refer
	// to the SDK documentation for more information.
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
	// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
	// in the Stream Manager, and registers itself as the File Location Resolver.
	if(g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return 0;
	}

	// Create the Sound Engine
	// Using default initialization parameters
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	// TODO : Review and edit later
	// Setting pool sizes for this game.
	// These sizes are tuned for this game, every game should determine its own optimal values.
	// 	initSettings.uDefaultPoolSize           = 4 * 1024 * 1024;  // 4 MB
	// 	initSettings.uMaxNumPaths               = 16;
	// 	initSettings.uMaxNumTransitions         = 128;
	
	//	platformInitSettings.uLEngineDefaultPoolSize    = 4 * 1024 * 1024;  // 4 MB

	if(AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return 0;
	}

	// Initialize the music engine
	// Using default initialization parameters
	// *****This must be done after the sound engine******
	AkMusicSettings musicInit;
	musicInit.fStreamingLookAheadRatio = 100;
	if(AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return 0;
	}
#ifndef AK_OPTIMIZED
	// Initialize communications
	// This is necessary to perform in-game mixing, profiling and troubleshooting.
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if(AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return 0;
	}
#endif // AK_OPTIMIZED
	// After all initialization is completed we will register
	// any plugins that are needed for the game. In this example,
	// we will use the Ogg Vorbis codec plugin.
	AK::SoundEngine::RegisterCodec(AKCOMPANYID_AUDIOKINETIC,
		AKCODECID_VORBIS,
		CreateVorbisFilePlugin,
		CreateVorbisBankPlugin);

	// Clearing out the listener position object
	memset(&listener, 0, sizeof(AkListenerPosition));
	memset(&soundPos, 0, sizeof(AkSoundPosition));
	// Initializing event flags
	triggerSoundEvents	= 0;

	LoadSoundBanks(szFilePath);
	RegisterGameObjects();
	
	printf_s("Successful Initialization\n");
	return 1;
}

int WwiseNS::ProcessAudio()
{
	// Process bank requests, events, positions, RTPC, etc.
	if(AK::SoundEngine::RenderAudio() != AK_Success)
		return 0;

	return 1;
}

void WwiseNS::TermSoundEngine()
{
#ifndef AK_OPTIMIZED
	// Terminate Communication Services
	AK::Comm::Term();
#endif // AK_OPTIMIZED
	UnregisterGameObjects();
	UnloadSoundBanks();
	// Terminate the music engine
	AK::MusicEngine::Term();
	// Terminate the sound engine
	AK::SoundEngine::Term();
	// Terminate the streaming device and streaming manager
	// CAkFilePackageLowLevelIOBlocking::Term() destroys the associated streaming device
	// that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
	g_lowLevelIO.Term();
	if(AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();
	// Terminate the Memory Manager
	AK::MemoryMgr::Term();
}

void WwiseNS::LevelMusicPlay(int _musicID)
{
	switch(_musicID)
	{
	case MENU_MUSIC:
		{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_MX_MUSICLOOP_06,OBJECT_PLAYER );
		}
		break;
	case LEVEL_MUSIC1:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_MX_MUSICLOOP_05,OBJECT_PLAYER );
		}
		break;
	};
}

void WwiseNS::LevelMusicStop(int _musicID)
{
	switch(_musicID)
	{
	case MENU_MUSIC:
		{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_MUSICLOOP_06, OBJECT_PLAYER );
		}
		break;
	case LEVEL_MUSIC1:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_MUSICLOOP_05, OBJECT_PLAYER );
		}
		break;
	};
}

void WwiseNS::PlaySFX(int SFXID)
{
	ToggleEventsOn((SoundEvents)SFXID);
	HandleEvents();
}

void WwiseNS::SetMXVolume(float _volume)
{
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::MX_VOLUME, _volume);
}

void WwiseNS::SetFXVolume(float _volume)
{
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::FX_VOLUME, _volume);
}

void WwiseNS::LoadSoundBanks(char* szFilePath)
{
	// Setup banks path
	//g_lowLevelIO.SetBasePath((AkOSChar*)szFilePath);
	g_lowLevelIO.SetBasePath(L"../AudioLibrary/Resources/Soundbanks/");
	// Load banks synchronously (from file name).

	// IMPORTANT NOTE-----------------------------------------------------------
	// You can load sound banks through the generated header file if and only
	// if the sound banks are generated with ID filenames and not the actual
	// sound bank filenames. The functions are written in the commented section
	// below.
	//--------------------------------------------------------------------------
	//eResult = AK::SoundEngine::LoadBank(AK::BANKS::INIT, AK_DEFAULT_POOL_ID);
	//eResult = AK::SoundEngine::LoadBank(AK::BANKS::SOUNDBANK, AK_DEFAULT_POOL_ID);
	//--------------------------------------------------------------------------

	AkBankID bankID;	// Not used in this tutorial. Banks can be unloaded with their file name, this can be useful to unload menu or intro sounds while in game and vice versa.
	AKRESULT eResult;
	eResult = AK::SoundEngine::LoadBank(BANKNAME_INIT, AK_DEFAULT_POOL_ID, bankID);
	if(eResult != AK_BankAlreadyLoaded)
		assert(eResult == AK_Success);

	
		eResult = AK::SoundEngine::LoadBank(BANKNAME_SOUND_BANK, AK_DEFAULT_POOL_ID, bankID);
		assert(eResult == AK_Success);
	
	printf_s("Loaded sound banks successfully\n\n");
}

void WwiseNS::UnloadSoundBanks()
{

		if(AK::SoundEngine::UnloadBank(BANKNAME_SOUND_BANK) != AK_Success)
			printf_s("Failed to unload sound bank\n");

	printf_s("Unloaded sound banks successfully.\n");
}


void WwiseNS::RegisterGameObjects()
{
	// Register game objects. Some global game objects might be registered
	// at initialization time, but most of your game objects will probably
	// be dynamically registered/unregistered as they spawn or get killed
	AK::SoundEngine::RegisterGameObj(OBJECT_PLAYER,"Player");

	// Clearing out the sound event toggles
	triggerSoundEvents = 0;
}

void WwiseNS::UnregisterGameObjects()
{
		AK::SoundEngine::UnregisterGameObj(OBJECT_PLAYER);
}

void WwiseNS::ToggleEventsOn(SoundEvents soundEvent)
{
	if(!(triggerSoundEvents & soundEvent))
		triggerSoundEvents |= soundEvent;
}

void WwiseNS::HandleEvents()
{
//////////// Menu Sounds ///////////////////////////////////////////////////////////////////
	if(triggerSoundEvents & EVENT_SOUND_MENUCHANGE)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_2D_MENUMOVE_HOVER, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_MENUCHANGE;
	}
	if(triggerSoundEvents & EVENT_SOUND_MENUSELECT)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_3D_LEVER_SWITCH, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_MENUSELECT;
	}
	if(triggerSoundEvents & EVENT_SOUND_GAMESTART)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_GAMESTART;
	}
//////////// Game Sounds ///////////////////////////////////////////////////////////////////
	if(triggerSoundEvents & EVENT_SOUND_PLAYER_DEATH)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DX_2D_DEATH, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_PLAYER_DEATH;
	}
	if(triggerSoundEvents & EVENT_SOUND_PLAYER_HURT)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DX_2D_HURT, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_PLAYER_HURT;
	}
	if(triggerSoundEvents & EVENT_SOUND_PLAYER_BULLET)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_2D_PISTOLSHOT, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_PLAYER_BULLET;
	}
	if(triggerSoundEvents & EVENT_SOUND_PLAYER_GETPICKUP)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_2D_HEALTHPICKUP, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_PLAYER_GETPICKUP;
	}
	if(triggerSoundEvents & EVENT_SOUND_PLAYER_SPECIAL)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_2D_SHEILDPICKUP, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_PLAYER_SPECIAL;
	}
	if(triggerSoundEvents & EVENT_SOUND_SMASHBOT_ATTACK)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_3D_SMASHCRATE, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_SMASHBOT_ATTACK;
	}
	if(triggerSoundEvents & EVENT_SOUND_SMASHBOT_IGNORE)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_2D_COINPICKUP, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_SMASHBOT_IGNORE;
	}
	if(triggerSoundEvents & EVENT_SOUND_TRASHBOT_ATTACK)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_3D_PUNCHES, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_TRASHBOT_ATTACK;
	}
	if(triggerSoundEvents & EVENT_SOUND_PATROLBOT_ATTACK)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_2D_WEAPONWHOOSH, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_PATROLBOT_ATTACK;
	}
	if(triggerSoundEvents & EVENT_SOUND_EXPLOSION)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_3D_EXPLOSION, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_EXPLOSION;
	}
	if(triggerSoundEvents & EVENT_SOUND_BURNALERT)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_2D_BEEP, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_BURNALERT;
	}
	if(triggerSoundEvents & EVENT_SOUND_PULSELASER)
	{
		// Post this event using its name
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT, OBJECT_PLAYER);
		triggerSoundEvents ^= EVENT_SOUND_PULSELASER;
	}
	
	// you can specify a game object id in order to stop all sounds associated with that individual object.
	if(triggerSoundEvents & EVENT_KILL_ALL)
		AK::SoundEngine::StopAll();
}

bool WwiseNS::Update()
{
	ProcessAudio();
	return true;
}