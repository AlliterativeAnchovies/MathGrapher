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
#include "InterpolationWrapper.hpp"

struct ArrowImage {
	double px = 0;
	double py = 0;
	double length = 0;
	double angle = 0;
	double thickness = 0;
	double headAngle = M_PI/4;
	double headSize = 0;
	Uint32 color = 0xff000000;
};

class Arrow: public DisplayObject,Counter<Arrow> {
	private:
		double px = 0;
        double py = 0;
        double length = 0;
        double angle = 0;
        double thickness = 0;
        double headAngle = M_PI/4;
        double headSize = 0;
        std::string name = "-ERROR-";
        //tagged for running interpolations
        bool running = false;
        //highlitations
        bool highlighted = false;
        Uint32 color = 0xff000000;
        ArrowImage image;
    public:
		void highlight() {highlighted = true;};
		bool clickedIn(double mouseX,double mouseY);
		std::string getID() {return "Arrow";};
		std::string getName() {return name;};
		void run();
		void reset();
		bool isRunning() {return running;};
		SDL_Surface* draw(double* x,double* y);
		void reclaim(SDL_Surface* reclaimed) {SDL_FreeSurface(reclaimed);};
		void move(double x,double y) {px+=x;py+=y;};
		Arrow(double x,double y,double s,double t,double hS,double a,double hA,std::string n);
		double getPX() {return px;}
		double getPY() {return py;}
		double getLength() {return length;}
		double getThickness() {return thickness;}
		double getHeadSize() {return headSize;}
		double getAngle() {return angle;}
		double getHeadAngle() {return headAngle;}
		Uint32 getColor() {return color;}
		void resizeSmooth(double lengthincrease,double thickincrease);
		void rescaleHead(double headsizechange);
		void changeColor(Uint32 nc) {color = nc;};
		std::vector<EditFieldMenu> getEditableFields();
		std::vector<SaveData> getSaveData();
		Arrow() {};
		Arrow(int x,int y) {
			px = x;
			py = y;
			length = 100;
			thickness = 20;
			headSize = 30;
			angle = 0;
			headAngle = M_PI/4;
			name ="Arrow "+std::to_string(Counter<Arrow>::current_obj);
		};
		std::vector<std::string> getValidInterpolations();
};

#endif /* Arrow_hpp */
