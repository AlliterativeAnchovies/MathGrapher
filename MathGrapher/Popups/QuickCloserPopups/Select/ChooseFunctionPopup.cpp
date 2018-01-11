//
//  ChooseFunctionPopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "ChooseFunctionPopup.hpp"

Uint8 ChooseFunctionPopup::handle(double mouseX,double mouseY) {
	Uint8 toReturn = 0x00;
    MouseClick clicked = prepareMouse(&leftMouseReleased);

	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	drawText("Choose Function", 22, px, py, 0xff000000);
	double cury = py+30;
	Graph* graphConcerned = (Graph*)getConcernation<Graph*>();
	Slider* sliderConcerned = (Slider*)getConcernation<Slider*>();
	bool boolConcerned = (bool)getConcernation<bool>();
	
	for (int i = 0;i<builtins.size();i++) {
		int w,h;
		TTF_SizeUTF8((*fontgrab)(16),builtins[i].x.c_str(),&w,&h);
		drawTextWithBackground(builtins[i].x, 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
		if (clicked.status()&&pointInBounds(mouseX, mouseY, px+5, px+5+w, cury, cury+h)) {
			clicked.unclick();
			toReturn = 0x02;
			if (graphConcerned!=NULL) {
				if (boolConcerned==X_AXIS) {graphConcerned->addXFunction(builtins[i].y);}
				else                       {graphConcerned->addYFunction(builtins[i].y);}
			}
			else if (sliderConcerned!=NULL) {
				sliderConcerned->setFunction(builtins[i].y);
			}
		}
		cury+=h+5;
		
	}
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
		toReturn = 0x01;
	}
	return toReturn;
}
