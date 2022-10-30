#pragma once
#include "skill.h"

class Thunder : public Skill
{
private:
	int currDuration;
	void CheckHit();
public:
	Thunder();
	Thunder(POINT _posXY, double _damage, float _duration, int _skillId, int width, int height, Layer layer);
	~Thunder();
	void SkillUpdate();
};