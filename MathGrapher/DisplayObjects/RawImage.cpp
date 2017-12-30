//
//  RawImage.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 29/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "RawImage.hpp"

void RawImage::run() {
	running = true;
	image.px = px;
	image.py = py;
	image.sx = sx;
	image.sy = sy;
}
void RawImage::update() {
	for (int i = 0;i<interpolations.size();i++) {
        if (interpolations[i]->update()) {
            std::vector<Interpolation*> followups = interpolations[i]->getFollowups();
            for (int j = 0;j<followups.size();j++) {
                followups[j]->wait();
                interpolations.push_back(followups[j]);
            }
            interpolations[i]->pause();
        }
    }
}
void RawImage::reset() {
	running = false;
	px = image.px;
	py = image.py;
	sx = image.sx;
	sy = image.sy;
    for (int i = 0;i<interpolations.size();i++) {
        interpolations[i]->reset();
    }
}

void RawImage::fix() {
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

void RawImage::changeTo(int which) {
    //change this image to a different one
    //we keep the size the same though, so
    //it'll probably be squishified
    SDL_FreeSurface(surfaceConcerned);
    surfaceConcerned = createBlankSurfaceWithSize(sx, sy);
    SDL_BlitScaled(gSurfaces[which],NULL,surfaceConcerned,NULL);
    origSurf = gSurfaces[which];
    origSurfName = gStrings[which];
}

void RawImage::reclaim(SDL_Surface* reclaimed) {
    //check is surface returned by draw() should be deleted or not
    if (reclaimed!=surfaceConcerned) {SDL_FreeSurface(reclaimed);}
}

SDL_Surface* RawImage::draw(double* x,double* y) {
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

RawImage::RawImage(double x,double y,int surfIndex,std::string n) {
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
