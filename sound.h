#pragma once
#include <map>
#include <fmod.hpp>

using namespace FMOD;

enum class Soundtype
{
	BGM,
	SFX
};

class CSound {
private:
	static CSound* SingletonObject;

	//fmod 클래스
	System* fmodSoundSystem;
	Sound** fmodSound;			// 리소스 자원
	Channel** fmodChannel;		// 재생 사운드 채널

	UINT buffer;	// 버퍼
	float bgmVolume;	// 볼륨
	float sfxVolume;	// 볼륨

	//사운드 저장 트리
	map<int, Sound**> sounds;
	map<int, Sound**>::iterator iter;

	// 사운드 연속재생 함수용 변수
	bool bcontinue_sound;	
	int continue_soundID;
	Channel* channelPlayed;

public:
	CSound();
	~CSound();

	static CSound* GetSingletonObject();
	static void ReleaseSingleton();

private:
	void Init();	
	void AddSound(int soundID, LPCTSTR fileName, bool loop = false);

public:	
	// 선택한 ID의 사운드 재생 (비어있는 채널에..)
	void Play(int soundID);
	// 연속재생 사운드용. 예:인트로/루프 음악을 재생
	void PlayIntroLoop(int firstID, int secondID);
	// 적응형 사운드용:업데이트 때 채널상태를 체크해서 다른사운드로 릴리즈.
	void NotifyRelease();
	// 재생중인 사운드 정지 (선택한 ID나 채널 내 선택한 타입 모두)
	void Stop(int soundID);
	void Stop(Soundtype type);
	// 재생중인 사운드 일시정지 (선택한 ID의..)
	void Pause(int soundID);
	void Pause(Soundtype type);
	// 일시 정지한 사운드 다시 재생
	void Resume(int soundID);
	void Resume(Soundtype type);
	// 모든 채널을 검사하여, 하나라도 플레이 중이라면 true반환
	bool Playing(int soundID);
	// 모든 채널을 검사하여, 하나라도 일시정지 중이라면 true반환
	bool Paused(int soundID);	

	//void Volume(int soundID, float volume);

	void SetVolume(Soundtype type,float volume);

	void Update();

	void Release();

};