#include "stdafx.h"
#include "hero.h"

Hero::Hero(int characterID, POINT& initialcoord, int width, int height)
{
	_characterID = characterID;
	_HeroData = TABLE_MANAGER->FindHeroDataFromID(_characterID);
	InitializeChar(_HeroData->HP, _HeroData->AttackRange, _HeroData->ATK, initialcoord, width, height, Layer::HERO);
	for (int i = 0; i < 4; i++)
	{
		_SkillData[i] = TABLE_MANAGER->FindSkillDataFromID(_HeroData->Skill_ID[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		_SkillEffectData[i] = TABLE_MANAGER->FindEffectDataFromID(_SkillData[i]->SkillEffectID);
	}
}

Hero::~Hero()
{
}

void Hero::FindEnemy()
{
	for (int i = 0; i < GAME_MANAGER->GetMonsterlistSize(); i++)
	{
		if ((GAME_MANAGER->GetMonsterdata(i)->GetPositionx()) - _coordinate.x <= _attackrange) {
			_target = GAME_MANAGER->GetMonsterdata(i);
			return;
		}
	}

	SetTarget(nullptr);
}

bool Hero::CheckAttackRange()
{
	if (GAME_MANAGER->GetMonsterFrontline() && GAME_MANAGER->GetMonsterFrontline()->GetPositionx() - GetPositionx() <= GetAttackRange())
	{
		return true;
	}

	return false;
}

void Hero::Attack()
{
	if (GetTarget() == nullptr)
	{
		return;
	}

	if (_characterID == 102)
	{
		if (_currentSprite == 7 && _currentDelay == 0)
		{
			GAME_MANAGER->InstantiateSkill(_coordinate, _SkillData[0]->Rate * _damage, _SkillEffectData[0]->EffectDuration, _SkillEffectData[0]->Id);
		}
	}
	else if (_characterID == 101)
	{
		if (_currentSprite == 7 && _currentDelay == 0)
		{
			GAME_MANAGER->InstantiateSkill(_coordinate, 100, _SkillEffectData[0]->EffectDuration, _SkillEffectData[0]->Id);
		}
	}
	else
	{
		if (_currentSprite == 7 && _currentDelay == 0)
		{
			GAME_MANAGER->InstantiateSkill(_coordinate, 100, _SkillEffectData[0]->EffectDuration, _SkillEffectData[0]->Id);
		}
	}
}

void Hero::Move()
{
	MoveRight(moveSpeed);

	//int s1 = SCENE_MANAGER->GetCamera();
	//int s2 = GAME_MANAGER->GetFrontline()->GetPositionx();
	if (SCENE_MANAGER->GetCamera() < GAME_MANAGER->GetFrontline()->GetPositionx() - 800)
	{
		SCENE_MANAGER->MoveCamera(moveSpeed);
	}
}

void Hero::Skills(int chainedNum)
{
	if (_state != AnimType::DEATH) {
		POINT pos;
		Setstate(AnimType::Skill);

		switch (_SkillEffectData[chainedNum]->Id)
		{
		case 9012:
			if (GAME_MANAGER->GetMonsterFrontline())
			{
				pos = { GAME_MANAGER->GetMonsterFrontline()->GetPositionx(), (int)WinInfo::Height - 180 };
			}
			else
			{
				pos = { GAME_MANAGER->GetFrontline()->GetPositionx() + 500, (int)WinInfo::Height - 180 };
			}
			//스킬아이디 접근.
			GAME_MANAGER->InstantiateSkill(pos, _SkillData[chainedNum]->Rate * _damage, _SkillEffectData[chainedNum]->EffectDuration, _SkillEffectData[chainedNum]->Id);
			break;
		case 9022:
			pos = { GetPositionx(), (int)WinInfo::Height - 180 };
			GAME_MANAGER->InstantiateSkill(pos, _SkillData[chainedNum]->Rate * _damage, _SkillEffectData[chainedNum]->EffectDuration, _SkillEffectData[chainedNum]->Id);
			break;
		case 9032:
			Character* temp = GAME_MANAGER->GetHerodata(0);
			pos.x = _coordinate.x;
			pos.y = _coordinate.y-50;
			GAME_MANAGER->InstantiateSkill(pos, _SkillData[chainedNum]->Rate * _damage, _SkillEffectData[chainedNum]->EffectDuration, _SkillEffectData[chainedNum]->Id);
			break;
		}
	}
	//if (_SkillEffectData[chainedNum]->Id ==  9032)
	//{

	//}
	//else
	//{
	//	if (GetTarget() == nullptr)
	//	{
	//		return;
	//	}
	//	pos = { GetTarget()->GetPositionx(), (int)WinInfo::Height - 180 };
	//	//스킬아이디 접근.
	//	GAME_MANAGER->InstantiateSkill(pos, 0, _SkillEffectData[chainedNum]->EffectDuration, _SkillEffectData[chainedNum]->Id);
	//}
}

void Hero::Update()
{
	if (++_currentDelay > _HeroData->AnimDelay[(int)_state])
	{
		if (_currentSprite + 1 >= _HeroData->AnimFrame[(int)_state])
		{
			if (_state == AnimType::DEATH)
			{
				return;
			}

			_currentSprite = 0;
		}
		else
		{
			_currentSprite++;
		}

		_currentDelay = 0;
	}

	FSM();
	Action();
}

void Hero::Render()
{
	int cameraX = SCENE_MANAGER->GetCamera();

	RENDER_MANAGER->DrawTransImage(_HeroData->AnimFirstIMG_ID[(int)_state] + _currentSprite, _coordinate.x - cameraX, _coordinate.y);
#ifdef DebugMode
	if (INPUT_MANAGER->KeyCheck(VK_SPACE, KeyState::TOGGLE) == true)
	{
		RENDER_MANAGER->HitBox({ _coordinate.x - _collider->collRect.right / 2 - cameraX, _coordinate.y - _collider->collRect.bottom }, { _collider->collRect.right, _collider->collRect.bottom },
			RGB(255, 0, 255));
	}
#endif
}

void Hero::Death()
{

}

int Hero::GetSkillEffectId(int chainedNum)
{
	return _SkillEffectData[chainedNum]->Id;
}

int Hero::GetMaxSprite() {
	return _HeroData->AnimFrame[(int)_state];
}