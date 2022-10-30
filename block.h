#pragma once
#include <list>

enum class BlockState
{
	NOTCHAIN,
	CHAINRIGHT,
	CHAINLEFT
};

class Block
{
	// ����
private:
	// ��� ��ġ �� ũ��
	RECT blockRect;
	int blockWidth;

	// ��� ����
	int skillID;
	int characterId;
	int blockIdx;
	bool canMove;
	int blockSpeed;

public:
	int chainCnt;
	BlockState blockState;

	// �Լ�
public:
	Block(int id, int idx, int y);
	~Block();

	void CheckChain(std::list<Block*>::iterator iter);

	bool Update();
	void ChangeCanMove(int idx);
	int GetSkillId();

	RECT GetBlockRect();

private:
	bool MoveBlock();
};

