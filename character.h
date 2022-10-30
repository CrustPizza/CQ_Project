#pragma once
#include <Windows.h>
#include "Collider.h"
#include "balloon.h"

class Character {
protected:
	// 캐릭터의 state 
	AnimType _state;
	AnimType _lastanim = AnimType::MOVE;

	// 캐릭터의 최대 HP와 현재 체력
	int _maxhp;
	int _currenthp;

	// _character의 jsontype num 임시로 저장해두고 쓰기
	int _characterID;
	int _currentDelay = 0;
	int _currentSprite = 0;

	// 캐릭터의 현재 좌표
	POINT _coordinate;

	// 캐릭터의 데미지
	int _damage;

	// 캐릭터의 공격사거리 원거리면 더크게,
	int _attackrange;
	int _attackspeed;

	// 캐릭터의 히트박스, 근데 영상을 보니 일반 공격은 단일공격이고, 스킬은 범위공격이다. 

	Character* _target = nullptr;
	Collider* _collider = nullptr;

	//하나뿐인 실드버프를 체크할 것.
	//언젠가 버프종류가 생긴다면... 배열로 관리해야하지않을까.
	Balloon* _buff = nullptr;

public:
	Character();
	//자식의 소멸자 호출을 위한 가상함수.
	virtual ~Character();

	//캐릭터 초기화, 생성될때 사용
	void InitializeChar(int maxhp, int attackrange, int damage, POINT initialcoord, int width, int height, Layer layer);

	//카메라 이동에 사용될 현재 캐릭터의 x좌표 가져오기.
	int GetPositionx();
	Collider* GetCollider();

	//버프를 추가해줄 것.
	void SetBuff(Balloon* input);
	Balloon* GetBuff();

	//현재 상태 설정, 반환
	AnimType Getstate();
	void Setstate(AnimType type);

	//그려줄 캐릭터 스프라이트의 json num
	int GetCharacternum();

	void CalcCurrentHP(int num);
	int GetAttackRange();
	int GetDamage();

	//이동제어
	void MoveLeft(int moveSpeed);
	void MoveRight(int moveSpeed);

	//타겟 설정
	void SetTarget(Character* target);
	Character* GetTarget();

	bool IsDead();

	void FSM();
	void Action();

	// HP
	int GetHp();
	int GetMaxHp();

	//virtual로 구현하여 히어로와 몬스터에 각각 동작할 것을 생각해보자.
	//공격할 적 찾기
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