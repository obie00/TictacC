#include "iostream"
#include "GameManager.h"
#include "ImageManager.h"

int main()
{
	/*
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened()) {
		return -1;
	}
	for (;;)
	{
		Mat frame;
		cap >> frame;
		imshow("edges", frame);
		if (waitKey(30) >= 0) break;
	}*/

	char file[40] = "TestImages\\tip1.jpg";
	Mat source = imread(file);
	if (source.data == NULL) {
		cout << "image not found\n";
		system("pause");
		return 0;
	}
	ImageManager TTTimage(source);

	playerOptions playerCharacter = X;
	GameManager TTT(playerCharacter);
	/*
	maybe move constructor into while loop but keep value of slotsLeft
	
	
	*/
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