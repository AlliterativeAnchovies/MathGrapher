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
#include "RenderingUtilities.hpp"


//Some functions that are needed to be declared here to interface well with main.cpp
//they are defined in main
void addGraph(double x,double y);
void addSlider(double x,double y);
void addImage(double x,double y,int which);
extern std::vector<SDL_Surface*> gSurfaces;
extern std::vector<SDL_Texture*> gTextures;
extern std::vector<std::string>  gStrings;
const int SLIDER_DEFAULT_SCALE_BUILTIN_INDEX = 7;

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
    DELAY,
};
class Graph;
class Interpolation;
class Function;
class PointOfInterest;
class Slider;
class RawImage;
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
};


#endif /* DisplayObjects_hpp */
