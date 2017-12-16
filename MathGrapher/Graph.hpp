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
    DELAY,
};

class Graph;
class Interpolation;
class Function;

class Interpolation {
    private:
        Uint8 type = NULL_INTERPOLATION;
        double px = 0;
        double py = 0;
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

class Graph {
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
};

typedef std::function<double(double,double,double,double)> internalFunc;

struct FunctionImage {
    double stretchx;
    double stretchy;
    double time;
    bool visible;
};

class Function {
    private:
        internalFunc function;
        std::function<bool(double,double)> range;
        std::string name = "-FUNCTION-";
        bool tagged = false;
        double stretchx = 1;
        double stretchy = 1;
        double time = 0;
        std::string stretchxstring = "1";
        std::string stretchystring = "1";
        bool visible = true;
        FunctionImage image;
    public:
        Function(internalFunc f);
        Function(internalFunc f,std::function<bool(double,double)> r,std::string n);
        double eval(double x);
        double operator() (double x);
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
};

Uint32 getColorOfInterpolation(Interpolation* i);
std::string stringifyID(Uint8 id);

#endif /* Graph_hpp */
