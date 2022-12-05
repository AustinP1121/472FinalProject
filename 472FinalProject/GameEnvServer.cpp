#include <WinSock2.h>
#include "GameplayManagerServer.h"

using namespace std;

void SendGameData(SOCKET inSocket, GameplayManager* obj);
void ReceiveGameData(SOCKET inSocket, GameplayManager* obj);
void ReceiveMetaData(SOCKET inSocket, GameplayManager* outObj);
void SendMetaData(SOCKET inSocket, GameplayManager* inObj);

int main()
{
	GameplayManager game;

	cout << "Let's Play Connect 4" << endl << endl;
	cout << "Player Two, please enter your name: ";
	cin >> game.playerName;
	game.playerID = 'X';
	game.secondPlayerID = 'O';

	SendMetaData(game.TempClientSocket, &game);

	ReceiveMetaData(game.TempClientSocket, &game);

	game.full = 0;
	game.win = 0;
	game.again = 0;

	game.DisplayBoard();

	do
	{
		game.dropChoice = game.PlayerDrop(game.playerName);
		SendGameData(game.TempClientSocket, &game);

		game.CheckBelow(game.playerID, game.dropChoice);

		game.DisplayBoard();

		game.win = game.CheckWin(&game.playerID);

		if (game.win == 1)
		{
			game.PlayerWin(game.playerName);

			game.again = game.restart();

			char tempBufffer = 's';

			send(game.TempClientSocket, &tempBufffer, sizeof(tempBufffer), 0);

			recv(game.TempClientSocket, &tempBufffer, 56, 0);

			WSACleanup();

			break;
		}

		cout << game.secondPlayerName << "'s Turn..." << endl;

		ReceiveGameData(game.TempClientSocket, &game);

		game.CheckBelow(game.secondPlayerID, game.dropChoice);

		game.DisplayBoard();

		game.win = game.CheckWin(&game.secondPlayerID);

		if (game.win == 1)
		{
			game.PlayerWin(game.secondPlayerName);

			game.again = game.restart();

			if (game.again == 2)
			{
				break;
			}
		}

		game.full = game.FullBoard();

		if (game.full == 7)
		{
			cout << "The board is full, it is a draw!" << endl;

			game.again = game.restart();
		}

	} while (game.again != 2);

	return 0;
}


void SendGameData(SOCKET inSocket, GameplayManager* obj)
{
	//sends the game data to peer

	OutputMemoryStream stream;

	obj->Write(stream);
	send(inSocket, stream.GetBufferPtr(), stream.GetLength(), 0);

	//increments the turn on finish
	obj->turn++;
}
void ReceiveGameData(SOCKET inSocket, GameplayManager* obj)
{
	//receives the game data from peer

	char* buffer = static_cast<char*>(std::malloc(56));
	size_t receivedByteCount = recv(inSocket, buffer, 56, 0);

	if (receivedByteCount > 0)
	{
		InputMemoryStream stream(buffer, static_cast<uint32_t> (56));

		obj->Read(stream);
	}
	else
	{
		std::free(buffer);
	}

	//increments the turn on finish
	obj->turn++;

}

void ReceiveMetaData(SOCKET inSocket, GameplayManager* outObj)
{
	//receives meta data from peer

	char* buffer = static_cast<char*>(std::malloc(56));
	size_t receivedByteCount = recv(inSocket, buffer, 56, 0);

	if (receivedByteCount > 0)
	{
		InputMemoryStream stream(buffer, static_cast<uint32_t> (56));

		outObj->ReadMetaData(stream);
	}
	else
	{
		std::free(buffer);
	}
}

void SendMetaData(SOCKET inSocket, GameplayManager* inObj)
{
	//sends meta data to peer

	OutputMemoryStream stream;

	inObj->WriteMetaData(stream);

	send(inSocket, stream.GetBufferPtr(), stream.GetLength(), 0);
}
