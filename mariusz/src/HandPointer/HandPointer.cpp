/* 
 * File:   main.cpp
 * Author: mar
 *
 * Created on 7 kwiecień 2013, 19:36
 */


#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <iostream>
#include <stdio.h> 
#include <assert.h>   
#include <unistd.h>  
#include <cvblob.h>
#include "opencv2/opencv.hpp"
#include <opencv/cv.h>
#include "Media.hpp"
using namespace cv;
using namespace std;
using namespace cvb;

int r = 51, g = 99, b = 57;
int rT = 90, gT = 255, bT = 129;
int maxArea=1000, minArea=170;
 
void setR(int v)  {r = v;}
void setG(int v)  {g = v;} 
void setB(int v)  {b = v;}
void setRT(int v) {rT = v;}
void setGT(int v) {gT = v;}
void setBT(int v) {bT = v;}


void setMax(int v){maxArea = v;}
void setMin(int v){minArea = v;} 

int main(int, char**) {
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded
        return -1;

    Media mouse;
    bool mouseon = true;
    int x, y;
    //   W   H
    pair<int,int> res = mouse.getResolution();
    
    namedWindow("output", 1);
    namedWindow("bin", 1);
    
    CvBlobs blobs;
    CvTracks tracks;
    
    IplImage *red = 0, *tmp = 0,* framee=0,*labelImg=0;
    
    Mat edges, frame;
    
    cap >> frame;
    
    red =       cvCreateImage(cvSize(frame.cols, frame.rows), IPL_DEPTH_8U, 1);
    tmp =       cvCreateImage(cvSize(frame.cols, frame.rows), IPL_DEPTH_8U, 3);
    labelImg =  cvCreateImage(cvGetSize(red), IPL_DEPTH_LABEL, 1);
    framee=     cvCreateImage(cvGetSize(red), 8, 3);
    
    cvCreateTrackbar("R","bin",&r,255,setR);
    cvCreateTrackbar("G","bin",&g,255,setG);
    cvCreateTrackbar("B","bin",&b,255,setB);
    
    cvCreateTrackbar("RT","bin",&rT,255,setRT);
    cvCreateTrackbar("BT","bin",&bT,255,setBT);
    cvCreateTrackbar("GT","bin",&gT,255,setGT);
   
    cvCreateTrackbar("min","bin",&minArea,1000,setMin);
    cvCreateTrackbar("max","bin",&maxArea,100000,setMax);

    

    for (;;) {
        cap >> frame; // gt a new frame from camera
        flip(frame, frame, 1);
        cvtColor(frame, edges, CV_BGR2HSV);

        tmp->imageData = (char*) edges.data;
        framee->imageData = (char*) frame.data;
        
        cvInRangeS(tmp, Scalar(r, g, b), Scalar(rT, gT, bT), red);
        
        cvDilate(red, red, NULL, 3);
        cvErode(red, red, NULL, 4);
        cvDilate(red, red, NULL, 2);
        
        cvLabel(red, labelImg, blobs);
        cvFilterByArea(blobs, minArea, maxArea);
        cvRenderBlobs(labelImg, blobs, framee, framee, CV_BLOB_RENDER_BOUNDING_BOX);
        cvUpdateTracks(blobs, tracks, 200, 5);
        cvRenderTracks(tracks, framee, framee, CV_TRACK_RENDER_ID | CV_TRACK_RENDER_BOUNDING_BOX);
        
        for (CvBlobs::const_iterator it = blobs.begin(); it != blobs.end(); ++it) {
            x = (int) it->second->centroid.x;
            y = (int) it->second->centroid.y;
            if (mouseon) {
                mouse.setMouse(x/(double)frame.cols*res.second,y/(double)frame.rows*res.first);
            }
            break;
        }


        imshow("output", Mat(framee));
        imshow("bin", Mat(red));
        
        char key = (char) waitKey(1);
        switch (key) {

            case 'a':
                mouseon = mouseon ? false : true;
                break;
        }
        if (key == 'b') break; //exit program

    }

    destroyAllWindows();
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

