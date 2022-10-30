#pragma once
/// <summary>
/// ������ ��ų�� ĳ���� �ϰ�
/// ĳ������ ��ų�� ���� �ϰ� �浹 üũ�ϱ� ���� ���̾�
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

	//�θ��� ��ġ�� �����ͷ� �Ѱ� �޾� �ʱ�ȭ
	POINT* pos; 

private:

public:
	Collider();
	Collider(int _width, int _height, Layer _layer, POINT* _pos);
	//Collider(int _width, int _height, Layer _layer, POINT* _pos, bool isTrigger = false);
	~Collider();

	bool CheckCollision(Collider* _coll);
};

