/* 
 * File:   mouse.hpp
 * Author: mar
 *
 * Created on 13 kwiecień 2013, 15:38
 */

#ifndef MOUSE_HPP
#define	MOUSE_HPP
#include <X11/Xlib.h>
#include <iostream> 
#include <X11/Xutil.h>
#include <X11/extensions/Xdbe.h>

class Media {
public:
    Media();
    Media(const Media& orig);
    void setMouse(const int,const int,double);
    void setPointer(const int,const int);
    std::pair<int,int> getResolution();
    void drawArc(int,int,double);
    void draw();
    virtual ~Media();
private:
    Display *dpy;
    Window root_window;
    GC gc;
    unsigned long valuemask;
    int screen;
    int xM;
    int yM;
    int xR;
    int yR;
    Pixmap double_buffer;
    int currXR;
    int currYR;
    int currXM;
    int currYM;
};

#endif	/* MOUSE_HPP */

