/* 
 * File:   mouse.cpp
 * Author: mar
 * 
 * Created on 13 kwiecień 2013, 15:38
 */

#include "Media.hpp"

Media::Media() {
    dpy = XOpenDisplay(0);
    root_window = XRootWindow(dpy, 0);
    XSelectInput(dpy, root_window, KeyReleaseMask);
    
}
void Media::setMouse(const int x,const int y){
    XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x, y);
    XFlush(dpy);
}
std::pair<int,int> Media::getResolution(){
    
        std::pair<int,int> resoluton;
        
        resoluton.first = XDisplayHeight(dpy,0);
        resoluton.second= XDisplayWidth(dpy,0);
        return resoluton;
        
}
Media::Media(const Media& orig) {
}

Media::~Media() {
    //free(&dpy);
}

