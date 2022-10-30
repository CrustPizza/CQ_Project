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

//���� ���ο� ������ ������Ʈ?���� ������ Ŭ����.
//���⼭ List���� �����ͼ� ������ �Ϸ����Ѵ�.

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

	//ĳ���� ����Ʈ
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

	//�̱��� ���� �� ��ȯ, ����.
	static GameManager* GetSingletonObject();
	static void	ReleaseSingleton();

	void monSort();

	//���� ������Ʈ�Լ�.
	void Update();
	void Render();
	void Reset();

	//����� ��� ����
	//�����͸� �о�� �־��ش�. 
	void InstantiateHeros();
	//���� ��� ����
	//�����͸� �о�� �־��ش�
	void InstantiateMonsters(int id, double statRate, int dx = 0);
	//�� ����
	//�����͸� �о�� �־��ش�.
	void InstantiateBlock();
	void InstantiateSkill(POINT pos, double _damage, float _duration, int _skillId);
	void ActivateMonsterSkill(POINT pos, double _damage, float _duration, int _skillId);

	// ��� Ŭ��
	void BlockClick();
	list<Block*>::iterator BlockErase(list<Block*>::iterator iter, int& count);
	void DrawBlock();

	//vector(_herolist) �ʱ�ȭ
	void ClearList();

	int GetHerolistSize();
	int GetMonsterlistSize();
	int GetGold();

	Character* GetHerodata(int i);
	Character* GetMonsterdata(int i);
	Skill* GetSkilldata(int i);

	//�ش� �����Ϳ� ���� ����� ���Ϳ��� ��������.
	void DeleteSkill(Skill* pskill);
	void DeleteMonster(Monster* monster);

	//�������� �ִ� ĳ������ ��ǥ���� �����ͼ� ī�޶� ���꿡 ������
	//n(�����, �߾����� �� ��ġ) + ��������ġ - ���� ĳ������ ��ġ x
	Character* GetFrontline();
	Character* GetMonsterFrontline();

	list<NumImageData>& GetNumImageQueue();

	void DrawHeroStatus();
	void DrawCoinStatus(int x, int y);
};