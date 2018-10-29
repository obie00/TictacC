#include "iostream"
#include "GameManager.h"
#include "ImageManager.h"

int main()
{
	char file[40] = "TestImages\\tip1.jpg";
	ImageManager TTTimage(file);

	playerOptions playerCharacter = X;
	GameManager TTT(playerCharacter);
	while (TTT.getGameState() != gameOver) {
		for (int i = 0; i < TTT.slotsLeft.size(); i++) {
			playerOptions choice = TTTimage.detectImage(TTT.slotsLeft[i]);
			if (choice == TTT.player) {
				TTT.playerPlay(TTT.slotsLeft[i]);
			}
			else if (choice == TTT.cpu) {
				TTT.setGameState(cheated);
			}
			else {
				continue;
			}
			break;
		}
	}
	return 0;
}