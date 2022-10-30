#include "stdafx.h"
#include "character.h"

//������ 
Character::Character()
{
}

//�Ҹ���
Character::~Character()
{
	delete(_collider);
}

//ĳ���� �̴ϼȶ���� �� �Լ���. 
void Character::InitializeChar(int maxhp, int attackrange, int damage, POINT initialcoord, int width, int height, Layer layer)
{
	_maxhp = maxhp;
	_currenthp = maxhp;
	_coordinate.x = initialcoord.x;
	_coordinate.y = initialcoord.y;
	_damage = damage;
	_attackrange = attackrange;
	_coordinate = initialcoord;
	_collider = new Collider(width, height, layer, &_coordinate);
}

///Ÿ�ټ��� �� ����
void Character::SetTarget(Character* target) {
	_target = target;
}
Character* Character::GetTarget() {
	return _target;
}


//�̵�����
void Character::MoveLeft(int moveSpeed) {
	_coordinate.x -= moveSpeed;
}
void Character::MoveRight(int moveSpeed) {
	_coordinate.x += moveSpeed;
}

//ī�޶� �̵��� ���� ���� ĳ������ x��ǥ ��������.
int Character::GetPositionx() {
	return _coordinate.x;
}

Collider* Character::GetCollider()
{
	return _collider;
}

//�����߰�
void Character::SetBuff(Balloon* input)
{
	_buff = input;
}
Balloon* Character::GetBuff()
{
	if (_buff == nullptr) {
		return nullptr;
	}

	return _buff;
}

//�ִϸ��̼� State ����
AnimType Character::Getstate() {
	return _state;
}

void Character::Setstate(AnimType type)
{
	if (_state == type)
	{
		return;
	}

	_state = type;
	_currentDelay = 0;
	_currentSprite = 0;
}

//�ǰݽ� �ҷ��� �Լ�.
void Character::CalcCurrentHP(int num) {
	if (_buff != nullptr && _buff->GetShieldCnt() > 0)
	{
		_buff->CaclShieldCnt();

		if (num > 0)
		{
			num = 0;
		}
	}

	_currenthp -= num;

	if (_currenthp < 0)
	{
		_currenthp = 0;
	}
	else if (_currenthp > _maxhp)
	{
		_currenthp = _maxhp;
	}

	GAME_MANAGER->GetNumImageQueue().push_back(
		{ num, 255,
		{ _coordinate.x, _coordinate.y - _collider->collRect.bottom } });
}

//����range Ȱ��
int Character::GetAttackRange() {
	return _attackrange;
}

int Character::GetDamage()
{
	return _damage;
}

//�׷��� ĳ���� ��������Ʈ�� json num
int Character::GetCharacternum() {
	return _characterID;
}

bool Character::IsDead() {
	if (_currenthp <= 0) {
		Setstate(AnimType::DEATH);
		return true;
	}
	return false;
}


void Character::FSM() {
	if (IsDead()) {

		Death();
	}
	else {
		FindEnemy();

		if (_state != AnimType::Skill) {
			if (_target != nullptr)
			{
				Setstate(AnimType::ATTACK);
			}
			else
			{
				Setstate(AnimType::MOVE);
			}
		}
	}
}

int Character::GetHp()
{
	return _currenthp;
}

int Character::GetMaxHp()
{
	// sp�� ����ϰԵȴٸ�
	return _maxhp;
}

void Character::Action() {

	switch (_state)
	{
	case AnimType::MOVE:
		Move();
		break;
	case AnimType::ATTACK:
		Attack();
		break;
	case AnimType::DEATH:
		Death();
		break;
	case AnimType::WIN:
		break;
	case AnimType::IDLE:
		break;
	case AnimType::Skill:
	{
		int a = GetMaxSprite();
		if (_currentSprite == GetMaxSprite() - 1)
		{
			Setstate(AnimType::ATTACK);
		}
	}
	break;
	default:
		break;
	}

}


// ��Ʈ�ڽ��׸��� 
