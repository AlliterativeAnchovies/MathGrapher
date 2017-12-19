//
//  Graph.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 13/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <stdio.h>
#include "SDL2/SDL.h"
#include <iostream>
#include <vector>
#include "RenderingUtilities.hpp"

//Some functions that are needed to be declared here to interface well with main.cpp
//they are defined in main
void addGraph(double x,double y);
void addSlider(double x,double y);

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

extern std::vector<PointOfInterest*> pointsOfInterest;

class Interpolation {
    private:
        Uint8 type = NULL_INTERPOLATION;
        double px = 0;
        double py = 0;
        double sx = 0;
        double sy = 0;
        int timeInterval = 1;//time measured in ticks
        int timeAt = 0;
        int timeStart = 0;
        int timeStartCounter = 0;
        bool paused = false;//ignores interpolation until told otherwise
        bool canceled = false;//deletes interpolation
        bool waiting = false;//ignores interpolation for 1 tick
        Graph* relatedGraph = NULL;
        Function* relatedFunction = NULL;
        std::vector<Interpolation*> followups = {};//when interpolation is complete, automatically call these interpolations
    public:
        std::string pxstringforedits = "0";
        std::string pystringforedits = "0";
        bool update(); //returns true if completed interpolation
        void pause();
        void unpause();
        void cancel();
        void wait();
        Interpolation(Uint8 t,double x,double y,int time_interval,Graph* rg);
        void addFollowup(Interpolation* i);
        std::vector<Interpolation*> getFollowups();
        Interpolation* cloneTo(Graph* concernedWith,bool addImmediately = true);
        Interpolation* cloneTo(Interpolation* concernedWith,bool addImmediately = true);
        std::string getDisplay();
        //get px string
        std::string getPXDisplay();
        //get py string
        std::string getPYDisplay();
        //get start string
        std::string getStartDisplay();
        //get duration string
        std::string getDurationDisplay();
        //get type
        Uint8 getType() {return type;}
        //toggle smart move origin
        void toggleSmartMove();
        //change px
        void changePX(double a);
        //change py
        void changePY(double a);
        //change start time
        void changeStart(int a);
        //change duration
        void changeDuration(int a);
        //reset for re-running
        void reset();
        //get start time
        int getStart() {return timeStartCounter;}
        //get duration
        int getDuration() {return timeInterval;}
        //see if canceled
        bool isCanceled() {return canceled;}
        //relates to function
        void relateFunction(Function* f) {relatedFunction = f;}
        //get sx string
        std::string getSXDisplay();
        //get sy string
        std::string getSYDisplay();
        //change sx
        void changeSX(double a) {sx=a;}
        //change sy
        void changeSY(double a) {sy=a;}
        //return true if still working
        bool isActive() {return timeAt<timeInterval&&!paused;}
};

class DisplayObject {
    public:
        //The comments are two options that accomplish the same thing - one gives a compiler error if
        //a function is not implemented, and one (the commented one) turns it into a runtime error.
        //Compiler error is safer, runtime_error better if its not working, you don't know why, and you
        //really really need a working version very soon.
        virtual void highlight() = 0;//{throw std::runtime_error("This object is missing a highlight function!");};
        virtual bool clickedIn(double mouseX,double mouseY) = 0;
            //{throw std::runtime_error("This object is missing a clickedIn function!");};
        virtual std::string getID() = 0;//{throw std::runtime_error("Should not use generic display objects!");};
        virtual std::string getName() = 0;//{throw std::runtime_error("This object is missing a getName function!");};
        virtual void run() =0;//{throw std::runtime_error("This object is missing a run function!");};
        virtual void reset()=0;// {throw std::runtime_error("This object is missing a reset function!");};
        virtual bool isRunning()=0;// {throw std::runtime_error("This object is missing an isRunning function!");};
        virtual void update()=0;// {throw std::runtime_error("This object is missing an update function!");};
};

struct GraphImage {
    //stores starting information for graphs
    //so they can reset after running.
    double px = 0;
    double py = 0;
    double sx = 0;
    double sy = 0;
    double ox = 0;
    double oy = 0;
    double gridSpacingX = 10;
    double gridSpacingY = 10;
    double gridAngleX = M_PI/2;
    double gridAngleY = 0;
};

class Graph: public DisplayObject {
    private:
        GraphImage image;
        double px = 0;
        double py = 0;
        double sx = 0;
        double sy = 0;
        double ox = 0;
        double oy = 0;
        double gridSpacingX = 10;
        double gridSpacingY = 10;
        double gridAngleX = M_PI/2;
        double gridAngleY = 0;
        std::string name = "-NONAME-";
        //stores current interpolation data
        std::vector<Interpolation*> interpolations = {};
        //all functions it should draw
        std::vector<Function*> functions = {};
        std::vector<Function*> yfunctions = {};
        //some asthetic choices:
        bool showGrid = true;
        bool showAxes = true;
        //tagged for running interpolations
        bool running = false;
    public:
        //full constructor
        Graph(double x,double y,double sizex,double sizey,double grid_spacing_x,double grid_spacing_y,double grid_angle_x,double grid_angle_y,std::string n="-NONAME-");
        //degenerate constructors for ease of use
        Graph(double x,double y,double sizex,double sizey,std::string n="-NONAME-");
        Graph(double x,double y,double sizex,double sizey,double grid_spacing_x,double grid_spacing_y,
            std::string n="-NONAME-");
        //moves incrementally
        void move(double x,double y);
        //moves smoothly
        Interpolation* smoothMove(double x,double y,int timeInterval,bool doNow = true);
        //moves immediately
        void changePosition(double x,double y);
        //returns surface of drawn graph, and stores its position in the input pointers
        SDL_Surface* draw(double* x,double* y);
        //updates the Graph so it can smoothly animate things
        void update();
        //moves the origin incrementally
        void moveOrigin(double x,double y);
        //moves origin smoothly
        Interpolation* smoothMoveOrigin(double x,double y,int timeInterval,bool doNow = true);
        //moves origin immediately
        void changeOrigin(double x,double y);
        //changes grid angle immediately
        void changeGridAngle(double x,double y);
        //changes grid angle incrementally
        void moveGridAngle(double x,double y);
        //changes grid angle smoothly
        Interpolation* smoothMoveGridAngle(double x,double y,int timeInterval,bool doNow = true);
        //changes grid scale immediately
        void changeGridScale(double x,double y);
        //changes grid scale incrementally
        void moveGridScale(double x,double y);
        //changes grid scale smoothly
        Interpolation* smoothMoveGridScale(double x,double y,int timeInterval,bool doNow = true);
        //resizes grid immediately
        void resizeGrid(double x,double y,bool moveCenter = true);
        //resizes grid incrementally
        void moveGridSize(double x,double y,bool moveCenter = true);
        //resizes grid smoothly
        Interpolation* smoothMoveGridSize(double x,double y,int timeInterval,bool moveCenter = true,bool doNow = true);
        //add a function to draw
        void addXFunction(Function* function);
        void addYFunction(Function* function);
        //add an interpolation
        void addInterpolation(Interpolation* i);
        //check if clicked on graph
        bool clickedIn(double mouseX,double mouseY);
        //draws rect around graph to indicate it is selected
        void highlight();
        //get name of graph
        std::string getName();
        //change name of graph
        void changeName(std::string newname);
        //get position
        Point<double> getPosition();
        //get size
        Point<double> getSize();
        //get grid scale
        Point<double> getGridScale();
        //get grid angles
        Point<double> getGridAngle();
        //get grid origin
        Point<double> getOrigin();
        //get x functions
        std::vector<Function*> getXFunctions();
        //get y functions
        std::vector<Function*> getYFunctions();
        //get rid of tagged functions
        void cleanFunctions();
        //toggle showing grid
        void toggleGrid() {showGrid=!showGrid;}
        //toggle showing axes
        void toggleAxes() {showAxes=!showAxes;}
        //get showing grid
        bool showingGrid() {return showGrid;}
        //get showing axes
        bool showingAxes() {return showAxes;}
        //get interpolations
        std::vector<Interpolation*> getInterpolations() {return interpolations;}
        //run graph interpolations
        void run();
        //resets graph interpolations
        void reset();
        //checks if graph is running
        bool isRunning();
        //gets rid of canceled interpolations
        void cleanInterpolations();
        //get name of class
        std::string getID() {return "Graph";}
};

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
        void setStretchX(std::string s) {stretchxstring = s;stretchx=numberFromString(s);}
        void setStretchY(std::string s) {stretchystring = s;stretchy=numberFromString(s);}
        void setTime(double t) {time = t;}
        bool isVisible() {return visible;}
        void toggleVisibility() {visible=!visible;}
        void stretch(double x,double y) {stretchx+=x;stretchy+=y;}
        void run(double x) {time+=x;}
        void saveImage();
        void addPoint(PointOfInterest* p) {importantPoints.push_back(p);};
        typeof(importantPoints) getImportantPoints() {return importantPoints;}
        bool isParametric() {return parametric;}
};

class PointOfInterest {
    private:
        Graph* graphOn = NULL;
        Function* functionOn = NULL;
        double px = 0;
        bool visible = true;
    public:
        std::string getDisplay();
        PointOfInterest(Graph* g,Function* f,double d,bool v);
        bool isVisible() {return visible;}
        std::string getDisplayLocation();
        std::string getDisplayPoint();
        double getPX() {return px;}
        double getPY() {return (functionOn->isParametric())?functionOn->parametricEval(px).y:(*functionOn)(px);}
};

struct SliderImage {
    double px;
    double py;
    double size;
    double angle;
    Function* incrementFunction;
    int tickAmount;
};

class Slider: public DisplayObject {
    private:
        double px = 0;
        double py = 0;
        double size = 0;
        double angle = 0;
        std::string name = "-SLIDER-";
        bool highlighted = false;
        double storedsx = 0;
        double storedsy = 0;
        Function* incrementFunction = NULL;
        int tickAmount = 4;
        bool running = false;
        SliderImage image;
        double pointery = 0;
        PointOfInterest* pointConcerned = NULL;
        std::string startingYString = "0";
    public:
        //creates default slider at (x,y) with size s
        Slider(double x,double y,double s,std::string n);
        //draws slider and stores (px,py) in (x,y)
        SDL_Surface* draw(double* x,double* y);
        //tags for highlightion
        void highlight() {highlighted = true;}
        bool clickedIn(double x,double y);
        std::string getName() {return name;}
        void run();
        Point<double> getPosition() {return Point<double>(px,py);}
        double getSize() {return size;}
        void changeName(std::string n) {name = n;}
        void changePX(double x) {px = x;}
        void changePY(double y) {py = y;}
        void changeSize(double s) {size = s;}
        double getAngle() {return angle;}
        void changeAngle(double x) {angle = x;}
        double getStartingY() {return pointery;}
        void changeStartingY(double s) {pointery = s;}
        void changeStartingYString(std::string s) {startingYString = s;pointery = numberFromString(s);}
        std::string getStartingYString() {return startingYString;}
        int getTicks() {return tickAmount;}
        void setTicks(int t) {tickAmount = t;}
        Function* getFunction() {return incrementFunction;}
        void setFunction(Function* f) {incrementFunction = f;}
        PointOfInterest* getPointConcerned() {return pointConcerned;}
        void setPointConcerned(PointOfInterest* p) {pointConcerned=p;}
        //get name of class
        std::string getID() {return "Slider";}
        void update() {};
        void reset() {};
        bool isRunning() {return running;};
};

Uint32 getColorOfInterpolation(Interpolation* i);
std::string stringifyID(Uint8 id);

#endif /* Graph_hpp */
