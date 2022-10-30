#include "stdafx.h"
#include "swordbasic.h"

bool SwordBasic::CheckCharList(Collider* other)
{
	if (std::find(charList.begin(), charList.end(), other) != charList.end())
	{
		return false;
	}

	return true;
}

void SwordBasic::CheckHit()
{
	Character* currMon;

	if (_layer == Layer::MONSTERSKILL)
	{
		for (int i = 0; i < GAME_MANAGER->GetHerolistSize(); i++)
		{
			currMon = GAME_MANAGER->GetHerodata(i);

			if (GetCollider()->collRect.right < currMon->GetCollider()->collRect.left)
			{
				break;
			}

			if (GetCollider()->CheckCollision(currMon->GetCollider()))
			{
				if (CheckCharList(currMon->GetCollider()))
				{
					currMon->MoveLeft(100);
					currMon->CalcCurrentHP(damage);
					charList.push_back(currMon->GetCollider());
				}
			}
		}
	}
	else
	{
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
}

SwordBasic::SwordBasic()
{
}

SwordBasic::SwordBasic(POINT _posXY, double _damage, float _duration, int _skillId, int width, int height, Layer layer)
{
	skillID = _skillId;
	skillImageID = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFirstIMG_ID;
	maxDelay = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectDelay;
	maxSprite = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFrame;
	damage = _damage;
	posXY = _posXY;
	duration = _duration;
	_layer = layer;

	switch (layer)
	{
	case Layer::HEROSKILL:
		posXY.x += 110;
		break;
	case Layer::MONSTERSKILL:
		posXY.x -= 250;
		break;
	default:
		break;
	}
	collider = new Collider(width, height, layer, &posXY);
}

SwordBasic::~SwordBasic()
{
	delete(collider);
}

void SwordBasic::SkillUpdate()
{
	CheckHit();
}
