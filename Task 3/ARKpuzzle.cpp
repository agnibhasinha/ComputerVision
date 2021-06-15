 #include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;
int main()
{

	Mat img = imread("Level1.png");
	cout << img.rows << " " << img.cols;
	Rect roi(0, 0, 177, 7);
	Mat crp = img(roi);
	//resize(crp, crp, Size(), 1, 1);
	Mat crpGray, imgG;
	cvtColor(crp, crpGray, COLOR_BGR2GRAY);
	cvtColor(img, imgG, COLOR_BGR2GRAY);
	//resize(imgG, imgG, Size(), 4, 4);
	/*for (int i=0;i< crp.rows; i++)
	{
		for (int j = 0; j < crp.cols; j++)
		{
			if(crp.at<Vec3b>(i,j)[0]<60 && crp.at<Vec3b>(i, j)[1]<60 && crp.at<Vec3b>(i, j)[2]<60)
			{
				crp.at<Vec3b>(i, j)[0] = 0;
				crp.at<Vec3b>(i, j)[1] = 0;
				crp.at<Vec3b>(i, j)[2] = 0;
			}
			else
			{
				crp.at<Vec3b>(i, j)[0] = 255;
				crp.at<Vec3b>(i, j)[1] = 255;
				crp.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}*/
	int start_x = 0, start_y = 0;

	for (int i = 0; i < crpGray.rows; i++)
	{
		for (int j = 0; j < crpGray.cols; j++)
		{
			cout << crpGray.at<uchar>(i, j);
			if (crpGray.at<uchar>(i, j) == ':')
			{
				start_x = i; start_y = j;
				break;
			}


		}

	}
	cout << start_x << " " << start_y << endl;
	int arr[30000];
	Mat imgex;
	int count = 0;
	start_y++;

	/*while (count <= 30000)
	{
		int img.at<Vec3b>(start_x,start_y)[0]

	}*/
	/*for (int i = 6;; i++)
	{
		for (int j = 94; j <img.cols; j++)
		{
			if (crp.at<Vec3b>(i, j)[0] < 60 && crp.at<Vec3b>(i, j)[1] < 60 && crp.at<Vec3b>(i, j)[2] < 60)
			{
				crp.at<Vec3b>(i, j)[0] = 0;
				crp.at<Vec3b>(i, j)[1] = 0;
				crp.at<Vec3b>(i, j)[2] = 0;
			}
			else
			{
				crp.at<Vec3b>(i, j)[0] = 255;
				crp.at<Vec3b>(i, j)[1] = 255;
				crp.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}*/
	Mat imgz = imread("zucky_elon.png");
	cout << "\n\n\n\n\n" << imgz.rows << " " << imgz.cols;
	int aaa = 0;
	cout << "\n 1st crd:" << (int)img.at<Vec3b>(6, 94)[0] << " " << (int)img.at<Vec3b>(6, 94)[1] << " " << (int)img.at<Vec3b>(6, 94)[2];
	cout << "\n2nd crd: " << (int)img.at<Vec3b>(6, 95)[0] << " " << (int)img.at<Vec3b>(6, 95)[1] << " " << (int)img.at<Vec3b>(6, 95)[2];
	for (int i = 0; i < imgz.rows; i++)
	{
		for (int j = 0; j < imgz.cols; j++)
		{
			if (imgz.at<Vec3b>(i, j)[0] == img.at<Vec3b>(6, 94)[0] &&
				imgz.at<Vec3b>(i, j)[1] == img.at<Vec3b>(6, 94)[1] &&
				imgz.at<Vec3b>(i, j)[2] == img.at<Vec3b>(6, 94)[2])
			{
				int flag = 1;
				for (int k = 1; k < 30; k++)
				{
					if (imgz.at<Vec3b>(i, j + k)[0] == img.at<Vec3b>(6, 94 + k)[0] &&
						imgz.at<Vec3b>(i, j + k)[1] == img.at<Vec3b>(6, 94 + k)[1] &&
						imgz.at<Vec3b>(i, j + k)[2] == img.at<Vec3b>(6, 94 + k)[2])
							{continue; }
					else {
						flag = 0;
						break;
					}



				}
				if (flag == 1)
				{
					aaa++;
					cout << "\n yes" << i << " " << j;
					cout << "    2nd: " << (int)imgz.at<Vec3b>(i, j + 1)[0] << " " << (int)imgz.at<Vec3b>(i, j + 1)[1] << " " << (int)imgz.at<Vec3b>(i, j + 1)[2];
				}
			}
		}
	}
		cout << "\n"<<aaa;
		//imshow("imgz", imgz);
		

		Mat img3 = imread("maze_lv3.png");
		cout << "\n\n\n";
		cout << img3.rows << " " << img3.cols;
		cout << "\n Part 3";
		imshow("img3", img3);
		Mat img3G;
		cvtColor(img3, img3G, COLOR_BGR2GRAY);
		for (int i = 0; i < img3G.rows; i++)
		{
			for (int j = 0; j < img3G.cols; j++)
			{
				//if ((int)img3G.at<uchar>(i, j) >= 229 && (int)img3G.at<uchar>(i,j)<=231)
				if((img3.at<Vec3b>(i,j)[0]==230 ))
				{
					//cout << "\nyesss";
					img3G.at<uchar>(i, j) = 255;
				}
				else
				{
					img3G.at<uchar>(i, j) = 0;
				}
			}
		}
		Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
		/*for(int i=1;i<5;i++)
		dilate(img3G, img3G, kernel);
		for(int i=1;i<5;i++)
		erode(img3G, img3G, kernel);
		imshow("img3G", img3G);*/
		/*Mat cdstP;
		cvtColor(img3G, cdstP, COLOR_GRAY2BGR);*/
		
		
		//int aa=20, bb=20, cc = 20;
		//namedWindow("Trackbars", (1000, 500));
		//createTrackbar("Hue Min", "Trackbars", &aa, 50);
		//createTrackbar("Hue Max", "Trackbars", &bb, 50);
		//createTrackbar("Sat Min", "Trackbars", &cc, 50);
		//while (1) {
		//	Mat cdstP;
		//	cvtColor(img3G, cdstP, COLOR_GRAY2BGR);
		//	vector<Vec4i> linesP; // will hold the results of the detection
		//	HoughLinesP(img3G, linesP, 1, CV_PI / 180, aa, bb, cc); // runs the actual detection
		//	for (size_t i = 0; i < linesP.size(); i++)
		//	{
		//		Vec4i l = linesP[i];
		//		if (fabs(l[0] - l[2]) < 5 || fabs(l[1] - l[3]) < 5)
		//			line(cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
		//	}
		//	imshow("img3g", img3G);
		//	imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
		//	//cout << "finally done";
		//	waitKey(1);
		//}
	
		Mat imgmp = imread("treasure_mp3.png");
		cvtColor(imgmp, imgmp, COLOR_BGR2GRAY);
		for (int i = 0; i < imgmp.rows/10; i++)
		{
			for (int j = 0; j < imgmp.cols; j++)
				cout << imgmp.at<uchar>(i, j);
		}
		cout << "\n\n finally done";

	waitKey(0);
	return 0;
}