#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
#include <list>
#include "character.h"
#include "Hero.h"
#include "monster.h"
#include "skill.h"
#include "block.h"
#include "swordWave.h"
#include "balloon.h"
#include "thunder.h"
#include "swordbasic.h"
#include "MagicAttack.h"
#include "bullet.h"
#include "pierceBullet.h"

using namespace std;

//게임 내부에 생성된 오브젝트?들을 관리할 클래스.
//여기서 List들을 가져와서 관리를 하려고한다.

typedef struct
{
	int num;
	int frame;
	POINT xy;
}NumImageData;

class GameManager {
private:
	// Singleton
	static GameManager* SingletonObject;

	//캐릭터 리스트
	vector<Hero*> _HeroList;
	vector<Monster*> _MonsterList;
	list<Block*> _BlockList;
	vector<Skill*> _SkillList;

	// Number Image Queue
	list<NumImageData> numImageList;

	// Game Over
	bool gameOver;
	int gold;

	// block
	int blockLocationY;

	Balloon* curBalloon;

public:
	GameManager();
	~GameManager();

	//싱글톤 생성 및 반환, 해제.
	static GameManager* GetSingletonObject();
	static void	ReleaseSingleton();

	void monSort();

	//게임 업데이트함수.
	void Update();
	void Render();
	void Reset();

	//히어로 목록 생성
	//데이터를 읽어와 넣어준다. 
	void InstantiateHeros();
	//몬스터 목록 생성
	//데이터를 읽어와 넣어준다
	void InstantiateMonsters(int id, double statRate, int dx = 0);
	//블럭 생성
	//데이터를 읽어와 넣어준다.
	void InstantiateBlock();
	void InstantiateSkill(POINT pos, double _damage, float _duration, int _skillId);
	void ActivateMonsterSkill(POINT pos, double _damage, float _duration, int _skillId);

	// 블록 클릭
	void BlockClick();
	list<Block*>::iterator BlockErase(list<Block*>::iterator iter, int& count);
	void DrawBlock();

	//vector(_herolist) 초기화
	void ClearList();

	int GetHerolistSize();
	int GetMonsterlistSize();
	int GetGold();

	Character* GetHerodata(int i);
	Character* GetMonsterdata(int i);
	Skill* GetSkilldata(int i);

	//해당 포인터와 같은 대상을 벡터에서 삭제해줌.
	void DeleteSkill(Skill* pskill);
	void DeleteMonster(Monster* monster);

	//최전선의 있는 캐릭터의 좌표값을 가져와서 카메라 무브에 쓸예정
	//n(상수값, 중앙점이 될 위치) + 최전선위치 - 현재 캐릭터의 위치 x
	Character* GetFrontline();
	Character* GetMonsterFrontline();

	list<NumImageData>& GetNumImageQueue();

	void DrawHeroStatus();
	void DrawCoinStatus(int x, int y);
};