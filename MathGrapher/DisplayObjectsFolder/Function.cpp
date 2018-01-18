//
//  Function.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Function.hpp"

/*Function::Function(internalFunc f) {
    function = f;
    parametric = false;
}*/

Function::Function(std::vector<double> t) {
	taylorSeries1 = t;
	parametric = false;
}

/*Function::Function(internalFunc f,internalRange r,std::string n) {
    function = f;
    name = n;
    range = r;
    parametric = false;
}*/

Function::Function(std::vector<double> t,std::vector<Point<double>> r,std::string n,double tsa) {
    taylorSeries1 = t;
    name = n;
    range = r;
    parametric = false;
    taylorSeriesAbout = tsa;
}

/*Function::Function(internalFunc f,internalFunc f2,internalRange r,std::string n) {
    function = f;
    function2 = f2;
    name = n;
    range = r;
    parametric = true;
}*/
Function::Function(std::vector<double> t,std::vector<double> t2,std::vector<Point<double>> r,std::string n,
					double tsa1,double tsa2) {
    taylorSeries1 = t;
    taylorSeries2 = t2;
    name = n;
    range = r;
    parametric = true;
    taylorSeriesAbout = tsa1;
    taylorSeriesAbout2 = tsa2;
}

Function::Function(ParsedFile* d,std::string n) {
	derived = true;
	derivation = d;
	name = n;
	parametric = false;
}


Function::~Function() {
    for (auto point : importantPoints) {point->prepareForDelete();}
}

double Function::evalTaylor(std::vector<double> taylor,double pointAt,double tsa) {
	double toReturn = 0;
	for (int i = 0;i<taylor.size();i++) {
		toReturn += (taylor[i] / tgamma(i+1)) * pow(pointAt-tsa,i);
	}
	return toReturn;
}

double Function::eval(double x,bool stretchyuse) {
	if (!derived) {
		return ((stretchyuse)?stretchy:1)*evalTaylor(taylorSeries1, stretchx*x+time/FRAME_RATE,taylorSeriesAbout);
	}
	else {
		std::vector<ParsedFile*> p = derivation->componentFromString("*");
		if (functionExists(p[0]->getValue())) {
			return ((stretchyuse)?stretchy:1)*functionFromTag(p[0]->getKey())->eval(x);
		}
		else {
			return 0;//finish this function later!
		}
		
	}
    //return function(x,time,stretchx,stretchy);
}
double Function::operator() (double x) {
    return eval(x);
}
Point<double> Function::parametricEval(double x) {
	double toretx = evalTaylor(taylorSeries1, x+time/FRAME_RATE, taylorSeriesAbout);
	double torety = evalTaylor(taylorSeries2, x+time/FRAME_RATE, taylorSeriesAbout2);
	return Point<double>(stretchx*toretx,stretchy*torety);
    //return Point<double>(function(x,time,stretchx,stretchy),function2(x,time,stretchx,stretchy));
}

double Function::inRange(double x) {
	for (auto p : range) {
		if (x<=p.y&&x>=p.x) {
			return false;
		}
	}
	return true;
    //return range(x,time,stretchx,stretchy);
}

std::string Function::getName() {
    return name;
}
void Function::setName(std::string n) {
    name = n;
}

Function::Function(Function* a) {
    name = a->name;
    //function = a->function;
    //function2 = a->function2;
    taylorSeries1 = a->taylorSeries1;
    taylorSeries2 = a->taylorSeries2;
    taylorSeriesAbout = a->taylorSeriesAbout;
    taylorSeriesAbout2 = a->taylorSeriesAbout2;
    parametric = a->parametric;
    range = a->range;
}

void Function::reset() {
    stretchx = image.stretchx;
    stretchy = image.stretchy;
    time    = image.time;
    visible = image.visible;
    //stretchxstring = std::to_string(stretchx);
    //stretchystring = std::to_string(stretchy);
}

void Function::saveImage() {
    image.stretchx = stretchx;
    image.stretchy = stretchy;
    image.time = time;
    image.visible = visible;
}

void Function::meshWith(Function* f) {
	//maintains all stretch/etc vals but changes the actual function to f.
	taylorSeries1 = f->taylorSeries1;
    taylorSeries2 = f->taylorSeries2;
    taylorSeriesAbout = f->taylorSeriesAbout;
    taylorSeriesAbout2 = f->taylorSeriesAbout2;
	parametric = f->parametric;
	range = f->range;
	derived = f->derived;
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
		{"Start Time: ",&time,_DOUBLE,20,true}
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



