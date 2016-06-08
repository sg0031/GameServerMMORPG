#include "stdafx.h"
#include "Object.h"


void Rabbit::upDate()
{
	switch (state)
	{
	case waitState:
	{
		if (true == hitDamge()) {
			state = chaseState;
		}
		else
			randomMove();
		break;
	}
	case attackState:
	{
		break;
	}
	case chaseState:
	{
		break;
	}
	case deadState:
	{

		break;
	}

	}
}
void Rabbit::move()
{
}
bool Rabbit::attakcRange()
{
	return false;
}
bool Rabbit::hitDamge()
{
	return false;
}
void Rabbit::randomMove()
{
	switch (rand()%3)
	{
	case 0:
	{
		int x = getPosX();
		setPosX(x++);
		break;
	}
	case 1:
	{
		int x = getPosX();
		setPosX(x--);
		break;
	}
	case 2:
	{
		int y = getPosY();
		setPosY(y++);
		break;
	}
	case 3:
	{
		int y = getPosY();
		setPosY(y--);
		break;
	}

	}
}