//
//  RenderingUtilities.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 13/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "RenderingUtilities.hpp"

Uint32 get_pixel32(SDL_Surface *surface, int x, int y) {
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

SDL_Surface* createBlankSurfaceWithSize(int sx,int sy) {
    return SDL_CreateRGBSurface(NULL, sx, sy, 32,0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
}

void drawLineOnSurface(SDL_Surface* theSurface,double x1,double y1,double x2,double y2,Uint32 color) {
    if (x1==x2) {
        //vertical line, have to handle differently
        double starty = (y1>y2)?y2:y1;
        double endy = (y1>y2)?y1:y2;
        for (int i = starty;i<=endy;i++) {
            if (i>=0&&i<theSurface->h&&x1>=0&&x1<theSurface->w) {
                put_pixel32(theSurface, x1, i, color);
            }
        }
        return;
    }

    double x = x2 - x1;
    double y = y2 - y1;
    double length = sqrt( x*x + y*y );

    double addx = x / length;
    double addy = y / length;

    x = x1;
    y = y1;
    for(double i = 0; i < length; i += 1) {
        if (y>=0&&y<theSurface->h&&x>=0&&x<theSurface->w) {
            put_pixel32( theSurface, x, y, color );
        }
        x += addx;
        y += addy;
    }
}

void drawGraphic(SDL_Renderer* gRenderer,int px,int py,int wid,int hei,SDL_Texture* graph,int angle) {
    SDL_Rect stretchRect;
    stretchRect.x = px;
    stretchRect.y = py;
    stretchRect.w = wid;
    stretchRect.h = hei;
    SDL_RenderCopyEx(gRenderer,graph,NULL,&stretchRect,angle,NULL,SDL_FLIP_NONE);
}

void drawRect(SDL_Renderer* gRenderer,int px,int py,int wid,int hei, Uint32 color) {
    SDL_Rect stretchRect;
    stretchRect.x = px;
    stretchRect.y = py;
    stretchRect.w = wid;
    stretchRect.h = hei;
    Uint8 r = (color<<8)>>24;
    Uint8 g = (color<<16)>>24;
    Uint8 b = (color<<24)>>24;
    Uint8 a = color>>24;
    SDL_SetRenderDrawColor(gRenderer,r,g,b,a);
    SDL_RenderFillRect(gRenderer,&stretchRect);
}
