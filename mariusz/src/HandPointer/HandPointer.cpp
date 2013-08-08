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
Point point; //punkt klikniecia mysza
Point matchLoc; //znaleźiony obszar przez matchtemplate
int drag = 0; 
int key = 0;
CvRect rect; //prostokat po nacisnieciu myszka ?
bool done; //pierwszy etap skonczyony uzytkownik klikna myszka na zadany obszar na kltce

void mouseHandler(int event, int x, int y, int flags, void* param) {
    if (event == CV_EVENT_LBUTTONUP) {
        point = Point(x, y);
        done = true;
    }
}

void matchImage(Mat &img1, Mat &img2) {
    Mat imgOutput;
    int row = img1.rows - img2.rows + 1; //wyliczenie ilosci pikseli w poziomie i w pionie
    int col = img1.cols - img2.cols + 1;
    imgOutput.create(col, row, CV_32FC1); // stworzenie nowej kopii danego wycinka na ktorym bedzie szykany wzorzec
    matchTemplate(img1, img2, imgOutput, CV_TM_CCOEFF_NORMED); 
    normalize(imgOutput, imgOutput, 0, 1, NORM_MINMAX, -1, Mat()); // to nie wiem po co jest
    Point minLoc, maxLoc; // w zaleznosci od uzytej funkcji w matchTemplate zostanie uzyta pierwsz albo druga zmienna patrz dokumentacja w opencv
    double minVal, maxVal; 
    minMaxLoc(imgOutput, &minVal, &maxVal, &minLoc, &maxLoc, Mat()); //wyznaczenie wspolrzednych na podstawie obrazu przychodzacego oraz wyliczonych danych
    matchLoc = maxLoc;

}

int main(int, char**) {

    Media mouse;
    
    int x, x1, y, y1; //punkty kotre beada tworzyc obwódkę ktora wskazuje na obszar prztwarzania
    int region = 20; // to chyba niepotrzebne
    int pointerSpeed = 3; //prszyspieszenie myszki x3
    bool pointerOn = true,mouseOn = true; //sterowanie myszka, pokazywanie pointera on/off
    //   W   H
    pair<int, int> res = mouse.getResolution();



    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded
        return 0;
    namedWindow("result", 1);
    namedWindow("output", 1);
    namedWindow("bin", 1);
    cap >> frame;
    int frameX = frame.cols, frameY = frame.rows; //rozdzielczosc kamerki
    flip(frame, frame, 1);


    Mat rawTemplate, rawSearch;
    while (!done) {

        cap >> frame;
        flip(frame, frame, 1); //odbicie lustrzane klatki (patrzac na monitor naturalnie sie porusza reka)
        imshow("result", frame);
        if (rect.width > 0)
            frame = frame(rect); // tego nie wiem musisz sobie sprawdzic
        setMouseCallback("result", mouseHandler, NULL); //przchwycenie eventu z myszki
        imshow("result", frame);
        cvWaitKey(10);
    }

    Point p1 = Point(point.x - 10, point.y - 10); //wyznaczenie wzorca o boku 20px
    Point p2 = Point(point.x + 10, point.y + 10);

    Rect ra = Rect(p1, p2);
    Mat imgTemplate = frame(ra);

    Point p3 = Point(p1.x - 30, p1.y - 30);//wyznaczenie obszaru do obrobki czyli bedzie to kwadrat gdzie w srodku znajduje startowy tamplate
    Point p4 = Point(p2.x + 60, p2.y + 60);

    Mat searchRegion = frame(Rect(p3, p4)); //wycinek obrazu na ktorym szukamy
    rawTemplate = imgTemplate = imgTemplate.clone(); //kopia wzorca
    rawSearch = searchRegion = searchRegion.clone(); //kopia obszaru
    matchImage(searchRegion, imgTemplate); // szukaj wzorca

    char key = -1;
    Rect solution = Rect(Point(matchLoc.x + p3.x, matchLoc.y + p3.y), Point(matchLoc.x + imgTemplate.cols + p3.x, matchLoc.y +
            imgTemplate.rows + p3.y)); //wylicz pozycje znaleźionego wzorca na podstawie duzego wycinka
/*    
(0,0)        frame.x
    ---------------------------
    |   p3(p1-30)             |
    |    |----------|         |f
    |    |   p1     |         |r
    |    |   |--|   |         |a
    |    |   |--|p2 |         |m
    |    |--------- |p4(p2+60)|e
    |                         |.
    |                         |y
    |                         |
    |                         |
    |-------------------------|
    
*/
    
    while (true) {
        cap >> frame;
        flip(frame, frame, 1);

        p1.x = matchLoc.x; //wspolzedne templata
        p1.y = matchLoc.y;

        p2.x = matchLoc.x + imgTemplate.cols; // druga para pobieram szerokosc i wysoksc tmaplata poniewaz chce miec taki sam obszar
        p2.y = matchLoc.y + imgTemplate.rows;

        imgTemplate = searchRegion.clone();//kopuje caly wycinek z obszaru poszukiwac 
        imgTemplate = imgTemplate(Rect(p1, p2)); // przycinam go do zadanych rozmiarow trzeba tak zrobic aby nie stracic obrzu z searchRegion 


        searchRegion = frame.clone(); //analogicznie jak wyzej
        searchRegion = searchRegion(Rect(p3, p4));

        matchImage(searchRegion, imgTemplate); // poszukuj nowego wzorca

        solution = Rect(Point(matchLoc.x + p3.x, matchLoc.y + p3.y), Point(matchLoc.x + imgTemplate.cols + p3.x, matchLoc.y +
                imgTemplate.rows + p3.y)); //jak wyzej 
        //solution.Rect_(p1.x + p3.x, p1.y + p3.x, p2.x + p3.x, p2.y + p3.y);

        /*
        * sprawdzanie czy rectangle ktory sie rysuje na obrazie nie wychodzi po zaniego efekt widoczny podczas dzilania programu
        * wodac jak niebieski kwadrat plywa przy krawedziach kaltaki oraz czerwnoy kwadrat sie automatycznie zmniejsza jezeli jest blizej krawedzi
        * co powoduje ze program nie lata po niewiadomej pamieci i nie sypier errorami :D
        */
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


        p3.x = x; //przypisanie do starych wartosci w taki sposob jest wygodniej bo dziala a podajac sama wspolrzedne mialem problem z wykmineniem kolejnosci wiec mi sie niechcialo tego robic no i jest czytelniej
        p3.y = y;
        p4.x = x1;
        p4.y = y1;

        rectangle(frame, p3, p4, Scalar(0, 0, 255), 2, 8, 0); //rysowanie na frame , scalar to jest wybor koloru , nastepne argumenty to jakies tam sytle i grubosci lini
        rectangle(frame, solution, Scalar(255, 0, 0), 2, 8, 0);



        x = (int) (solution.x + solution.x + solution.width) / 2; //srodka znalexionego wzorca 
        y = (int) (solution.y + solution.y + solution.height) / 2;
        if(mouseOn)
                mouse.setMouse(x / (double) frame.cols * res.first, y / (double) frame.rows * res.second, pointerSpeed);
        if(pointerOn)
                mouse.drawArc(x / (double) frame.cols * res.first, y / (double) frame.rows * res.second, pointerSpeed);


        imshow("result3", searchRegion); //prezetacja wynikow
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


