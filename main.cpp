#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <deque>

using namespace cv;
using namespace std; 

RNG rng(12345);
char mode = 0;  //debug mode    0 = disable  1 = enable
int tail = 10;   //tail on object in debug
int flag_bird = 0;  //exist bird
Rect min_birdsize = Rect(0,0,20,20);    //0 ,0, min width pixel , min height pixel

Rect ContoursNBoxing(Mat src);
Mat MovingMat(Mat src, Mat pre_src);

int main() {
	//init
	Mat frame;
	Mat pre_frame;
	Mat res;
	Mat cam;
	Rect TRect;
	Point2f center;
	Point cent = Point(0, 0);
	deque<Point> cents;
	for (int t = 0; t < tail; t++)
		cents.push_back(Point(0, 0));
	float radius;

	//frame init
	VideoCapture video(0);
	video >> frame;
	cvtColor(frame, frame, COLOR_BGR2GRAY);

	while (true){
		//get frame
		frame.copyTo(pre_frame);
		video >> frame;
		if (mode)
			frame.copyTo(cam);
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		res = MovingMat(frame, pre_frame);   //detect moving
		TRect = ContoursNBoxing(res);	//Contours & Boxing
		cent = ((TRect.br() + TRect.tl()) * 0.5 ); //get result center

		if (TRect.height < min_birdsize.height && TRect.width < min_birdsize.width)
			flag_bird = 0;
		else
			flag_bird = 1;

		//debug
		if (mode) {
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			cents.pop_front();
			cents.push_back(cent);
			rectangle(res, TRect, color, 2);
			rectangle(cam, TRect, color, 2);
			for (int i = 0; i < tail-1; i++) {
				if (cents[i] == Point(0, 0) || cents[i + 1] == Point(0, 0))
					continue;
				line(cam, cents[i], cents[i + 1], color, 2);
			}
			//circle(res, center, (int)radius, color, 2, 8, 0);
			//circle(cam, center, (int)radius, color, 2, 8, 0);
			imshow("debug", res);
			imshow("camera", cam);
			cout << "center = " << cent << endl;
		}
		cout << "bird = " << flag_bird << endl;
		if (waitKey(50) == 27) {
			break;
		}
	}
	return 0;
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
