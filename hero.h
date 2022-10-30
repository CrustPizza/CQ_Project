#pragma once
class Hero : public Character
{
private:
	const HeroTable* _HeroData;
	const SkillTable* _SkillData[4];	
	const EffectTable* _SkillEffectData[4];

	int moveSpeed = 5;

public:
	Hero(int characterID, POINT& initialcoord, int width, int height);
	~Hero();

	void FindEnemy();
	bool CheckAttackRange();
	void Attack();
	void Move();
	void Skills(int chainedNum = 1);
	void Death();
	int GetSkillEffectId(int chainedNum);
	void Update();
	void Render();
	int GetMaxSprite();
};
