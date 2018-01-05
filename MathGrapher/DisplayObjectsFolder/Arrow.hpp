//
//  Arrow.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 05/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#ifndef Arrow_hpp
#define Arrow_hpp

#include "DisplayObjects.hpp"

class Arrow: public DisplayObject {
	private:
		double px = 0;
        double py = 0;
        double length = 0;
        double angle = 0;
        double thickness = 0;
        double headAngle = M_PI/4;
        double headSize = 0;
        std::string name = "-ERROR-";
        //stores current interpolation data
        std::vector<Interpolation*> interpolations = {};
        //tagged for running interpolations
        bool running = false;
        //highlitations
        bool highlighted = false;
        Uint32 color = 0xff000000;
    public:
		void highlight() {highlighted = true;};
		bool clickedIn(double mouseX,double mouseY);
		std::string getID() {return "Arrow";};
		std::string getName() {return name;};
		void run();
		void reset();
		bool isRunning() {return running;};
		void update();
		SDL_Surface* draw(double* x,double* y);
		void reclaim(SDL_Surface* reclaimed) {SDL_FreeSurface(reclaimed);};
		void move(double x,double y) {px+=x;py+=y;};
		void addInterpolation(Interpolation* i) {interpolations.push_back(i);};
		std::vector<Interpolation*> getInterpolations() {return interpolations;};
		Arrow(double x,double y,double s,double t,double hS,double a,double hA,std::string n);
		std::string* ptmName() {return &name;}
		double getPX() {return px;}
		double getPY() {return py;}
		double* ptmPX() {return &px;}
		double* ptmPY() {return &py;}
};

#endif /* Arrow_hpp */
