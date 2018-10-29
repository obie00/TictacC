#pragma once
#include <vector>
enum gameStates {
	notStarted,
	waitingforPlayer,
	waitingforCpu,
	gameOver,
};
enum playerOptions {
	X,
	O,
	E,
};
class GameManager
{
public:
	gameStates gameState = notStarted;
	void startGame(playerOptions playerCharacter);
	void cpuPlay();
	void playerPlay(int tile);
	GameManager(playerOptions playerCharacter);
	~GameManager();

private:
	playerOptions gridSlots[9];
	playerOptions cpu;
	playerOptions player;
	std::vector<int> slotsLeft;
	std::vector<int> cpuSlotsPlayed;
	int movesLeft = 9;
	int randomTile();
	void updateGrid(int tile, playerOptions option);
	void continueGame();
};

