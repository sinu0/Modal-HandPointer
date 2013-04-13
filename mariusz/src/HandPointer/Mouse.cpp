/* 
 * File:   mouse.cpp
 * Author: mar
 * 
 * Created on 13 kwiecień 2013, 15:38
 */

#include "Mouse.hpp"

Mouse::Mouse() {
    dpy = XOpenDisplay(0);
    root_window = XRootWindow(dpy, 0);
    XSelectInput(dpy, root_window, KeyReleaseMask);
    
}
void Mouse::setMouse(const int x,const int y){
    XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x, y);
    XFlush(dpy);
}
Mouse::Mouse(const Mouse& orig) {
}

Mouse::~Mouse() {
    //free(&dpy);
}

