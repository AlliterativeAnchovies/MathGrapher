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
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        //internalFunc function=NULL;
        //internalFunc function2=NULL;//if parametric
        std::vector<double> taylorSeries1 = {0};
        bool parametric = false;
        Function* parafunc1 = NULL;//if parametric;
        Function* parafunc2 = NULL;//if parametric
        //internalRange range;
        std::vector<Point<double>> range = {};//for every input point (a,b), the function is NOT defined on that range
=======
        internalFunc function=NULL;
        internalFunc function2=NULL;//if parametric
        bool parametric = false;
        internalRange range;
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
=======
        internalFunc function=NULL;
        internalFunc function2=NULL;//if parametric
        bool parametric = false;
        internalRange range;
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
=======
        internalFunc function=NULL;
        internalFunc function2=NULL;//if parametric
        bool parametric = false;
        internalRange range;
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
        std::string name = "-FUNCTION-";
        ParsedFile* derivation = NULL;
        bool derived = false;//is a composition of other functions
        bool tagged = false;
        bool hidden = false;
        double stretchx = 1;
        double stretchy = 1;
        double time = 0;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        double taylorSeriesAbout = 0;
        [[deprecated]] std::string stretchxstring = "1";
        [[deprecated]] std::string stretchystring = "1";
=======
        std::string stretchxstring = "1";
        std::string stretchystring = "1";
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
=======
        std::string stretchxstring = "1";
        std::string stretchystring = "1";
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
=======
        std::string stretchxstring = "1";
        std::string stretchystring = "1";
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
        bool visible = true;
        FunctionImage image;
        std::vector<PointOfInterest*> importantPoints = {};
        Graph* graphOn = NULL;
        std::vector<Wrap2<std::string, double>> flags = {};
        bool hasFlag(std::string s);
        double flagValue(std::string s);
    public:
        //basic definition ('degenerate')
        Function(internalFunc f);
        //standard definition
        Function(internalFunc f,internalRange r,std::string n);
        //parametric definition
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        //Function(internalFunc f,internalFunc f2,internalRange r,std::string n);
        Function(std::vector<double> t1);//degenerate definition, use others in general
        Function(std::vector<double> t1,std::vector<Point<double>> r,std::string n,double tsa = 0);
		Function(ParsedFile* d,std::string n);
		Function(Function* f1,Function* f2,Point<double> r,std::string n);//for parametric functions
=======
        Function(internalFunc f,internalFunc f2,internalRange r,std::string n);
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
=======
        Function(internalFunc f,internalFunc f2,internalRange r,std::string n);
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
=======
        Function(internalFunc f,internalFunc f2,internalRange r,std::string n);
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
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
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        static double evalTaylor(std::vector<double> taylor,double pointAt,double tsa);
        void hide() {hidden=true;}
        bool isHidden() {return hidden;}
        void addFlag(Wrap2<std::string,double> f) {flags.push_back(f);}
=======
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
=======
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
=======
>>>>>>> parent of aa7b4bf... Started reworking functions to be defined by taylor series instead of raw code definitions
};

extern std::vector<PointOfInterest*> pointsOfInterest;

class PointOfInterest: public SavableData {
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
        std::vector<SaveData> getSaveData();
        std::string getID() {return "Point_Of_Interest";};
        PointOfInterest() {};
        void giveGraph(Graph* g) {graphOn = g;};
        void giveFunction(Function* f) {functionOn = f;}
        std::vector<EditFieldMenu> getEditableFields();
};

Function* functionFromName(std::string name);//defined in functionwrapper
bool functionExists(std::string name);
Function* functionFromTag(std::string t);//defined in functionwrapper
bool functionExistsFromTag(std::string t);

#endif /* Function_hpp */
