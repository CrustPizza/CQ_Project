#include "stdafx.h"
#include "pierceBullet.h"

bool PierceBullet::CheckCharList(Collider* other)
{
	if (std::find(charList.begin(), charList.end(), other) != charList.end())
	{
		return false;
	}

	return true;
}

void PierceBullet::Move()
{
	POINT pos = GetPosition();
	pos.x -= moveSpeed;

	SetPosition(pos);
}

void PierceBullet::CheckHit()
{
	Character* currHero;

	for (int i = 0; i < GAME_MANAGER->GetHerolistSize(); i++)
	{
		currHero = GAME_MANAGER->GetHerodata(i);

		if (GetCollider()->collRect.right < currHero->GetCollider()->collRect.left)
		{
			break;
		}

		if (GetCollider()->CheckCollision(currHero->GetCollider()))
		{
			if (CheckCharList(currHero->GetCollider()))
			{
				currHero->CalcCurrentHP(damage);
				charList.push_back(currHero->GetCollider());
			}
		}
	}
}

PierceBullet::PierceBullet()
{

}

PierceBullet::PierceBullet(POINT _posXY, double _damage, float _duration, int _skillId, int width, int height, Layer layer)
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

PierceBullet::~PierceBullet()
{
	delete(collider);
}

void PierceBullet::SkillUpdate()
{
	Move();
	CheckHit();
}
