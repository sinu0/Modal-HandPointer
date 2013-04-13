/* 
 * File:   mouse.hpp
 * Author: mar
 *
 * Created on 13 kwiecień 2013, 15:38
 */

#ifndef MOUSE_HPP
#define	MOUSE_HPP
#include <X11/Xlib.h>
class Mouse {
public:
    Mouse();
    Mouse(const Mouse& orig);
    void setMouse(const int,const int);
    virtual ~Mouse();
private:
    Display *dpy;
    Window root_window;

};

#endif	/* MOUSE_HPP */

