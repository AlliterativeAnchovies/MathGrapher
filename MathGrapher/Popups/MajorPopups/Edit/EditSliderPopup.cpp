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
    bool clicked = clickAllowed(leftMouseReleased);
	
	drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
	bool clickedEdit = false;
	int curx = px+5;
	int cury = py+5;
	int offy;
	
	Slider* sliderConcerned = (Slider*)getConcernation<Slider*>();
	
	//Edit fields
	clickedEdit = handleEditableInfo(curx, cury, sliderConcerned, clicked, mouseX, mouseY, &offy);
	cury+=offy;
	
	//Select the function that will determine the spacing of the ticks
	int w9,h9;
	drawTextWithBackground("Tick Function: "+sliderConcerned->getFunction()->getName(), 20, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(20), ("Tick Function: "+sliderConcerned->getFunction()->getName()).c_str(), &w9, &h9);
	if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+w9, cury, cury+h9)) {
		createPopup(CHOOSE_FUNCTION_POPUP, mouseX, mouseY)
			->concernWith(sliderConcerned);
		clicked = false;
		toReturn = 0x01;
	}
	cury += h9;
	//Select a point to hook the slider up to
	int w10,h10;
	std::string pointConcernedString = "Point Concerned: None";
	if (sliderConcerned->getPointConcerned()!=NULL) {
		pointConcernedString = "Point Concerned: "+sliderConcerned->getPointConcerned()->getDisplayLocation();
	}
	drawTextWithBackground(pointConcernedString, 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(16), (pointConcernedString).c_str(), &w10, &h10);
	if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+w10, cury, cury+h10)) {
		createPopup(CHOOSE_POINT_CONCERNED_FOR_LINKING_POPUP, mouseX, mouseY)
			->concernWith(sliderConcerned);
		clicked = false;
		toReturn = 0x01;
	}

	//now we'll do the Interpolations stuff
	bool clickedInterpol = drawInterpolationSidebar(px+5*sx/8,py,clicked,mouseX,mouseY,sliderConcerned);
	if (clickedInterpol) {
		clicked = false;
		toReturn = 0x01;
	}

	//The close button
	drawBorderedRect(px+sx-20, py, 20, 20, 0xffff0000, 0xff000000);
	drawText("x", 20, px+sx-20+5, py-3, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+sx-20, px+sx, py, py+20)) {
		toReturn = 0x02;
		clicked = false;
		thingForInString = NULL;
		instringswitch = -1;
	}

	if (clicked&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
		toReturn = 0x01;
	}

	if (clickedEdit) {
		toReturn=0x01;
	}
	
    return toReturn;
}
