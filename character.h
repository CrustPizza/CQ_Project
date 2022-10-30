#pragma once
#include <Windows.h>
#include "Collider.h"
#include "balloon.h"

class Character {
protected:
	// ĳ������ state 
	AnimType _state;
	AnimType _lastanim = AnimType::MOVE;

	// ĳ������ �ִ� HP�� ���� ü��
	int _maxhp;
	int _currenthp;

	// _character�� jsontype num �ӽ÷� �����صΰ� ����
	int _characterID;
	int _currentDelay = 0;
	int _currentSprite = 0;

	// ĳ������ ���� ��ǥ
	POINT _coordinate;

	// ĳ������ ������
	int _damage;

	// ĳ������ ���ݻ�Ÿ� ���Ÿ��� ��ũ��,
	int _attackrange;
	int _attackspeed;

	// ĳ������ ��Ʈ�ڽ�, �ٵ� ������ ���� �Ϲ� ������ ���ϰ����̰�, ��ų�� ���������̴�. 

	Character* _target = nullptr;
	Collider* _collider = nullptr;

	//�ϳ����� �ǵ������ üũ�� ��.
	//������ ���������� ����ٸ�... �迭�� �����ؾ�����������.
	Balloon* _buff = nullptr;

public:
	Character();
	//�ڽ��� �Ҹ��� ȣ���� ���� �����Լ�.
	virtual ~Character();

	//ĳ���� �ʱ�ȭ, �����ɶ� ���
	void InitializeChar(int maxhp, int attackrange, int damage, POINT initialcoord, int width, int height, Layer layer);

	//ī�޶� �̵��� ���� ���� ĳ������ x��ǥ ��������.
	int GetPositionx();
	Collider* GetCollider();

	//������ �߰����� ��.
	void SetBuff(Balloon* input);
	Balloon* GetBuff();

	//���� ���� ����, ��ȯ
	AnimType Getstate();
	void Setstate(AnimType type);

	//�׷��� ĳ���� ��������Ʈ�� json num
	int GetCharacternum();

	void CalcCurrentHP(int num);
	int GetAttackRange();
	int GetDamage();

	//�̵�����
	void MoveLeft(int moveSpeed);
	void MoveRight(int moveSpeed);

	//Ÿ�� ����
	void SetTarget(Character* target);
	Character* GetTarget();

	bool IsDead();

	void FSM();
	void Action();

	// HP
	int GetHp();
	int GetMaxHp();

	//virtual�� �����Ͽ� ����ο� ���Ϳ� ���� ������ ���� �����غ���.
	//������ �� ã��
	virtual void FindEnemy() = 0;
	virtual bool CheckAttackRange() = 0;
	virtual void Attack() = 0;
	virtual void Move() = 0;
	virtual void Death() = 0;
	virtual void Skills(int chainedNum = 1) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual int GetMaxSprite() = 0;
};