#include "stdafx.h"
#include "block.h"

Block::Block(int id, int idx, int y)
{
	skillID = id;
	blockIdx = idx;
	blockWidth = 150;
	blockRect = { 0, y, blockWidth, y + blockWidth };
	chainCnt = 1;
	blockState = BlockState::NOTCHAIN;
	canMove = true;
	blockSpeed = 10000 / (int)WinInfo::Frame;
}

Block::~Block()
{

}

void Block::CheckChain(list<Block*>::iterator iter)
{
	list<Block*>::iterator right = iter;
	right--;

	if ((*right)->GetSkillId() == skillID && (*right)->chainCnt < 3)
	{
		blockState = BlockState::CHAINRIGHT;

		if ((*right)->blockState == BlockState::CHAINRIGHT)
		{
			(*right--)->chainCnt = 3;
			(*right)->chainCnt = 3;
			chainCnt = 3;
		}
		else
		{
			(*right)->blockState = BlockState::CHAINLEFT;
			(*right)->chainCnt = 2;
			chainCnt = 2;
		}
	}
}

/// <summary>
/// 게임 매니저 업데이트에서 호출해줄 업데이트
/// </summary>
bool Block::Update()
{
	// 블럭의 움직임이 멈추면 오른쪽 블록과의 체인상태 체크
	if (MoveBlock() == false)
	{
		if (blockIdx > 0)
		{
			return true;
		}
	}

	return false;
}

/// <summary>
/// 게임 매니저에서 블록을 움직이게 만들어야 할때 호출 해줄 함수
/// </summary>
void Block::ChangeCanMove(int idx)
{
	canMove = true;
	blockState = BlockState::NOTCHAIN;
	chainCnt = 1;
	blockIdx = idx;
}

int Block::GetSkillId()
{
	return skillID;
}

RECT Block::GetBlockRect()
{
	return blockRect;
}

/// <summary>
/// 블럭의 위치가 인덱스에 맞는 x 값이 아니면 이동
/// 맞으면 움직임을 멈춤
/// </summary>
bool Block::MoveBlock()
{
	if (canMove == true)
	{
		if (blockRect.left <= 1650 - blockSpeed - blockIdx * 190)
		{
			blockRect.left += blockSpeed;
			blockRect.right += blockSpeed;
		}
		else
		{
			blockRect.left = 1650 - blockIdx * 190;
			blockRect.right = blockRect.left + blockWidth;
			canMove = false;
			return false;
		}
	}

	return true;
}
