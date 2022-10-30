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

	//fmod Ŭ����
	System* fmodSoundSystem;
	Sound** fmodSound;			// ���ҽ� �ڿ�
	Channel** fmodChannel;		// ��� ���� ä��

	UINT buffer;	// ����
	float bgmVolume;	// ����
	float sfxVolume;	// ����

	//���� ���� Ʈ��
	map<int, Sound**> sounds;
	map<int, Sound**>::iterator iter;

	// ���� ������� �Լ��� ����
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
	// ������ ID�� ���� ��� (����ִ� ä�ο�..)
	void Play(int soundID);
	// ������� �����. ��:��Ʈ��/���� ������ ���
	void PlayIntroLoop(int firstID, int secondID);
	// ������ �����:������Ʈ �� ä�λ��¸� üũ�ؼ� �ٸ������ ������.
	void NotifyRelease();
	// ������� ���� ���� (������ ID�� ä�� �� ������ Ÿ�� ���)
	void Stop(int soundID);
	void Stop(Soundtype type);
	// ������� ���� �Ͻ����� (������ ID��..)
	void Pause(int soundID);
	void Pause(Soundtype type);
	// �Ͻ� ������ ���� �ٽ� ���
	void Resume(int soundID);
	void Resume(Soundtype type);
	// ��� ä���� �˻��Ͽ�, �ϳ��� �÷��� ���̶�� true��ȯ
	bool Playing(int soundID);
	// ��� ä���� �˻��Ͽ�, �ϳ��� �Ͻ����� ���̶�� true��ȯ
	bool Paused(int soundID);	

	//void Volume(int soundID, float volume);

	void SetVolume(Soundtype type,float volume);

	void Update();

	void Release();

};