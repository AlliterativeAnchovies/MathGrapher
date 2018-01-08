//
//  Slider.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Slider.hpp"

Slider::Slider(double x,double y,double s,std::string n) {
    px = x;
    py = y;
    size = s;
    name = n;
    tickAmount = 4;
    incrementFunction = new Function(builtins[SLIDER_DEFAULT_SCALE_BUILTIN_INDEX].y);
}

void Slider::run() {
    running = true;
    image.px = px;
    image.py = py;
    image.size = size;
    image.angle = angle;
    image.incrementFunction = incrementFunction;
    image.tickAmount = tickAmount;
}

void Slider::reset() {
	running = false;
	px = image.px;
	py = image.py;
	size = image.size;
	angle = image.angle;
	visible = true;
	incrementFunction = image.incrementFunction;
	tickAmount = image.tickAmount;
	for (int i = 0;i<interpolations.size();i++) {
        interpolations[i]->reset();
    }
}

void Slider::update() {
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

const int SLIDER_WIDTH = 30;
SDL_Surface* Slider::draw(double* x,double* y) {
	if (!visible) {*x=0;*y=0;return createBlankSurfaceWithSize(0, 0);}
    double sliderstartx = SLIDER_WIDTH/2;
    double sliderstarty = -size/2;
    double sliderendx = SLIDER_WIDTH/2;
    double sliderendy = size/2;
    double sin,cos;
    fastSineCosine(&sin, &cos, angle);
    double rotslidx =  sliderstartx*cos+sliderstarty*sin;
    double rotslidy = -sliderstartx*sin+sliderstarty*cos;
    double rotslidbotx =  sliderendx*cos+sliderendy*sin;
    double rotslidboty = -sliderendx*sin+sliderendy*cos;
    //we've calculated its rotated endpoints.  Its graphic will sized to accomodate
    SDL_Surface* toReturn = createBlankSurfaceWithSize
            (abs(rotslidbotx-rotslidx)+SLIDER_WIDTH, abs(rotslidboty-rotslidy)+SLIDER_WIDTH);
    storedsx = toReturn->w;
    storedsy = toReturn->h;
    //now draw the actual mainline
    double startx=rotslidx-((rotslidx>rotslidbotx)?rotslidbotx:rotslidx)+SLIDER_WIDTH/2;
    double starty=rotslidy-((rotslidy>rotslidboty)?rotslidboty:rotslidy)+SLIDER_WIDTH/2;
    double endx=rotslidbotx-((rotslidx>rotslidbotx)?rotslidbotx:rotslidx)+SLIDER_WIDTH/2;
    double endy=rotslidboty-((rotslidy>rotslidboty)?rotslidboty:rotslidy)+SLIDER_WIDTH/2;
    drawLineOnSurface(toReturn, startx, starty, endx, endy, 0xff000000);
    //find the origin
    Point<double> st = Point<double>(startx,starty);//STart
    Point<double> en = Point<double>(endx,endy);//ENd
    Point<double> mi = en+(st-en)/2.0; //MIddle, the origin
    //find normal vector
    Point<double> normal = 3.0*normalTo(st-en).norm();
    //draw tick at origin
    Point<double> off1 = mi-normal;
    Point<double> off2 = mi+normal;
    drawLineOnSurface(toReturn, off1.x, off1.y, off2.x, off2.y, 0xff000000);
    //now draw all other ticks
    //there are tickAmount ticks after the origin, in both directions.  We have
    //a function, incrementFunction, that tells us how much to move forward
    //each time.
    Point<double> upVec = (st-en)/2.0;
    for (double i = 1;i<=tickAmount;i++) {
        double moveRateUp = abs((*incrementFunction)(i/tickAmount));
        double moveRateDown = -abs((*incrementFunction)(i/tickAmount));
        Point<double> upPoint=mi+moveRateUp*upVec;
        Point<double> downPoint=mi+moveRateDown*upVec;
        off1 = upPoint-normal;
        off2 = upPoint+normal;
        drawLineOnSurface(toReturn, off1.x, off1.y, off2.x, off2.y, 0xff000000);
        off1 = downPoint-normal;
        off2 = downPoint+normal;
        drawLineOnSurface(toReturn, off1.x, off1.y, off2.x, off2.y, 0xff000000);
    }
    
    //now draw pointer
    double rawpointery = pointery;
    if (pointConcerned!=NULL) {
        rawpointery+=pointConcerned->getPY();
    }
    Point<double> pointerpoint = upVec*rawpointery+mi;
    drawCircleOnSurface(toReturn, pointerpoint.x, pointerpoint.y, 3, 0xff000099);
    
    
    if (highlighted) {
        SDL_Surface* highlight = createBlankSurfaceWithSize(toReturn->w,toReturn->h);
        SDL_FillRect(highlight, NULL, 0x6600ff00);
        SDL_BlitSurface(highlight,NULL,toReturn,NULL);
        SDL_FreeSurface(highlight);
    }
    offx_stored = startx;
    offy_stored = starty;
    *x = px-startx;
    *y = py-starty;
    highlighted = false;
    return toReturn;
}

bool Slider::clickedIn(double x,double y) {
    return pointInBounds(x, y, px-offx_stored, px+storedsx-offx_stored, py-offy_stored, py+storedsy-offy_stored);
}

void Slider::addInterpolation(Interpolation* i) {
	interpolations.push_back(i);
}

std::vector<EditFieldMenu> Slider::getEditableFields() {
	return {
		{"",&name,_STRING,24,true},
		{"PX: ",&px,_DOUBLE,20,false},
		{"PY: ",&py,_DOUBLE,20,true},
		{"Size: ",&size,_DOUBLE,20,true},
		{"θ: ",&angle,_DOUBLE,20,true},
		{"Starting Y: ",&pointery,_DOUBLE,20,true},
		{"Ticks: ",&tickAmount,_INT,20,true},
	};
};

std::vector<SaveData> Slider::getSaveData() {
	return {
		{"Name",&name,_STRING},
		{"PX",&px,_DOUBLE},
		{"PY",&py,_DOUBLE},
		{"Size",&size,_DOUBLE},
		{"Angle",&angle,_DOUBLE},
		{"Starting_Y",&pointery,_DOUBLE},
		{"Tick_Amount",&tickAmount,_INT},
		{"Point_Of_Interest",&pointConcerned,_POINT_OF_INTEREST},
		{"Tick_Function",&incrementFunction,_FUNCTION},
		{"Interpolations",&interpolations,_VECTOR}
	};
};
