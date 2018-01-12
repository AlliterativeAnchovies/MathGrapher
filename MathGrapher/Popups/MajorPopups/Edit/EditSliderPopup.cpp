//
//  EditSliderPopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "EditSliderPopup.hpp"

Uint8 EditSliderPopup::handle(double mouseX,double mouseY) {
	Uint8 toReturn = 0x00;
    MouseClick clicked = prepareMouse(&leftMouseReleased);
	
	drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
	bool clickedEdit = false;
	int curx = px+5;
	int cury = py+5;
	int offy;
	
	Slider* sliderConcerned = (Slider*)getConcernation<Slider*>();
	
	//Edit fields
	clickedEdit = handleEditableInfo(curx, cury, sliderConcerned, &clicked, mouseX, mouseY, &offy);
	cury+=offy;
	
	sliderConcerned->handleExtraData(&curx, &cury, mouseX, mouseY, {&clicked}, &toReturn);

	//now we'll do the Interpolations stuff
	bool clickedInterpol = drawInterpolationSidebar(px+5*sx/8,py,&clicked,mouseX,mouseY,sliderConcerned);
	if (clickedInterpol) {
		clicked.unclick();
		toReturn = 0x01;
	}

	//The close button
	drawBorderedRect(px+sx-20, py, 20, 20, 0xffff0000, 0xff000000);
	drawText("x", 20, px+sx-20+5, py-3, 0xff000000);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px+sx-20, px+sx, py, py+20)) {
		toReturn = 0x02;
		clicked.unclick();
		thingForInString = NULL;
		instringswitch = -1;
	}

	if (clicked.status()&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
		toReturn = 0x01;
	}

	if (clickedEdit) {
		toReturn=0x01;
	}
	
    return toReturn;
}
