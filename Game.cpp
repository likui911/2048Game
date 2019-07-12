#include "Game.h"
#include "util.h"
#include <ctime>

Game::Game() :m_iScore{ 0 }, m_bWon{ false }, m_chessboard{ {0} }, m_rcChessRect{ 0 }, m_rcTopRect{ 0 }
{
	srand((unsigned)time(0));
	this->InitialChessBoard();
	m_iTopScore = readTopScore();
}

Game::~Game()
{
	if (this->m_iTopScore < this->m_iScore)
	{
		this->m_iTopScore = this->m_iScore;
	}
	saveTopScore(this->m_iTopScore);
}

void Game::SetRect(const RECT& rect)
{
	m_rcTotal = rect;

	m_rcTopRect.left = 0 + GAP / 2;
	m_rcTopRect.top = 0;
	m_rcTopRect.right = rect.right;
	// make sure chessRest is a square
	m_rcTopRect.bottom = rect.bottom - rect.right;

	m_rcLogoRect.left = rect.left - GAP / 2;
	m_rcLogoRect.right = m_rcLogoRect.left + 200;
	m_rcLogoRect.top = (int)(getRectHeight(m_rcTopRect) * 0.2);
	m_rcLogoRect.bottom = (int)(getRectHeight(m_rcTopRect) * 0.8);

	m_rcTopScoreRect.right = rect.right - GAP / 2;
	m_rcTopScoreRect.left = m_rcTopScoreRect.right - 100;
	m_rcTopScoreRect.top = (int)(getRectHeight(m_rcTopRect) * 0.20);
	m_rcTopScoreRect.bottom = (int)(getRectHeight(m_rcTopRect) * 0.80);

	m_rcScoreRect.right = m_rcTopScoreRect.left - GAP;
	m_rcScoreRect.left = m_rcScoreRect.right - 100;
	m_rcScoreRect.top = m_rcTopScoreRect.top;
	m_rcScoreRect.bottom = m_rcTopScoreRect.bottom;

	m_rcChessRect.left = 0 + GAP / 2;
	m_rcChessRect.top = m_rcTopRect.bottom + GAP / 2;
	m_rcChessRect.right = rect.right - GAP / 2;
	m_rcChessRect.bottom = rect.bottom - GAP / 2;
}

void Game::SetGraphics(Graphics* pGraphic)
{
	this->m_pGraphic = pGraphic;
}

void Game::Restart()
{
	if (this->m_iScore > this->m_iTopScore)
	{
		this->m_iTopScore = this->m_iScore;
	}
	this->m_iScore = 0;
	this->m_bWon = false;

	memset(this->m_chessboard, 0, WIDTH * WIDTH * sizeof(int));
	this->InitialChessBoard();
}

void Game::InitialChessBoard()
{
	for (int i = 0; i < RANDOM_CELL; ++i)
	{
		this->GenerateRandomCell();
	}
}

void Game::GenerateRandomCell()
{
	int num = this->GetRandomAvaliableCell();
	int row = num / WIDTH;
	int col = num % WIDTH;
	//rand() / double(RAND_MAX) 取得0～1之间的浮点数
	m_chessboard[row][col] = rand() / double(RAND_MAX) > 0.9 ? 4 : 2;
}

int Game::GetRandomAvaliableCell() const
{
	while (true)
	{
		//a + rand() % n；其中的a是起始值，n是整数的范围。
		int random = rand() % (WIDTH * WIDTH);
		int row = random / WIDTH;
		int col = random % WIDTH;
		if (m_chessboard[row][col] == 0)
		{
			return random;
		}
	}
	return -1;
}

bool Game::MoveUp()
{
	if (!this->CanMoveUp())
		return false;
	for (int j = 0; j < WIDTH; ++j)
	{
		for (int i = 0; i < WIDTH; ++i)
		{
			if (this->m_chessboard[i][j] != 0)
			{
				for (int z = i - 1; z >= 0; z--)
				{
					if (this->m_chessboard[z][j] == 0)
					{
						this->m_chessboard[z][j] = this->m_chessboard[z + 1][j];
						this->m_chessboard[z + 1][j] = 0;
					}
					else if (this->m_chessboard[z][j] == this->m_chessboard[z + 1][j])
					{
						this->m_chessboard[z][j] = 2 * this->m_chessboard[z + 1][j];
						this->m_chessboard[z + 1][j] = 0;

						this->m_iScore += this->m_chessboard[z][j];

						if (this->m_chessboard[z][j] == 2048)
							this->m_bWon = true;

						break;
					}
				}
			}
		}
	}

	this->GenerateRandomCell();
	return true;
}

bool Game::MoveLeft()
{
	if (!this->CanMoveLeft())
		return false;

	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if (this->m_chessboard[i][j] != 0)
			{
				for (int z = j - 1; z >= 0; z--)
				{
					if (this->m_chessboard[i][z] == 0)
					{
						this->m_chessboard[i][z] = this->m_chessboard[i][z + 1];
						this->m_chessboard[i][z + 1] = 0;
					}
					else if (this->m_chessboard[i][z] == this->m_chessboard[i][z + 1])
					{
						this->m_chessboard[i][z] = 2 * this->m_chessboard[i][z + 1];
						this->m_chessboard[i][z + 1] = 0;

						this->m_iScore += this->m_chessboard[i][z];

						if (this->m_chessboard[z][j] == 2048)
							this->m_bWon = true;

						break;
					}
				}
			}
		}
	}

	this->GenerateRandomCell();
	return true;
}

bool Game::MoveRight()
{
	if (!this->CanMoveRight())
		return false;
	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = WIDTH - 1; j >= 0; --j)
		{
			if (this->m_chessboard[i][j] != 0)
			{
				for (int z = j + 1; z <= WIDTH - 1; z++)
				{
					if (this->m_chessboard[i][z] == 0)
					{
						this->m_chessboard[i][z] = this->m_chessboard[i][z - 1];
						this->m_chessboard[i][z - 1] = 0;
					}
					else if (this->m_chessboard[i][z] == this->m_chessboard[i][z - 1])
					{
						this->m_chessboard[i][z] = 2 * this->m_chessboard[i][z - 1];
						this->m_chessboard[i][z - 1] = 0;

						this->m_iScore += this->m_chessboard[i][z];

						if (this->m_chessboard[z][j] == 2048)
							this->m_bWon = true;

						break;
					}
				}
			}
		}
	}

	this->GenerateRandomCell();
	return true;
}

bool Game::MoveDown()
{
	if (!this->CanMoveDown())
		return false;
	for (int j = 0; j < WIDTH; ++j)
	{
		for (int i = WIDTH - 1; i >= 0; --i)
		{
			if (this->m_chessboard[i][j] != 0)
			{
				for (int z = i + 1; z <= WIDTH - 1; z++)
				{
					if (this->m_chessboard[z][j] == 0)
					{
						this->m_chessboard[z][j] = this->m_chessboard[z - 1][j];
						this->m_chessboard[z - 1][j] = 0;
					}
					else if (this->m_chessboard[z][j] == this->m_chessboard[z - 1][j])
					{
						this->m_chessboard[z][j] = 2 * this->m_chessboard[z - 1][j];
						this->m_chessboard[z - 1][j] = 0;

						this->m_iScore += this->m_chessboard[z][j];

						if (this->m_chessboard[z][j] == 2048)
							this->m_bWon = true;

						break;
					}
				}
			}
		}
	}

	this->GenerateRandomCell();
	return true;
}

void Game::Draw2D()
{
	this->DrawBackground2D();
	this->DrawTopBar2D();
	this->DrawChessboard2D();
}

void Game::DrawBackground2D()
{
	this->m_pGraphic->ClearScreen(RGB(250, 248, 239));
}

void Game::DrawTopBar2D()
{
	this->DrawLogoLabel2D();
	this->DrawScore2D(m_rcTopScoreRect, L"BEST", m_iTopScore);
	this->DrawScore2D(m_rcScoreRect, L"SCORE", m_iScore);
}

void Game::DrawLogoLabel2D()
{
	m_pGraphic->DrawLogo(m_rcLogoRect);
}

void Game::DrawScore2D(RECT rect, LPCTSTR title, int score)
{
	m_pGraphic->DrawScore(rect, title, score);
}

void Game::DrawChessboard2D()
{
	this->m_pGraphic->drawRoundRect(m_rcChessRect, RGB(187, 173, 160));
	this->DrawCells2D();
}

void Game::DrawCells2D()
{
	int width = (m_rcChessRect.right - m_rcChessRect.left - 5 * GAP) / 4;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			RECT rect;
			rect.top = m_rcChessRect.top + GAP + (GAP + width) * i;
			rect.left = m_rcChessRect.left + GAP + (GAP + width) * j;
			rect.bottom = m_rcChessRect.top + (GAP + width) * (i + 1);
			rect.right = m_rcChessRect.left + (GAP + width) * (j + 1);
			this->DrawCell2D(rect, m_chessboard[i][j]);
		}
	}
}

int Game::GetScore()
{
	return this->m_iScore;
}

RECT Game::GetChessRect()
{
	return m_rcChessRect;
}

RECT Game::GetTopRect()
{
	return m_rcTopRect;
}

RECT Game::GetRect()
{
	return m_rcTotal;
}

bool Game::IsGameOver()
{
	if (this->CanMoveLeft() || this->CanMoveRight() || this->CanMoveUp() || this->CanMoveDown())
	{
		return false;
	}
	return true;
}

bool Game::IsWon()
{
	return this->m_bWon;
}

bool Game::IsTerminated()
{
	return IsGameOver() || IsWon();
}

bool Game::CanMoveLeft()
{
	for (int i = 0; i < WIDTH; i++)
	{
		bool ret = false;
		for (int j = 0; j < WIDTH; j++)
		{
			if (this->m_chessboard[i][j] == 0)
				ret = true;
			else if ((j != 0 && (this->m_chessboard[i][j] == this->m_chessboard[i][j - 1])) || ret)
				return true;
		}
	}
	return false;
}

bool Game::CanMoveRight()
{
	for (int i = 0; i < WIDTH; ++i)
	{
		bool ret = false;
		for (int j = WIDTH - 1; j >= 0; --j)
		{
			if (this->m_chessboard[i][j] == 0)
				ret = true;
			else if ((j != WIDTH - 1 && this->m_chessboard[i][j] == this->m_chessboard[i][j + 1]) || ret)
				return true;
		}
	}
	return false;
}

bool Game::CanMoveUp()
{
	for (int j = 0; j < WIDTH; j++)
	{
		bool ret = false;
		for (int i = 0; i < WIDTH; i++)
		{
			if (this->m_chessboard[i][j] == 0)
				ret = true;
			else if ((i != 0 && (this->m_chessboard[i][j] == this->m_chessboard[i - 1][j])) || ret)
				return true;
		}
	}
	return false;
}

bool Game::CanMoveDown()
{
	for (int j = 0; j < WIDTH; ++j)
	{
		bool ret = false;
		for (int i = WIDTH - 1; i >= 0; --i)
		{
			if (this->m_chessboard[i][j] == 0)
				ret = true;
			else if ((i != WIDTH - 1 && this->m_chessboard[i][j] == this->m_chessboard[i + 1][j]) || ret)
				return true;
		}
	}
	return false;
}

void Game::DrawCell2D(RECT rect, int num)
{
	COLORREF bgColor;
	COLORREF fontColor;
	getCellColour(num, &bgColor, &fontColor);
	this->m_pGraphic->drawRoundRect(rect, bgColor);
	if (num != 0)
	{
		this->m_pGraphic->DrawCellText(rect, fontColor, num);
	}
}