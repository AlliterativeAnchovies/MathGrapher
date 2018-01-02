//
//  ChooseFunctionInterpolation.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "ChooseFunctionInterpolation.hpp"

Uint8 ChooseFunctionInterpolation::handle(double mouseX,double mouseY,bool clicked) {
	Uint8 toReturn = 0x00;
    clicked = clicked&&successfulRaycast&&!locked;
	
	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	drawText("Interpolations", 22, px, py, 0xff000000);
	double cury = py+30;
	int runsx,runsy,stretchsx,stretchsy;
	TTF_SizeUTF8((*fontgrab)(16),"Run",&runsx,&runsy);
	TTF_SizeUTF8((*fontgrab)(16),"Stretch",&stretchsx,&stretchsy);
	drawTextWithBackground("Run", 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+runsx, cury, cury+runsy)) {
		createPopup(CREATE_FUNCTION_RUN_INTERPOLATION, mouseX, mouseY)
			->concernWith(graphConcerned)
			->concernWith(std::string("Run"))
			->concernWith(functionConcerned)
			->setUpInterpolation();
		clicked = false;
		toReturn = 0x00;
	}
	drawTextWithBackground("Stretch", 16, px+5+runsx+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+5+runsx+5, px+5+runsx+5+stretchsx, cury, cury+stretchsy)) {
		createPopup(CREATE_SIMPLE_INTERPOLATION, mouseX, mouseY)
			->concernWith(graphConcerned)
			->concernWith(std::string("Stretch"))
			->concernWith(functionConcerned)
			->setUpInterpolation();
		clicked = false;
		toReturn = 0x00;
	}
	
    return toReturn;
}
