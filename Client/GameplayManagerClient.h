#pragma once

#include "NetworkManagerClient.h"
#include "MemoryStream.h"

class GameplayManager : public NetworkManagerClient
{
public:
	GameplayManager();
	~GameplayManager();

	/// <summary>
	/// Gameplay code
	/// </summary>

	int PlayerDrop(char*);
	void CheckBelow(char, int dropChoice);
	void DisplayBoard();
	int CheckWin(char*);
	int FullBoard();
	void PlayerWin(char*);
	int restart();

	/// <summary>
	/// Network interface
	/// </summary>

	void Write(OutputMemoryStream&);
	void WriteMetaData(OutputMemoryStream&);
	void Read(InputMemoryStream&);
	void ReadMetaData(InputMemoryStream&);

	int turn = 1;

	char playerName[23];
	char secondPlayerName[23];

	char playerID = ' ';
	char secondPlayerID = ' ';

	char board[9][10];
	int trueWidth = 7;
	int trueLength = 6;
	int dropChoice = 1, win = 0, full = 0, again = 0;

private:

};

GameplayManager::GameplayManager()
{
}

GameplayManager::~GameplayManager()
{
}

int GameplayManager::PlayerDrop(char* name)
{
	int dropChoice;
	do
	{
		std::cout << name << "'s Turn, Turn " << turn << "\n\n";
		std::cout << "Please enter a number between 1 and 7: ";
		std::cin >> dropChoice;

		while (board[1][dropChoice] == 'X' || board[1][dropChoice] == 'O')
		{
			std::cout << "That row is full, please enter a new row: ";
			std::cin >> dropChoice;
		}

	} while (dropChoice < 1 || dropChoice > 7);

	return dropChoice;
}

void GameplayManager::CheckBelow(char ID, int dropChoice)
{
	int length, turn;
	length = 6;
	turn = 0;

	do
	{
		if (dropChoice < 0)
		{
			break;
		}
		else if (board[length][dropChoice] != 'X' && board[length][dropChoice] != 'O')
		{
			board[length][dropChoice] = ID;
			turn = 1;
		}
		else
			--length;
	} while (turn != 1);
}

void GameplayManager::DisplayBoard()
{
	int rows = 6, columns = 7, i, ix;

	for (i = 1; i <= rows; i++)
	{
		std::cout << "\t|";
		for (ix = 1; ix <= columns; ix++)
		{
			if (board[i][ix] != 'X' && board[i][ix] != 'O')
				board[i][ix] = '*';

			std::cout << board[i][ix];

		}

		std::cout << "|" << std::endl;
	}
}

int GameplayManager::CheckWin(char* ID)
{
	char XO;
	int win;

	XO = *ID;
	win = 0;

	for (int i = 8; i >= 1; --i)
	{

		for (int ix = 9; ix >= 1; --ix)
		{

			if (board[i][ix] == XO &&
				board[i - 1][ix - 1] == XO &&
				board[i - 2][ix - 2] == XO &&
				board[i - 3][ix - 3] == XO)
			{
				win = 1;
			}


			if (board[i][ix] == XO &&
				board[i][ix - 1] == XO &&
				board[i][ix - 2] == XO &&
				board[i][ix - 3] == XO)
			{
				win = 1;
			}

			if (board[i][ix] == XO &&
				board[i - 1][ix] == XO &&
				board[i - 2][ix] == XO &&
				board[i - 3][ix] == XO)
			{
				win = 1;
			}

			if (board[i][ix] == XO &&
				board[i - 1][ix + 1] == XO &&
				board[i - 2][ix + 2] == XO &&
				board[i - 3][ix + 3] == XO)
			{
				win = 1;
			}

			if (board[i][ix] == XO &&
				board[i][ix + 1] == XO &&
				board[i][ix + 2] == XO &&
				board[i][ix + 3] == XO)
			{
				win = 1;
			}
		}

	}

	return win;
}

int GameplayManager::FullBoard()
{
	int full;
	full = 0;
	for (int i = 1; i <= 7; ++i)
	{
		if (board[1][i] != '*')
			++full;
	}

	return full;
}

void GameplayManager::PlayerWin(char* name)
{
	std::cout << std::endl << name << " Connected Four, You Win!" << std::endl;
}

int GameplayManager::restart()
{
	int restart;

	std::cout << "Would you like to restart? Yes(1) No(2): ";
	std::cin >> restart;
	if (restart == 1)
	{
		for (int i = 1; i <= 8; i++)
		{
			for (int ix = 1; ix <= 7; ix++)
			{
				board[i][ix] = '*';
			}
		}
	}
	else {
		std::cout << "Goodbye!" << std::endl;
	}
	return restart;
}

void GameplayManager::Write(OutputMemoryStream& inStream)
{
	//writes drop choiuce to memory stream
	inStream.Write(dropChoice);
}

void GameplayManager::Read(InputMemoryStream& outStream)
{
	//reads drop choice from memory stream
	outStream.Read(dropChoice);
}

void GameplayManager::ReadMetaData(InputMemoryStream& outStream)
{
	//retrieves second player's name from memory stream
	for (int i = 0; i < sizeof(secondPlayerName); i++)
	{
		outStream.Read(secondPlayerName[i]);
	}
}

void GameplayManager::WriteMetaData(OutputMemoryStream& inStream)
{
	//writes player name to memory stream
	for (int i = 0; i < sizeof(playerName); i++)
	{
		inStream.Write(playerName[i]);
	}
}