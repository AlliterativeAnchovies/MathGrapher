//
//  DisplayWrapper.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef DisplayWrapper_hpp
#define DisplayWrapper_hpp

//This file merely serves to wrap up all the DisplayObjects classes
//So other files only have to import 1 file (this one)
//It also contains the getValidInterpolations code, just because it's
//pretty useful to have those all in one place.
//Think of the architecture like this:
// DisplayObjects.hpp -> [All the display stuffs] -> DisplayWrapper.hpp
//   declares stuff   ->      defines stuff  ->   groups everything together

#include "DisplayObjects/DisplayObjects.hpp"
#include "DisplayObjects/Graph.hpp"
#include "DisplayObjects/Slider.hpp"
#include "DisplayObjects/RawImage.hpp"
#include "DisplayObjects/Interpolation.hpp"
#include "DisplayObjects/Function.hpp"
#include "DisplayObjects/RawText.hpp"

//Returns a vector containing all valid interpolations
template<typename T> std::vector<std::string> getValidInterpolations() {
	throw std::runtime_error("Error!  Have not defined interpolations for this object!");
}
template<> std::vector<std::string> getValidInterpolations<DisplayObject>();
template<> std::vector<std::string> getValidInterpolations<RawImage>();
template<> std::vector<std::string> getValidInterpolations<Slider>();
template<> std::vector<std::string> getValidInterpolations<Graph>();
template<> std::vector<std::string> getValidInterpolations<RawText>();

//Things to draw/interpolate
extern std::vector<DisplayObject*> objects;

//Object adders
void addGraph(double x,double y);
void addSlider(double x,double y);
void addImage(double x,double y,int which);
void addText(double x,double y);

#endif /* DisplayWrapper_hpp */
