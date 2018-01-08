//
//  CreateFunctionRunInterpolation.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "CreateFunctionRunInterpolation.hpp"

Uint8 CreateFunctionRunInterpolation::handle(double mouseX,double mouseY,bool clicked) {
	Uint8 toReturn = 0x00;
    clicked = clicked&&successfulRaycast&&!locked;
	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	drawText(stringConcerned, 24, px+5, py+5, 0xff000000);
	std::string beep = (ticks%60<30)?"|":" ";
	int curx = px+10;
	int cury = py+35;
	int offx,offy;
	bool clickedEdit = false;
	
	clickedEdit = handleEditableInfo(curx, cury, interpolationConcerned, clicked, mouseX, mouseY, &offy);
	cury+=offy;
	
	/*//Edit field for interpolation delta
	clickedEdit = handleEditableInfo_internal(curx,cury,20,11,mouseX,mouseY,
		"∆X: ",tostring(interpolationConcerned->getPX()),interpolationConcerned->ptmPX()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	
	//Edit field for starting time
	clickedEdit = handleEditableInfo_internal(curx,cury,20,13,mouseX,mouseY,
		"Start: ",tostring(interpolationConcerned->getStart()),interpolationConcerned->ptmStart()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	
	//Edit field for duration
	clickedEdit = handleEditableInfo_internal(curx,cury,20,14,mouseX,mouseY,
		"Duration: ",tostring(interpolationConcerned->getDuration()),interpolationConcerned->ptmDuration()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;*/

	int addx,addy;
	TTF_SizeUTF8((*fontgrab)(16)," Add ",&addx,&addy);
	drawTextWithBackground(" Add ", 16, px+sx-40, py+5, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+sx-40, px+sx-40+addx, py+5, py+5+addy)) {
		clicked = false;
		toReturn = 0x02;
		if (popupConcerned==NULL) {
			interpolationConcerned->reset();
			if (graphConcerned!=NULL) {
				graphConcerned->addInterpolation(interpolationConcerned);
			}
			else if (sliderConcerned!=NULL) {
				sliderConcerned->addInterpolation(interpolationConcerned);
			}
			else if (imageConcerned!=NULL) {
				imageConcerned->addInterpolation(interpolationConcerned);
			}
			else if (textConcerned!=NULL) {
				textConcerned->addInterpolation(interpolationConcerned);
			}
			else {
				throw std::runtime_error("Need to hook up interpolation to display object");
			}
			interpolationConcerned->relateData(functionConcerned);
			//if popup not NULL, then the interpolation already exists,
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
