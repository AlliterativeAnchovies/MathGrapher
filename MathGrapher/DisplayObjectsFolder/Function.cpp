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
double Function::operator() (double x) {
    return eval(x);
}
Point<double> Function::parametricEval(double x) {
    return Point<double>(function(x,time,stretchx,stretchy),function2(x,time,stretchx,stretchy));
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
		{"Visible",&visible,_BOOLEAN}
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



