#include "stdafx.h"
#include "monster.h"

Monster::Monster(int characterID, double statRate, POINT& initialcoord)
{
	_characterID = characterID;
	_MonsterData = TABLE_MANAGER->FindMonsterDataFromID(_characterID);
	InitializeChar(_MonsterData->HP * statRate, _MonsterData->AttackRange, _MonsterData->ATK * statRate, initialcoord, _MonsterData->HitBox[0], _MonsterData->HitBox[1], Layer::MONSTER);
	
	for (int i = 0; i < 3; i++)
	{
		_SkillData[i] = TABLE_MANAGER->FindSkillDataFromID(_MonsterData->Skill_ID[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		_SkillEffectData[i] = TABLE_MANAGER->FindEffectDataFromID(_SkillData[i]->SkillEffectID);
	}
}

Monster::~Monster()
{
}

void Monster::FindEnemy()
{
	//SetTarget(GAME_MANAGER->GetFrontline());

	Character* temptarget = GAME_MANAGER->GetFrontline();
	if (temptarget != nullptr) {
		if (_coordinate.x - temptarget->GetPositionx() <= _attackrange)
		{
			_target = temptarget;
			return;
		}
	}


	SetTarget(nullptr);
}

bool Monster::CheckAttackRange()
{
	if (GAME_MANAGER->GetFrontline() && GetPositionx() - GAME_MANAGER->GetFrontline()->GetPositionx() <= GetAttackRange())
	{
		return true;
	}
	return false;
}

void Monster::Attack()
{
	if (GetTarget() == nullptr) {
		return;
	}

	switch (_characterID)
	{
	case 201:
		if (_currentSprite == 0 && _currentDelay == 0) {
			SOUND_MANAGER->Play(510007);
		}

		if (_currentSprite == 50 && _currentDelay == 0)
		{
			int skillEffectId = TABLE_MANAGER->FindSkillDataFromID(_MonsterData->Skill_ID[0])->SkillEffectID;
			GAME_MANAGER->InstantiateSkill(_coordinate, TABLE_MANAGER->FindSkillDataFromID(_MonsterData->Skill_ID[0])->Rate * _damage,
				TABLE_MANAGER->FindEffectDataFromID(skillEffectId)->EffectDuration,
				TABLE_MANAGER->FindEffectDataFromID(skillEffectId)->Id);
		}
		break;
	case 202: {
		if (_currentSprite == 13 && _currentDelay == 0) {
			int skillEffectId = TABLE_MANAGER->FindSkillDataFromID(_MonsterData->Skill_ID[0])->SkillEffectID;
			GAME_MANAGER->InstantiateSkill(_coordinate, TABLE_MANAGER->FindSkillDataFromID(_MonsterData->Skill_ID[0])->Rate * _damage,
				TABLE_MANAGER->FindEffectDataFromID(skillEffectId)->EffectDuration,
				TABLE_MANAGER->FindEffectDataFromID(skillEffectId)->Id);
		}
	}
			break;
	case 203:
		if (_currentSprite == 13 && _currentDelay == 0)
		{
			int skillEffectId = TABLE_MANAGER->FindSkillDataFromID(_MonsterData->Skill_ID[0])->SkillEffectID;
			GAME_MANAGER->InstantiateSkill(_coordinate, TABLE_MANAGER->FindSkillDataFromID(_MonsterData->Skill_ID[0])->Rate * _damage,
				TABLE_MANAGER->FindEffectDataFromID(skillEffectId)->EffectDuration,
				TABLE_MANAGER->FindEffectDataFromID(skillEffectId)->Id);
		}
		break;
	case 204:
		if (_currentSprite == 20 && _currentDelay == 0)
		{
			int skillEffectId = TABLE_MANAGER->FindSkillDataFromID(_MonsterData->Skill_ID[0])->SkillEffectID;
			GAME_MANAGER->InstantiateSkill(_coordinate, TABLE_MANAGER->FindSkillDataFromID(_MonsterData->Skill_ID[0])->Rate * _damage,
				TABLE_MANAGER->FindEffectDataFromID(skillEffectId)->EffectDuration,
				TABLE_MANAGER->FindEffectDataFromID(skillEffectId)->Id);
		}
		break;
	default:
		if (_currentSprite == 20 && _currentDelay == 0)
		{
			GetTarget()->CalcCurrentHP(_damage);
		}

		break;
	}
}

void Monster::Move()
{
	MoveLeft(moveSpeed);
}

void Monster::Skills(int skillnum)
{
	if (_state != AnimType::DEATH)
	{
		Setstate(AnimType::Skill);

		GAME_MANAGER->ActivateMonsterSkill(_coordinate, _SkillData[skillnum]->Rate * _damage, _SkillEffectData[skillnum]->EffectDuration, _SkillEffectData[skillnum]->Id);
	}
}

void Monster::Update()
{
	if (_characterID == 201)
	{
		currDuration++;
		if ((int)WinInfo::Frame == currDuration)
		{
			currSP += 10;
			currDuration = 0;
			if (currSP > maxSP)
			{
				currSP = 0;
				Skills(1 + pattern);
				pattern = !pattern;
			}
		}
	}
	if (++_currentDelay > _MonsterData->AnimDelay[(int)_state])
	{
		if (++_currentSprite >= _MonsterData->AnimFrame[(int)_state])
		{
			_currentSprite = 0;
		}
		_currentDelay = 0;
	}

	FSM();
	Action();
}

void Monster::Render()
{
	int cameraX = SCENE_MANAGER->GetCamera();

	RENDER_MANAGER->DrawTransImage(_MonsterData->AnimFirstIMG_ID[(int)_state] + _currentSprite, _coordinate.x - cameraX, _coordinate.y);
#ifdef DebugMode
	if (INPUT_MANAGER->KeyCheck(VK_SPACE, KeyState::TOGGLE) == true)
	{
		RENDER_MANAGER->HitBox({ _coordinate.x - _collider->collRect.right / 2 - cameraX, _coordinate.y - _collider->collRect.bottom }, { _collider->collRect.right, _collider->collRect.bottom },
			RGB(255, 0, 255));
	}
#endif
}

int Monster::GetMaxSprite()
{
	return _MonsterData->AnimFrame[(int)_state];
}

void Monster::Death() {

	if (_currentSprite == GetMaxSprite() - 1) {
		GAME_MANAGER->DeleteMonster(this);
		delete(this);
	}
}