//
//  EditImagePopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "EditImagePopup.hpp"

Uint8 EditImagePopup::handle(double mouseX,double mouseY,bool clicked) {
	Uint8 toReturn = 0x00;
    clicked = clicked&&successfulRaycast&&!locked;
	
	//draw background
	drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
	//initialize variables
	bool clickedEdit = false;
	int curx = px+5;
	int cury = py+5;
	int offy;
	
	RawImage* imageConcerned = (RawImage*)getConcernation<RawImage*>();
	
	//Edit fields
	clickedEdit = handleEditableInfo(curx, cury, imageConcerned, clicked, mouseX, mouseY, &offy);
	cury+=offy;

	//change the image
	int whichimgw,whichimgh;
	TTF_SizeUTF8((*fontgrab)(16),imageConcerned->getOrigName().c_str(),&whichimgw,&whichimgh);
	drawTextWithBackground(imageConcerned->getOrigName(), 16, curx, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, curx, curx+whichimgw, cury, cury+whichimgh)) {
		clicked = false;
		toReturn = 0x01;
		createPopup(CHOOSE_WHICH_IMAGE_POPUP, mouseX, mouseY)
			->concernWith(this);
	}
	curx+=whichimgw+5;

	//rescale ("fix") to normal resolution so that its not weird
	//only useful if you've changed the image
	int fiximgw,fiximgh;
	TTF_SizeUTF8((*fontgrab)(16),"Fix", &fiximgw, &fiximgh);
	drawTextWithBackground("Fix", 16, curx, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, curx, curx+fiximgw, cury, cury+fiximgh)) {
		clicked = false;
		toReturn = 0x01;
		imageConcerned->fix();
	}
	curx = px+5;

	//now we'll do the Interpolations stuff
	bool clickedInterpol = drawInterpolationSidebar(px+5*sx/8,py,clicked,mouseX,mouseY,imageConcerned);
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
