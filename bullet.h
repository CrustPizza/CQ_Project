#pragma once
#include "skill.h"
class Bullet : public Skill
{
private:
	bool ishit = false;

private:
	bool CheckHit();
	void MoveToTarget();

public:
	Bullet();
	Bullet(POINT _posXY, int _damage, float _duration, int _skillId, int width, int height, Layer layer);
	~Bullet();
	void SkillUpdate();
};

