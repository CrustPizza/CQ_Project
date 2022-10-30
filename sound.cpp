#include "stdafx.h"
#include "sound.h"

// Singleton Initialize
CSound* CSound::SingletonObject = nullptr;

// �ʱ�ȭ ������
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

// �Ҹ���
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
	// �ý��� ����
	System_Create(&fmodSoundSystem);

	// ���� ���ҽ� �� ä�� ����
	fmodSoundSystem->init(buffer, FMOD_INIT_NORMAL, 0);
	fmodSound = new Sound * [buffer];		// ���� ũ�� ��ŭ ������ ����Ű�� ������ �迭
	fmodChannel = new Channel * [buffer];	// ���� ũ�� ��ŭ ä���� ����Ű�� ������ �迭

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
	// ���� ���θ� �Ǻ��Ͽ�, ���� �߰�
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

	// �߰��� ���� �ʿ� �־��ֱ�
	sounds.insert(make_pair(soundID, &fmodSound[sounds.size()]));
}

void CSound::Play(int soundID)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); ++iter)
	{
		count++;
		// ���� ID�� ��ġ�ϴ� ������ �ִٸ�..
		if (soundID == iter->first)
		{
			// �����
			fmodSoundSystem->playSound(
				*iter->second,
				NULL,
				false,
				&fmodChannel[count]
			);
			// ������ ��������..
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
		// ���� ID�� ��ġ�ϴ� ������ �ִٸ�..
		if (IntroID == iter->first)
		{
			// �����
			fmodSoundSystem->playSound(
				*iter->second,
				NULL,
				false,
				&fmodChannel[count]
			);
			// ������� ä���� ����Ѵ�..
			channelPlayed = fmodChannel[count];

			// ������ ��������..
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
			// ���� ID�� ��ġ�ϴ� ������ �ִٸ�..
			if (continue_soundID == iter->first)
			{
				// �����
				fmodSoundSystem->playSound(
					*iter->second,
					NULL,
					false,
					&fmodChannel[count]
				);

				// ������ ��������..
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
		// ���� ID�� ��ġ�ϴ� ������ �ִٸ�..
		if (soundID == iter->first)
		{
			//����~!
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
		// ���� ID�� ��ġ�ϴ� ������ �ִٸ�..
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
		// ���� ID�� ��ġ�ϴ� ������ �ִٸ�..
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
		// ���� ID�� ��ġ�ϴ� ������ �ִٸ�..
		if (soundID == iter->first)
		{
			// ������̶��, bPlay�� true�� ����
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
		// ���� ID�� ��ġ�ϴ� ������ �ִٸ�..
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
//		// ���� ID�� ��ġ�ϴ� ������ �ִٸ�..
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
		// temp soundId�� ���� ���� ä�ο� �ִ� ID�� �޾ƿͼ�..
		soundID = iter->first;
		// �ش� ���尡 bgm����, sfx������ �Ǻ��Ͽ�, ���� ������� ���忡�� �ش� ���������� �����Ѵ�.
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
			// ���ҽ� ����
			if (fmodSound != nullptr && fmodSound[i])
			{
				fmodSound[i]->release();
			}

		}
	}
	delete(fmodChannel);
	delete(fmodSound);

	// �ý��� �ݱ�

	if (fmodSoundSystem != nullptr)
	{
		fmodSoundSystem->release();
		fmodSoundSystem->close();
	}

	// MAP ����
	sounds.clear();
}
