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
extern std::vector<PointOfInterest*> pointsOfInterest;


Uint32 getColorOfInterpolation(Interpolation* i);
std::string stringifyID(Uint8 id);

#endif /* DisplayObjects_hpp */
