#include "iostream"
#include "GameManager.h"
#include "ImageManager.h"

int main()
{
	playerOptions playerCharacter = X;
	GameManager TTT(playerCharacter);
	VideoCapture cap(0);
	Mat frame;
	Mat display;

	cap.open("http://192.168.0.2:8080/video?dummy=param.mjpgr");
	
	if (!cap.isOpened()) {
		return -1;
	}

	for (; TTT.getGameState() != gameOver;)
	{
		cap >> frame;
		display = frame;

		//imshow("Frame", frame);

		ImageManager TTTimage(frame);

		/*
		maybe move constructor into while loop but keep value of slotsLeft
		*/

		if (TTTimage.foundBoard == true) {
			for (int i = 0; i < TTT.slotsLeft.size(); i++) {

				playerOptions choice = TTTimage.detectImage(TTT.slotsLeft[i]);

				if (choice == TTT.player) {
					TTT.playerPlay(TTT.slotsLeft[i]);

					display = TTTimage.getOutputImg(TTT.chosen_tile, TTT.cpu);
				}
				else if (choice == TTT.cpu) {
					cout << "Player has cheated." << endl << endl;
					TTT.setGameState(cheated);
				}
			}
		}

		imshow("Output", display);
		waitKey(1);
	}

	cvDestroyAllWindows();

	return 0;
}