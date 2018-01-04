//
//  RawText.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 30/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "RawText.hpp"

RawText::RawText(double x,double y,int fsize,std::string tobename) {
	px = x;
	py = y;
	fontSize = fsize;
	name = tobename;
}

SDL_Surface* RawText::draw(double* x,double* y) {
	*x = px;
	*y = py;
	SDL_Surface* toReturn = getTextSurface(actualText, fontSize, px, py, theColor);
	if (highlighted) {
		SDL_Surface* highlight = createBlankSurfaceWithSize(toReturn->w,toReturn->h);
		SDL_FillRect(highlight, NULL, 0x6600ff00);
		SDL_BlitSurface(highlight,NULL,toReturn,NULL);
		SDL_FreeSurface(highlight);
	}
	highlighted = false;
	return toReturn;
}

void RawText::run() {
	running = true;
	image.px = px;
	image.py = py;
	image.fontSize = fontSize;
	image.actualText = actualText;
}
void RawText::update() {
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
void RawText::reset() {
	running = false;
	px = image.px;
	py = image.py;
	fontSize = image.fontSize;
	actualText = image.actualText;
    for (int i = 0;i<interpolations.size();i++) {
        interpolations[i]->reset();
    }
}
