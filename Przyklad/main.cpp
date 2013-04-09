/* 
 * File:   main.cpp
 * Author: mar
 *
 * Created on 7 kwiecień 2013, 19:36
 */


#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    Mat edges,frame;
    namedWindow("edges",1);
    for(;;) {    
        cap >> frame; // gt a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
        equalizeHist(edges,edges);
        GaussianBlur(edges, edges, Size(13,13), 2, 2);
        Canny(edges, edges, 0,50, 3);
        flip(edges,edges,1);
        imshow("edges", edges);
        if(waitKey(30) >= 0) break;
    }                    
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

