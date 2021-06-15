#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
//480x640


#define VX 15
#define VY 30

//int main()
//{
//	Mat img(500, 500, CV_8UC3, Scalar(0,0,0));
//	int velx = 2;
//	int x = img.rows / 2;
//	int y = img.cols / 2;
//	int vely = 1;
//	circle(img, Point(x, y), 3, Scalar(0, 0, 255), FILLED, LINE_8);
//	while (1)
//	{
//		circle(img, Point(x, y), 3, Scalar(0, 0, 0), FILLED, LINE_8);
//		if (x + velx >= img.rows || x+velx<0)
//			velx *= -1;
//		if (y + vely >= img.cols || y+vely<0)
//			vely *= -1;
//
//		x += velx;
//		y += vely;
//		if (x >= img.rows)
//			x = img.rows - 1;
//		if (x < 0)
//			x = 0;
//		if (y >= img.cols)
//			y = img.cols - 1;
//		if (y < 0)
//			y = 0;
//		
//		circle(img,	Point(x,y),3,	Scalar(0, 0, 255),FILLED,LINE_8);
//		imshow("img", img);
//		waitKey(1);
//	}
//	waitKey(0);
//	return 0;
//	
//
//}






// Function for Face Detection
Point detectAndDraw(Mat& img, CascadeClassifier& cascade,
    CascadeClassifier& nestedCascade, double scale);
string cascadeName, nestedCascadeName;

int score = 0; int lives = 3;
int main(int argc, const char** argv)
{
    // VideoCapture class for playing video for which faces to be detected
    VideoCapture capture;
    Mat frame, image;

    // PreDefined trained XML classifiers with facial features
    CascadeClassifier cascade, nestedCascade;
    double scale = 1;

    // Load classifiers from "opencv/data/haarcascades" directory 
    nestedCascade.load("haarcascade_eye_tree_eyeglasses.xml");

    // Change path before execution 
    cascade.load("haarcascade_frontalcatface.xml");

    Mat img(500, 500, CV_8UC3, Scalar(0, 0, 0));
    int velx = VX;
    int x = 100;
    int y = 200;
    int vely = VY;
    circle(img, Point(x, y), 3, Scalar(0, 0, 255), FILLED, LINE_8);




    // Start Video..1) 0 for WebCam 2) "Path to Video" for a Local Video
    capture.open(0);
    if (capture.isOpened())
    {
        // Capture frames from video and detect faces
        cout << "Face Detection Started...." << endl;
        while (1)
        {
            capture >> frame;
            if (frame.empty())
                break;
            Mat frame1 = frame.clone();
            Point abc=detectAndDraw(frame1, cascade, nestedCascade, scale);
            cout << abc.x << " " << abc.y <<"  "<<x<<" "<<y<<"\n";
           // cout << frame.rows << " " << frame.cols << "\n";



            circle(img, Point(x, y), 3, Scalar(0, 0, 0), FILLED, LINE_8);
            if (x + velx >= img.rows || x + velx < 0)
                velx *= -1;
            if (y + vely >= img.cols || y + vely < 0)
                vely *= -1;

            x += velx;
            y += vely;
            if (x >= img.rows)
                x = img.rows - 1;
            if (x < 0)
                x = 0;
            if (y >= img.cols)
                y = img.cols - 1;
            if (y < 0)
                y = 0;
            //(500, 500, Scalar(0, 0, 0));
            circle(img, Point(x, y), 3, Scalar(0, 0, 255), FILLED, LINE_8);
            if (abc.y == 0)
                abc.y = img.cols / 2;
            else
                abc.y = abc.y * 500 / 480;
            if (abc.x == 0)
                abc.x = img.rows / 2;
            else
            {
                abc.x = abc.x * 500 / 640;
                
            }
            if (y>400)
            {
                if (x<abc.x + 50 && x>abc.x - 50)
                {
                    vely *= -1;
                    score++;
                }
                else
                {
                    lives--;
                    x = 100; y = 200;
                    if (lives == 0)
                    {
                        cout << "\n\n Game over!!";
                        cout << "\nscore = " << score;
                        break;
                    }
                }
            }
            line(img, Point(0,400), Point( img.rows - 1,400), Scalar(0, 0, 0), 2);
            line(img, Point(abc.x + 50,400), Point(abc.x - 50,400),Scalar(0,255,0),2);

            imshow("image", img);

            char c = 'a';
            waitKey(1);
            // Press q to exit from window
            if ( c == 'q' || c == 'Q')
                break;
        }
    }
    else
        cout << "Could not Open Camera";
    waitKey(0);
    return 0;
}

Point detectAndDraw(Mat& img, CascadeClassifier& cascade,
    CascadeClassifier& nestedCascade,
    double scale)
{
    vector<Rect> faces, faces2;
    Mat gray, smallImg;

    cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to Gray Scale
    double fx = 1 / scale;

    // Resize the Grayscale Image 
    resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
    equalizeHist(smallImg, smallImg);

    // Detect faces of different sizes using cascade classifier 
    cascade.detectMultiScale(smallImg, faces, 1.1,
        2, 0 | CASCADE_SCALE_IMAGE, Size(10, 10),Size(500,500));
    Point center;
    // Draw circles around the faces
    for (size_t i = 0; i < faces.size(); i++)
    {
        Rect r = faces[i];
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        
        Scalar color = Scalar(255, 0, 0); // Color for Drawing tool
        int radius;

        double aspect_ratio = (double)r.width / r.height;
        if (0.075 < aspect_ratio && aspect_ratio < 10.3)
        {
            center.x = cvRound((r.x + r.width * 0.5) * scale);
            center.y = cvRound((r.y + r.height * 0.5) * scale);
            radius = cvRound((r.width + r.height) * 0.25 * scale);
            circle(img, center, radius, color, 3, 8, 0);
        }
        else
            rectangle(img, cvPoint(cvRound(r.x * scale), cvRound(r.y * scale)),
                cvPoint(cvRound((r.x + r.width - 1) * scale),
                    cvRound((r.y + r.height - 1) * scale)), color, 3, 8, 0);
        if (nestedCascade.empty())
            continue;
        smallImgROI = smallImg(r);

        // Detection of eyes int the input image
        nestedCascade.detectMultiScale(smallImgROI, nestedObjects, 1.1, 2,
            0 | CASCADE_SCALE_IMAGE, Size(30, 30));

        //// Draw circles around eyes
        //for (size_t j = 0; j < nestedObjects.size(); j++)
        //{
        //    Rect nr = nestedObjects[j];
        //    center.x = cvRound((r.x + nr.x + nr.width * 0.5) * scale);
        //    center.y = cvRound((r.y + nr.y + nr.height * 0.5) * scale);
        //    radius = cvRound((nr.width + nr.height) * 0.25 * scale);
        //    circle(img, center, radius, color, 3, 8, 0);
        //}
    }

    // Show Processed Image with detected faces
    imshow("Face Detection", img);
    return center;
}