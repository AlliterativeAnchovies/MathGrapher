//
//  Graph.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 13/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp

#include "DisplayObjects.hpp"
#include "Function.hpp"
#include "InterpolationWrapper.hpp"


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
        bool visible = true;
    public:
        //full constructor
        Graph(double x,double y,double sizex,double sizey,double grid_spacing_x,double grid_spacing_y,double grid_angle_x,double grid_angle_y,std::string n="-NONAME-");
        //degenerate constructors for ease of use
        Graph(double x,double y,double sizex,double sizey,std::string n="-NONAME-");
        Graph(double x,double y,double sizex,double sizey,double grid_spacing_x,double grid_spacing_y,
            std::string n="-NONAME-");
		Graph() {};
        //moves incrementally
        void move(double x,double y);
        //moves smoothly
        [[deprecated]] Interpolation* smoothMove(double x,double y,int timeInterval,bool doNow = true);
        //moves immediately
        void changePosition(double x,double y);
        //returns surface of drawn graph, and stores its position in the input pointers
        SDL_Surface* draw(double* x,double* y);
        //moves the origin incrementally
        void moveOrigin(double x,double y);
        //moves origin smoothly
        [[deprecated]] Interpolation* smoothMoveOrigin(double x,double y,int timeInterval,bool doNow = true);
        //moves origin immediately
        void changeOrigin(double x,double y);
        //changes grid angle immediately
        void changeGridAngle(double x,double y);
        //changes grid angle incrementally
        void moveGridAngle(double x,double y);
        //changes grid angle smoothly
        [[deprecated]] Interpolation* smoothMoveGridAngle(double x,double y,int timeInterval,bool doNow = true);
        //changes grid scale immediately
        void changeGridScale(double x,double y);
        //changes grid scale incrementally
        void moveGridScale(double x,double y);
        //changes grid scale smoothly
        [[deprecated]] Interpolation* smoothMoveGridScale(double x,double y,int timeInterval,bool doNow = true);
        //resizes grid immediately
        void resizeGrid(double x,double y,bool moveCenter = true);
        //resizes grid incrementally
        void moveGridSize(double x,double y,bool moveCenter = true);
        //resizes grid smoothly
        [[deprecated]] Interpolation* smoothMoveGridSize(double x,double y,int timeInterval,bool moveCenter = true,bool doNow = true);
        //add a function to draw
        void addXFunction(Function* function);
        void addYFunction(Function* function);
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
        //run graph interpolations
        void run();
        //resets graph interpolations
        void reset();
        //checks if graph is running
        bool isRunning();
        //get name of class
        std::string getID() {return "Graph";}
        //custom deleter to clean up constituents
        ~Graph();
        void reclaim(SDL_Surface* reclaimed) {
            //check is surface returned by draw() should be deleted or not
            SDL_FreeSurface(reclaimed);
        }
        void addXFunction_nocopy(Function* f) {functions.push_back(f);}
		void addYFunction_nocopy(Function* f) {yfunctions.push_back(f);}
		void makeInvisible() {visible=false;};
		void makeVisibile() {visible=true;};
		std::vector<EditFieldMenu> getEditableFields();
		std::vector<SaveData> getSaveData();
		std::vector<std::string> getValidInterpolations();
};




#endif /* Graph_hpp */
