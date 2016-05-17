#include"stdafx.h"
#include "GameBoard.h"



GameBoard::GameBoard()
{
}


GameBoard::~GameBoard()
{
}

void GameBoard::setBoardx(int x)
{
	boardX = x;
}
void GameBoard::setBoardy(int y)
{
	boardY = y;
}
void GameBoard::setPiece(int piece)
{
	chessPiece = piece;
}
void GameBoard::setType(int type)
{
	boardType = type;
}


int GameBoard::getBoardx()
{
	return boardX;
}
int GameBoard::getBoardy()
{
	return boardY;
}
int GameBoard::getchessPiece()
{
	return chessPiece;
}
int GameBoard::getType()
{
	return boardType;
}