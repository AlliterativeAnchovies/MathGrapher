//
//  Functions.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 14/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Functions.hpp"

FunctionList builtins = {};

bool tangentRange(double in) {
    in*=180/M_PI;
    int rounded = ((int)in);
    while (rounded<0) {rounded+=360;}
    rounded%=90;
    return (rounded>5&&rounded<85);
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
/*void initBuiltins() {
	builtins = {
		//Standard Functions
		{"Identity",
			new Function({0,1},{},"x")
		},
		{"Parabola",
			new Function({0,0,1},{},"x^2")
		},
		{"Sine",
			new Function({0,1,0,-1,0,1,0,-1},{},"sin(x)")
		},
		{"Cosine",
			new Function({1,0,-1,0,1,0,-1},{},"cos(x)")
		},
		{"Tangent",
			new Function({1,0,3,0,16,0,272},{},"tan(x)")
		},
		{"Reciprocal",
			new Function({1,-1,2,-6,24,-120},{},"1/x",1)//note that the taylor series is about 1 here, not 0 (the default)
		},
		{"Slider Scale",
			new Function({1,0.5,-0.25,0.375,-0.9375},{},"sqrt(|x|)",1)//about 1 not 0 b/c absolute value gets funky at 0
		},
		//Parametric Functions
		//Remember that these MUST have a range
		{"Circle",
			new Function({1,0,-1,0,1,0,-1},{0,1,0,-1,0,1,0,-1},{Point<double>(0,1)},"x^2+y^2=1")
		},
	};
}*/

/*void initBuiltins() {
=======
void initBuiltins() {
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
=======
void initBuiltins() {
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
=======
void initBuiltins() {
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
    builtins = {
        //standard functions
        {"Identity",
            new Function([](double x,double t,double sx,double sy){return sx*(x+t/FRAME_RATE)/sy;},
            [](double x,double t,double sx,double sy){return true;},
            "x")},
        {"Parabola",
            new Function([](double x,double t,double sx,double sy){return pow(sx*(x+t/FRAME_RATE),2)/sy;},
            [](double x,double t,double sx,double sy){return true;},
            "x^2")},
        {"Sine",
            new Function([](double x,double t,double sx,double sy){return sin(sx*(x+t/FRAME_RATE))/sy;},
            [](double x,double t,double sx,double sy){return true;},
            "sin(x)")},
        {"Cosine",
            new Function([](double x,double t,double sx,double sy){return cos(sx*(x+t/FRAME_RATE))/sy;},
            [](double x,double t,double sx,double sy){return true;},
            "cos(x)")},
        {"Tangent",
            new Function([](double x,double t,double sx,double sy){return tan(sx*(x+t/FRAME_RATE))/sy;},
            [](double x,double t,double sx,double sy){return tangentRange(x+t/FRAME_RATE);},
            "tan(x)")},
        {"Reciprocal",
            new Function([](double x,double t,double sx,double sy){return 1/(sy*sx*(x+t/FRAME_RATE));},
            [](double x,double t,double sx,double sy){return (x+t/FRAME_RATE) != 0;},
            "1/x")},
        {"Reciprocal^2",
            new Function([](double x,double t,double sx,double sy){return 1/(sy*(sx*(x+t/FRAME_RATE))*(sx*(x+t/FRAME_RATE)));},
            [](double x,double t,double sx,double sy){return (x+t/FRAME_RATE) != 0;},
            "1/x^2")},
        {"Slider Scale",
            new Function([](double x,double t,double sx,double sy){return pow(sx*(abs(x+t/FRAME_RATE)),0.5)/sy;},
            [](double x,double t,double sx,double sy){return true;},
            "sqrt(1-x)")},
        //parametric functions
        {"Circle",
            new Function(
                [](double x,double t,double sx,double sy){return cos(sx*((x+t)*M_PI/180))/sy;},
                [](double x,double t,double sx,double sy){return sin(sx*((x+t)*M_PI/180))/sy;},
                [](double x,double t,double sx,double sy){
                    return sx*(x*M_PI/180)>=0&&sx*(x*M_PI/180)<2*M_PI&&sx!=0;
                },
            "x^2+y^2=1")},
    };
}
<<<<<<< HEAD
<<<<<<< HEAD

bool functionExists(std::string name) {
	for (auto f : builtins) {
		if (f.y->getName()==name) {
			return true;
		}
	}
	return false;
}

bool functionExistsFromTag(std::string t) {
	for (auto f : builtins) {
		if (f.x==t) {
			return true;
		}
	}
	return false;
}

Function* functionFromTag(std::string t) {
	for (auto f : builtins) {
		if (f.x==t) {
			return f.y;
		}
	}
	throw std::runtime_error("Error! No such function: "+t);
}
=======
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
=======
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions

Function* functionFromName(std::string name) {
	for (auto f : builtins) {
		if (f.y->getName()==name) {
			return f.y;
		}
	}
	throw std::runtime_error("Error! No such function: "+name);
}
