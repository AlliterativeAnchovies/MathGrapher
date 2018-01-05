//
//  Arrow.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 05/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "Arrow.hpp"

void Arrow::run() {
	running = true;
}

void Arrow::reset() {
	running = false;
}

void Arrow::update() {

}

bool Arrow::clickedIn(double mouseX,double mouseY) {
	return pointInArrow(mouseX-px,mouseY-py,length,thickness,headSize,angle,headAngle);
}

SDL_Surface* Arrow::draw(double* x,double* y) {
	double offx,offy;
	SDL_Surface* toReturn = makeArrow(length,thickness,headSize,angle,headAngle,color,&offx,&offy);
	*x = px+offx;
	*y = py+offy;
	return toReturn;
}

Arrow::Arrow(double x,double y,double s,double t,double hS,double a,double hA,std::string n) {
	px = x;
	py = y;
	length = s;
	thickness = t;
	angle = a;
	name = n;
	headSize = hS;
	headAngle = hA;
}
