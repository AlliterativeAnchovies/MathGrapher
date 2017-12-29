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
    return function(x,time,stretchx,stretchy);
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

