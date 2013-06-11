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
#include <cvblob.h>

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
int searchSize = 20;
Point p1, p2, p3, p4;
Mat imgTemplate, searchRegion, rawTemplate, rawSearch, tran;
Rect solution;
bool noHand = false;
int r = 51, g = 99, b = 57;
int rT = 90, gT = 255, bT = 129;
int maxArea = 10000, minArea = 500;
bool hideWindow = false;
IplImage *red = 0, *tmp = 0, * framee = 0, *labelImg = 0;
CvBlobs blobs;
CvTracks tracks;

void setR(int v) {
    r = v;
}

void setG(int v) {
    g = v;
}

void setB(int v) {
    b = v;
}

void setRT(int v) {
    rT = v;
}

void setGT(int v) {
    gT = v;
}

void setBT(int v) {
    bT = v;
}

void setMax(int v) {
    maxArea = v;
}

void setMin(int v) {
    minArea = v;
}

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

void setRegion(Point point,int frameX,int frameY) {
    int setRegionSize = 20;
    if ((p1 = point.x - setRegionSize) > frameX ) p1 = frameX;
    else
    if ((p1 = point.x - setRegionSize) < 0) p1 = 0;
    
    if ((p2 = point.y - setRegionSize) > frameY) p2 = frameY;
        else
    if ((p2 = point.y - setRegionSize) < 0) p2 = 0;


    p1 = Point(point.x - 20, point.y - 20);
    p2 = Point(point.x + 20, point.y + 20);


    Rect ra = Rect(p1, p2);
    imgTemplate = frame(ra);
    p3 = Point(p1.x - 30, p1.y - 30);
    p4 = Point(p2.x + 60, p2.y + 60);

    searchRegion = frame(Rect(p3, p4));
    rawTemplate = imgTemplate = imgTemplate.clone();
    rawSearch = searchRegion = searchRegion.clone();
    matchImage(searchRegion, imgTemplate);


    solution = Rect(Point(matchLoc.x + p3.x, matchLoc.y + p3.y), Point(matchLoc.x + imgTemplate.cols + p3.x, matchLoc.y +
            imgTemplate.rows + p3.y));
}

Point searchHand(Mat &img) {

    cvtColor(frame, tran, CV_BGR2HSV);

    tmp->imageData = (char*) tran.data;

    cvInRangeS(tmp, Scalar(r, g, b), Scalar(rT, gT, bT), red);
    cvSmooth(red, red, CV_BLUR, 3);
    cvSmooth(red, red, CV_MEDIAN, 3);
    cvDilate(red, red, NULL, 3);
    cvErode(red, red, NULL, 8);
    cvDilate(red, red, NULL, 2);
    imshow("cos", Mat(red));
    ////cvtColor(frame, frame, CV_RGB2GRAY);
    framee->imageData = (char*) img.data;
    cvLabel(red, labelImg, blobs);
    cvFilterByArea(blobs, minArea, maxArea);
    cvRenderBlobs(labelImg, blobs, framee, framee, CV_BLOB_RENDER_BOUNDING_BOX);
    cvUpdateTracks(blobs, tracks, 200, 5);
    cvRenderTracks(tracks, framee, framee, CV_TRACK_RENDER_ID | CV_TRACK_RENDER_BOUNDING_BOX);
    frame = Mat(framee);
    cout << "jestem" << endl;
    for (CvBlobs::const_iterator it = blobs.begin(); it != blobs.end(); ++it) {
        int x = (int) it->second->centroid.x;
        int y = (int) it->second->centroid.y;
        return Point(x, y);
    }
    return Point(0, 0);
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

    cvCreateTrackbar("R", "bin", &r, 255, setR);
    cvCreateTrackbar("G", "bin", &g, 255, setG);
    cvCreateTrackbar("B", "bin", &b, 255, setB);

    cvCreateTrackbar("RT", "bin", &rT, 255, setRT);
    cvCreateTrackbar("BT", "bin", &bT, 255, setBT);
    cvCreateTrackbar("GT", "bin", &gT, 255, setGT);

    cvCreateTrackbar("min", "bin", &minArea, 1000, setMin);
    cvCreateTrackbar("max", "bin", &maxArea, 100000, setMax);

    cap >> frame;
    int frameX = frame.cols, frameY = frame.rows;
    flip(frame, frame, 1);
    red = cvCreateImage(cvSize(frame.cols, frame.rows), IPL_DEPTH_8U, 1);
    tmp = cvCreateImage(cvSize(frame.cols, frame.rows), IPL_DEPTH_8U, 3);
    labelImg = cvCreateImage(cvGetSize(red), IPL_DEPTH_LABEL, 1);
    framee = cvCreateImage(cvGetSize(red), 8, 3);


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

    setRegion(point);
    char key = -1;
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


        if ((x = solution.x - searchSize) <= 0)
            x = 0;
        else
            if ((x = solution.x - searchSize) >= frameX)
            x = frameX;
        if ((y = solution.y - searchSize) <= 0)
            y = 0;
        else
            if ((y = solution.y - searchSize) >= frameY)
            y = frameY;
        if ((x1 = solution.x + solution.width + searchSize) <= 0)
            x1 = 0;
        else
            if ((x1 = solution.x + solution.width + searchSize) >= frameX)
            x1 = frameX;
        if ((y1 = solution.y + solution.height + searchSize) <= 0)
            y1 = 0;
        else
            if ((y1 = solution.y + solution.height + searchSize) >= frameY)
            y1 = frameY;


        p3 = Point(x, y);
        p4 = Point(x1, y1);
        if ((solution.x + solution.width == frameX || solution.y + solution.height ==
                frameY || solution.x == 0 || solution.y == 0)) {
            Point center = searchHand(frame);
            setRegion(center);
        }
        rectangle(frame, p3, p4, Scalar(0, 0, 255), 2, 8, 0);
        rectangle(frame, solution, Scalar(255, 0, 0), 2, 8, 0);



        x = (int) (solution.x + solution.x + solution.width) / 2;
        y = (int) (solution.y + solution.y + solution.height) / 2;

        //mouse.setMouse(x / (double) frame.cols * res.first, y / (double) frame.rows * res.second, 3);
        //mouse.drawArc(x / (double) frame.cols * res.first, y / (double) frame.rows * res.second, 3);


        imshow("result3", searchRegion);
        imshow("result", frame);
        imshow("output", imgTemplate);
        imshow("bin", Mat(red));

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


