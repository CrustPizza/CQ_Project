#include "stdafx.h"
#include "thunder.h"

void Thunder::CheckHit()
{
	Character* currMon;

	for (int i = 0; i < GAME_MANAGER->GetMonsterlistSize(); i++)
	{
		currMon = GAME_MANAGER->GetMonsterdata(i);

		if (GetCollider()->collRect.right < currMon->GetCollider()->collRect.left)
		{
			break;
		}

		if (GetCollider()->CheckCollision(currMon->GetCollider()))
		{
			currMon->CalcCurrentHP(damage);
		}
	}
}

Thunder::Thunder()
{
}

Thunder::Thunder(POINT _posXY, double _damage, float _duration, int _skillId, int width, int height, Layer layer)
{
	skillID = _skillId;
	skillImageID = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFirstIMG_ID;
	maxDelay = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectDelay;
	maxSprite = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFrame;
	damage = _damage;
	posXY = _posXY;
	duration = _duration;
	currDuration = 0;
	collider = new Collider(width, height, layer, &posXY);
}

Thunder::~Thunder()
{
	delete(collider);
}

void Thunder::SkillUpdate()
{
	currDuration++;

	if (currDuration / 10 > 0)
	{
		currDuration -= 10;
		CheckHit();
	}
}
