#pragma once

class Monster : public Character
{
private:
	const MonsterTable* _MonsterData;
	const SkillTable* _SkillData[4];
	const EffectTable* _SkillEffectData[4];
	int moveSpeed = 2;
	int currSP = 0;
	int maxSP = 100;
	int currDuration = 0;
	int pattern = 0;

public:
	Monster(int characterID, double statRate, POINT& initialcoord);
	~Monster();

	void FindEnemy();
	bool CheckAttackRange();
	void Attack();
	void Move();
	void Skills(int chainedNum = 1);
	void Death();

	void Update();
	void Render();
	int GetMaxSprite();
};