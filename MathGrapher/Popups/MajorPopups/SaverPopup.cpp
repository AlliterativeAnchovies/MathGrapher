//
//  SaverPopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 02/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "SaverPopup.hpp"

Uint8 SaverPopup::handle(double mouseX,double mouseY,bool clicked) {
	Uint8 toReturn = 0x00;
    clicked = clicked&&successfulRaycast&&!locked;
	
	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	//drawText(stringConcerned, 20, px+5, py+sy/2-10, 0xff000000);
	int offx,offy;
	bool clickedEdit = handleEditableInfo(px+5,py+sy/2-10,20,39,mouseX,mouseY,
		"Save To: ",stringConcerned,&stringConcerned,clicked,&offx,&offy);
	if (clickedEdit) {toReturn = 0x01;clicked=false;}
	TTF_SizeUTF8((*fontgrab)(16), "Save", &offx, &offy);
	drawTextWithBackground("Save", 16, px+sx-offx, py, 0xff000000, 0xff9fc9f2, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+sx-offx, px+sx, py, py+offy)) {
		doSave(stringConcerned);
		toReturn = 0x02;
	}
	TTF_SizeUTF8((*fontgrab)(16), "Cancel", &offx, &offy);
	drawTextWithBackground("Cancel", 16, px+sx-offx, py+sy-offy, 0xff000000, 0xff9fc9f2, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+sx-offx, px+sx, py+sy-offy, py+sy)) {
		toReturn = 0x02;
	}
	
    return toReturn;
}
