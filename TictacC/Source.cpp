#include "iostream"
#include "GameManager.h"
#include "ImageManager.h"


int main()
{
	int temp = 0;
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
					temp = 1;
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
		if (temp == 1) {
			//imwrite("TestImages\\outb3.jpg", TTTimage.outputImg);
			temp = 0;
		}
		if (TTT.finalState != 0) {
			TTTimage.getWinImg(TTT.finalState);
			imshow("winnImg", TTTimage.winImg);
			imwrite("TestImages\\winboutb3.jpg", TTTimage.outputImg);
			cvWaitKey(0);
		}
	}
	cvDestroyAllWindows();
	return 0;
}