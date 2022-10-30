#pragma once
#include "stdafx.h"

//캐릭터들 생성하고 관리하는 게임캐릭터매니저.

//생각해야할것.
//카메라 위치는 가장 앞에있는 hero를 기준으로?
//적을만나면 attackrange에 따라 안쪽에 있을때 공격시작.
//
//

//싱글톤 선언
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

/// 싱글톤 생성 및 반환
GameManager* GameManager::GetSingletonObject()
{
	// 싱글톤 객체가 없을 때만 생성하고 리턴한다.
	// 이미지 매니저의 경우 초기 값이 필요해서 처음에 인자를 전달 받아서 생성하고
	// 후에 디폴트로 0이 적용되어 인수없이 사용할 수 있다.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new GameManager();
	}

	return SingletonObject;
}

/// 싱글톤 해제
void GameManager::ReleaseSingleton()
{
	// 싱글톤 객체가 있다면 해제한다.
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

	//각 개별 몬스터의 FSM, 렌더관리
	for (int i = 0; i < _HeroList.size(); i++)
	{
		_HeroList[i]->Update();

		if (_HeroList[i]->Getstate() != AnimType::DEATH)
		{
			gameOver = false;
		}
	}

	// 게임 오버
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

	// 몬스터
	for (int i = 0; i < _MonsterList.size(); i++)
	{
		_MonsterList[i]->Update();
	}
	monSort();

	// 블럭
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

	// 스킬
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

	// 영웅
	for (int i = 0; i < _HeroList.size(); i++)
	{
		if (_HeroList[i]->GetPositionx() - cameraX < -50
			|| _HeroList[i]->GetPositionx() - cameraX >(int)WinInfo::Width + 50)
		{
			continue;
		}

		_HeroList[i]->Render();
	}

	// 몬스터
	for (int i = 0; i < _MonsterList.size(); i++)
	{
		if (_MonsterList[i]->GetPositionx() - cameraX < -50
			|| _MonsterList[i]->GetPositionx() - cameraX >(int)WinInfo::Width + 50)
		{
			continue;
		}

		_MonsterList[i]->Render();
	}

	// 스킬
	for (int i = 0; i < _SkillList.size(); i++)
	{
		if (_SkillList[i]->GetPosition().x - cameraX < -50
			|| _SkillList[i]->GetPosition().x - cameraX >(int)WinInfo::Width + 50)
		{
			continue;
		}

		_SkillList[i]->Render();
	}

	// 데미지 & 힐
	for (auto iter = numImageList.begin(); iter != numImageList.end(); )
	{
		if (iter->num >= 0)
		{
			// 데미지
			RENDER_MANAGER->DrawAlphaNumberImage(true, iter->num, iter->xy.x - cameraX, iter->xy.y, iter->frame -= 300 / (int)WinInfo::Frame);
		}
		else
		{
			// 힐
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

	// 스테이터스 창
	DrawHeroStatus();

	// 코인 상태창
	DrawCoinStatus((int)WinInfo::Width - 310, 5);

	if (gameOver == true)
	{
		RENDER_MANAGER->DrawTransImage(UI_GAME_OVER,
			((int)WinInfo::Width - IMAGE_MANAGER->GetImage(UI_GAME_OVER)->width) / 2,
			((int)WinInfo::Height - IMAGE_MANAGER->GetImage(UI_GAME_OVER)->height) / 2);

		// 코인 상태창
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

//히어로 목록 생성
//데이터를 읽어와 넣어준다. 
void GameManager::InstantiateHeros()
{
	//반복문으로 데이터 넣고, 생성해서 vector배열에 pushback 해준다.
	//데이터 테이블 받아서 수정할곳
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

//몬스터 목록 생성
//데이터를 읽어와 넣어준다
void GameManager::InstantiateMonsters(int id, double statRate, int dx)
{
	//반복문으로 데이터 넣고, 생성해서 vector배열에 pushback 해준다.
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
	//수정해줘야할 곳.
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
/// 블럭을 클릭했을 때 해당 블럭을 삭제하고 캐릭터 스킬 발동
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
/// 클릭된 블럭과 체인된 블럭들을 삭제하는 함수
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
	// 블록
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

//vector(_herolist) 초기화
void GameManager::ClearList() {

	_HeroList.clear();
	_MonsterList.clear();
	_BlockList.clear();
}

//벡터의 크기반환
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

//i번째에 있는 index 반환
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
//최전선의 있는 캐릭터의 좌표값을 가져와서 스프라이트 출력위치에 쓸 예정
//n(상수값, 중앙점이 될 위치) + 최전선위치 - 현재 캐릭터의 위치 x
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

		// HP 상태창
		RENDER_MANAGER->DrawBarImage(UI_STATUS_EMPTY_HP, (i * 200) + 105, 55, (float)_HeroList[i]->GetHp() / _HeroList[i]->GetMaxHp());
		RENDER_MANAGER->DrawColorNumberImage(true, _HeroList[i]->GetHp(), (i * 200) + 198, 62);

		// SP를 사용하게 된다면 상수가 아닌 데이터를 넣을 예정
		RENDER_MANAGER->DrawBarImage(UI_STATUS_EMPTY_SP, (i * 200) + 105, 80, 1.0f);
		RENDER_MANAGER->DrawColorNumberImage(true, 100, (i * 200) + 198, 87);
	}
}

void GameManager::DrawCoinStatus(int x, int y)
{
	RENDER_MANAGER->DrawTransImage(UI_STATUS_COIN, x, y);
	RENDER_MANAGER->DrawColorNumberImage(false, GAME_MANAGER->GetGold(), x + IMAGE_MANAGER->GetImage(UI_STATUS_COIN)->width - 10, y + 10);
}