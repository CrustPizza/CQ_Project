#pragma once
/// <summary>
/// 몬스터의 스킬은 캐릭터 하고만
/// 캐릭터의 스킬은 몬스터 하고만 충돌 체크하기 위한 레이어
/// </summary>
enum class Layer
{
	HERO,
	HEROSKILL,
	MONSTER,
	MONSTERSKILL
};

class Collider
{

private:
	Layer layer;
	bool isTrigger = false;

public:
	RECT collRect;

	//부모의 위치를 포인터로 넘겨 받아 초기화
	POINT* pos; 

private:

public:
	Collider();
	Collider(int _width, int _height, Layer _layer, POINT* _pos);
	//Collider(int _width, int _height, Layer _layer, POINT* _pos, bool isTrigger = false);
	~Collider();

	bool CheckCollision(Collider* _coll);
};

