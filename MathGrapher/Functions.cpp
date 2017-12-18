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
        (FuncWrap){"Identity",
            new Function([](double x,double t,double sx,double sy){return sx*(x+t/60)/sy;},
            [](double x,double t,double sx,double sy){return true;},
            "x")},
        (FuncWrap){"Parabola",
            new Function([](double x,double t,double sx,double sy){return pow(sx*(x+t/60),2)/sy;},
            [](double x,double t,double sx,double sy){return true;},
            "x^2")},
        (FuncWrap){"Sine",
            new Function([](double x,double t,double sx,double sy){return sin(sx*(x+t/60))/sy;},
            [](double x,double t,double sx,double sy){return true;},
            "sin(x)")},
        (FuncWrap){"Cosine",
            new Function([](double x,double t,double sx,double sy){return cos(sx*(x+t/60))/sy;},
            [](double x,double t,double sx,double sy){return true;},
            "cos(x)")},
        (FuncWrap){"Tangent",
            new Function([](double x,double t,double sx,double sy){return tan(sx*(x+t/60))/sy;},
            [](double x,double t,double sx,double sy){return tangentRange(x+t/60);},
            "tan(x)")},
        (FuncWrap){"Reciprocal",
            new Function([](double x,double t,double sx,double sy){return 1/(sy*sx*(x+t/60));},
            [](double x,double t,double sx,double sy){return (x+t/60) != 0;},
            "1/x")},
        (FuncWrap){"Reciprocal^2",
            new Function([](double x,double t,double sx,double sy){return 1/(sy*(sx*(x+t/60))*(sx*(x+t/60)));},
            [](double x,double t,double sx,double sy){return (x+t/60) != 0;},
            "1/x^2")},
        //parametric functions
        (FuncWrap){"Circle",
            new Function(
                [](double x,double t,double sx,double sy){return cos(sx*((x+t)*M_PI/180))/sy;},
                [](double x,double t,double sx,double sy){return sin(sx*((x+t)*M_PI/180))/sy;},
                [](double x,double t,double sx,double sy){
                    return sx*(x*M_PI/180)>=0&&sx*(x*M_PI/180)<2*M_PI&&sx!=0;
                },
            "x^2+y^2=1")},
    };
}
