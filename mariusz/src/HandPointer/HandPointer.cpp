/* 
 * File:   main.cpp
 * Author: mar
 *
 * Created on 7 kwiecień 2013, 19:36
 */


#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <opencv/cv.h>
#include <math.h>
#include <iostream>
#include <stdio.h> 
#include <assert.h>   
#include <unistd.h>  
#include <cvblob.h>

#include "Media.hpp"
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include<stdio.h>
#include <math.h>


using namespace cv;
using namespace std;
using namespace cvb;
Mat frame, img1;
Point point;
Point matchLoc;
int drag = 0;
int key = 0;
CvRect rect;
bool done;

void mouseHandler(int event, int x, int y, int flags, void* param) {
    if (event == CV_EVENT_LBUTTONUP) {
        point = Point(x, y);
        done = true;
    }
}


void matchImage(Mat &img1, Mat &img2) {
    Mat imgOutput;
    int row = img1.rows - img2.rows + 1;
    int col = img1.cols - img2.cols + 1;
    imgOutput.create(col, row, CV_32FC1);
    matchTemplate(img1, img2, imgOutput, CV_TM_CCOEFF_NORMED);
    normalize(imgOutput, imgOutput, 0, 1, NORM_MINMAX, -1, Mat());
    Point minLoc, maxLoc;
    double minVal, maxVal;
    minMaxLoc(imgOutput, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    matchLoc = maxLoc;

}

int main(int, char**) {

    Media mouse;
    bool mouseon = true;
    int x, x1, y, y1;
    //   W   H
    pair<int, int> res = mouse.getResolution();



    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded
        return 0;
    namedWindow("result", 1);
    namedWindow("output", 1);
    namedWindow("bin", 1);
    cap >> frame;
    int frameX=frame.cols,frameY=frame.rows;
    flip(frame, frame, 1);


    Mat rawTemplate, rawSearch;
    while (!done) {

        cap >> frame;
        flip(frame, frame, 1);
        imshow("result", frame);
        if (rect.width > 0)
            frame = frame(rect);
        setMouseCallback("result", mouseHandler, NULL);
        imshow("result", frame);
        cvWaitKey(10);
    }

    Point p1 = Point(point.x - 20, point.y - 20);
    Point p2 = Point(point.x + 20, point.y + 20);

    rectangle(frame, p1, p2, CV_RGB(255, 0, 0), 1, 8, 0);
    Rect ra = Rect(p1, p2);
    Mat imgTemplate = frame(ra);
    Point p3 = Point(p1.x - 30, p1.y - 30);
    Point p4 = Point(p2.x + 60, p2.y + 60);

    Mat searchRegion = frame(Rect(p3, p4));
    rawTemplate = imgTemplate = imgTemplate.clone();
    rawSearch = searchRegion = searchRegion.clone();
    matchImage(searchRegion, imgTemplate);

    char key = -1;
    Rect solution = Rect(Point(matchLoc.x + p3.x, matchLoc.y + p3.y), Point(matchLoc.x + imgTemplate.cols + p3.x, matchLoc.y +
            imgTemplate.rows + p3.y));

    while (true) {
        cap >> frame;
        flip(frame, frame, 1);
        Point p1 = Point(matchLoc.x, matchLoc.y);
        Point p2 = Point(matchLoc.x + imgTemplate.cols, matchLoc.y + imgTemplate.rows);


        imgTemplate = searchRegion.clone();
        imgTemplate = imgTemplate(Rect(p1, p2));


        searchRegion = frame.clone();
        searchRegion = searchRegion(Rect(p3, p4));



        matchImage(searchRegion, imgTemplate);
        solution = Rect(Point(matchLoc.x + p3.x, matchLoc.y + p3.y), Point(matchLoc.x + imgTemplate.cols + p3.x, matchLoc.y +
                imgTemplate.rows + p3.y));


        if ((x = solution.x - 50) <= 0)
            x = 0;
        else
            if ((x = solution.x - 50) >= frameX)
            x = frameX;
        if ((y = solution.y - 50) <= 0)
            y = 0;
        else
            if ((y = solution.y - 50) >= frameY)
            y = frameY;
        if ((x1 = solution.x + solution.width + 50) <= 0)
            x1 = 0;
        else
            if ((x1 = solution.x + solution.width + 50) >= frameX)
            x1 = frameX;
        if ((y1 = solution.y + solution.height + 50) <= 0)
            y1 = 0;
        else
            if ((y1 = solution.y + solution.height + 50) >= frameY)
            y1 = frameY;


        p3 = Point(x, y);
        p4 = Point(x1, y1);
        rectangle(frame, p3, p4, Scalar(0, 0, 255), 2, 8, 0);
        rectangle(frame, solution, Scalar(255, 0, 0), 2, 8, 0);



        x = (int) (solution.x + solution.x + solution.width) / 2;
        y = (int) (solution.y + solution.y + solution.height) / 2;
        
        mouse.setMouse(x / (double) frame.cols * res.first, y / (double) frame.rows * res.second,3);
        mouse.drawArc(x / (double) frame.cols * res.first, y / (double) frame.rows * res.second, 3);
       

        imshow("result3", searchRegion);
        imshow("result", frame);
        imshow("output", imgTemplate);

        key = waitKey(1);

        if (key == 'q')
            break;
        switch (key) {
            case 'r':
                imgTemplate = rawTemplate.clone();
                searchRegion = frame.clone();
                break;
        }

    }

    return 0;
}


