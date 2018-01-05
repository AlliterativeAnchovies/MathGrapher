//
//  EditArrowPopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 05/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "EditArrowPopup.hpp"

Uint8 EditArrowPopup::handle(double mouseX,double mouseY,bool clicked) {
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
	clickedEdit = handleEditableInfo(curx,cury,24,40,mouseX,mouseY,
		"",tostring(arrowConcerned->getName()),arrowConcerned->ptmName()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	//edit field for graph position (X)
	clickedEdit = handleEditableInfo(curx,cury,20,41,mouseX,mouseY,
		"PX: ",tostring(arrowConcerned->getPX()),arrowConcerned->ptmPX(),clicked,&offx,&offy)
		|| clickedEdit;
	//edit field for graph position (Y)
	clickedEdit = handleEditableInfo(curx+offx,cury,20,42,mouseX,mouseY,
		"PY: ",tostring(arrowConcerned->getPY()),arrowConcerned->ptmPY(),clicked,&offx,&offy)
		|| clickedEdit;
	cury+=offy;
	//edit field for length
	clickedEdit = handleEditableInfo(curx,cury,20,43,mouseX,mouseY,
		"Length: ",tostring(arrowConcerned->getLength()),arrowConcerned->ptmLength(),clicked,&offx,&offy)
		|| clickedEdit;
	//edit field for thickness
	clickedEdit = handleEditableInfo(curx+offx,cury,20,44,mouseX,mouseY,
		"Thickness: ",tostring(arrowConcerned->getThickness()),arrowConcerned->ptmThickness(),clicked,&offx,&offy)
		|| clickedEdit;
	cury+=offy;
	//edit field for head size
	clickedEdit = handleEditableInfo(curx,cury,20,45,mouseX,mouseY,
		"Head Size: ",tostring(arrowConcerned->getHeadSize()),arrowConcerned->ptmHeadSize(),clicked,&offx,&offy)
		|| clickedEdit;
	cury+=offy;
	//edit field for angle
	clickedEdit = handleEditableInfo(curx,cury,20,46,mouseX,mouseY,
		"Angle: ",tostring(arrowConcerned->getAngle()),arrowConcerned->ptmAngle(),clicked,&offx,&offy)
		|| clickedEdit;
	//edit field for head angle
	clickedEdit = handleEditableInfo(curx+offx,cury,20,47,mouseX,mouseY,
		"Head Angle: ",tostring(arrowConcerned->getHeadAngle()),arrowConcerned->ptmHeadAngle(),clicked,&offx,&offy)
		|| clickedEdit;
	cury+=offy;
	//edit field for color
	clickedEdit = handleEditableInfo(curx,cury,20,48,mouseX,mouseY,
		"Head Size: ",tostring(arrowConcerned->getColor()),arrowConcerned->ptmColor(),clicked,&offx,&offy)
		|| clickedEdit;
	cury+=offy;
	
	//now we'll do the Interpolations stuff
	bool clickedInterpol = drawInterpolationSidebar(px+5*sx/8,py,clicked,mouseX,mouseY,arrowConcerned);
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
