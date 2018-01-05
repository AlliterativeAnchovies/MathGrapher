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
	image.px = px;
	image.py = py;
	image.length = length;
	image.angle = angle;
	image.thickness = thickness;
	image.headAngle = headAngle;
	image.headSize = headSize;
	image.color = color;
}

void Arrow::reset() {
	running = false;
	px = image.px;
	py = image.py;
	length = image.length;
	angle = image.angle;
	thickness = image.thickness;
	headAngle = image.headAngle;
	headSize = image.headSize;
	color = image.color;
}

void Arrow::update() {
	for (int i = 0;i<interpolations.size();i++) {
        if (interpolations[i]->update()) {
            std::vector<Interpolation*> followups = interpolations[i]->getFollowups();
            for (int j = 0;j<followups.size();j++) {
                followups[j]->wait();
                interpolations.push_back(followups[j]);
            }
            interpolations[i]->pause();
        }
    }
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

void Arrow::resizeSmooth(double lengthincrease,double thickincrease) {
	thickness+=thickincrease;
	length+=lengthincrease;
}

void Arrow::rescaleHead(double headsizechange) {
	headSize+=headsizechange;
}
