//
//  EditTextPopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "EditTextPopup.hpp"

Uint8 EditTextPopup::handle(double mouseX,double mouseY,bool clicked) {
	Uint8 toReturn = 0x00;
    clicked = clicked&&successfulRaycast&&!locked;
	
	//draw background
	drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
	//initialize variables
	bool clickedEdit = false;
	int curx = px+5;
	int cury = py+5;
	int offx,offy;

	//Edit field for name
	clickedEdit = handleEditableInfo(curx,cury,24,33,mouseX,mouseY,
		"",tostring(textConcerned->getName()),textConcerned->ptmName()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	//Edit field for position (x)
	clickedEdit = handleEditableInfo(curx,cury,20,34,mouseX,mouseY,
		"PX: ",tostring(textConcerned->getPX()),textConcerned->ptmPX()
		,clicked,&offx,&offy) || clickedEdit;
	//Edit field for position (y)
	clickedEdit = handleEditableInfo(curx+offx,cury,20,35,mouseX,mouseY,
		"PY: ",tostring(textConcerned->getPY()),textConcerned->ptmPY()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	//Edit field for actual text
	clickedEdit = handleEditableInfo(curx,cury,20,36,mouseX,mouseY,
		"Display Text: ",tostring(textConcerned->getActualText()),textConcerned->ptmActualText()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	//Edit field for font size
	clickedEdit = handleEditableInfo(curx,cury,20,37,mouseX,mouseY,
		"Font Size: ",tostring(textConcerned->getFontSize()),textConcerned->ptmFontSize()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	//Edit field for color
	clickedEdit = handleEditableInfo(curx,cury,20,38,mouseX,mouseY,
		"Color: 0x",tostring(textConcerned->getColor()),textConcerned->ptmColor()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;

	//now we'll do the Interpolations stuff
	bool clickedInterpol = drawInterpolationSidebar(px+5*sx/8,py,clicked,mouseX,mouseY,textConcerned);
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

	if (clickedEdit) {//Believe it or not, this is not redundant
		//there are cases when the edit could have been drawn off the side of the popup
		//due to it not being able to fit.  While ideally this wouldn't happen, it could
		//happen during intermediate builds.  If this isn't here, then trying to click it
		//will not send the right toReturn code back (it'd send 0x00)
		toReturn = 0x01;
	}
	
    return toReturn;
}
