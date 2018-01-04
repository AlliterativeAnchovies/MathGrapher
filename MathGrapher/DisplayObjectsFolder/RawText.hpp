//
//  RawText.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 30/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef RawText_hpp
#define RawText_hpp

#include "DisplayObjects.hpp"
#include "Interpolation.hpp"

struct RawTextImage {
	double px;
	double py;
	int fontSize;
	std::string actualText;
};

class RawText: public DisplayObject {
	private:
		bool highlighted = false;
		bool running = false;
		std::string name = "-ERROR-";
		double px = 0;
		double py = 0;
		int fontSize = 16;
		Uint32 theColor = 0xff000000;
		std::string actualText = "Default";
		RawTextImage image;
		std::vector<Interpolation*> interpolations = {};
	public:
		RawText(double x,double y,int fsize,std::string tobename);
		void highlight() {highlighted = true;};
		bool clickedIn(double mouseX,double mouseY) {
			int w,h;
			TTF_SizeUTF8((*fontgrab)(fontSize), actualText.c_str(), &w, &h);
			return pointInBounds(mouseX,mouseY,px,px+w,py,py+h);
		};
		std::string getID() {return "Text";};
		std::string getName() {return name;};
		void run();
		void reset();
		bool isRunning() {return running;};
		void update();
		SDL_Surface* draw(double* x,double* y);
		void reclaim(SDL_Surface* reclaimed) {}
		void move(double x,double y) {px+=x;py+=y;}
		void addInterpolation(Interpolation* i) {interpolations.push_back(i);}
		std::vector<Interpolation*> getInterpolations() {return interpolations;};
		std::string getActualText() {return actualText;}
		double getPX() {return px;}
		double getPY() {return py;}
		int getFontSize() {return fontSize;}
		Uint32 getColor() {return theColor;}
		std::string* ptmName() {return &name;}
		double* ptmPX() {return &px;}
		double* ptmPY() {return &py;}
		int* ptmFontSize() {return &fontSize;}
		std::string* ptmActualText() {return &actualText;}
		Uint32* ptmColor() {return &theColor;}
	
};

#endif /* RawText_hpp */
