#include "GameManager.h"
#include "cstdlib"

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
		setGameState(gameOver);
		break;
	case gameOver:
		break;
	case waitingforCpu:
		cpuPlay();
		break;
	case waitingforPlayer:
		if (winCondition()) {
			setGameState(gameOver);
		}
		break;
	}
	if (state == gameOver) {
		//end game
	}
	else if (state = cheated)
	{
		//end game
	}
}

bool GameManager::winCondition() {
	if (checkifWon(X) || checkifWon(O)) {
		return true;
	}
	else {
		return false;
	}
}

bool GameManager::checkifWon(playerOptions checkPlayer) {
	if ((gridSlots[0] == checkPlayer) && (gridSlots[1] == checkPlayer) && (gridSlots[2] == checkPlayer)) {
		finalState = 1;
		return true;
	}
	if ((gridSlots[3] == checkPlayer) && (gridSlots[4] == checkPlayer) && (gridSlots[5] == checkPlayer)) {
		finalState = 2;
		return true;
	}
	if ((gridSlots[6] == checkPlayer) && (gridSlots[7] == checkPlayer) && (gridSlots[8] == checkPlayer)) {
		finalState = 3;
		return true;
	}
	if ((gridSlots[0] == checkPlayer) && (gridSlots[3] == checkPlayer) && (gridSlots[6] == checkPlayer)) {
		finalState = 4;
		return true;
	}
	if ((gridSlots[1] == checkPlayer) && (gridSlots[4] == checkPlayer) && (gridSlots[7] == checkPlayer)) {
		finalState = 5;
		return true;
	}
	if ((gridSlots[2] == checkPlayer) && (gridSlots[5] == checkPlayer) && (gridSlots[8] == checkPlayer)) {
		finalState = 6;
		return true;
	}
	if ((gridSlots[0] == checkPlayer) && (gridSlots[4] == checkPlayer) && (gridSlots[8] == checkPlayer)) {
		finalState = 7;
		return true;
	}
	if ((gridSlots[2] == checkPlayer) && (gridSlots[4] == checkPlayer) && (gridSlots[6] == checkPlayer)) {
		finalState = 8;
		return true;
	}
	return false;
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
	if (slotsLeft.size() == 0) {
		setGameState(gameOver);
	}
}

void GameManager::cpuPlay(){
	int tile = randomTile();
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
		setGameState(waitingforPlayer);
	}
}