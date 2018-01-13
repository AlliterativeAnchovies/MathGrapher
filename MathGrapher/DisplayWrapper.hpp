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

#include "DisplayObjectsFolder/DisplayObjects.hpp"
#include "DisplayObjectsFolder/Graph.hpp"
#include "DisplayObjectsFolder/Slider.hpp"
#include "DisplayObjectsFolder/RawImage.hpp"
#include "DisplayObjectsFolder/InterpolationWrapper.hpp"
#include "DisplayObjectsFolder/Function.hpp"
#include "DisplayObjectsFolder/RawText.hpp"
#include "DisplayObjectsFolder/Arrow.hpp"

//Things to draw/interpolate
extern std::vector<DisplayObject*> objects;

//Object adders
/*void addGraph(double x,double y);
void addSlider(double x,double y);
void addImage(double x,double y,int which);
void addText(double x,double y);
void addArrow(double x,double y);*/
template<typename T> T* addDisplayObject(int x,int y) {
	T* toReturn = new T(x,y);
	objects.push_back(toReturn);
	return toReturn;
}

//Things to display that are selected
extern std::vector<DisplayObject*> selectedObjects;
extern DisplayObject* dragObject;

//Draw code
void drawDisplayObject(DisplayObject* d);

//Get a created object from the ID
SavableData* dataFromID(std::string theID);

#endif /* DisplayWrapper_hpp */
