/* 
 * File:   mouse.cpp
 * Author: mar
 * 
 * Created on 13 kwiecień 2013, 15:38
 */

#include "Media.hpp"

Media::Media() {
    dpy = XOpenDisplay(0);
    if (dpy == NULL) {
        std::cout << "Err:";
        return;
    }
    XColor colorval;
    Colormap colormap;
    XGCValues gc_val;
    XWindowAttributes wa;

    char color[] = "#FF0000";

    screen = DefaultScreen(dpy);
    root_window = DefaultRootWindow(dpy);
    XGetWindowAttributes(dpy, root_window, &wa);
    gc_val.function = GXxor;
    gc_val.plane_mask = AllPlanes;
    gc_val.foreground = WhitePixel(dpy, screen);
    gc_val.background = BlackPixel(dpy, screen);
    gc_val.line_width = 4;
    gc_val.line_style = LineSolid;
    gc_val.cap_style = CapButt;
    gc_val.join_style = JoinMiter;
    gc_val.fill_style = FillOpaqueStippled;
    gc_val.fill_rule = WindingRule;
    gc_val.graphics_exposures = False;
    gc_val.clip_x_origin = 0;
    gc_val.clip_y_origin = 0;
    gc_val.clip_mask = None;
    gc_val.subwindow_mode = IncludeInferiors;

    colormap = DefaultColormap(dpy, 0);
    XMapWindow(dpy, root_window);

    gc = XCreateGC(dpy, root_window, GCFunction | GCPlaneMask | GCForeground |
            GCBackground | GCLineWidth | GCLineStyle |
            GCCapStyle | GCJoinStyle | GCFillStyle |
            GCFillRule | GCGraphicsExposures |
            GCClipXOrigin | GCClipYOrigin | GCClipMask |
            GCSubwindowMode, &gc_val);
    double_buffer = XCreatePixmap(dpy, root_window,
            wa.width, wa.height, wa.depth);
    XParseColor(dpy, colormap, color, &colorval);
    XAllocColor(dpy, colormap, &colorval);
    XSetForeground(dpy, gc, colorval.pixel);
    XFillRectangle(dpy, double_buffer, gc, 0, 0, wa.width, wa.height);
    currXM = 1600 / 2;
    currYM = 900 / 2;
    currXR = 1600 / 2;
    currYR = 900 / 2;
}

void Media::setMouse(const int _x, const int _y, double speed) {


    currXM -= speed * (xM - _x);
    currYM -= speed * (yM - _y);
    if (currXM > 1600) currXM = 1600;
    if (currXM < 0) currXM = 0;
    if (currYM > 900) currYM = 900;
    if (currYM < 0) currYM = 0;
    xM = _x;
    yM = _y;
    XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, currXM, currYM);

    XFlush(dpy);
}

std::pair<int, int> Media::getResolution() {
    std::pair<int, int> resoluton;
    resoluton.first = XDisplayHeight(dpy, 0);
    resoluton.second = XDisplayWidth(dpy, 0);
    return resoluton;

}

void Media::drawArc(int _x, int _y, double speed) {



    
    draw();
    currXR -= speed * (xR - _x);
    currYR -= speed * (yR - _y);
    if (currXR > 1600) currXR = 1600;
    if (currXR < 0) currXR = 0;
    if (currYR > 900) currYR = 900;
    if (currYR < 0) currYR = 0;
    xR = _x;
    yR = _y;
    draw();
    XFlush(dpy);
}

void Media::draw() {

    XFillRectangle(dpy, root_window, gc, currXR, currYR, 50, 50);
}

Media::Media(const Media & orig) {
}

Media::~Media() {
    //free(&dpy);
}
