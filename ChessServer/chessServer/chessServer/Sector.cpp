#include "stdafx.h"
#include "Sector.h"


Sector::Sector()
{
	player.clear();
	object.clear();
	int randBuff = rand() % 3;
	switch (randBuff)
	{
	case 0:
		debuff = noBuff;
		break;
	case 1:
		debuff = attackDown;
		break;
	case 2:
		debuff = dependDown;
		break;
	case 3:
		break;
	default:
		break;
	}
}


Sector::~Sector()
{
}
