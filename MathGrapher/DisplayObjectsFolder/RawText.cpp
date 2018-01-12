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
	doublesize = fontSize;
}

SDL_Surface* RawText::draw(double* x,double* y) {
	if (!visible) {*x=0;*y=0;return createBlankSurfaceWithSize(0, 0);}
	*x = px;
	*y = py;
	SDL_Surface* toReturn = getTextSurface(actualText, doublesize, px, py, theColor);
	if (highlighted) {
		SDL_Surface* highlight = createBlankSurfaceWithSize(toReturn->w,toReturn->h);
		SDL_Surface* temp = SDL_ConvertSurface(toReturn, gScreenSurface->format, NULL);
		SDL_FreeSurface(toReturn);
		toReturn = temp;
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
	doublesize = fontSize;
}
void RawText::reset() {
	running = false;
	px = image.px;
	py = image.py;
	visible = true;
	fontSize = image.fontSize;
	actualText = image.actualText;
	doublesize = fontSize;
    for (int i = 0;i<interpolations.size();i++) {
        interpolations[i]->reset();
    }
}

std::vector<EditFieldMenu> RawText::getEditableFields() {
	return {
		{"",&name,_STRING,24,true},
		{"PX: ",&px,_DOUBLE,20,false},
		{"PY: ",&py,_DOUBLE,20,true},
		{"Display Text: ",&actualText,_STRING,20,true},
		{"Font Size: ",&fontSize,_INT,20,true},
		{"Color: ",&theColor,_HEXADECIMAL,20,true}
	};
};

std::vector<SaveData> RawText::getSaveData() {
	return {
		{"Name",&name,_STRING},
		{"PX",&px,_DOUBLE},
		{"PY",&py,_DOUBLE},
		{"Font_Size",&fontSize,_INT},
		{"Text",&actualText,_STRING},
		{"Color",&theColor,_HEXADECIMAL},
		{"Interpolations",&interpolations,_VECTOR}
	};
};

std::vector<std::string> RawText::getValidInterpolations() {
	return {"Move","Disappear","Appear","Rescale Text"};
}

RawText::RawText(int x,int y) {
	px = x;
	py = y;
	fontSize = 16;
	doublesize = 16;
	name = "Text "+std::to_string(Counter<RawText>::current_obj);
}
