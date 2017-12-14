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


void drawLineThroughPointWithAngleInBounds(SDL_Surface* surface, double px,double py,double angle,
                                        double xbound_low,double xbound_high,double ybound_low,double ybound_high,
                                        Uint32 color,double wiggleroom) {
    double deltax = 0;
    double deltay = 0;
    fastSineCosine(&deltay, &deltax, angle);
    double totalOffsetX = 0;
    double totalOffsetY = 0;
    bool firstInBounds = true;
    bool secondInBounds = true;
    while (firstInBounds||secondInBounds) {
        firstInBounds = pointInBounds(px+totalOffsetX,py+totalOffsetY,
                xbound_low-wiggleroom,xbound_high+wiggleroom,ybound_low-wiggleroom,ybound_high+wiggleroom);
        secondInBounds = pointInBounds(px-totalOffsetX,py-totalOffsetY,
                xbound_low-wiggleroom,xbound_high+wiggleroom,ybound_low-wiggleroom,ybound_high+wiggleroom);
        if (firstInBounds) {
            if (py+totalOffsetY>=0&&py+totalOffsetY<surface->h&&px+totalOffsetX>=0&&px+totalOffsetX<surface->w) {
                put_pixel32(surface, px+totalOffsetX, py+totalOffsetY, color);
            }
        }
        if (secondInBounds) {
            if (py-totalOffsetY>=0&&py-totalOffsetY<surface->h&&px-totalOffsetX>=0&&px-totalOffsetX<surface->w) {
                put_pixel32(surface, px-totalOffsetX, py-totalOffsetY, color);
            }
        }
        totalOffsetX+=deltax;
        totalOffsetY-=deltay;
    }
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
    else if (y1==y2) {
        //horizontal line, have to handle differently
        double startx = (x1>x2)?x2:x1;
        double endx = (x1>x2)?x1:x2;
        for (int i = startx;i<=endx;i++) {
            if (y1>=0&&y1<theSurface->h&&i>=0&&i<theSurface->w) {
                put_pixel32(theSurface, i, y1, color);
            }
        }
        return;
    }
    
    /*//make x1<x2,y1<y2
    int t = x1;
    x1 = (x1<x2)?x1:x2;
    x2 = (t<x2)?x2:t;
    t = y1;
    y1 = (y1<y2)?y1:y2;
    y2 = (t<y2)?y2:t;
    
    //cap ends to boundaries of surface
    double m = (y2-y1)/(x2-x1);
    //point slope formula: y = m(x-x1)+y1
    //                     x = (1/m)(y-y1)+x1
    double leftWallIntersect = m*(0-x1)+y1;
    double topWallIntersect = (1/m)*(0-y1)+x1;
    double rightWallIntersect = m*(theSurface->w-x1)+y1;
    double bottomWallIntersect = (1/m)*(theSurface->h-y1)+x1;
    x1 = (x1>leftWallIntersect)?x1:leftWallIntersect;
    x2 = (x2<rightWallIntersect)?x2:rightWallIntersect;
    y1 = (y1>topWallIntersect)?y1:topWallIntersect;
    y2 = (y2<bottomWallIntersect)?y2:bottomWallIntersect;*/

    double x = x2 - x1;
    double y = y2 - y1;
    double length = sqrt( x*x + y*y );
    
    if (length > 100000) {
        //give up, wtf is this???
        return;
    }

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

bool pointInBounds(int px,int py,int bx,int bxf,int by,int byf) {
    if (px>=bx&&px<=bxf&&py>=by&&py<=byf) {
        return true;
    }
    return false;
}
