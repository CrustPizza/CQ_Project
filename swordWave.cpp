#include "stdafx.h"
#include "SwordWave.h"

bool SwordWave::CheckCharList(Collider* other)
{
	if (std::find(charList.begin(), charList.end(), other) != charList.end())
	{
		return false;
	}

	return true;
}

void SwordWave::Move()
{
	POINT pos = GetPosition();
	pos.x += moveSpeed;

	SetPosition(pos);
}

void SwordWave::CheckHit()
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
			if (CheckCharList(currMon->GetCollider()))
			{
				currMon->CalcCurrentHP(damage);
				charList.push_back(currMon->GetCollider());
			}
		}
	}
}

SwordWave::SwordWave()
{

}

SwordWave::SwordWave(POINT _posXY, double _damage, float _duration, int _skillId, int width, int height, Layer layer)
{
	skillID = _skillId;
	skillImageID = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFirstIMG_ID;
	maxDelay = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectDelay;
	maxSprite = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFrame;
	damage = _damage;
	posXY = _posXY;
	duration = _duration;

	collider = new Collider(width, height, layer, &posXY);
}

SwordWave::~SwordWave()
{
	delete(collider);
}

void SwordWave::SkillUpdate()
{
	Move();
	CheckHit();
}
