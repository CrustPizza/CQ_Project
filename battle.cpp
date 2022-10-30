#include "stdafx.h"
#include "battle.h"

Battle::Battle()
{
	waveData = TABLE_MANAGER->FindGameRuleData();
	Init();
}

Battle::~Battle()
{

}

void Battle::Init()
{
	closeScene = false;
	startScene = true;
	index = 0;
	delay = 0;
	enemyStatRate = 1.0f;
	GAME_MANAGER->InstantiateHeros();
	RoundStart();
}

void Battle::Update()
{
	if (closeScene == true)
	{
		if (OPTION_MANAGER->SceneClose() != true)
		{
			return;
		}

		GAME_MANAGER->Reset();
		SCENE_MANAGER->SceneChange("Intro");
		SOUND_MANAGER->Stop(500002);
		Init();
		return;
	}

	// ������ ���۵Ǿ��� �� ȭ�� ��ȯ ȿ�� ����
	if (startScene == true)
	{
		if (OPTION_MANAGER->SceneOpen() != true)
		{
			return;
		}
		SOUND_MANAGER->PlayIntroLoop(500002, 500003);
		startScene = false;
	}

	SOUND_MANAGER->NotifyRelease();

	//������Ʈ�Լ�
	GAME_MANAGER->Update();

	RoundStart();
}

void Battle::Render()
{
	// ���
	DrawBackGround(0, (int)WinInfo::Height - 720);

	GAME_MANAGER->Render();

	// ����
	DrawBlockBoard(0, (int)WinInfo::Height - 160);

	// �ڽ� ����â
	DrawBoxStatus((int)WinInfo::Width - 450, 5);
}

void Battle::SetChange()
{
	SOUND_MANAGER->Stop(500002);
	SOUND_MANAGER->Stop(500003);
	closeScene = true;
}

void Battle::RoundStart()
{
	if (delay++ >= waveData->WaveDelay[index] * (int)WinInfo::Frame)
	{
		GAME_MANAGER->InstantiateMonsters(waveData->WaveMonsterIDs[index++], enemyStatRate, (int)WinInfo::Width + SCENE_MANAGER->GetCamera() + 100);
		
		if (index >= waveData->WaveMonsterNumber)
		{
			index = 0;
			delay = 0;
			enemyStatRate *= 1.1f;
		}
	}
}

/// <summary>
/// ��� �׸���
/// </summary>
void Battle::DrawBackGround(int x, int y)
{
	RENDER_MANAGER->DrawScrollImage(BG_LAYER1, x, y - 180, ScrollLayer::GROUND);
	RENDER_MANAGER->DrawScrollTransImage(BG_LAYER2, x, y, ScrollLayer::BACK01);
	RENDER_MANAGER->DrawScrollTransImage(BG_LAYER3, x, y, ScrollLayer::BACK02);
	RENDER_MANAGER->DrawScrollTransImage(BG_LAYER4, x, y, ScrollLayer::BACK03);
}

/// <summary>
/// ����� �׸���
/// </summary>
void Battle::DrawBlockBoard(int x, int y)
{
	RENDER_MANAGER->DrawImage(UI_BOARD_01, x, y);
	GAME_MANAGER->DrawBlock();
	RENDER_MANAGER->DrawTransImage(UI_BOARD_02, x, y);
}

/// <summary>
/// �ڽ� ���� �׸���
/// </summary>
void Battle::DrawBoxStatus(int x, int y)
{
	RENDER_MANAGER->DrawTransImage(UI_STATUS_BOX, x, y);
}

/// <summary>
/// ���� ����â �׸���
/// </summary>
void Battle::DrawCoinStatus(int x, int y)
{
	RENDER_MANAGER->DrawTransImage(UI_STATUS_COIN, x, y);
	RENDER_MANAGER->DrawColorNumberImage(false, GAME_MANAGER->GetGold(), x + IMAGE_MANAGER->GetImage(UI_STATUS_COIN)->width - 10, y + 10);
}