#pragma once
#include "skill.h"
class SwordBasic : public Skill
{
private:
	vector<Collider*> charList;
	Layer _layer;

private:
	bool CheckCharList(Collider* other);
	void CheckHit();

public:
	SwordBasic();
	SwordBasic(POINT _posXY, double _damage, float _duration, int _skillId, int width, int height, Layer layer);
	~SwordBasic();
	void SkillUpdate();
};

