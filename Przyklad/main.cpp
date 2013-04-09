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

#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>  

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
    int x,y = 0;
    const int maxY = 900;
    const int maxX = 1600;
    Display *dpy;
    Window root_window;

    dpy = XOpenDisplay(0);
    root_window = XRootWindow(dpy, 0);
    XSelectInput(dpy, root_window, KeyReleaseMask);

    XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, 100, 100);

    XFlush(dpy);
    for(;;) {    
        cap >> frame; // gt a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
        equalizeHist(edges,edges);
        GaussianBlur(edges, edges, Size(13,13), 2, 2);
        Canny(edges, edges, 0,50, 3);
        flip(edges,edges,1);
        imshow("edges", edges);
        XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x++, y++);
        if (x > maxX) x=0;
        if (y > maxY) y=0;
        XFlush(dpy);
        if(waitKey(30) >= 0) break;
    }                    
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

