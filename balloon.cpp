#include "stdafx.h"
#include "balloon.h"

Balloon::Balloon()
{
}

Balloon::Balloon(POINT _posXY, double _damage, float _duration, int _skillId, int width, int height, Layer layer)
{
	skillID = _skillId;
	damage = _damage;
	skillImageID = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFirstIMG_ID;
	maxDelay = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectDelay;
	maxSprite = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFrame;
	posXY = _posXY;
	Init(_duration, _damage);
	collider = new Collider(width * 3, height, layer, &posXY);
}

Balloon::~Balloon()
{
	Character* currHero;

	for (int i = 0; i < GAME_MANAGER->GetHerolistSize(); i++)
	{
		currHero = GAME_MANAGER->GetHerodata(i);
		if (currHero->GetBuff() == this) {
			currHero->SetBuff(nullptr);
		}
	}
	delete(collider);
}

void Balloon::Init(float _duration, int _damage)
{
	damage = _damage;

	Character* lowesthero = GAME_MANAGER->GetHerodata(0);

	for (int i = 1; i < GAME_MANAGER->GetHerolistSize(); i++)
	{
		Character* temp = GAME_MANAGER->GetHerodata(i);

		if (temp->Getstate() != AnimType::DEATH && temp->GetMaxHp() - temp->GetHp() > lowesthero->GetMaxHp() - lowesthero->GetHp())
		{
			lowesthero = GAME_MANAGER->GetHerodata(i);
		}
	}

	lowesthero->CalcCurrentHP(-damage);

	lowesthero->SetBuff(this);

	shieldCnt = 1;
	duration = durationFrame + _duration;
}

void Balloon::SkillUpdate()
{
	if (currentSprite == maxSprite-1) {
		currentSprite = 15;
	}

	if (posXY.x < GAME_MANAGER->GetFrontline()->GetPositionx()) {
		posXY.x += 10;
	}

	if (shieldCnt <= 0) {
		duration = 0;
	}
	// shieldCnt 이 0이되면 바로 삭제
}

void Balloon::CaclShieldCnt()
{
	shieldCnt--;
}

int Balloon::GetShieldCnt()
{
	return shieldCnt;
}
