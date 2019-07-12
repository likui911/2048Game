#pragma once
#include "framework.h"
#include "Graphics.h"
#include "util.h"

constexpr int GAP = 10;
constexpr int WIDTH = 4;
constexpr int RANDOM_CELL = 2;

class Game
{
public:
	Game();
	~Game();
	void SetRect(const RECT& rect);
	void SetGraphics(Graphics* pGraphic);
	int GetScore();
	RECT GetChessRect();
	RECT GetTopRect();
	RECT GetRect();

	void Draw2D();
	void Restart();

	bool MoveUp();
	bool MoveLeft();
	bool MoveRight();
	bool MoveDown();

	bool IsGameOver();
	bool IsWon();
	bool IsTerminated();

private:
	void InitialChessBoard();
	void GenerateRandomCell();
	int  GetRandomAvaliableCell()const;

	void DrawBackground2D();
	void DrawTopBar2D();
	void DrawLogoLabel2D();
	void DrawChessboard2D();
	void DrawCells2D();
	void DrawScore2D(RECT rect, LPCTSTR title, int score);
	void DrawCell2D(RECT rect, int num);

	bool CanMoveLeft();
	bool CanMoveRight();
	bool CanMoveUp();
	bool CanMoveDown();

private:
	int m_chessboard[WIDTH][WIDTH];
	int m_iScore;
	int m_iTopScore;
	bool m_bWon;
	Graphics* m_pGraphic;

	RECT m_rcTotal;
	RECT m_rcTopRect;
	RECT m_rcChessRect;
	RECT m_rcLogoRect;
	RECT m_rcScoreRect;
	RECT m_rcTopScoreRect;
};
