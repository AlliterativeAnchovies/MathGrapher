//
//  RenderingUtilities.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 13/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef RenderingUtilities_hpp
#define RenderingUtilities_hpp

#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"
#include <iostream>
#include <vector>

//My color palet:
//0xff9fc9f2 - nice blue
//0xffffcf9e - nice light brown
//0xffbd854d - nice darker brown
//0xffaaf2aa - nice hover over border highlights
//0x2200ff00 - nice green highlight
//0xff597bf5 - nice darker blue


//Graphics drawing variables
extern SDL_Window* gWindow;
extern SDL_Surface* gScreenSurface;
extern SDL_Renderer* gRenderer;
extern std::string dumstupidcurrentdirectorybs;
class Font;
extern Font* fontgrab;
class Popup;
extern std::vector<Popup*> popups;
//Screen dimension constants
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
//screen reading constants
extern std::string instring;
extern void* thingForInString;
extern int instringswitch;
extern int ticks;

class Font {
    private:
        std::vector<TTF_Font*> fonts;
    public:
        Font(int howmany);
        TTF_Font* operator() (int which);
};

SDL_Surface* createBlankSurfaceWithSize(int sx,int sy);
void drawLineOnSurface(SDL_Surface* theSurface,double x1,double y1,double x2,double y2,Uint32 color);
Uint32 get_pixel32(SDL_Surface *surface, int x, int y);
void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel);
void drawGraphic(int px,int py,int wid,int hei,SDL_Texture* graph,int angle=0);
void drawRect(int px,int py,int wid,int hei, Uint32 color);
void drawLineThroughPointWithAngleInBounds(SDL_Surface* surface, double px,double py,double angle,
                                        double xbound_low,double xbound_high,double ybound_low,double ybound_high,
                                        Uint32 color,double wiggleroom = 0);
bool pointInBounds(int px,int py,int bx,int bxf,int by,int byf);
void drawText(std::string text,int text_size,int x,int y, Uint32 color);
void drawTextWithBackground(std::string text,int text_size,int x,int y, Uint32 txtColor, Uint32 bgColor, Uint32 edgeColor);
Uint8 getA(Uint32 color);
Uint8 getR(Uint32 color);
Uint8 getG(Uint32 color);
Uint8 getB(Uint32 color);
void drawBorderedRect(int px,int py,int wid,int hei,Uint32 fillColor,Uint32 borderColor);
double numberFromString(std::string theString);
int hexCharToInt(char in);

template<typename T> void fastSineCosine(T* sine,T* cosine,T angle) {
    //It's faster if I need a sine and cosine of 1 angle to use this
    //function than to calculate them individually
    *sine = sin(angle);
    *cosine = sqrt(1-((*sine)*(*sine)))*((angle>M_PI/2&&angle<3*M_PI/2)?-1:1);
}
template<class A,class B,class C> C foldr(A f1,std::vector<B> vec,C first) {
    C output = first;
    for (int i = 0;i<vec.size();i++) {
        output = f1(vec[i],output);
    }
    return output;
}
template<class A,class B> B foldr1(A f1,std::vector<B> vec) {
    B output = vec[0];
    for (int i = 1;i<vec.size();i++) {
        output = f1(vec[i],output);
    }
    return output;
}
template<class A,class B> auto map(A toMap,std::vector<B> mapOnto) {
    auto toReturn = std::vector<decltype(toMap(mapOnto[0]))>(mapOnto.size());
    for (int i = 0;i<mapOnto.size();i++) {
        toReturn[i] = toMap(mapOnto[i]);
    }
    return toReturn;
}
template<class A,class B> auto mapWithIndex(A toMap,std::vector<B> mapOnto) {
    auto toReturn = std::vector<decltype(toMap(mapOnto[0],0))>(mapOnto.size());
    for (int i = 0;i<mapOnto.size();i++) {
        toReturn[i] = toMap(mapOnto[i],i);
    }
    return toReturn;
}

template<class A,class B> auto filter(A toFilter,std::vector<B> filterOnto) {
    std::vector<B> toReturn = {};
    for (B b : filterOnto) {
        if (toFilter(b)) {toReturn.push_back(b);}
    }
    return toReturn;
}

template<typename T> class Point {
    public:
        T x=0;
        T y=0;
        Point(T inx,T iny) {
            x = inx;
            y = iny;
        }
};

template<typename A,typename B> struct Wrap2 {
    A x;
    B y;
};


#endif /* RenderingUtilities_hpp */
