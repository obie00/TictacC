#include "iostream"
#include "GameManager.h"
#include "ImageManager.h"

int main()
{
	playerOptions playerCharacter = X;
	GameManager TTT(playerCharacter);
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened()) {
		return -1;
	}
	for (; TTT.getGameState() != gameOver;)
	{
		Mat frame;
		cap >> frame;
		imshow("edges", frame);
		if (waitKey(30) >= 0) break;


		ImageManager TTTimage(frame);
		/*
		maybe move constructor into while loop but keep value of slotsLeft


		*/
		if (TTTimage.foundBoard == true) {
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
	}

	
	return 0;
}