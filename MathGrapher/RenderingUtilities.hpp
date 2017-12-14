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

//Some functions that are needed to be declared here to interface well with main.cpp
//they are defined in main
void addGraph(double x,double y);

//Graphics drawing variables
extern SDL_Window* gWindow;
extern SDL_Surface* gScreenSurface;
extern SDL_Renderer* gRenderer;
extern std::string dumstupidcurrentdirectorybs;
class Font;
extern Font* fontgrab;
class Popup;
extern std::vector<Popup*> popups;

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


template<typename T> void fastSineCosine(T* sine,T* cosine,T angle) {
    //It's faster if I need a sine and cosine of 1 angle to use this
    //function than to calculate them individually
    *sine = sin(angle);
    *cosine = sqrt(1-((*sine)*(*sine)))*((angle>M_PI/2&&angle<3*M_PI/2)?-1:1);
}

enum POPUP_IDS {
    NULL_POPUP,
    ADD_OBJECT_POPUP
};

class Popup {
    private:
        Uint8 popupID = NULL_POPUP;
        double px = 0;
        double py = 0;
        double sx = 0;
        double sy = 0;
        bool taggedForDeletion = false;
    public:
        Popup(Uint8 popup_id,double x,double y,double xsize,double ysize);
        Uint8 handle(double mouseX,double mouseY,bool clicked);
        void tag();
        bool isTagged();
        Uint8 getID();
};

void createPopup(Uint8 popup_id,double x,double y);
bool isQuickCloser(Uint8 popup_id);

#endif /* RenderingUtilities_hpp */
