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
/// ���� �Ŵ��� ������Ʈ���� ȣ������ ������Ʈ
/// </summary>
bool Block::Update()
{
	// ���� �������� ���߸� ������ ��ϰ��� ü�λ��� üũ
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
/// ���� �Ŵ������� ����� �����̰� ������ �Ҷ� ȣ�� ���� �Լ�
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
/// ���� ��ġ�� �ε����� �´� x ���� �ƴϸ� �̵�
/// ������ �������� ����
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
