#pragma once
#include "skill.h"
class MagicAttack : public Skill
{
private:
	bool ishit = false;
	Layer _layer;

private:
	bool CheckHit();
	void MoveToTarget();

public:
	MagicAttack();
	MagicAttack(POINT _posXY, int _damage, float _duration, int _skillId, int width, int height, Layer layer);
	~MagicAttack();
	void SkillUpdate();
};