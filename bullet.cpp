#include "stdafx.h"
#include "bullet.h"

bool Bullet::CheckHit()
{
	if (skillID == 9131 && currentSprite != 1) 
		return true;

	Character* currChar = GAME_MANAGER->GetFrontline();

	if (currChar == nullptr) return false;

	if (GetCollider()->CheckCollision(currChar->GetCollider()))
	{
		currChar->CalcCurrentHP(damage);
		return true;
	}

	return false;
}

void Bullet::MoveToTarget()
{
	posXY.x -= 20;
}

Bullet::Bullet()
{
}

Bullet::Bullet(POINT _posXY, int _damage, float _duration, int _skillId, int width, int height, Layer layer)
{
	skillID = _skillId;
	skillImageID = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFirstIMG_ID;
	maxDelay = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectDelay;
	maxSprite = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFrame;
	damage = _damage;
	posXY = _posXY;
	duration = _duration + 100;

	collider = new Collider(width, height, layer, &posXY);
}

Bullet::~Bullet()
{
}

void Bullet::SkillUpdate()
{
	if (!CheckHit())
	{
		currentSprite = 0;
		MoveToTarget();
	}
	else
	{
		if (skillID == 9131 && currentSprite == 1)
		{
			currentSprite = 1;
			duration = 5;
			durationFrame = 0;
		}
		else
		{
			GAME_MANAGER->DeleteSkill(this);
		}
	}
}
