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

void drawGraphic(int px,int py,int wid,int hei,SDL_Texture* graph,int angle) {
    SDL_Rect stretchRect;
    stretchRect.x = px;
    stretchRect.y = py;
    stretchRect.w = wid;
    stretchRect.h = hei;
    SDL_RenderCopyEx(gRenderer,graph,NULL,&stretchRect,angle,NULL,SDL_FLIP_NONE);
}

void drawRect(int px,int py,int wid,int hei, Uint32 color) {
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

void drawText(std::string text,int text_size,int x,int y, Uint32 color) {
    if (text=="") {
        return;//Why draw if not drawing anything?
    }
    SDL_Color textColor = {getR(color),getG(color),getB(color)};
    SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid((*fontgrab)(text_size),text.c_str(),textColor);
    if(surfaceMessage == NULL) {
        printf("Unable to render text surface: %s\n",TTF_GetError());
        throw std::runtime_error("Error unable to render text surface!");
    }
    SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer,surfaceMessage);
    int text_width = surfaceMessage->w;
    int text_height = surfaceMessage->h;
    drawGraphic(x, y, text_width, text_height, message);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
    message=NULL;
    surfaceMessage=NULL;
}

void drawTextWithBackground(std::string text,int text_size,int x,int y, Uint32 txtColor, Uint32 bgColor, Uint32 edgeColor) {
    if (text=="") {
        return;//Why draw if not drawing anything?
    }
    SDL_Color textColor = {getR(txtColor),getG(txtColor),getB(txtColor)};
    SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid((*fontgrab)(text_size),text.c_str(),textColor);
    if(surfaceMessage == NULL) {
        printf("Unable to render text surface: %s\n",TTF_GetError());
    }
    SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer,surfaceMessage);
    int text_width = surfaceMessage->w;
    int text_height = surfaceMessage->h;
    drawBorderedRect(x, y, text_width, text_height, bgColor, edgeColor);
    drawGraphic(x, y, text_width, text_height, message);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
    message=NULL;
    surfaceMessage=NULL;
}

Uint8 getA(Uint32 color) {
    return (color&0xFF000000)>>24;
}

Uint8 getR(Uint32 color) {
    return (color&0x00FF0000)>>16;
}

Uint8 getG(Uint32 color) {
    return (color&0x0000FF00)>>8;
}
Uint8 getB(Uint32 color) {
    return (color&0x000000FF);
}

void drawBorderedRect(int px,int py,int wid,int hei,Uint32 fillColor,Uint32 borderColor) {
    SDL_Rect stretchRect;
    stretchRect.x = px;
    stretchRect.y = py;
    stretchRect.w = wid;
    stretchRect.h = hei;
    SDL_SetRenderDrawColor(gRenderer,getR(fillColor),getG(fillColor),getB(fillColor),getA(fillColor));
    SDL_RenderFillRect(gRenderer,&stretchRect);
    SDL_SetRenderDrawColor(gRenderer,getR(borderColor),getG(borderColor),getB(borderColor),getA(borderColor));
    SDL_RenderDrawRect(gRenderer,&stretchRect);
}

Font::Font(int howmany) {
    fonts = std::vector<TTF_Font*>(howmany,NULL);
}
TTF_Font* Font::operator() (int which) {
    if (fonts.size()>which) {
        if (fonts[which]==NULL) {
            fonts[which] = TTF_OpenFont((dumstupidcurrentdirectorybs+"/resources/Fonts/Times New Roman.ttf").c_str(), which);
        }
        return fonts[which];
    }
    fonts.resize(which+1,NULL);
    fonts[which] = TTF_OpenFont((dumstupidcurrentdirectorybs+"/resources/Fonts/Times New Roman.ttf").c_str(), which);
    return fonts[which];
}

Popup::Popup(Uint8 popup_id,double x,double y,double xsize,double ysize) {
    popupID = popup_id;
    px = x;
    py = y;
    sx = xsize;
    sy = ysize;
}
//returns 0 if nothing happened,
//return  2 if should delete,
//return  1 if clicked in
Uint8 Popup::handle(double mouseX,double mouseY,bool clicked) {
    Uint8 toReturn = 0x00;
    switch (popupID) {
        case NULL_POPUP:
            return 0x02;
        case ADD_OBJECT_POPUP:
            //main backgroundd
            drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
            drawText("Add Object", 24, px+5, py+5, 0xff000000);
            drawTextWithBackground(" Graph ", 20, px+5, py+35, 0xff000000, 0xffffcf9e, 0xff000000);
            int w,h;
            TTF_SizeUTF8((*fontgrab)(20), " Graph ", &w, &h);
            if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+w, py+35, py+35+h)) {
                //add graph!
                addGraph(px, py);
                clicked = false;
                toReturn = 0x02;
            }
            break;
    }
    return toReturn;
}

void Popup::tag() {
    taggedForDeletion = true;
}

bool Popup::isTagged() {
    return taggedForDeletion;
}

Uint8 Popup::getID() {
    return popupID;
}

bool isQuickCloser(Uint8 popup_id) {
    return popup_id==ADD_OBJECT_POPUP;
}

void createPopup(Uint8 popup_id,double x,double y) {
    double sx = 0;
    double sy = 0;
    switch (popup_id) {
        case NULL_POPUP:
            throw std::runtime_error("Tried to create null popup");
            break;
        case ADD_OBJECT_POPUP:
            sx = 150;
            sy = 200;
            break;
    }
    
    Popup* blargh = new Popup(popup_id,x,y,sx,sy);
    for (int i = 0;i<popups.size();i++) {
        //get rid of all popups that close upon the creation of a new one
        if (popups[i]==NULL) {break;}
        if (isQuickCloser(popup_id)) {
            popups[i]->tag();
        }
    }
    popups.push_back(blargh);
}
