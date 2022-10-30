#include "stdafx.h"
#include "Collider.h"


Collider::Collider()
{

}

Collider::Collider(int _width, int _height, Layer _layer, POINT* _pos)
{
	collRect = { 0, 0, _width, _height };
	layer = _layer;
	pos = _pos;
}

Collider::~Collider()
{

}

bool Collider::CheckCollision(Collider* _coll)
{
 	int thisLeft = collRect.left + pos->x;		int collLeft = _coll->collRect.left + _coll->pos->x;
	int	thisRight = collRect.right + pos->x;	int collRight = _coll->collRect.right + _coll->pos->x;
	//int thisTop = collRect.top + pos->y;		int collTop = _coll->collRect.top + _coll->pos->y;
	//int thisBot = collRect.bottom + pos->y;		int collBot = _coll->collRect.bottom + _coll->pos->y;

	if (thisLeft < collRight && thisRight > collLeft /*&& thisTop < collBot && thisBot > collTop*/)
	{
		return true;
	}

	return false;
}
