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
	}
	if (state == gameOver) {
		//end game
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
		setGameState(waitingforCpu);
	}
}