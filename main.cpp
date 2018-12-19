#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <deque>
#include <thread> 

#include "UKC_GPIO.h"

using namespace cv;
using namespace std; 

RNG rng(12345);
char mode = 0;  //debug mode    0 = disable  1 = enable
int tail = 10;   //tail on object in debug
int flag_bird = 0;  //exist bird

int flag_stat = 0;

Rect min_birdsize = Rect(0,0,20,20);    //0 ,0, min width pixel , min height pixel

void bird();
Rect ContoursNBoxing(Mat src);
Mat MovingMat(Mat src, Mat pre_src);

void GPIO_F();
void OPENCV_F();

UKC_GPIO GPIO;

//////////////////////////////// ** main ** //////////////////////////////////////////////


int main(){

    GPIO.GPIO_init();
    
    thread GPIO_th(&GPIO_F);
    thread OpenCV_th(&OPENCV_F);    

    GPIO_th.join();
    OpenCV_th.join();


    return 0;
}


////////////////////////////// ** Thread ** /////////////////////////////////////

void GPIO_F(){
    while(1){
        if(flag_bird == 0){
            GPIO.Motor1_off();
        }
        else if (flag_bird == 1){
            GPIO.Motor1_on();
            delay(300); 
        }

        delay(30);
    }
}


void OPENCV_F(){
    mode = 1;
    bird();
}


///////////////////////////////** OpenCV **//////////////////////////////////////////////

void bird() {
	//init
	Mat frame;
	Mat pre_frame;
	Mat res;
    	Mat cam;
	Rect TRect;
	Point2f center;
	Point cent = Point(0, 0);

	//frame init
	VideoCapture video(0);
	video >> cam;
	cvtColor(cam, frame, COLOR_BGR2GRAY);

	while (true){
		//get frame
		frame.copyTo(pre_frame);
		video >> cam;
		cvtColor(cam, frame, COLOR_BGR2GRAY);

		res = MovingMat(frame, pre_frame);   //detect moving
		TRect = ContoursNBoxing(res);	//Contours & Boxing
		cent = ((TRect.br() + TRect.tl()) * 0.5 ); //get result center

		if (TRect.height < min_birdsize.height && TRect.width < min_birdsize.width)
			flag_bird = 0;
		else
			flag_bird = 1;

	        if(mode){
			cout << "bird = " << cent << endl;
            cout << flag_bird <<endl;
            rectangle(res,TRect,Scalar(255,255,255));
			rectangle(cam,TRect,Scalar(0,255,0),1);
			imshow("bird",cam);
            imshow("move",res);
        	}

       		if (waitKey(1) == 27) {
			break;
		}
	}
	return;
}

Mat MovingMat(Mat src , Mat pre_src) {
	Mat res;
	Mat T1;
	Mat T2;
	blur(src, src, Size(5, 5));
	T1 = pre_src - src;
	T2 = src - pre_src;
	res = T1 + T2;
	threshold(res, res, 20, 255, THRESH_BINARY);

	return res;
}

Rect ContoursNBoxing(Mat src) {
	//Contours & Boxing
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	Rect TRect = Rect(Point(0, 0), Point(0, 0));

	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		if ((TRect.height * TRect.width) < (boundingRect(Mat(contours_poly[i])).height * boundingRect(Mat(contours_poly[i])).width))
			TRect = boundingRect(Mat(contours_poly[i]));
	}

	return TRect;
}
