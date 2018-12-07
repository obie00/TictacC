

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
		Mat display;
		cap >> frame;
		display = frame;
		//imshow("edges", frame);
		//imshow("displ", display);
		//cvWaitKey(0);
		if (waitKey(30) >= 0) break;


		ImageManager TTTimage(frame);
		
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
		TTTimage.getOutputImg(TTT);
		imshow("Output2", TTTimage.outputImg);
		//imshow("Output2", TTTimage.outputImg);
		cvWaitKey(250);
	}
	
	cvDestroyAllWindows();
	return 0;
}