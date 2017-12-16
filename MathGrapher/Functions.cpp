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
        (FuncWrap){"Identity",
            new Function([](double x,double t,double sx,double sy){return sx*(x+t/60)/sy;},
            [](double x,double t){return true;},
            "x")},
        (FuncWrap){"Sine",
            new Function([](double x,double t,double sx,double sy){return sin(sx*(x+t/60))/sy;},
            [](double x,double t){return true;},
            "sin(x)")},
        (FuncWrap){"Cosine",
            new Function([](double x,double t,double sx,double sy){return cos(sx*(x+t/60))/sy;},
            [](double x,double t){return true;},
            "cos(x)")},
        (FuncWrap){"Tangent",
            new Function([](double x,double t,double sx,double sy){return tan(sx*(x+t/60))/sy;},
            [](double x,double t){return tangentRange(x+t/60);},
            "tan(x)")},
        (FuncWrap){"Reciprocal",
            new Function([](double x,double t,double sx,double sy){return 1/(sy*sx*(x+t/60));},
            [](double x,double t){return (x+t/60) != 0;},
            "1/x")},
        (FuncWrap){"Reciprocal^2",
            new Function([](double x,double t,double sx,double sy){return 1/(sy*(sx*(x+t/60))*(sx*(x+t/60)));},
            [](double x,double t){return (x+t/60) != 0;},
            "1/x^2")}
    };
}
