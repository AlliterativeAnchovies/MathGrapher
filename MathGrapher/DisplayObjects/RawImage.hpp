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

#endif /* RawImage_hpp */
