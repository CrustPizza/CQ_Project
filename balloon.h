#pragma once
#include "skill.h"

class Balloon : public Skill
{
private:
	// ���������� ��ȣ���� 1�� ����
	int shieldCnt;
	

public:
	Balloon();
	Balloon(POINT _posXY, double _damage, float _duration, int _skillId, int width, int height, Layer layer);
	~Balloon();

	void Init(float _duration, int _damage);

	void SkillUpdate();
	void CaclShieldCnt();
	int GetShieldCnt();
};