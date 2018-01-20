//
//  RawImage.hpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#ifndef RawImage_hpp
#define RawImage_hpp

#include "DisplayObjects.hpp"
#include "InterpolationWrapper.hpp"

struct RawImageImage {
	double px;
	double py;
	double sx;
	double sy;
};

class RawImage: public DisplayObject,Counter<RawImage> {
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
    double sx = 0;//size
    double sy = 0;
    SDL_Surface* surfaceConcerned = NULL;
    SDL_Surface* origSurf = NULL;//store original surface for scaling and such
    std::string origSurfName = "";
    std::string name = "-IMAGE-";
    RawImageImage image;
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
        std::string getOrigName() {return origSurfName;}
        bool needsResize() {return (int)sx!=surfaceConcerned->w || (int)sy!=surfaceConcerned->h;}
        RawImage(double x,double y,int surfIndex,std::string n);
        SDL_Surface* draw(double* x,double* y);
        void reclaim(SDL_Surface* reclaimed);
        void changeTo(int which);
        void fix();
        void move(double x,double y) {px+=x;py+=y;};
        void resize(double x,double y);
        void resizeSmooth(double x,double y);
		std::vector<EditFieldMenu> getEditableFields();
		std::vector<SaveData> getSaveData();
		RawImage():Counter<RawImage>(false) {};
		std::vector<std::string> getValidInterpolations();
		void handleExtraData(int* curx_,int* cury_,int mouseX,int mouseY,std::vector<MouseClick*> clicked_,
			Uint8* toReturn_,int locx,int locy);
		RawImage(int x,int y);
		Point<double> getCenterOffsetForDrag() {return Point<double>(sx/2,py/2);}
};

#endif /* RawImage_hpp */
