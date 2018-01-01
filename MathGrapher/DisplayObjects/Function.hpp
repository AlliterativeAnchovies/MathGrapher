//
//  Function.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef Function_hpp
#define Function_hpp

#include "DisplayObjects.hpp"
#include "Graph.hpp"

typedef std::function<double(double,double,double,double)> internalFunc;
typedef std::function<bool(double,double,double,double)> internalRange;

struct FunctionImage {
    double stretchx;
    double stretchy;
    double time;
    bool visible;
};

class Function {
    private:
        internalFunc function=NULL;
        internalFunc function2=NULL;//if parametric
        bool parametric = false;
        internalRange range;
        std::string name = "-FUNCTION-";
        bool tagged = false;
        double stretchx = 1;
        double stretchy = 1;
        double time = 0;
        std::string stretchxstring = "1";
        std::string stretchystring = "1";
        bool visible = true;
        FunctionImage image;
        std::vector<PointOfInterest*> importantPoints = {};
    public:
        //basic definition ('degenerate')
        Function(internalFunc f);
        //standard definition
        Function(internalFunc f,internalRange r,std::string n);
        //parametric definition
        Function(internalFunc f,internalFunc f2,internalRange r,std::string n);
        double eval(double x);
        double operator() (double x);
        Point<double> parametricEval(double x);
        double inRange(double x);
        std::string getName();
        void setName(std::string n);
        void tag() {tagged=true;}
        bool isTagged() {return tagged;}
        Function(Function* a);
        void reset();
        double getTime() {return time;}
        double getStretchX() {return stretchx;}
        double getStretchY() {return stretchy;}
        std::string getStretchXString() {return stretchxstring;}
        std::string getStretchYString() {return stretchystring;}
        void setStretchX(std::string s) {stretchxstring = s;stretchx=numberFromString(s);}//deprecated
        void setStretchY(std::string s) {stretchystring = s;stretchy=numberFromString(s);}//deprecated
        void setTime(double t) {time = t;}
        bool isVisible() {return visible;}
        void toggleVisibility() {visible=!visible;}
        void stretch(double x,double y) {stretchx+=x;stretchy+=y;}
        void run(double x) {time+=x;}
        void saveImage();
        void addPoint(PointOfInterest* p) {importantPoints.push_back(p);};
        decltype(importantPoints) getImportantPoints() {return importantPoints;}
        bool isParametric() {return parametric;}
        //custom deleter to clean up points of interest
        ~Function();
        double* ptmTime() {return &time;}
        double* ptmStretchX() {return &stretchx;}
        double* ptmStretchY() {return &stretchy;}
        int tagForSaving = -1;//used for linking things while saving stuffs
		void setStretchX(double s) {stretchx=s;}
        void setStretchY(double s) {stretchy=s;}
};

extern std::vector<PointOfInterest*> pointsOfInterest;

class PointOfInterest {
    private:
        Graph* graphOn = NULL;
        Function* functionOn = NULL;
        double px = 0;
        bool visible = true;
        bool taggedForDeletion = false;
    public:
    	int tagForSaving = -1;//used for linking things while saving stuffs
        PointOfInterest(Graph* g,Function* f,double d,bool v);
        bool isVisible() {return visible;}
        std::string getDisplayLocation();
        std::string getDisplayPoint();
        double getPX() {return px;}
        double getPY();
        bool shouldDelete() {return taggedForDeletion;}
        void prepareForDelete() {taggedForDeletion=true;}
};


#endif /* Function_hpp */
