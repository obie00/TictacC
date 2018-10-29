#include "GameManager.h"
#include "cstdlib"

GameManager::GameManager(playerOptions playerCharacter)
{
	startGame(playerCharacter);
}


GameManager::~GameManager()
{

}

int GameManager::randomTile() {
	int tile = rand() % slotsLeft.size;
	tile = slotsLeft[tile];
	return tile;
}

void GameManager::updateGrid(int tile, playerOptions option) {
	slotsLeft.erase(slotsLeft.begin() + tile);
	gridSlots[tile] = option;
	if (slotsLeft.size == 0) {
		gameState = gameOver;
	}
}

void GameManager::cpuPlay(){
	int tile = randomTile();
	updateGrid(tile, cpu);
	cpuSlotsPlayed.push_back(tile);
	gameState = waitingforPlayer;
}

void GameManager::playerPlay(int tile) {
	updateGrid(tile, player);
	gameState = waitingforCpu;
}

void GameManager::startGame(playerOptions playerCharacter) {
	if (playerCharacter == X) {
		player = X;
		cpu = O;
		gameState = waitingforPlayer;
	}
	else {
		player = O;
		cpu = X;
		gameState = waitingforCpu;
	}
}