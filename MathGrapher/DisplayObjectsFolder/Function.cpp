//
//  Function.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Function.hpp"

Function::Function(internalFunc f) {
    function = f;
    parametric = false;
}

Function::Function(internalFunc f,internalRange r,std::string n) {
    function = f;
    name = n;
    range = r;
    parametric = false;
}

Function::Function(internalFunc f,internalFunc f2,internalRange r,std::string n) {
    function = f;
    function2 = f2;
    name = n;
    range = r;
    parametric = true;
}
Function::~Function() {
    for (auto point : importantPoints) {point->prepareForDelete();}
}

double Function::eval(double x) {
	return nthDerivative([=](double n){return function(n,time,stretchx,stretchy);} , x, whichDeriv);
    //return function(x,time,stretchx,stretchy);
}
double Function::getSlope(double x) {
	if (isParametric()) {
		auto xderi = [=](double m){return derivative([=](double n){return function(n,time,stretchx,stretchy);} , m);};
    	auto yderi = [=](double m){return derivative([=](double n){return function2(n,time,stretchx,stretchy);} , m);};
    	auto deri = [=](double n){return yderi(n)/xderi(n);};
		return nthDerivative(deri, x, whichDeriv);
	}
	else {
		return nthDerivative([=](double n){return function(n,time,stretchx,stretchy);} , x, whichDeriv+1);
	}
}
double Function::operator() (double x) {
    return eval(x);
}
Point<double> Function::parametricEval(double x) {
	if (whichDeriv==0) {
    	return Point<double>(function(x,time,stretchx,stretchy),function2(x,time,stretchx,stretchy));
    }
    else if (whichDeriv>0) {
    	//parametric derivative is expressed as [y'(t)]/[x'(t)]
    	auto xderi = [=](double m){return derivative([=](double n){return function(n,time,stretchx,stretchy);} , m);};
    	auto yderi = [=](double m){return derivative([=](double n){return function2(n,time,stretchx,stretchy);} , m);};
    	auto deri = [=](double n){return yderi(n)/xderi(n);};
		return Point<double> (x, nthDerivative(deri, x, whichDeriv-1));
	}
	throw std::runtime_error("ERR NEG DERIV NO NO ;)");
}

double Function::inRange(double x) {
    return range(x,time,stretchx,stretchy);
}

std::string Function::getName() {
    return name;
}
void Function::setName(std::string n) {
    name = n;
}

Function::Function(Function* a) {
    name = a->name;
    function = a->function;
    function2 = a->function2;
    parametric = a->parametric;
    range = a->range;
}

void Function::reset() {
    stretchx = image.stretchx;
    stretchy = image.stretchy;
    time    = image.time;
    visible = image.visible;
    stretchxstring = std::to_string(stretchx);
    stretchystring = std::to_string(stretchy);
}

void Function::saveImage() {
    image.stretchx = stretchx;
    image.stretchy = stretchy;
    image.time = time;
    image.visible = visible;
}

void Function::meshWith(Function* f) {
	function = f->function;
	function2 = f->function2;
	parametric = f->parametric;
	range = f->range;
}

std::vector<PointOfInterest*> pointsOfInterest = {};

std::string PointOfInterest::getDisplayLocation() {
    return "@"+functionOn->getName()+";"+graphOn->getName();
}

std::string PointOfInterest::getDisplayPoint() {
    if (functionOn->isParametric()) {
        auto thePoint = functionOn->parametricEval(px);
        return "("+std::to_string(thePoint.x)+","+std::to_string(thePoint.y)+")";
    }
    else {
        return "("+std::to_string(px)+","+std::to_string((*functionOn)(px))+")";
    }
}


PointOfInterest::PointOfInterest(Graph* g,Function* f,double d,bool v) {
    graphOn = g;
    functionOn = f;
    px = d;
    visible = v;
}

double PointOfInterest::getPY() {
    return (functionOn->isParametric())?functionOn->parametricEval(px).y:(*functionOn)(px);
}

std::vector<SaveData> Function::getSaveData() {
	return {
		{"Name",&name,_STRING},
		{"Tag",this,_FUNCTION_TAG},
		{"Stretch_X",&stretchx,_DOUBLE},
		{"Stretch_Y",&stretchy,_DOUBLE},
		{"Start_Time",&time,_DOUBLE},
		{"Visible",&visible,_BOOLEAN},
		{"Points_Of_Interest",&importantPoints,_VECTOR}
	};
}

std::vector<SaveData> PointOfInterest::getSaveData() {
	return {
		{"Tag",this,_POINT_TAG},
		{"PX",&px,_DOUBLE},
		{"Visible",&visible,_BOOLEAN},
		{"ShowsSlope",&showsSlope,_BOOLEAN}
	};
}

std::vector<EditFieldMenu> Function::getEditableFields() {
	return {
		{"Stretch X: ",&stretchx,_DOUBLE,20,false},
		{"Stretch Y: ",&stretchy,_DOUBLE,20,true},
		{"Start Time: ",&time,_DOUBLE,20,true},
		{"Nth Derivative: ",&whichDeriv,_INT,20,true}
	};
}

std::vector<EditFieldMenu> PointOfInterest::getEditableFields() {
	if (functionOn->isParametric()) {
		auto bothvals = functionOn->parametricEval(px);
		return {
			{"T: |__| PX: "+std::to_string(bothvals.x)+" PY: "+std::to_string(bothvals.y),&px,_DOUBLE,20,true}
		};
	}
	else {
		return {
			{"PX: |__| PY: "+std::to_string((*functionOn)(px)),&px,_DOUBLE,20,true}
		};
	}
}

void PointOfInterest::draw(Function* f,double pixelToXValRatio,double pixelToYValRatio,double ox,double oy,
        			double s1,double c1,double s2,double c2,SDL_Surface* toReturn,bool axis) {
	if (!isVisible()) {return;}
	double finalX;
	double finalY;
	if (f->isParametric()) {
		Point<double> rawPoint = f->parametricEval(getPX());
		if (axis) {//x
			finalX = rawPoint.x*c1/pixelToXValRatio-rawPoint.y*s2/pixelToYValRatio;
			finalY = rawPoint.x*s1/pixelToXValRatio+rawPoint.y*c2/pixelToYValRatio;
		}
		else {//y
			finalY = rawPoint.y*c1/pixelToXValRatio-rawPoint.x*s2/pixelToYValRatio;
			finalX = rawPoint.y*s1/pixelToXValRatio+rawPoint.x*c2/pixelToYValRatio;
		}
		finalX+=ox;
		finalY*=-1;//invert y coord because programming coords start in top not bottom
		finalY+=oy;
		drawCircleOnSurface(toReturn, finalX, finalY, 3, 0xff000099);
	}
	else {
		double rawX = getPX();
		if (!f->inRange(rawX)) {return;}
		double rawY = (*f)(rawX);
		if (axis) {//x
			finalX = rawX*c1/pixelToXValRatio-rawY*s2/pixelToYValRatio;
			finalY = rawX*s1/pixelToXValRatio+rawY*c2/pixelToYValRatio;
		}
		else {//y
			finalY = rawY*c1/pixelToXValRatio-rawX*s2/pixelToYValRatio;
			finalX = rawY*s1/pixelToXValRatio+rawX*c2/pixelToYValRatio;
		}
		finalX+=ox;
		finalY*=-1;//invert y coord because programming coords start in top not bottom
		finalY+=oy;
		drawCircleOnSurface(toReturn, finalX, finalY, 3, 0xff000099);
	}
	//now, we try to get the slope and display that if showing slopes
	if (showsSlope) {
		double theSlope = f->getSlope(getPX());
		double startx1;
		double startx2;
		double starty1;
		double starty2;
		if (axis) {
			startx1 = finalX+(1/pixelToXValRatio);
			startx2 = finalX-(1/pixelToXValRatio);
			starty1 = finalY-(1/pixelToXValRatio)*theSlope;//subtractions/additions swapped for ys because, well, you know:
			starty2 = finalY+(1/pixelToXValRatio)*theSlope;//programming goes down up rather than up down
		}
		else {
			startx1 = finalX+(1/pixelToXValRatio)*theSlope;
			startx2 = finalX-(1/pixelToXValRatio)*theSlope;
			starty1 = finalY-(1/pixelToXValRatio);
			starty2 = finalY+(1/pixelToXValRatio);
		}
		drawLineOnSurface(toReturn, startx1, starty1, startx2, starty2, 0xff000000);
	}
}


