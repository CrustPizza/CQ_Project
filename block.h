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
	// 변수
private:
	// 블록 위치 및 크기
	RECT blockRect;
	int blockWidth;

	// 블록 정보
	int skillID;
	int characterId;
	int blockIdx;
	bool canMove;
	int blockSpeed;

public:
	int chainCnt;
	BlockState blockState;

	// 함수
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

