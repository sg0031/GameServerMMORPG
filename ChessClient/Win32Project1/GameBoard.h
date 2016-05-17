#pragma once
class GameBoard
{
	int boardX;
	int boardY;
	int chessPiece;
	int boardType;
public:
	GameBoard();
	~GameBoard();
	void setBoardx(int x);
	void setBoardy(int y);
	void setPiece(int piece);
	void setType(int type);
	int getBoardx();
	int getBoardy();
	int getchessPiece();
	int getType();
};

