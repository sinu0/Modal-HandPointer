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
    
    int x, x1, y, y1;
    int region = 20;
    int pointerSpeed = 3;
    bool pointerOn = true,mouseOn = true;
    //   W   H
    pair<int, int> res = mouse.getResolution();



    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded
        return 0;
    namedWindow("result", 1);
    namedWindow("output", 1);
    namedWindow("bin", 1);
    cap >> frame;
    int frameX = frame.cols, frameY = frame.rows;
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

    Point p1 = Point(point.x - 10, point.y - 10);
    Point p2 = Point(point.x + 10, point.y + 10);

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

        p1.x = matchLoc.x;
        p1.y = matchLoc.y;

        p2.x = matchLoc.x + imgTemplate.cols;
        p2.y = matchLoc.y + imgTemplate.rows;

        imgTemplate = searchRegion.clone();
        imgTemplate = imgTemplate(Rect(p1, p2));


        searchRegion = frame.clone();
        searchRegion = searchRegion(Rect(p3, p4));

        matchImage(searchRegion, imgTemplate);

        solution = Rect(Point(matchLoc.x + p3.x, matchLoc.y + p3.y), Point(matchLoc.x + imgTemplate.cols + p3.x, matchLoc.y +
                imgTemplate.rows + p3.y));
        //solution.Rect_(p1.x + p3.x, p1.y + p3.x, p2.x + p3.x, p2.y + p3.y);

        x = solution.x - region;
        y = solution.y - region;
        x1 = solution.x + solution.width + region;
        y1 = solution.y + solution.height + region;

        if (x <= 0)
            x = 0;
        else
            if (x >= frameX)
            x = frameX;
        if (y <= 0)
            y = 0;
        else
            if (y >= frameY)
            y = frameY;
        if (x1 <= 0)
            x1 = 0;
        else
            if (x1 >= frameX)
            x1 = frameX;
        if (y1 <= 0)
            y1 = 0;
        else
            if (y1 >= frameY)
            y1 = frameY;


        p3.x = x;
        p3.y = y;
        p4.x = x1;
        p4.y = y1;

        rectangle(frame, p3, p4, Scalar(0, 0, 255), 2, 8, 0);
        rectangle(frame, solution, Scalar(255, 0, 0), 2, 8, 0);



        x = (int) (solution.x + solution.x + solution.width) / 2;
        y = (int) (solution.y + solution.y + solution.height) / 2;
        if(mouseOn)
                mouse.setMouse(x / (double) frame.cols * res.first, y / (double) frame.rows * res.second, pointerSpeed);
        if(pointerOn)
                mouse.drawArc(x / (double) frame.cols * res.first, y / (double) frame.rows * res.second, pointerSpeed);


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
            case 'p':
                mouseOn = mouseOn ? false : true;
                break;
            case 'm':
                pointerOn = pointerOn ? false : true;
                break;
        }

    }

    return 0;
}


