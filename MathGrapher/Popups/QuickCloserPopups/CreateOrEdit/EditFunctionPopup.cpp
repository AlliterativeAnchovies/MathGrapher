//
//  EditFunctionPopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "EditFunctionPopup.hpp"

Uint8 EditFunctionPopup::handle(double mouseX,double mouseY,bool clicked) {
	Uint8 toReturn = 0x00;
    clicked = clicked&&successfulRaycast&&!locked;
	
	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	drawText(functionConcerned->getName(), 24, px+5, py+5, 0xff000000);
	int curx = px+10;
	int cury = py+35;
	int offx,offy;
	bool clickedEdit = false;
	
	clickedEdit = handleEditableInfo(curx, cury, functionConcerned, clicked, mouseX, mouseY, &offy);
	cury+=offy;
	//draw visibility
	int visx,visy;
	drawTextWithBackground((functionConcerned->isVisible())?"Is Visible":"Is Hidden", 16, curx+5, cury, 0xff000000, (functionConcerned->isVisible())?0xffffcf9e:0xffbd854d, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(16),((functionConcerned->isVisible())?"Is Visible":"Is Hidden"),&visx,&visy);
	if (clicked&&pointInBounds(mouseX, mouseY, curx+5, curx+5+visx, cury, cury+visy)) {
		functionConcerned->toggleVisibility();
		clicked = false;
		toReturn = 0x01;
	}
	cury+=visy;
	
	//edit field for starting time.
	/*clickedEdit = handleEditableInfo_internal(curx,cury,20,15,mouseX,mouseY,
		"Start Time: ",tostring(functionConcerned->getTime()),functionConcerned->ptmTime()
		,clicked,&offx,&offy) || clickedEdit;
	curx+=offx;

	//draw visibility
	int visx,visy;
	drawTextWithBackground((functionConcerned->isVisible())?"Is Visible":"Is Hidden", 16, curx+5, cury, 0xff000000, (functionConcerned->isVisible())?0xffffcf9e:0xffbd854d, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(16),((functionConcerned->isVisible())?"Is Visible":"Is Hidden"),&visx,&visy);
	if (clicked&&pointInBounds(mouseX, mouseY, curx+5, curx+5+visx, cury, cury+visy)) {
		functionConcerned->toggleVisibility();
		clicked = false;
		toReturn = 0x01;
	}

	cury+=offy;
	curx = px+10;
	//Edit field for stretch (X)
	clickedEdit = handleEditableInfo_internal(curx,cury,20,16,mouseX,mouseY,
		"X Stretch: ",tostring(functionConcerned->getStretchX()),functionConcerned->ptmStretchX()
		,clicked,&offx,&offy) || clickedEdit;
	curx+=offx;
	//Edit field for stretch (Y)
	clickedEdit = handleEditableInfo_internal(curx,cury,20,17,mouseX,mouseY,
		"Y Stretch: ",tostring(functionConcerned->getStretchY()),functionConcerned->ptmStretchY()
		,clicked,&offx,&offy) || clickedEdit;
	cury+=offy;
	curx = px+10;*/

	//draw add interpolations
	int intpx,intpy;
	drawTextWithBackground("Add Interpolation", 16, curx, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(16),"Add Interpolation",&intpx,&intpy);
	if (clicked&&pointInBounds(mouseX, mouseY, curx, curx+intpx, cury, cury+intpy)) {
		clicked = false;
		toReturn = 0x01;
		createPopup(CHOOSE_FUNCTION_INTERPOLATION, mouseX, mouseY)
			->concernWith(graphConcerned)
			->concernWith(functionConcerned);
	}

	//draw add points of interest
	int poix,poiy;
	drawTextWithBackground("Add Point of Interest", 16, curx+intpx+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(16),"Add Point of Interest",&poix,&poiy);
	if (clicked&&pointInBounds(mouseX, mouseY, curx+intpx+5, curx+intpx+5+poix, cury, cury+poiy)) {
		clicked = false;
		toReturn = 0x01;
		createPopup(CREATE_POINT_OF_INTEREST, mouseX, mouseY)
			->concernWith(graphConcerned)
			->concernWith(functionConcerned);
	}

	if (clickedEdit) {
		toReturn = 0x01;
	}
	
    return toReturn;
}
