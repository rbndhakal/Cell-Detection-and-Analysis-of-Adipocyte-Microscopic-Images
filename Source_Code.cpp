#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include <fstream>
#include "opencv2/imgcodecs.hpp"
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;



int main( )
{
    // calibration factor
    double coe=1;
    //cout << "Please enter Calibration Factor: " << endl;
    //cin >> coe;

    Mat image;
    image = imread("E:/Image_IP1.jpg", 1);
    namedWindow( "Original Image", WINDOW_AUTOSIZE );
    imshow( "Original Image", image );
    Mat gray,num;
    num=image;

    cvtColor(image, gray, COLOR_BGR2GRAY);
    imshow( "Gray Scale", gray );

    threshold( gray, gray, 212,255,THRESH_BINARY );
    imshow( "Binary", gray );

    // Find contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);
    findContours( gray, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    // Draw contours
    Mat drawing = Mat::zeros( gray.size(), CV_8UC3 );


    for( int i = 0; i< contours.size(); i++ ) // iterate through each contour.
    {
      if (contourArea(contours[i])<=10)
      {
       contours.erase(contours.begin() + i);
      }
    }


    for( int i = 0; i< contours.size(); i++ )
    {
        if (contourArea(contours[i])>=10)
        {
        Scalar color = Scalar( 255, 255, 255 );
        //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, -1, 8, hierarchy, 0, Point() );
        //drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
        }
    }

      vector<vector<Point> > contours_poly( contours.size() );
      vector<Rect> boundRect( contours.size() );
      vector<Point2f>center( contours.size() );
      vector<float>radius( contours.size() );

      char str[5];

      for( int i = 0; i< contours.size(); i++ )
         {
          approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
           boundRect[i] = boundingRect( Mat(contours_poly[i]) );
           minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
           Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

           sprintf(str,"%d",i);
           putText(drawing, str, center[i], FONT_HERSHEY_PLAIN, 0.5,  Scalar(0,0,0,255));
           //putText(drawing,str, center, FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0,0,255,255));

      }

      // Show in a window
      //imshow( "Contours", drawing );

    cout << contours.size() << endl;
    imshow( "Result window", drawing );

    ofstream outputFile,fs;

    outputFile.open("E:\\test1.csv");
    outputFile << "Cell Number" << "," << "Cell Area" << endl;

    for( int i = 0; i< contours.size(); i++ )
    {
        outputFile << i << "," << contourArea(contours[i])*coe*coe  << endl;
    }
    outputFile.close();

    //SAVE image
    imwrite("image1_result.jpg",drawing);// it will store the image in name "result.jpg"


    waitKey(0);
    return 0;
}
