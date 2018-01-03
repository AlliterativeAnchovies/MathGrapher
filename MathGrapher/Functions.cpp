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

void initBuiltins() {
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
            "(1-x)^2")},
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

Function* functionFromName(std::string name) {
	for (auto f : builtins) {
		if (f.y->getName()==name) {
			return f.y;
		}
	}
	throw std::runtime_error("Error! No such function: "+name);
}
