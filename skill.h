#pragma once
#include "Collider.h"

class Skill
{
	// 변수
protected:
	int duration;
	int durationFrame = 0;
	int maxSprite;
	int currentSprite = 0;
	int maxDelay;
	int currentDelay = 0;

	int skillID;
	int skillImageID;

	double damage;

	POINT posXY;
	Collider* collider;

	// 함수
public:
	/*Skill(POINT _posXY, int _damage, float _duration, int _skillId, int width, int height, Layer layer);
	*/
	virtual ~Skill();

	bool Update();
	virtual void SkillUpdate();

	POINT GetPosition();
	Collider* GetCollider();
	int GetDamage();
	int GetSkillID();
	int GetSkillImageID();
	int GetCurrentSprite();
	int GetCurrentDelay();

	void SetPosition(POINT pos);
	void SetCurrentDelay(int i);
	void SetCurrentSprite(int i);

	void Render();
};