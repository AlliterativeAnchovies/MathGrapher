//
//  DisplayObjects.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef DisplayObjects_hpp
#define DisplayObjects_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <functional>
//#if defined _WINDOWS
	#include "../BulkUtilities.hpp"
//#else
//	#include "BulkUtilities.hpp"
//#endif



enum INTERPOLATIONS {
    NULL_INTERPOLATION,
    SMOOTH_TRANSLATE,
    SMOOTH_ORIGIN_TRANSLATE,
    SMOOTH_GRID_ROTATE,
    SMOOTH_GRID_SCALE,
    SMOOTH_GRID_RESIZE_STATIC_CENTER,
    SMOOTH_GRID_RESIZE_SMART_CENTER,
    SMOOTH_FUNCTION_RUN,
    SMOOTH_FUNCTION_STRETCH,
    HIGHLIGHT_GRAPH,
    FIX_THINGAMAJIG,
    DELAY
};
class Graph;
class Interpolation;
class Function;
class PointOfInterest;
class Slider;
class RawImage;
class RawText;
typedef Wrap2<std::string,Function*> FuncWrap;
typedef std::vector<FuncWrap> FunctionList;
extern FunctionList builtins;//defined in Functions.cpp

class DisplayObject {
    public:
        //IF YOU GET A "MISSING VTABLE" ERROR, it'll be caused by you not having defined all of these functions
        //in the child object (although the error only seems to happen if you give your class a nondefault constructor
        //because otherwise the compiler can treat it as a virtual object too.)
        virtual void highlight() = 0;
        virtual bool clickedIn(double mouseX,double mouseY) = 0;
        virtual std::string getID() = 0;
        virtual std::string getName() = 0;
        virtual void run() =0;
        virtual void reset()=0;
        virtual bool isRunning()=0;
        virtual void update()=0;
        virtual ~DisplayObject() {};//in a class C, the method ~C is the destructor of C.  Usually implicitly
                                    //defined, but if the class is virtual we also need to give it a virtual
                                    //destructor.  The function body is empty which means we just use the standard
                                    //deletion method. (if it were full of an ordered set of instructions A, the
                                    //object would be deleted by instructions A union S where S is the standard
                                    //deletion method).
        virtual SDL_Surface* draw(double* x,double* y)=0;
        virtual void reclaim(SDL_Surface* reclaimed)=0;
        virtual void move(double x,double y)=0;
		virtual void addInterpolation(Interpolation* i)=0;
		virtual std::vector<Interpolation*> getInterpolations() = 0;
};


#endif /* DisplayObjects_hpp */
