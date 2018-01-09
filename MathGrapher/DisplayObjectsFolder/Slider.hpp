//
//  Slider.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef Slider_hpp
#define Slider_hpp

#include "DisplayObjects.hpp"
#include "InterpolationWrapper.hpp"
#include "Function.hpp"

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
        double pointery = 0;//starting height of pointer
        PointOfInterest* pointConcerned = NULL;
        std::string startingYString = "0";
        //stores current interpolation data
        std::vector<Interpolation*> interpolations = {};
        double offx_stored = 0;//stored offsets to do collision code
        double offy_stored = 0;//correctly
        bool visible = true;
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
        void reset();
        bool isRunning() {return running;};
        void reclaim(SDL_Surface* reclaimed) {
            //check is surface returned by draw() should be deleted or not
            SDL_FreeSurface(reclaimed);
        }
        void move(double x,double y) {px+=x;py+=y;};
		Function* getTickFunction() {return incrementFunction;}
		Slider() {};
		void makeInvisible() {visible=false;};
		void makeVisibile() {visible=true;};
		std::vector<EditFieldMenu> getEditableFields();
		std::vector<SaveData> getSaveData();
		int tagForLoading = -1;
};


#endif /* Slider_hpp */
