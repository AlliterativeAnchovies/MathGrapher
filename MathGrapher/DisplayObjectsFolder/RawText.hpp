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
#include "InterpolationWrapper.hpp"

struct RawTextImage {
	double px;
	double py;
	int fontSize;
	std::string actualText;
};

class RawText: public DisplayObject,Counter<RawText> {
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
		double doublesize = 16;
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
		SDL_Surface* draw(double* x,double* y);
		void reclaim(SDL_Surface* reclaimed) {}
		void move(double x,double y) {px+=x;py+=y;}
		std::string getActualText() {return actualText;}
		double getPX() {return px;}
		double getPY() {return py;}
		int getFontSize() {return fontSize;}
		Uint32 getColor() {return theColor;}
		void resizeText(double c) {doublesize+=c;};
		std::vector<EditFieldMenu> getEditableFields();
		std::vector<SaveData> getSaveData();
		RawText() {};
		std::vector<std::string> getValidInterpolations();
		RawText(int x,int y) {
			px = x;
			py = y;
			fontSize = 16;
			doublesize = 16;
			name = "Text "+std::to_string(Counter<RawText>::current_obj);
		}
};

#endif /* RawText_hpp */
