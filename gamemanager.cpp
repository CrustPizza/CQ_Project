#pragma once
#include "stdafx.h"

//ĳ���͵� �����ϰ� �����ϴ� ����ĳ���͸Ŵ���.

//�����ؾ��Ұ�.
//ī�޶� ��ġ�� ���� �տ��ִ� hero�� ��������?
//���������� attackrange�� ���� ���ʿ� ������ ���ݽ���.
//
//

//�̱��� ����
GameManager* GameManager::SingletonObject = nullptr;

GameManager::GameManager()
{
	blockLocationY = (int)WinInfo::Height - 155;
	gold = 0;
	gameOver = 0;
}

GameManager::~GameManager()
{

}

/// �̱��� ���� �� ��ȯ
GameManager* GameManager::GetSingletonObject()
{
	// �̱��� ��ü�� ���� ���� �����ϰ� �����Ѵ�.
	// �̹��� �Ŵ����� ��� �ʱ� ���� �ʿ��ؼ� ó���� ���ڸ� ���� �޾Ƽ� �����ϰ�
	// �Ŀ� ����Ʈ�� 0�� ����Ǿ� �μ����� ����� �� �ִ�.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new GameManager();
	}

	return SingletonObject;
}

/// �̱��� ����
void GameManager::ReleaseSingleton()
{
	// �̱��� ��ü�� �ִٸ� �����Ѵ�.
	if (SingletonObject != nullptr)
	{
		delete SingletonObject;
		SingletonObject = nullptr;
	}
}

bool sortPred(Monster* a, Monster* b)
{
	return a->GetPositionx() < b->GetPositionx();
}

void GameManager::monSort()
{
	sort(_MonsterList.begin(), _MonsterList.end(), sortPred);
}

static int timer = 0;

void GameManager::Update()
{
	gameOver = true;

	//�� ���� ������ FSM, ��������
	for (int i = 0; i < _HeroList.size(); i++)
	{
		_HeroList[i]->Update();

		if (_HeroList[i]->Getstate() != AnimType::DEATH)
		{
			gameOver = false;
		}
	}

	// ���� ����
	if (gameOver == true)
	{
		if (INPUT_MANAGER->KeyCheck(VK_SPACE, KeyState::DOWN) == true
			|| INPUT_MANAGER->KeyCheck(VK_LBUTTON, KeyState::DOWN) == true
			|| INPUT_MANAGER->KeyCheck(VK_RETURN, KeyState::DOWN) == true)
		{
			SCENE_MANAGER->SceneBack();
		}

		return;
	}

	// ����
	for (int i = 0; i < _MonsterList.size(); i++)
	{
		_MonsterList[i]->Update();
	}
	monSort();

	// ��
	for (auto iter = _BlockList.begin(); iter != _BlockList.end(); iter++)
	{
		if ((*iter)->Update() == true)
		{
			(*iter)->CheckChain(iter);
		}
	}

	if (_BlockList.size() < 9 && ++timer % 30 == 0)
	{
		InstantiateBlock();
	}

	BlockClick();

	// ��ų
	for (int i = 0; i < _SkillList.size(); )
	{
		if (_SkillList[i]->Update() == true)
		{
			if (_SkillList[i]->GetSkillID() == 9032)
			{
				curBalloon = nullptr;
			}

			delete _SkillList[i];
			DeleteSkill(_SkillList[i]);
		}
		else
		{
			i++;
		}
	}
}

void GameManager::Render()
{
	int cameraX = SCENE_MANAGER->GetCamera();

	// ����
	for (int i = 0; i < _HeroList.size(); i++)
	{
		if (_HeroList[i]->GetPositionx() - cameraX < -50
			|| _HeroList[i]->GetPositionx() - cameraX >(int)WinInfo::Width + 50)
		{
			continue;
		}

		_HeroList[i]->Render();
	}

	// ����
	for (int i = 0; i < _MonsterList.size(); i++)
	{
		if (_MonsterList[i]->GetPositionx() - cameraX < -50
			|| _MonsterList[i]->GetPositionx() - cameraX >(int)WinInfo::Width + 50)
		{
			continue;
		}

		_MonsterList[i]->Render();
	}

	// ��ų
	for (int i = 0; i < _SkillList.size(); i++)
	{
		if (_SkillList[i]->GetPosition().x - cameraX < -50
			|| _SkillList[i]->GetPosition().x - cameraX >(int)WinInfo::Width + 50)
		{
			continue;
		}

		_SkillList[i]->Render();
	}

	// ������ & ��
	for (auto iter = numImageList.begin(); iter != numImageList.end(); )
	{
		if (iter->num >= 0)
		{
			// ������
			RENDER_MANAGER->DrawAlphaNumberImage(true, iter->num, iter->xy.x - cameraX, iter->xy.y, iter->frame -= 300 / (int)WinInfo::Frame);
		}
		else
		{
			// ��
			RENDER_MANAGER->DrawAlphaNumberImage(false, -iter->num, iter->xy.x - cameraX, iter->xy.y, iter->frame -= 300 / (int)WinInfo::Frame);
		}

		if (iter->frame <= 30)
		{
			iter = numImageList.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	// �������ͽ� â
	DrawHeroStatus();

	// ���� ����â
	DrawCoinStatus((int)WinInfo::Width - 310, 5);

	if (gameOver == true)
	{
		RENDER_MANAGER->DrawTransImage(UI_GAME_OVER,
			((int)WinInfo::Width - IMAGE_MANAGER->GetImage(UI_GAME_OVER)->width) / 2,
			((int)WinInfo::Height - IMAGE_MANAGER->GetImage(UI_GAME_OVER)->height) / 2);

		// ���� ����â
		DrawCoinStatus(((int)WinInfo::Width - IMAGE_MANAGER->GetImage(UI_STATUS_COIN)->width) / 2,
			((int)WinInfo::Height + IMAGE_MANAGER->GetImage(UI_GAME_OVER)->height) / 2 + 50);
	}
}

void GameManager::Reset() {
	for (int i = 0; i < _HeroList.size(); i++) {
		delete(_HeroList.at(i));
	}
	_HeroList.clear();
	for (int i = 0; i < _MonsterList.size(); i++) {
		delete(_MonsterList.at(i));
	}
	_MonsterList.clear();
	for (int i = 0; i < _SkillList.size(); i++) {
		delete(_SkillList.at(i));
	}
	_SkillList.clear();
	list<Block*>::iterator iter;
	for (iter = _BlockList.begin(); iter != _BlockList.end(); iter++) {
		delete(*iter);
	}
	_BlockList.clear();

	numImageList.clear();

	gameOver = false;
	gold = 0;
}

//����� ��� ����
//�����͸� �о�� �־��ش�. 
void GameManager::InstantiateHeros()
{
	//�ݺ������� ������ �ְ�, �����ؼ� vector�迭�� pushback ���ش�.
	//������ ���̺� �޾Ƽ� �����Ұ�
	POINT initialcoord = { 100, (int)WinInfo::Height - 190 };

	Hero* _newChar = new Hero(101, initialcoord, 100, 150);

	_HeroList.push_back(_newChar);

	initialcoord.x += 100;
	_newChar = new Hero(103, initialcoord, 100, 150);

	_HeroList.push_back(_newChar);

	initialcoord.x += 200;
	_newChar = new Hero(102, initialcoord, 100, 150);

	_HeroList.push_back(_newChar);
}

//���� ��� ����
//�����͸� �о�� �־��ش�
void GameManager::InstantiateMonsters(int id, double statRate, int dx)
{
	//�ݺ������� ������ �ְ�, �����ؼ� vector�迭�� pushback ���ش�.
	POINT initialcoord = { dx, (int)WinInfo::Height - 190 };

	Monster* _newChar = new Monster(id, statRate, initialcoord);

	_MonsterList.push_back(_newChar);
}

void GameManager::InstantiateBlock()
{
	int randnum = 0;
	int id = 9012;

	if (GetHerolistSize() != 0)
	{
		randnum = rand() % _HeroList.size();
	}

	Block* _newBlock = new Block(id + 10 * randnum, _BlockList.size(), blockLocationY);

	_BlockList.push_back(_newBlock);
}

void GameManager::InstantiateSkill(POINT pos, double _damage, float _duration, int _skillId)
{
	//����������� ��.
	Skill* _newSkill = nullptr;
	switch (_skillId)
	{
		case 9011:
			_newSkill = new MagicAttack(pos, _damage, _duration, _skillId, 50, 50, Layer::HEROSKILL);
			SOUND_MANAGER->Play(510004);
			break;
		case 9012:
			_newSkill = new Thunder(pos, _damage, _duration, _skillId, 200, 500, Layer::HEROSKILL);
			SOUND_MANAGER->Play(510010);
			break;
		case 9021:
			_newSkill = new SwordBasic(pos, _damage, _duration, _skillId, 117, 189, Layer::HEROSKILL);
			SOUND_MANAGER->Play(510006);
			break;
		case 9022:
			_newSkill = new SwordWave(pos, _damage, _duration, _skillId, 100, 100, Layer::HEROSKILL);
			SOUND_MANAGER->Play(510009);
			break;
		case 9031:
			_newSkill = new SwordBasic(pos, _damage, _duration, _skillId, 117, 189, Layer::HEROSKILL);
			SOUND_MANAGER->Play(510006);
			break;
		case 9032:
			if (curBalloon == nullptr)
			{
				curBalloon = new Balloon(pos, _damage, _duration, _skillId, 100, 100, Layer::HEROSKILL);
				_newSkill = (Skill*)curBalloon;
			}
			else
			{
				curBalloon->Init(_duration, _damage);
			}
			SOUND_MANAGER->Play(510003);
			break;
		case 9111:
			_newSkill = new Bullet(pos, _damage, _duration, _skillId, 50, 50, Layer::MONSTERSKILL);
			break;
		case 9121: 
			_newSkill = new Bullet(pos, _damage, _duration, _skillId, 50, 50, Layer::MONSTERSKILL);
			SOUND_MANAGER->Play(510004);
			break;
		case 9131:
			_newSkill = new Bullet(pos, _damage, _duration, _skillId, 50, 50, Layer::MONSTERSKILL);
			SOUND_MANAGER->Play(510004);
			break;
		case 9141:
			_newSkill = new SwordBasic(pos, _damage, _duration, _skillId, 200, 400, Layer::MONSTERSKILL);
			SOUND_MANAGER->Play(510008);
			break;
	}
	if (_newSkill) _SkillList.push_back(_newSkill);
}

void GameManager::ActivateMonsterSkill(POINT pos, double _damage, float _duration, int _skillId)
{
	Skill* _newSkill = nullptr;

	switch (_skillId)
	{
	case 9112:
		GAME_MANAGER->InstantiateMonsters(204, 4.0f, pos.x);
		GAME_MANAGER->InstantiateMonsters(204, 4.0f, pos.x);
		break;
	case 9113:
		_newSkill = new PierceBullet(pos, _damage, _duration, _skillId, 50, 50, Layer::MONSTERSKILL);
		break;
	default:
		break;
	}

	if (_newSkill != nullptr)
	{
		_SkillList.push_back(_newSkill);
	}
}

/// <summary>
/// ���� Ŭ������ �� �ش� ���� �����ϰ� ĳ���� ��ų �ߵ�
/// </summary>
void GameManager::BlockClick()
{
	bool blockErased = false;
	int count = 0;

	for (auto iter = _BlockList.begin(); iter != _BlockList.end(); )
	{
		if (INPUT_MANAGER->IsMouseOn((*iter)->GetBlockRect()) == false
			|| INPUT_MANAGER->KeyCheck(VK_LBUTTON, KeyState::DOWN) == false)
		{
			if (blockErased == true)
			{
				(*iter)->ChangeCanMove(count);
			}

			count++;
			iter++;
			continue;
		}

		for (int i = 0; i < _HeroList.size(); i++)
		{
			if ((*iter)->GetSkillId() == _HeroList.at(i)->GetSkillEffectId((*iter)->chainCnt))
			{
				_HeroList.at(i)->Skills((*iter)->chainCnt);
			}
		}

		iter = BlockErase(iter, count);
		blockErased = true;
	}
}

/// <summary>
/// Ŭ���� ���� ü�ε� ������ �����ϴ� �Լ�
/// </summary>
list<Block*>::iterator GameManager::BlockErase(list<Block*>::iterator iter, int& count)
{
	while ((*iter)->blockState == BlockState::CHAINRIGHT)
	{
		iter--;
		count--;
	}

	for (int i = (*iter)->chainCnt; i > 0; i--)
	{
		iter = _BlockList.erase(iter);
	}

	return iter;
}

void GameManager::DrawBlock()
{
	// ���
	for (auto iter = _BlockList.begin(); iter != _BlockList.end(); iter++)
	{
		RENDER_MANAGER->DrawTransImage((*iter)->GetSkillId(),
			(*iter)->GetBlockRect().left, (*iter)->GetBlockRect().top);

		if ((*iter)->blockState == BlockState::CHAINRIGHT)
		{
			RENDER_MANAGER->DrawTransImage(UI_CHAIN_01 + ((*iter)->GetSkillId() - 9012) / 10,
				(*iter)->GetBlockRect().right - 5, (*iter)->GetBlockRect().top + 10);
		}
	}
}

//vector(_herolist) �ʱ�ȭ
void GameManager::ClearList() {

	_HeroList.clear();
	_MonsterList.clear();
	_BlockList.clear();
}

//������ ũ���ȯ
int GameManager::GetHerolistSize() {
	return _HeroList.size();
}

int GameManager::GetMonsterlistSize() {
	return _MonsterList.size();
}

int GameManager::GetGold()
{
	return gold;
}

//i��°�� �ִ� index ��ȯ
Character* GameManager::GetHerodata(int i) {
	if (_HeroList.at(i) == NULL) {
		return NULL;
	}
	return _HeroList.at(i);
}

Character* GameManager::GetMonsterdata(int i) {
	if (_MonsterList.at(i) == NULL)
	{
		return NULL;
	}
	return _MonsterList.at(i);
}

Skill* GameManager::GetSkilldata(int i) {
	if (_SkillList.at(i) == NULL)
	{
		return NULL;
	}
	return _SkillList.at(i);
}

void GameManager::DeleteSkill(Skill* pskill)
{
	for (int i = 0; i < _SkillList.size(); i++)
	{
		if (_SkillList.at(i) == pskill)
		{
			swap(_SkillList[i], _SkillList.back());
			_SkillList.pop_back();
			return;
		}
	}
}

void GameManager::DeleteMonster(Monster* monster)
{
	for (int i = 0; i < _MonsterList.size(); i++)
	{
		if (_MonsterList.at(i) == monster)
		{
			swap(_MonsterList[i], _MonsterList.back());
			_MonsterList.pop_back();
			gold += rand() % 5 + 1;
			return;
		}
	}
}
//�������� �ִ� ĳ������ ��ǥ���� �����ͼ� ��������Ʈ �����ġ�� �� ����
//n(�����, �߾����� �� ��ġ) + ��������ġ - ���� ĳ������ ��ġ x
Character* GameManager::GetFrontline()
{
	int max = 0;

	for (int i = 0; i < _HeroList.size(); i++)
	{
		if (_HeroList.at(i)->Getstate() != AnimType::DEATH && (_HeroList.at(max)->GetPositionx() < _HeroList.at(i)->GetPositionx()))
		{
			max = i;
		}
	}

	return _HeroList.at(max);

}

Character* GameManager::GetMonsterFrontline()
{
	if (_MonsterList.size() == 0) return nullptr;

	return _MonsterList.at(0);
}

list<NumImageData>& GameManager::GetNumImageQueue()
{
	return numImageList;
}

void GameManager::DrawHeroStatus()
{
	for (int i = 0; i < _HeroList.size(); i++)
	{
		RENDER_MANAGER->DrawTransImage(UI_STATUS_HERO, (i * 200) + 5, 5);
		RENDER_MANAGER->DrawTransImage(ARCHON_PROFILE + i, (i * 200) + 16, 16);

		// HP ����â
		RENDER_MANAGER->DrawBarImage(UI_STATUS_EMPTY_HP, (i * 200) + 105, 55, (float)_HeroList[i]->GetHp() / _HeroList[i]->GetMaxHp());
		RENDER_MANAGER->DrawColorNumberImage(true, _HeroList[i]->GetHp(), (i * 200) + 198, 62);

		// SP�� ����ϰ� �ȴٸ� ����� �ƴ� �����͸� ���� ����
		RENDER_MANAGER->DrawBarImage(UI_STATUS_EMPTY_SP, (i * 200) + 105, 80, 1.0f);
		RENDER_MANAGER->DrawColorNumberImage(true, 100, (i * 200) + 198, 87);
	}
}

void GameManager::DrawCoinStatus(int x, int y)
{
	RENDER_MANAGER->DrawTransImage(UI_STATUS_COIN, x, y);
	RENDER_MANAGER->DrawColorNumberImage(false, GAME_MANAGER->GetGold(), x + IMAGE_MANAGER->GetImage(UI_STATUS_COIN)->width - 10, y + 10);
}