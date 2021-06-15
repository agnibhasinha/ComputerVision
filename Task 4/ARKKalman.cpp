#include<iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <queue>
#include <deque>


using namespace std;
using namespace cv;

double mean(int n, double elem, double pmean)
{
	double sum = (n - 1) * pmean;
	double ans = elem + sum / n * 1.0;
	return ans;
}
double variance(int n, double elem, double pvar, double pmean)
{
	double sqsum = (n - 1) * pvar + (n - 1) * pow(pmean, 2);
	double nmean = mean(n, elem, pmean);
	double ans = ((elem * elem + sqsum) / n * 1.0) - (nmean * nmean);
	return ans;

}
double p = 0.02; double h = 1.00;// error in estimate
double kalman(double u,double prev,int count,double r2=0) //prev is the previous beleif for time=t. u is the current measurement
{
	double r = r2; // error in the measurement
	
	double q = 0.01; //process noise
	double unew; // new estimate
	double k = 0; //kalman gain
	//static double meanx=0.01;
	
	k = p * h / (h * p * h + r);// calculating kalman gain
	unew = prev + k * (u - h * prev); // caluclating new estimate at time=t
	p = (1 - k * h) * p + q; // updating the error in estimate for next iteration


	//r = variance(count, prev-u, r, meanx);
	//meanx = mean(count, u, meanx);
	return unew;  // unew is the current beleif for time=t
}


double py = 0.02; // error in estimate
double kalmany(double u, double prev, int count, double r2 = 0) //prev is the previous beleif for time=t. u is the current measurement
{
	double r = r2; // error in the measurement
	double q = 0.01; //process noise
	double unew; // new estimate
	double k = 0; //kalman gain
	k = py * h / (h * py * h + r);// calculating kalman gain
	unew = prev + k * (u - h * prev); // caluclating new estimate at time=t
	py = (1 - k * h) * py + q; // updating the error in estimate for next iteration
	return unew;  // unew is the current beleif for time=t

}

double pz = 0.02; // error in estimate
double kalmanz(double u, double prev, int count, double r2 = 0) //prev is the previous beleif for time=t. u is the current measurement
{
	double r = r2; // error in the measurement
	double q = 0.01; //process noise
	double unew; // new estimate
	double k = 0; //kalman gain
	k = pz * h / (h * pz * h + r);// calculating kalman gain
	unew = prev + k * (u - h * prev); // caluclating new estimate at time=t
	pz = (1 - k * h) * pz + q; // updating the error in estimate for next iteration
	return unew;  // unew is the current beleif for time=t

}

int main()
{
	Mat img(1000, 1000, CV_8UC3, Scalar(0, 0, 0));
	//namedWindow("image", WINDOW_NORMAL);
	ifstream file("Data1.csv");
	//double x_0, y_0, v_x0 = 0, v_y0 = 0;
	//double z_0 = 0;
	double x, y, z;
	deque<double> qx,qy,qz;
	for (int i = 0; i < 10; i++)
	{
		qx.push_back(0.00);
		qy.push_back(0.00);
		qz.push_back(0.00);
	}

	char c1, c2, c3;
	if (!file.is_open())
	{
		cout << "file not open";
		return 0;
	}
	cout << "hello !!\n";
	//file >> x_0 >> c1 >> y_0;

	double x_1=0, y_1=0, z_1=0;
	double x_2 = 0, y_2 = 0, z_2 = 0;
	double x_3 = 0, y_3 = 0, z_3 = 0;
	double x_4 = 0, y_4 = 0, z_4 = 0;
	int count = 0;
	double x_est=0, y_est=0, z_est=0;
	double v_x = 0, v_y = 0, v_z = 0;
	double avx=0.0,avy=0.0,avz=0.0;
	double dummy;
	char dummy2;
	while (!file.eof())
	{
		count++;
		file >> x >> c1 >> y >> c2 >> z >> c3;
		for (int dum = 1; dum <= 15; dum++)
			file >> dummy >> dummy2;
		avx = 0; avy = 0.0, avz = 0.0;
		for (int i = 0; i < 10; i++)
		{
			avx += qx[i]+(i+1)*0.001;
			avy += qy[i]+(i+1)*0.001;
			avz += qz[i]+(i+1)*0.001;
			
		}
		avx /= 10; avy /= 10; avz /= 10;
		v_x = (x_4 - x_1) / 3;
		x_est = kalman(x, x_est + 0.001, count,fabs(x-avx));
		y_est = kalmany(y, y_est + 0.001, count, fabs(y - avy));
		z_est = kalmanz(z, z_est + 0.001, count, fabs(z - avz));
		x_4 = x_3; y_4 = y_3; z_4 = z_3;
		x_3 = x_2; y_3 = y_2; z_3 = z_2;
		x_2 = x_1; y_2 = y_1; z_2 = z_1;
		x_1 = x; y_1 = y; z_1 = z;
		qx.pop_front(); qy.pop_front(); qz.pop_front();
		qx.push_back(x); qy.push_back(y); qz.push_back(z);



		/*x_0 += v_x0; y_0 += v_y0;
		double err_x = kalman(x - x_0);
		double err_y = kalman(y - y_0);
		x_0 += 400; y_0 += 400;
		int k = (int)x_0, l = (int)y_0;
		if (k > 800)
			k = 800;
		if (l > 800)
			l = 800;
		img.at<Vec3b>(k, l)[0] = 255;
		img.at<Vec3b>(k, l)[1] = 0;
		img.at<Vec3b>(k, l)[2] = 255;*/

		int k = (int)(x*400)+50, l = (int)(y*400)+400;
		if (k >= 1000)
			k = 999;
		if (l >= 1000)
			l = 999;
		if (k < 0)
			k = 0;
		if (l < 0)
			l = 0;
		img.at<Vec3b>(k, l)[0] = 255;
		img.at<Vec3b>(k, l)[1] = 0;
		img.at<Vec3b>(k, l)[2] = 255;

		cout << x << " " << y << " " << z<<"    "<<x_est<<"  "<<y_est<<" "<<z_est<<"  "<<(x-x_est)*100<<" "<<(y-y_est)*100<<" "<<(z-z_est)*100<< endl;
		
		
		/*x_0 = x; y_0 = y; v_x0 = v_x; v_y0 = v_y;
		x += 400, y += 400;
		int i, j;
		i = (int)x;
		j = (int)y;
		if (i > 800)
			i = 800;
		if (j > 800)
			j = 800;
		if (i < 0)
			i = 0;
		if (j < 0)
			j = 0;

		img.at<Vec3b>(i, j)[0] = 255;
		img.at<Vec3b>(i, j)[1] = 255;
		img.at<Vec3b>(i, j)[2] = 255;
		imshow("image", img);
		waitKey(1);*/
		imshow("image", img);
		waitKey(1);


	}
	//imshow("image", img);
	waitKey(0);
	return 0;
}