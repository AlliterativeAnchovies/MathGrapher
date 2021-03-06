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
	visible=true;
	for (int i = 0;i<interpolations.size();i++) {
        interpolations[i]->reset();
    }
}


bool Arrow::clickedIn(double mouseX,double mouseY) {
	return pointInArrow(mouseX-px,mouseY-py,length,thickness,headSize,angle,headAngle);
}

SDL_Surface* Arrow::draw(double* x,double* y) {
	if (!visible) {*x=0;*y=0;return createBlankSurfaceWithSize(0, 0);}
	double offx,offy;
	SDL_Surface* toReturn = makeArrow(length,thickness,headSize,angle,headAngle,color,&offx,&offy);
	*x = px+offx;
	*y = py+offy;
	if (highlighted) {
		SDL_Surface* highlight = createBlankSurfaceWithSize(toReturn->w,toReturn->h);
        SDL_FillRect(highlight, NULL, 0x6600ff00);
        SDL_BlitSurface(highlight,NULL,toReturn,NULL);
        SDL_FreeSurface(highlight);
        highlighted=false;
	};
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

std::vector<EditFieldMenu> Arrow::getEditableFields() {
	return {
		{"",&name,_STRING,24,true},
		{"PX: ",&px,_DOUBLE,20,false},
		{"PY: ",&py,_DOUBLE,20,true},
		{"Length: ",&length,_DOUBLE,20,false},
		{"Thickness: ",&thickness,_DOUBLE,20,true},
		{"Head Size: ",&headSize,_DOUBLE,20,true},
		{"Angle: ",&angle,_DOUBLE,20,false},
		{"Head Angle: ",&headAngle,_DOUBLE,20,true},
		{"Color: ",&color,_HEXADECIMAL,20,true}
	};
}

std::vector<SaveData> Arrow::getSaveData() {
	return {
		{"Name",&name,_STRING},
		{"PX",&px,_DOUBLE},
		{"PY",&py,_DOUBLE},
		{"Length",&length,_DOUBLE},
		{"Angle",&angle,_DOUBLE},
		{"Thickness",&thickness,_DOUBLE},
		{"Head_Size",&headSize,_DOUBLE},
		{"Head_Angle",&headAngle,_DOUBLE},
		{"Color",&color,_HEXADECIMAL},
		{"Interpolations",&interpolations,_VECTOR}
	};
};

std::vector<std::string> Arrow::getValidInterpolations() {
	return {"Move","Scale","Rescale Head","Disappear","Appear"};
}

Arrow::Arrow(int x,int y) {
			px = x;
			py = y;
			length = 100;
			thickness = 20;
			headSize = 30;
			angle = 0;
			headAngle = M_PI/4;
			name ="Arrow "+std::to_string(Counter<Arrow>::current_obj);
		};

