#include "stdafx.h"
#include "skill.h"

//Skill::Skill(POINT _posXY, int _damage, float _duration, int _skillId, int width, int height, Layer layer)
//{
//	skillID = _skillId;
//	skillImageID = TABLE_MANAGER->FindEffectDataFromID(skillID)->EffectFirstIMG_ID;
//	damage = _damage;
//	posXY = _posXY;
//	duration = _duration;
//
//	collider = new Collider(width, height, layer, &posXY);
//}
//
Skill::~Skill()
{

}

bool Skill::Update()
{
	if (++currentDelay >= maxDelay)
	{
		if (++currentSprite >= maxSprite/* && TABLE_MANAGER->FindSkillDataFromID(skillID)->SkillType != "Projectile"*/)
		{
			currentSprite = 0;
		}

		currentDelay = 0;

		SkillUpdate();
	}

	if (++durationFrame >= duration)
	{
		return true;
	}

	return false;
}

void Skill::SkillUpdate()
{
}

POINT Skill::GetPosition()
{
	return posXY;
}

Collider* Skill::GetCollider()
{
	return collider;
}

int Skill::GetDamage()
{
	return damage;
}

int Skill::GetSkillID()
{
	return skillID;
}

int Skill::GetSkillImageID()
{
	return skillImageID;
}

int Skill::GetCurrentSprite()
{
	return currentSprite;
}

int Skill::GetCurrentDelay()
{
	return currentDelay;
}

void Skill::SetPosition(POINT _pos)
{
	posXY = _pos;
}

void Skill::SetCurrentDelay(int i)
{
	if (i == 0) {
		currentDelay = 0;
	}

	currentDelay += i;
}

void Skill::SetCurrentSprite(int i)
{
	if (i == 0) {
		currentSprite = 0;
	}

	currentSprite += i;
}

void Skill::Render()
{
	int cameraX = SCENE_MANAGER->GetCamera();

	RENDER_MANAGER->DrawTransImage(skillImageID + currentSprite, posXY.x - cameraX, posXY.y);
#ifdef DebugMode
	if (INPUT_MANAGER->KeyCheck(VK_SPACE, KeyState::TOGGLE) == true)
	{
		RENDER_MANAGER->HitBox({ posXY.x - collider->collRect.right / 2 - cameraX, posXY.y - collider->collRect.bottom }, { collider->collRect.right, collider->collRect.bottom },
			RGB(255, 0, 255));
	}
#endif
}