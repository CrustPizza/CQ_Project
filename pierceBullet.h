#pragma once

#include "skill.h"

class PierceBullet : public Skill
{
private:
	int moveSpeed = 20;
	vector<Collider*> charList;

private:
	bool CheckCharList(Collider* other);
	void CheckHit();
	void Move();

public:
	PierceBullet();
	PierceBullet(POINT _posXY, double _damage, float _duration, int _skillId, int width, int height, Layer layer);
	~PierceBullet();
	void SkillUpdate();
};