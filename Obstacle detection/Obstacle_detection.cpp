#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
void Both(Mat mask1, Mat mask2, Mat imgori)
{
	Mat newimg; Mat abc;
	resize(imgori, imgori, Size(500, 500));
	cvtColor(mask2, newimg, CV_GRAY2BGR);
	//cvtColor(mask1, abc, CV_GRAY2BGR);
	resize(mask1, abc, Size(500, 500));
	resize(newimg, newimg, Size(500, 500));
	for (int i = 0; i < newimg.rows; i++)
	{
		for (int j = 0; j < newimg.cols; j++)
		{
			if (newimg.at<Vec3b>(i, j)[0] <= 10)
			{
				imgori.at<Vec3b>(i, j)[0] = 0;
				imgori.at<Vec3b>(i, j)[1] = 0;
				imgori.at<Vec3b>(i, j)[2] = 0;

			}
			if (abc.at<Vec3b>(i, j)[0] >= 245)
			{
				imgori.at<Vec3b>(i, j)[0] = 255;
				imgori.at<Vec3b>(i, j)[1] = 0;
				imgori.at<Vec3b>(i, j)[2] = 255;

			}
		}
	}
	imshow("both", imgori);
}
void ObstaclesNew(Mat mask2, Mat imgori)
{
	Mat newimg;
	resize(imgori, imgori, Size(500, 500));
	cvtColor(mask2, newimg, CV_GRAY2BGR);
	resize(newimg, newimg, Size(500, 500));
	for (int i = 0; i < newimg.rows; i++)
	{
		for (int j = 0; j < newimg.cols; j++)
		{
			if (newimg.at<Vec3b>(i, j)[0] <= 10)
			{
				imgori.at<Vec3b>(i, j)[0] = 0;
				imgori.at<Vec3b>(i, j)[1] = 0;
				imgori.at<Vec3b>(i, j)[2] = 0;

			}
		}
	}
	//imshow("mask2", mask2);
	imshow("obstacles", imgori);
}

Mat Lane(Mat mask,Mat imgori,int noi,int noi2)
{
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat imgErode; Mat cpyimg;
	resize(imgori, cpyimg, Size(500, 500));
	erode(mask, imgErode, kernel);
	for (int i = 1; i <= noi; i++)
	{
		erode(imgErode, imgErode, kernel);
	}
	for (int i = 1; i <= noi + 1; i++)
	{
		dilate(imgErode, imgErode, kernel);
	}
	Mat fimg;
	subtract(mask, imgErode, fimg);

	for (int i = 1; i <= noi2; i++)
	{
		erode(fimg, fimg, kernel);
	}
	for (int i = 1; i <= noi2; i++)
	{
		dilate(fimg, fimg, kernel);
	}
	Mat cfimg(500, 500, CV_8UC3);
	cvtColor(fimg, fimg, COLOR_GRAY2BGR);
	resize(fimg, cfimg, Size(500, 500));

	
	for (int i = 0; i < cfimg.rows; i++)
	{
		for (int j = 0; j < cfimg.cols; j++)
		{
			if (cfimg.at<Vec3b>(i, j)[0] >= 230)
			{
				cpyimg.at<Vec3b>(i, j)[0] = 255;
				cpyimg.at<Vec3b>(i, j)[1] = 0;
				cpyimg.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}
	//imshow("not minus image", mask);

	imshow("img lane", cfimg);
	imshow("colored lane", cpyimg);
	return fimg;


}

int main()
{
	int hmin = 0, smin = 0, vmin = 160;
	int hmax = 80, smax = 100, vmax = 255;
	int noi = 4; int noi2 = 3;
	VideoCapture cap("lane_video.mp4");
	Mat img, imgHSV, mask,mask2;
	Mat imgBlur, imgCanny;
	namedWindow("Trackbars", (1000, 500));
	createTrackbar("Hue Min", "Trackbars", &hmin, 180);
	createTrackbar("Hue Max", "Trackbars", &hmax, 180);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);
	createTrackbar("noi", "Trackbars", &noi, 10);
	createTrackbar("noi2", "Trackbars", &noi2, 10);

	int hmin2 = 10, hmax2 = 80, smin2 = 0, smax2 = 255, vmin2 = 0, vmax2 = 245;
	namedWindow("Obstacles", (1000, 500));
	createTrackbar("Hue Min", "Obstacles", &hmin2, 180);
	createTrackbar("Hue Max", "Obstacles", &hmax2, 180);
	createTrackbar("Sat Min", "Obstacles", &smin2, 255);
	createTrackbar("Sat Max", "Obstacles", &smax2, 255);
	createTrackbar("Val Min", "Obstacles", &vmin2, 255);
	createTrackbar("Val Max", "Obstacles", &vmax2, 255);

	while (true) {
		cap.read(img);
		Mat imgHSV;
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		Scalar lower2(hmin2, smin2, vmin2);
		Scalar upper2(hmax2, smax2, vmax2);
		inRange(imgHSV, lower, upper, mask);
		inRange(imgHSV, lower2, upper2, mask2);
		Mat imgGray; Mat fimg;
		Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat kernel2 = getStructuringElement(MORPH_RECT, Size(5, 5));
		Mat imgErode;
		cvtColor(img, imgGray, CV_BGR2GRAY);
		Mat aaa;
		aaa=Lane(mask, img,noi, noi2);
		ObstaclesNew(mask2, img);
		Both(aaa, mask2, img);
		cout << "start\n";
		
		cout << "done\n";
		waitKey(1);
	}
	waitKey(0);
	return 0;
}