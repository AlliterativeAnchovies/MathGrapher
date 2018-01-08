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
    RESCALE_HEAD,
    APPEAR,
    DISAPPEAR,
    DELAY
};
class Graph;
class Interpolation;
class Function;
class PointOfInterest;
class Slider;
class RawImage;
class RawText;
class Arrow;


typedef Wrap2<std::string,Function*> FuncWrap;
typedef std::vector<FuncWrap> FunctionList;
extern FunctionList builtins;//defined in Functions.cpp

enum VALUE_TYPES {
	//raw types
	_DOUBLE,
	_STRING,
	_INT,
	_HEXADECIMAL,//uint32
	_BOOLEAN,
	_MINIHEX,//uint8
	//types that it needs to know about for saving data
	_FUNCTION,
	_INTERPOLATION,
	_POINT_OF_INTEREST,
	_VECTOR,//things in the vector must derive from class "Data"
	_POINT_TAG,
	_POINT_HOOK,
	_FUNCTION_TAG,
	_FUNCTION_HOOK
};

struct EditFieldMenu {
	std::string prefix = "";
	void* toEdit;
	Uint32 valueType = _DOUBLE;
	int fontSize = 16;
	bool newLine = false;
};

struct SaveData {
	std::string prefix = "";
	void* data;
	Uint32 valueType = _DOUBLE;
};

class Data {
	public:
		virtual std::vector<SaveData> getSaveData()=0;
		virtual std::string getID()=0;
		virtual bool isDisplayObject() {return false;}
		virtual bool isInterpolation() {return false;}
		virtual std::vector<EditFieldMenu> getEditableFields()=0;
		virtual void init() {};
};

class DisplayObject: public Data {
    public:
        //IF YOU GET A "MISSING VTABLE" ERROR, it'll be caused by you not having defined all of these functions
        //in the child object (although the error only seems to happen if you give your class a nondefault constructor
        //or call the default constructor because otherwise the compiler can treat it as a virtual class too.)
        virtual void highlight()=0;
        virtual bool clickedIn(double mouseX,double mouseY)=0;
        virtual std::string getName()=0;
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
		virtual std::vector<Interpolation*> getInterpolations()=0;
		virtual void makeInvisible()=0;
		virtual void makeVisibile()=0;
		bool isDisplayObject() {return true;}
};


#endif /* DisplayObjects_hpp */
