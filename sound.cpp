#include "stdafx.h"
#include "sound.h"

// Singleton Initialize
CSound* CSound::SingletonObject = nullptr;

// 초기화 생성자
CSound::CSound()
{
	fmodSoundSystem = nullptr;
	fmodSound = nullptr;
	fmodChannel = nullptr;
	channelPlayed = nullptr;
	bcontinue_sound = false;
	buffer = 15;
	bgmVolume = 1.0f;
	sfxVolume = 1.0f;
	Init();
}

// 소멸자
CSound::~CSound()
{
	Release();
}

CSound* CSound::GetSingletonObject()
{
	if (SingletonObject == nullptr)
	{
		SingletonObject = new CSound();
	}
	return SingletonObject;
}

void CSound::ReleaseSingleton()
{
	if (SingletonObject != nullptr)
	{
		SingletonObject->Release();

		delete SingletonObject;
		SingletonObject = nullptr;
	}
}

void CSound::Init()
{
	// 시스템 형성
	System_Create(&fmodSoundSystem);

	// 사운드 리소스 및 채널 생성
	fmodSoundSystem->init(buffer, FMOD_INIT_NORMAL, 0);
	fmodSound = new Sound * [buffer];		// 버퍼 크기 만큼 음원을 가리키는 포인터 배열
	fmodChannel = new Channel * [buffer];	// 버퍼 크기 만큼 채널을 가리키는 포인터 배열

	memset(fmodSound, 0, sizeof(Sound*) * buffer);
	memset(fmodChannel, 0, sizeof(Channel*) * buffer);

	for (int i = 0; i < TABLE_MANAGER->FindLastIndexOfTable(TableType::Sound); i++)
	{
		AddSound(
			TABLE_MANAGER->FindIDfromIndex(i, TableType::Sound),
			TABLE_MANAGER->FindSoundDataFromID(TABLE_MANAGER->FindIDfromIndex(i, TableType::Sound))->SoundFileName,
			TABLE_MANAGER->FindSoundDataFromID(TABLE_MANAGER->FindIDfromIndex(i, TableType::Sound))->IsLoof
		);

	}


}

void CSound::AddSound(int soundID, LPCTSTR fileName, bool loop)
{
	// 루프 여부를 판별하여, 음원 추가
	if (loop == true)
	{
		fmodSoundSystem->createStream(
			fileName,
			FMOD_LOOP_NORMAL,
			nullptr,
			&fmodSound[sounds.size()]
		);
	}
	else
	{
		fmodSoundSystem->createStream(
			fileName,
			FMOD_DEFAULT,
			nullptr,
			&fmodSound[sounds.size()]
		);
	}

	// 추가된 음원 맵에 넣어주기
	sounds.insert(make_pair(soundID, &fmodSound[sounds.size()]));
}

void CSound::Play(int soundID)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;
		// 만약 ID와 일치하는 음원이 있다면..
		if (soundID == iter->first)
		{
			// 재생함
			fmodSoundSystem->playSound(
				*iter->second,
				NULL,
				false,
				&fmodChannel[count]
			);
			// 지정된 볼륨으로..
			if (TABLE_MANAGER->FindSoundDataFromID(soundID)->IsBgm)
				fmodChannel[count]->setVolume(bgmVolume);
			else
				fmodChannel[count]->setVolume(sfxVolume);
		}
	}
}

void CSound::PlayIntroLoop(int IntroID, int LoopID)
{
	int count = 0;
	bcontinue_sound = true;
	continue_soundID = LoopID;

	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;
		// 만약 ID와 일치하는 음원이 있다면..
		if (IntroID == iter->first)
		{
			// 재생함
			fmodSoundSystem->playSound(
				*iter->second,
				NULL,
				false,
				&fmodChannel[count]
			);
			// 재생중인 채널을 기억한다..
			channelPlayed = fmodChannel[count];

			// 지정된 볼륨으로..
			if (TABLE_MANAGER->FindSoundDataFromID(IntroID)->IsBgm)
				fmodChannel[count]->setVolume(bgmVolume);
			else
				fmodChannel[count]->setVolume(sfxVolume);
		}
	}
}

void CSound::NotifyRelease()
{
	bool bPlay;
	channelPlayed->isPlaying(&bPlay);
	if (bcontinue_sound == true
		&& bPlay == false)
	{
		int count = 0;
		iter = sounds.begin();
		for (iter; iter != sounds.end(); ++iter)
		{
			count++;
			// 만약 ID와 일치하는 음원이 있다면..
			if (continue_soundID == iter->first)
			{
				// 재생함
				fmodSoundSystem->playSound(
					*iter->second,
					NULL,
					false,
					&fmodChannel[count]
				);

				// 지정된 볼륨으로..
				if (TABLE_MANAGER->FindSoundDataFromID(continue_soundID)->IsBgm)
					fmodChannel[count]->setVolume(bgmVolume);
				else
					fmodChannel[count]->setVolume(sfxVolume);
			}
		}
		bcontinue_sound = false;
	}
}

void CSound::Stop(int soundID)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;
		// 만약 ID와 일치하는 음원이 있다면..
		if (soundID == iter->first)
		{
			//멈춰~!
			fmodChannel[count]->stop();
			break;
		}
	}
}

void CSound::Stop(Soundtype type)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;

		if (type == Soundtype::BGM)
		{
			if (TABLE_MANAGER->FindSoundDataFromID(iter->first)->IsBgm)
			{
				fmodChannel[count]->stop();
			}
		}
		if (type == Soundtype::SFX)
		{
			if (TABLE_MANAGER->FindSoundDataFromID(iter->first)->IsBgm == false)
			{
				fmodChannel[count]->stop();
			}
		}
	}
}

void CSound::Pause(Soundtype type)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;

		if (type == Soundtype::BGM)
		{
			if (TABLE_MANAGER->FindSoundDataFromID(iter->first)->IsBgm)
			{
				fmodChannel[count]->setPaused(true);
			}
		}
		if (type == Soundtype::SFX)
		{
			if (TABLE_MANAGER->FindSoundDataFromID(iter->first)->IsBgm == false)
			{
				fmodChannel[count]->setPaused(true);
			}
		}
	}
}

void CSound::Pause(int soundID)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;
		// 만약 ID와 일치하는 음원이 있다면..
		if (soundID == iter->first)
		{
			fmodChannel[count]->setPaused(true);
			break;
		}
	}
}

void CSound::Resume(int soundID)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;
		// 만약 ID와 일치하는 음원이 있다면..
		if (soundID == iter->first)
		{
			fmodChannel[count]->setPaused(false);
			break;
		}
	}
}

void CSound::Resume(Soundtype type)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;

		if (type == Soundtype::BGM)
		{
			if (TABLE_MANAGER->FindSoundDataFromID(iter->first)->IsBgm)
			{
				fmodChannel[count]->setPaused(false);
			}
		}
		if (type == Soundtype::SFX)
		{
			if (TABLE_MANAGER->FindSoundDataFromID(iter->first)->IsBgm == false)
			{
				fmodChannel[count]->setPaused(false);
			}
		}
	}
}

bool CSound::Playing(int soundID)
{
	bool bPlay = false;
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;
		// 만약 ID와 일치하는 음원이 있다면..
		if (soundID == iter->first)
		{
			// 재생중이라면, bPlay를 true로 변경
			fmodChannel[count]->isPlaying(&bPlay);
			break;
		}
	}
	return bPlay;
}

bool CSound::Paused(int soundID)
{
	bool bPause = false;
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;
		// 만약 ID와 일치하는 음원이 있다면..
		if (soundID == iter->first)
		{

			fmodChannel[count]->getPaused(&bPause);
			break;
		}
	}
	return bPause;
}

//void CSound::Volume(int soundID, float volume)
//{
//	int count = 0;
//	iter = sounds.begin();
//	for (iter; iter != sounds.end(); ++iter)
//	{
//		count++;
//		// 만약 ID와 일치하는 음원이 있다면..
//		if (soundID == iter->first)
//		{
//			if (TABLE_MANAGER->FindSoundDataFromID(soundID)->IsBgm)
//				fmodChannel[count]->setVolume(volume * bgmVolume);
//			else
//				fmodChannel[count]->setVolume(volume * sfxVolume);						
//			break;
//		}
//	}
//}

void CSound::Update()
{
	fmodSoundSystem->update();
}

void CSound::SetVolume(Soundtype type, float volume)
{
	if (type == Soundtype::BGM)
	{
		bgmVolume = volume;
	}
	else
	{
		sfxVolume = volume;
	}

	int count = 0;
	int soundID;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;
		// temp soundId에 현재 사운드 채널에 있는 ID를 받아와서..
		soundID = iter->first;
		// 해당 사운드가 bgm인지, sfx인지를 판별하여, 현재 재생중인 사운드에도 해당 볼륨값으로 조정한다.
		if (TABLE_MANAGER->FindSoundDataFromID(soundID)->IsBgm)
			fmodChannel[count]->setVolume(bgmVolume);
		else
			fmodChannel[count]->setVolume(sfxVolume);

	}


}

void CSound::Release()
{
	if (fmodChannel != nullptr || fmodSound != nullptr)
	{
		for (UINT i = 0; i < buffer; i++)
		{
			if (fmodChannel != nullptr && fmodChannel[i])
			{
				fmodChannel[i]->stop();
			}
			// 리소스 해제
			if (fmodSound != nullptr && fmodSound[i])
			{
				fmodSound[i]->release();
			}

		}
	}
	delete(fmodChannel);
	delete(fmodSound);

	// 시스템 닫기

	if (fmodSoundSystem != nullptr)
	{
		fmodSoundSystem->release();
		fmodSoundSystem->close();
	}

	// MAP 비우기
	sounds.clear();
}
