//
//  CreateHighlightInterpolation.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "CreateHighlightInterpolation.hpp"

Uint8 CreateHighlightInterpolation::handle(double mouseX,double mouseY,bool clicked) {
	Uint8 toReturn = 0x00;
    clicked = clicked&&successfulRaycast&&!locked;
	
	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	drawText(stringConcerned, 24, px+5, py+5, 0xff000000);
	bool clickedEdit = false;
	int curx = px+5;
	int cury = py+30;
	int offx,offy;
	//Edit field for top left position (X)
	clickedEdit = handleEditableInfo(curx,cury,20,11,mouseX,mouseY,
		"Top Left X: ",tostring(interpolationConcerned->getPX()),interpolationConcerned->ptmPX()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	//Edit field for top left position (Y)
	clickedEdit = handleEditableInfo(curx,cury,20,12,mouseX,mouseY,
		"Top Left Y: ",tostring(interpolationConcerned->getPY()),interpolationConcerned->ptmPY()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	//Edit field for bottom right position (X)
	clickedEdit = handleEditableInfo(curx,cury,20,19,mouseX,mouseY,
		"Bottom Right X: ",tostring(interpolationConcerned->getSX()),interpolationConcerned->ptmSX()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	//Edit field for bottom right position (Y)
	clickedEdit = handleEditableInfo(curx,cury,20,20,mouseX,mouseY,
		"Bottom Right Y: ",tostring(interpolationConcerned->getSY()),interpolationConcerned->ptmSY()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	//Edit field for starting time
	clickedEdit = handleEditableInfo(curx,cury,20,13,mouseX,mouseY,
		"Start Time: ",tostring(interpolationConcerned->getStart()),interpolationConcerned->ptmStart()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	//Eddit field for duration
	clickedEdit = handleEditableInfo(curx,cury,20,14,mouseX,mouseY,
		"Duration: ",tostring(interpolationConcerned->getDuration()),interpolationConcerned->ptmDuration()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;

	//Add button
	int addx,addy;
	TTF_SizeUTF8((*fontgrab)(16)," Add ",&addx,&addy);
	drawTextWithBackground(" Add ", 16, px+sx-40, py+5, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+sx-40, px+sx-40+addx, py+5, py+5+addy)) {
		clicked = false;
		toReturn = 0x02;
		if (popupConcerned==NULL) {
			interpolationConcerned->reset();
			graphConcerned->addInterpolation(interpolationConcerned);
			interpolationConcerned->relateFunction(functionConcerned);
			//if not NULL, then the interpolation already exists,
			//shouldn't add it twice!
		}
		else {
			popupConcerned->unlock();
		}
		instring = "";
		thingForInString = NULL;
		instringswitch = -1;
	}

	if (clickedEdit) {
		toReturn = 0x01;
	}
	
    return toReturn;
}
