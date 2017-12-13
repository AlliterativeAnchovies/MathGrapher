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

SDL_Surface* createBlankSurfaceWithSize(int sx,int sy);
void drawLineOnSurface(SDL_Surface* theSurface,double x1,double y1,double x2,double y2,Uint32 color);
Uint32 get_pixel32(SDL_Surface *surface, int x, int y);
void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel);
void drawGraphic(SDL_Renderer* gRenderer,int px,int py,int wid,int hei,SDL_Texture* graph,int angle=0);
void drawRect(SDL_Renderer* gRenderer,int px,int py,int wid,int hei, Uint32 color);
void drawLineThroughPointWithAngleInBounds(SDL_Surface* surface, double px,double py,double angle,
                                        double xbound_low,double xbound_high,double ybound_low,double ybound_high,
                                        Uint32 color=0xff000000);
bool pointInBounds(int px,int py,int bx,int bxf,int by,int byf);

template<typename T> void fastSineCosine(T* sine,T* cosine,T angle) {
    //It's faster if I need a sine and cosine of 1 angle to use this
    //function than to calculate them individually
    *sine = sin(angle);
    *cosine = sqrt(1-((*sine)*(*sine)))*((angle>M_PI/2&&angle<3*M_PI/2)?-1:1);
}


#endif /* RenderingUtilities_hpp */
