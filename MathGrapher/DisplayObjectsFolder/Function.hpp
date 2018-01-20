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

class Function: public SavableData {
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
        Graph* graphOn = NULL;
        int whichDeriv = 0;
    public:
        //basic definition ('degenerate')
        Function(internalFunc f);
        //standard definition
        Function(internalFunc f,internalRange r,std::string n);
        //parametric definition
        Function(internalFunc f,internalFunc f2,internalRange r,std::string n);
        Function() {};
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
        int tagForSaving = -1;//used for linking things while saving stuffs
		void setStretchX(double s) {stretchx=s;}
        void setStretchY(double s) {stretchy=s;}
        std::vector<SaveData> getSaveData();
        std::string getID() {return "Function";}
        void giveGraph(Graph* g) {graphOn = g;}//ONLY USED WHEN LOADING
        Graph* getGraph() {return graphOn;}//ONLY USED WHEN LOADING
        void meshWith(Function* f);//takes all non-interpolatable fields from f and puts them on this
        std::vector<EditFieldMenu> getEditableFields();
        double getSlope(double x);
};

extern std::vector<PointOfInterest*> pointsOfInterest;

class PointOfInterest: public SavableData {
    private:
        Graph* graphOn = NULL;
        Function* functionOn = NULL;
        double px = 0;
        bool visible = true;
        bool taggedForDeletion = false;
        bool showsSlope = false;
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
        std::vector<SaveData> getSaveData();
        std::string getID() {return "Point_Of_Interest";};
        PointOfInterest() {};
        void giveGraph(Graph* g) {graphOn = g;};
        void giveFunction(Function* f) {functionOn = f;}
        std::vector<EditFieldMenu> getEditableFields();
        bool isShowingSlope() {return showsSlope;}
        void showSlope() {showsSlope=true;}
        void dontShowSlope() {showsSlope=false;}
        void toggleSlope() {showsSlope=!showsSlope;}
        void toggleVisibility() {visible=!visible;}
        void draw(Function* f,double pixelToXValRatio,double pixelToYValRatio,double ox,double oy,
        			double s1,double c1,double s2,double c2,SDL_Surface* toReturn,bool axis);
};


#endif /* Function_hpp */
