#ifndef _CAUDIO_H_
#define _CAUDIO_H_

#ifdef AUDIOLIBRARY_EXPORTS
#define AUDIOLIBRARY_API __declspec(dllexport)
#else
#define AUDIOLIBRARY_API __declspec(dllimport)
#endif

#define AK_ENABLE_PROFILING 1
#define DEFAULT_SOUNDBANK_PATH L"../AudioLibrary/Resources/Soundbanks/"

#include <cstdio>
#include <cstdlib>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <conio.h>
#include <AK/SoundEngine/Common/AkTypes.h>

namespace WwiseNS
{
	enum eObjectID
	{
		OBJECT_PLAYER,
		NUM_OBJECT_IDS
	};

	enum MusicTracks
	{
		MENU_MUSIC = 1,
		LEVEL_MUSIC1 ,
		LEVEL_MUSIC2 ,
	};

	enum SoundEvents
	{
		EVENT_KILL_ALL					= 0x40000000,
		EVENT_SOUND_PLAYER_DEATH		= 0x01,
		EVENT_SOUND_PLAYER_HURT			= 0x02,
		EVENT_SOUND_PLAYER_BULLET		= 0x04,
		EVENT_SOUND_PLAYER_GETPICKUP	= 0x08,
		EVENT_SOUND_PLAYER_SPECIAL		= 0x10,
		EVENT_SOUND_SMASHBOT_ATTACK		= 0x20,
		EVENT_SOUND_SMASHBOT_IGNORE		= 0x40,
		EVENT_SOUND_TRASHBOT_ATTACK		= 0x80,
		EVENT_SOUND_PATROLBOT_ATTACK	= 0x100,
		EVENT_SOUND_EXPLOSION			= 0x200,
		EVENT_SOUND_BURNALERT			= 0x400,
		EVENT_SOUND_PULSELASER			= 0x800,

		EVENT_SOUND_MENUCHANGE			= 0x1000,
		EVENT_SOUND_MENUSELECT			= 0x2000,
		EVENT_SOUND_GAMESTART			= 0x4000,

		EVENT_SOUND_LEVEL_MUSIC	= 0x20000000,
	};

	int InitSoundEngine(char* szFilePath,float,float,float,float,float,float);
	void TermSoundEngine();
	bool Update();
	void PlaySFX(int SFXID);
	void LevelMusicPlay(int _musicstatus);
	void LevelMusicStop(int _musicstatus);

	void SetMXVolume(float _volume);
	void SetFXVolume(float _volume);

	float GetMXVolume(void);
	float GetFXVolume(void);
};

namespace AK
{
    namespace EVENTS
    {
        static const AkUniqueID PAUSE_ALL = 3864097025U;
        static const AkUniqueID PLAY_DX_2D_BREATHING_LOOP = 3412940309U;
        static const AkUniqueID PLAY_DX_2D_DEATH = 2068802240U;
        static const AkUniqueID PLAY_DX_2D_HURT = 1203171421U;
        static const AkUniqueID PLAY_DX_2D_JUMP = 1763800590U;
        static const AkUniqueID PLAY_DX_2D_LAND = 4126538873U;
        static const AkUniqueID PLAY_DX_2D_UNDERATTACK = 430329040U;
        static const AkUniqueID PLAY_DX_2D_UNITCOMMANDS = 2337306830U;
        static const AkUniqueID PLAY_DX_3D_AGGRO = 3761829895U;
        static const AkUniqueID PLAY_DX_3D_ATTACK = 969992391U;
        static const AkUniqueID PLAY_DX_3D_DEATH = 3432959431U;
        static const AkUniqueID PLAY_DX_3D_HURT = 3836275424U;
        static const AkUniqueID PLAY_FX_2D_ALARM_LOOP = 3110195632U;
        static const AkUniqueID PLAY_FX_2D_AMMOPICKUP = 1107108682U;
        static const AkUniqueID PLAY_FX_2D_ASSAULTRIFLE_LOOP = 425165382U;
        static const AkUniqueID PLAY_FX_2D_BEEP = 2882299664U;
        static const AkUniqueID PLAY_FX_2D_BODYFALL = 4042539875U;
        static const AkUniqueID PLAY_FX_2D_CAR_LOOP = 905521127U;
        static const AkUniqueID PLAY_FX_2D_COINPICKUP = 1456069603U;
        static const AkUniqueID PLAY_FX_2D_FOOTSTEPS = 2067887017U;
        static const AkUniqueID PLAY_FX_2D_FOREST_LOOP = 726857588U;
        static const AkUniqueID PLAY_FX_2D_HEALTHPICKUP = 1728663298U;
        static const AkUniqueID PLAY_FX_2D_JUMP = 987385568U;
        static const AkUniqueID PLAY_FX_2D_KLAXON_LOOP = 4123835918U;
        static const AkUniqueID PLAY_FX_2D_LAND = 2415006107U;
        static const AkUniqueID PLAY_FX_2D_MAGICPICKUP = 696917685U;
        static const AkUniqueID PLAY_FX_2D_MENUACCEPT = 1526375285U;
        static const AkUniqueID PLAY_FX_2D_MENUBACKUP = 4292106387U;
        static const AkUniqueID PLAY_FX_2D_MENUCANCEL = 2078529791U;
        static const AkUniqueID PLAY_FX_2D_MENUMOVE_HOVER = 3621386429U;
        static const AkUniqueID PLAY_FX_2D_MENUPAUSE = 4195054897U;
        static const AkUniqueID PLAY_FX_2D_MENUUNPAUSE = 3637374546U;
        static const AkUniqueID PLAY_FX_2D_PISTOLSHOT = 3618765313U;
        static const AkUniqueID PLAY_FX_2D_PUNCHES = 2632226688U;
        static const AkUniqueID PLAY_FX_2D_RAIN_LOOP = 2666978119U;
        static const AkUniqueID PLAY_FX_2D_SHEILDPICKUP = 3973443319U;
        static const AkUniqueID PLAY_FX_2D_UNDERWATER_LOOP = 2622520724U;
        static const AkUniqueID PLAY_FX_2D_WEAPONWHOOSH = 3135678882U;
        static const AkUniqueID PLAY_FX_2D_WIND_LOOP = 3407866903U;
        static const AkUniqueID PLAY_FX_3D_ASSAULTRIFLE_LOOP = 3295936277U;
        static const AkUniqueID PLAY_FX_3D_BODYFALL = 891453498U;
        static const AkUniqueID PLAY_FX_3D_CAR_LOOP = 3814004282U;
        static const AkUniqueID PLAY_FX_3D_DOOR_CLOSE = 2046371004U;
        static const AkUniqueID PLAY_FX_3D_DOOR_OPEN = 1585471016U;
        static const AkUniqueID PLAY_FX_3D_EXPLOSION = 73471290U;
        static const AkUniqueID PLAY_FX_3D_FOOTSTEPS = 21750526U;
        static const AkUniqueID PLAY_FX_3D_JUMP = 4050058769U;
        static const AkUniqueID PLAY_FX_3D_LAND = 4112129774U;
        static const AkUniqueID PLAY_FX_3D_LEVER_SWITCH = 3553836542U;
        static const AkUniqueID PLAY_FX_3D_MISSLE_LOOP = 163704345U;
        static const AkUniqueID PLAY_FX_3D_PISTOLSHOT = 3359821848U;
        static const AkUniqueID PLAY_FX_3D_PUNCHES = 1070288727U;
        static const AkUniqueID PLAY_FX_3D_SMASHCRATE = 1902835760U;
        static const AkUniqueID PLAY_FX_3D_WATERSPLASH = 1491931047U;
        static const AkUniqueID PLAY_FX_3D_WEAPONWHOOSH = 4023349039U;
        static const AkUniqueID PLAY_MX_MUSICLOOP_01 = 2230580979U;
        static const AkUniqueID PLAY_MX_MUSICLOOP_02 = 2230580976U;
        static const AkUniqueID PLAY_MX_MUSICLOOP_03 = 2230580977U;
        static const AkUniqueID PLAY_MX_MUSICLOOP_04 = 2230580982U;
        static const AkUniqueID PLAY_MX_MUSICLOOP_05 = 2230580983U;
        static const AkUniqueID PLAY_MX_MUSICLOOP_06 = 2230580980U;
        static const AkUniqueID PLAY_PS_CHECKPOINT = 1253233846U;
        static const AkUniqueID PLAY_PS_CREDIT = 2839419803U;
        static const AkUniqueID PLAY_PS_ENEMY_JUNKHULK = 2160640979U;
        static const AkUniqueID PLAY_PS_ENEMY_LILGOO = 171115091U;
        static const AkUniqueID PLAY_PS_ENEMY_SLIMEMONSTER = 3082611329U;
        static const AkUniqueID PLAY_PS_LEVEL1 = 3112785291U;
        static const AkUniqueID PLAY_PS_LEVEL2 = 3112785288U;
        static const AkUniqueID PLAY_PS_LEVEL3 = 3112785289U;
        static const AkUniqueID PLAY_PS_LEVEL4 = 3112785294U;
        static const AkUniqueID PLAY_PS_LEVELBOSS = 507373585U;
        static const AkUniqueID PLAY_PS_LEVELDARKCARNIVAL = 1773672128U;
        static const AkUniqueID PLAY_PS_MENU = 2991831705U;
        static const AkUniqueID RESUME_ALL = 3679762312U;
        static const AkUniqueID STOP_ALL = 452547817U;
        static const AkUniqueID STOP_DX_2D_BREATHING_LOOP = 943723731U;
        static const AkUniqueID STOP_FX_2D_ALARM_LOOP = 1262658854U;
        static const AkUniqueID STOP_FX_2D_ASSAULTRIFLE_LOOP = 3877919460U;
        static const AkUniqueID STOP_FX_2D_FOREST_LOOP = 4240821802U;
        static const AkUniqueID STOP_FX_2D_KLAXON_LOOP = 4219667864U;
        static const AkUniqueID STOP_FX_2D_RAIN_LOOP = 3290877581U;
        static const AkUniqueID STOP_FX_2D_UNDERWATER_LOOP = 3178288442U;
        static const AkUniqueID STOP_FX_2D_WIND_LOOP = 4017587645U;
        static const AkUniqueID STOP_FX_3D_ASSAULTRIFLE_LOOP = 1042256331U;
        static const AkUniqueID STOP_FX_3D_CAR_LOOP = 3939718988U;
        static const AkUniqueID STOP_FX_3D_MISSLE_LOOP = 3286584031U;
        static const AkUniqueID STOP_MX_MUSICLOOP_01 = 3567964133U;
        static const AkUniqueID STOP_MX_MUSICLOOP_02 = 3567964134U;
        static const AkUniqueID STOP_MX_MUSICLOOP_03 = 3567964135U;
        static const AkUniqueID STOP_MX_MUSICLOOP_04 = 3567964128U;
        static const AkUniqueID STOP_MX_MUSICLOOP_05 = 3567964129U;
        static const AkUniqueID STOP_MX_MUSICLOOP_06 = 3567964130U;
        static const AkUniqueID STOP_PS_CHECKPOINT = 2676530772U;
        static const AkUniqueID STOP_PS_CREDIT = 1071468557U;
        static const AkUniqueID STOP_PS_ENEMY_JUNKHULK = 1026588013U;
        static const AkUniqueID STOP_PS_ENEMY_LILGOO = 1469364825U;
        static const AkUniqueID STOP_PS_ENEMY_SLIMEMONSTER = 3168318615U;
        static const AkUniqueID STOP_PS_LEVEL1 = 915619433U;
        static const AkUniqueID STOP_PS_LEVEL2 = 915619434U;
        static const AkUniqueID STOP_PS_LEVEL3 = 915619435U;
        static const AkUniqueID STOP_PS_LEVEL4 = 915619436U;
        static const AkUniqueID STOP_PS_LEVELBOSS = 3293679603U;
        static const AkUniqueID STOP_PS_LEVELDARKCARNIVAL = 2660629218U;
        static const AkUniqueID STOP_PS_MENU = 1411903855U;
    } // namespace EVENTS

    namespace GAME_PARAMETERS
    {
        static const AkUniqueID DX_VOLUME = 3728565946U;
        static const AkUniqueID FX_VOLUME = 3350225276U;
        static const AkUniqueID MX_VOLUME = 1441253587U;
    } // namespace GAME_PARAMETERS

    namespace BANKS
    {
        static const AkUniqueID INIT = 1355168291U;
        static const AkUniqueID SOUNDBANK = 1661994096U;
    } // namespace BANKS

    namespace BUSSES
    {
        static const AkUniqueID DX = 1836525785U;
        static const AkUniqueID FX = 1802970371U;
        static const AkUniqueID MASTER_AUDIO_BUS = 3803692087U;
        static const AkUniqueID MX = 1685527054U;
    } // namespace BUSSES

}// namespace AK

#endif