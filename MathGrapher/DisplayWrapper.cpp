//
//  DisplayWrapper.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "DisplayWrapper.hpp"

template<> std::vector<std::string> getValidInterpolations<DisplayObject>() {
	throw std::runtime_error("Error!  Trying to get genericly valid interpolations - be specific!");
}
template<> std::vector<std::string> getValidInterpolations<RawImage>() {
	return {"Move","Rescale","Fix","Disappear","Appear"};
}
template<> std::vector<std::string> getValidInterpolations<Slider>() {
	return {"Move","Disappear","Appear"};
}
template<> std::vector<std::string> getValidInterpolations<Graph>() {
	return {"Move","Resize","Rescale","Rotate","Re-Origin","Stretch","Highlight","Disappear","Appear"};
}

template<> std::vector<std::string> getValidInterpolations<RawText>() {
	return {"Move","Disappear","Appear","Rescale"};
}

template<> std::vector<std::string> getValidInterpolations<Arrow>() {
	return {"Move","Rescale","Rescale Head","Disappear","Appear"};
}

//Things to draw
std::vector<DisplayObject*> objects = {};
std::vector<DisplayObject*> selectedObjects = {};

//Internal counters of things
int TOTAL_GRAPHS = 0;
int TOTAL_SLIDERS = 0;
int TOTAL_IMAGES = 0;
int TOTAL_TEXTS = 0;
int TOTAL_ARROWS = 0;


void addGraph(double x,double y) {
    objects.push_back(new Graph(x,y,100,100,"Graph "+std::to_string(TOTAL_GRAPHS)));
    TOTAL_GRAPHS++;
}

void addSlider(double x,double y) {
    objects.push_back(new Slider(x,y,100,"Slider "+std::to_string(TOTAL_SLIDERS)));
    TOTAL_SLIDERS++;
}
void addImage(double x,double y,int which) {
    objects.push_back(new RawImage(x,y,which,"Image "+std::to_string(TOTAL_IMAGES)));
    TOTAL_IMAGES++;
}
void addText(double x,double y) {
    objects.push_back(new RawText(x,y,16,"Text "+std::to_string(TOTAL_TEXTS)));
    TOTAL_TEXTS++;
}

void addArrow(double x,double y) {
    objects.push_back(new Arrow(x,y,100,20,30,0,M_PI/4,"Arrow "+std::to_string(TOTAL_ARROWS)));
    TOTAL_ARROWS++;
}

void drawDisplayObject(DisplayObject* d) {
    double xdraw,ydraw = 0;
    SDL_Surface* tempSurf = d->draw(&xdraw,&ydraw);
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(gRenderer, tempSurf);
    drawGraphic(xdraw, ydraw, tempSurf->w, tempSurf->h, tempTexture);
    d->reclaim(tempSurf);
    SDL_DestroyTexture(tempTexture);
}
