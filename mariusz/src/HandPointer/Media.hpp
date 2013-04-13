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

class Media {
public:
    Media();
    Media(const Media& orig);
    void setMouse(const int,const int);
    std::pair<int,int> getResolution();
    virtual ~Media();
private:
    Display *dpy;
    Window root_window;

};

#endif	/* MOUSE_HPP */

