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
//#include "SDL2/SDL.h"
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
        //get values for editing
        double getPX() {return px;}
        double* ptmPX() {return &px;}
        double getPY() {return py;}
        double* ptmPY() {return &py;}
        double getSX() {return sx;}
        double* ptmSX() {return &sx;}
        double getSY() {return sy;}
        double* ptmSY() {return &sy;}
        int* ptmStart() {return &timeStartCounter;}
        int* ptmDuration() {return &timeInterval;}
};

class DisplayObject {
    public:
        //The comments are two options that accomplish the same thing - one gives a compiler error if
        //a function is not implemented, and one (the commented one) turns it into a runtime error.
        //Compiler error is safer, runtime_error better if you're implementing a new type of DisplayObject
        //step-by-step and haven't implemented all the functions yet.
        //IF YOU GET A "MISSING VTABLE" ERROR, it'll be caused by you not having defined all of these functions
        //in the child object (although the error only seems to happen if you give your class a nondefault constructor...
        virtual void highlight() = 0;//{throw std::runtime_error("This object is missing a highlight function!");};
        virtual bool clickedIn(double mouseX,double mouseY) = 0;
            //{throw std::runtime_error("This object is missing a clickedIn function!");};
        virtual std::string getID() = 0;//{throw std::runtime_error("Should not use generic display objects!");};
        virtual std::string getName() = 0;//{throw std::runtime_error("This object is missing a getName function!");};
        virtual void run() =0;//{throw std::runtime_error("This object is missing a run function!");};
        virtual void reset()=0;// {throw std::runtime_error("This object is missing a reset function!");};
        virtual bool isRunning()=0;// {throw std::runtime_error("This object is missing an isRunning function!");};
        virtual void update()=0;// {throw std::runtime_error("This object is missing an update function!");};
        virtual ~DisplayObject() {};//in a class C, the method ~C is the destructor of C.  Usually implicitly
                                    //defined, but if the class is virtual we also need to give it a virtual
                                    //destructor.  The function body is empty which means we just use the standard
                                    //deletion method. (if it were full of an ordered set of instructions A, the
                                    //object would be deleted by instructions A union S where S is the standard
                                    //deletion method).
        virtual SDL_Surface* draw(double* x,double* y)=0;//{throw std::runtime_error("This object is missing a draw function!");}
        virtual void reclaim(SDL_Surface* reclaimed)=0;//{throw std::runtime_error("This object is missing a reclaim function!");}
};

class RawImage: public DisplayObject {
    //If this class turns out to be an efficiency drain, there are numerous fixes:
    //Firstly, save 2 surfaces, one highlighted and one not, so that we don't have
    //to re-calculate the highlitations each frame.
    //Secondly, save the textures of the surfaces as well.  I didn't do this because draw()
    //returns a surface, so to do a texture I'd have to go through and change all of them
    //to return textures.  There's nothing wrong with this, I just am lazy.  Although I
    //probably could have done that in the time it took to write this comment.  Comments
    //are more fun though.
    bool running = false;
    bool highlighted = false;
    double px = 0;
    double py = 0;
    double sx = 0;
    double sy = 0;
    SDL_Surface* surfaceConcerned = NULL;
    SDL_Surface* origSurf = NULL;//store original surface for scaling and such
    std::string origSurfName = "";
    std::string name = "-IMAGE-";
    public:
        double getPX() {return px;}
        double getPY() {return py;}
        double getSX() {return sx;}
        double getSY() {return sy;}
        SDL_Surface* getSurface() {return surfaceConcerned;}
        bool isRunning() {return running;}
        bool isHighlighted() {return highlighted;}
        void highlight() {highlighted=true;}
        bool clickedIn(double mouseX,double mouseY) {return   mouseX>=px&&mouseX<=px+sx
                                                            &&mouseY>=py&&mouseY<=py+sy;}
        std::string getID() {return "Image";}
        std::string getName() {return name;}
        void run();
        void reset();
        void update();
        std::string* ptmName() {return &name;}
        double* ptmPX() {return &px;}
        double* ptmPY() {return &py;}
        double* ptmSX() {return &sx;}
        double* ptmSY() {return &sy;}
        std::string getOrigName() {return origSurfName;}
        bool needsResize() {
            return (int)sx!=surfaceConcerned->w || (int)sy!=surfaceConcerned->h;
        }
        RawImage(double x,double y,int surfIndex,std::string n) {
            px = x;py = y;sx = gSurfaces[surfIndex]->w;sy = gSurfaces[surfIndex]->h;name = n;
            //scale image down to a reasonable starting height
            double scalar = (sx>200)?200/sx:1;
            sx*=scalar;
            sy*=scalar;
            scalar = (sy>200)?200/sy:1;
            sx*=scalar;
            sy*=scalar;
            //copy input surface
            surfaceConcerned = createBlankSurfaceWithSize(sx, sy);
            SDL_BlitScaled(gSurfaces[surfIndex],NULL,surfaceConcerned,NULL);
            origSurf = gSurfaces[surfIndex];
            origSurfName = gStrings[surfIndex];
        }
        SDL_Surface* draw(double* x,double* y) {
            //check if should resize
            if (needsResize()) {
                SDL_Surface* temp = createBlankSurfaceWithSize(sx, sy);
                SDL_BlitScaled(surfaceConcerned,NULL,temp,NULL);
                SDL_FreeSurface(surfaceConcerned);
                surfaceConcerned = temp;
            }
            *x = px;
            *y = py;
            if (highlighted) {
                auto texturethingy = createBlankSurfaceWithSize(sx, sy);
                SDL_FillRect(texturethingy, NULL, 0x6600ff00);
                auto copy = SDL_ConvertSurface(surfaceConcerned, gScreenSurface->format, NULL);
                SDL_BlitSurface(texturethingy,NULL,copy,NULL);
                highlighted=false;
                SDL_FreeSurface(texturethingy);
                return copy;
            }
            else {return surfaceConcerned;}
        }
        void reclaim(SDL_Surface* reclaimed) {
            //check is surface returned by draw() should be deleted or not
            if (reclaimed!=surfaceConcerned) {SDL_FreeSurface(reclaimed);}
        }
        void changeTo(int which) {
            //change this image to a different one
            //we keep the size the same though, so
            //it'll probably be squishified
            SDL_FreeSurface(surfaceConcerned);
            surfaceConcerned = createBlankSurfaceWithSize(sx, sy);
            SDL_BlitScaled(gSurfaces[which],NULL,surfaceConcerned,NULL);
            origSurf = gSurfaces[which];
            origSurfName = gStrings[which];
        }
        void fix() {
            //"fixing" the image is defined as making it
            //have the same resolution as its base image,
            //while preserving its area.
            
            //first we find its current area and original area
            double curarea = sx*sy;
            double origarea = origSurf->w*origSurf->h;
            //then find the factor we multiply by to scale origarea to curarea
            double scalefactor = curarea/origarea;
            //but we need to square root this to find the scaling factor
            //for each side (rather than the whole area)
            scalefactor = sqrt(scalefactor);
            //find new sizes:
            sx = origSurf->w*scalefactor;
            sy = origSurf->h*scalefactor;
            //free the old surface
            SDL_FreeSurface(surfaceConcerned);
            //and replace it with the new one!
            surfaceConcerned = createBlankSurfaceWithSize(sx, sy);
            SDL_BlitScaled(origSurf,NULL,surfaceConcerned,NULL);
            //and ta-didly-da, we're done!
        }
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
        //custom deleter to clean up constituents
        ~Graph();
        //pointers (used by value editors)
        std::string* ptmName() {return &name;}
        double* ptmPX() {return &px;}
        double* ptmPY() {return &py;}
        double* ptmSX() {return &sx;}
        double* ptmSY() {return &sy;}
        double* ptmOX() {return &ox;}
        double* ptmOY() {return &oy;}
        double* ptmGridSpacingX() {return &gridSpacingX;}
        double* ptmGridSpacingY() {return &gridSpacingY;}
        double* ptmGridAngleX() {return &gridAngleX;}
        double* ptmGridAngleY() {return &gridAngleY;}
        void reclaim(SDL_Surface* reclaimed) {
            //check is surface returned by draw() should be deleted or not
            SDL_FreeSurface(reclaimed);
        }
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
        decltype(importantPoints) getImportantPoints() {return importantPoints;}
        bool isParametric() {return parametric;}
        //custom deleter to clean up points of interest
        ~Function();
        double* ptmTime() {return &time;}
        double* ptmStretchX() {return &stretchx;}
        double* ptmStretchY() {return &stretchy;}
};

class PointOfInterest {
    private:
        Graph* graphOn = NULL;
        Function* functionOn = NULL;
        double px = 0;
        bool visible = true;
        bool taggedForDeletion = false;
    public:
        std::string getDisplay();
        PointOfInterest(Graph* g,Function* f,double d,bool v);
        bool isVisible() {return visible;}
        std::string getDisplayLocation();
        std::string getDisplayPoint();
        double getPX() {return px;}
        double getPY() {return (functionOn->isParametric())?functionOn->parametricEval(px).y:(*functionOn)(px);}
        bool shouldDelete() {return taggedForDeletion;}
        void prepareForDelete() {taggedForDeletion=true;}
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
        std::string* ptmName() {return &name;}
        double* ptmPX() {return &px;}
        double* ptmPY() {return &py;}
        double* ptmSize() {return &size;}
        double* ptmAngle() {return &angle;}
        int* ptmTicks() {return &tickAmount;}
        double* ptmStartingY() {return &pointery;}
        void reclaim(SDL_Surface* reclaimed) {
            //check is surface returned by draw() should be deleted or not
            SDL_FreeSurface(reclaimed);
        }
};

Uint32 getColorOfInterpolation(Interpolation* i);
std::string stringifyID(Uint8 id);

#endif /* Graph_hpp */
