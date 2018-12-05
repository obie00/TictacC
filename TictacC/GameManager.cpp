#include "GameManager.h"
#include "cstdlib"
#include "iostream"

using namespace std;

GameManager::GameManager(playerOptions playerCharacter)
{
	startGame(playerCharacter);
}


GameManager::~GameManager()
{

}

gameStates GameManager::getGameState() {
	return gameState;
}

void GameManager::setGameState(gameStates state) {

	if (gameState == gameOver) {
		return;
	}

	gameState = state;

	switch (state) {
	case cheated:
		cout << "Cheating detected." << endl << endl;
		setGameState(gameOver);
		break;
	case gameOver:
		break;
	case gamewon:
		setGameState(gameOver);
		break;
	case gameTied:
		setGameState(gameOver);
		break;
	case waitingforCpu:
		cpuPlay();
		break;
	}

	if (state == gameOver) {
		cout << "GAME OVER" << endl;
	}
	else if (state = cheated)
	{
		//end game
	}
}

int GameManager::randomTile() {
	int tile = (int)rand() % slotsLeft.size();
	tile = slotsLeft[tile];
	return tile;
}

void GameManager::removeSlot(int tile) {
	std::vector<int>::iterator position = std::find(slotsLeft.begin(), slotsLeft.end(), tile);
	if (position != slotsLeft.end()) {
		slotsLeft.erase(position);
	}
}

void GameManager::updateGrid(int tile, playerOptions option) {
	removeSlot(tile);
	gridSlots[tile] = option;
	bool isGameWon = false;
	victory_symbol = E;

	// Same three symbols in a row.
	if ((gridSlots[0] == gridSlots[1]) && (gridSlots[1] == gridSlots[2]) && (gridSlots[0] != E)) {
		isGameWon = true;
		victory_symbol = gridSlots[0];
	}
	else if ((gridSlots[3] == gridSlots[4]) && (gridSlots[4] == gridSlots[5]) && (gridSlots[3] != E)) {
		isGameWon = true;
		victory_symbol = gridSlots[3];
	}
	else if ((gridSlots[6] == gridSlots[7]) && (gridSlots[7] == gridSlots[8]) && (gridSlots[6] != E)) {
		isGameWon = true;
		victory_symbol = gridSlots[6];
	}
	// Same three symbols in a column.
	else if ((gridSlots[0] == gridSlots[3]) && (gridSlots[3] == gridSlots[6]) && (gridSlots[0] != E)) {
		isGameWon = true;
		victory_symbol = gridSlots[0];
	}
	else if ((gridSlots[1] == gridSlots[4]) && (gridSlots[4] == gridSlots[7]) && (gridSlots[1] != E)) {
		isGameWon = true;
		victory_symbol = gridSlots[1];
	}
	else if ((gridSlots[2] == gridSlots[5]) && (gridSlots[5] == gridSlots[8]) && (gridSlots[2] != E)) {
		isGameWon = true;
		victory_symbol = gridSlots[2];
	}
	// Same three symbols diagonally.
	else if ((gridSlots[0] == gridSlots[4]) && (gridSlots[4] == gridSlots[8]) && (gridSlots[0] != E)) {
		isGameWon = true;
		victory_symbol = gridSlots[0];
	}
	else if ((gridSlots[2] == gridSlots[4]) && (gridSlots[4] == gridSlots[6]) && (gridSlots[0] != E)) {
		isGameWon = true;
		victory_symbol = gridSlots[2];
	}

	if ((isGameWon == true) && (victory_symbol != E)) {
		if (victory_symbol == cpu) {
			cout << "CPU WINS" << endl << endl;
		}
		else if (victory_symbol == player) {
			cout << "PLAYER WINS" << endl << endl;
		}

		setGameState(gamewon);
	}
	else if (slotsLeft.size() == 0) {
		cout << "GAME IS TIED" << endl << endl;
		setGameState(gameTied);
	}
}

void GameManager::cpuPlay() {
	int tile = randomTile();
	chosen_tile = tile;

	updateGrid(tile, cpu);
	cpuSlotsPlayed.push_back(tile);
	setGameState(waitingforPlayer);
}

void GameManager::playerPlay(int tile) {
	updateGrid(tile, player);
	setGameState(waitingforCpu);
}

void GameManager::startGame(playerOptions playerCharacter) {
	if (playerCharacter == X) {
		player = X;
		cpu = O;
		setGameState(waitingforPlayer);
	}
	else {
		player = O;
		cpu = X;
		setGameState(waitingforCpu);
	}
}