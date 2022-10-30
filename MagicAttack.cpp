#include "stdafx.h"
#include "MagicAttack.h"

bool MagicAttack::CheckHit()
{
	Character* currChar = GAME_MANAGER->GetMonsterFrontline();

	if (currChar == nullptr) return false;

	if (GetCollider()->CheckCollision(currChar->GetCollider()))
	{
		currChar->CalcCurrentHP(damage);
		return true;
	}

	return false;
}

//void MagicAttack::CheckHit()
//{
//	Character* currEnemy = nullptr;
//
//	if (_layer == Layer::HEROSKILL)
//	{
//		currEnemy = GAME_MANAGER->GetMonsterFrontline();
//	}
//
//	if (currEnemy == nullptr)
//	{
//		return;
//	}
//
//	if (GetCollider()->CheckCollision(currEnemy->GetCollider()))
//	{
//		currEnemy->CalcCurrentHP(damage);
//		return;
//	}
//}

void MagicAttack::MoveToTarget()
{
	posXY.x += 20;
}

MagicAttack::MagicAttack()
{

}

MagicAttack::MagicAttack(POINT _posXY, int _damage, float _duration, int _skillId, int width, int height, Layer layer)
{
	skillID = _skillId;
	skillImageID = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFirstIMG_ID;
	maxDelay = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectDelay;
	maxSprite = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFrame;
	damage = _damage;
	posXY = _posXY;
	duration = _duration + 100;
	//posXY.x += 1000;
	collider = new Collider(width, height, layer, &posXY);
}

MagicAttack::~MagicAttack()
{
}

void MagicAttack::SkillUpdate()
{
	currentSprite = 0;
	if (!CheckHit())
	{
		MoveToTarget();
	}
	else
	{
		GAME_MANAGER->DeleteSkill(this);
	}
}