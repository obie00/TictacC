#pragma once
#include <vector>
enum gameStates {
	notStarted,
	waitingforPlayer,
	waitingforCpu,
	gameOver,
	cheated,
};
enum playerOptions {
	E,
	X,
	O,
};
class GameManager
{
public:
	playerOptions cpu;
	playerOptions player;
	void startGame(playerOptions playerCharacter);
	void cpuPlay();
	void playerPlay(int tile);
	std::vector<int> slotsLeft = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	gameStates getGameState();
	void setGameState(gameStates state);
	GameManager(playerOptions playerCharacter);
	~GameManager();

private:
	gameStates gameState = notStarted;
	playerOptions gridSlots[9];
	std::vector<int> cpuSlotsPlayed;
	int movesLeft = 9;
	int randomTile();
	void updateGrid(int tile, playerOptions option);
	void removeSlot(int tile);
};

