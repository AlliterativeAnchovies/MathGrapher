//
//  ChooseWhichImagePopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "ChooseWhichImagePopup.hpp"

Uint8 ChooseWhichImagePopup::handle(double mouseX,double mouseY) {
	Uint8 toReturn = 0x00;
    MouseClick clicked = prepareMouse(&leftMouseReleased);
	
	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	drawText("Choose Image", 22, px, py, 0xff000000);
	double cury = py+30;
	for (int i = 0;i<gSurfaces.size();i++) {
		int w,h;
		TTF_SizeUTF8((*fontgrab)(16),gStrings[i].c_str(),&w,&h);
		drawTextWithBackground(gStrings[i], 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
		if (clicked.status()&&pointInBounds(mouseX, mouseY, px+5, px+5+w, cury, cury+h)) {
			clicked.unclick();
			toReturn = 0x02;
			if (getImageConcerned()==NULL) {//creating a new image
				//addImage(px, py,i);
				auto img = addDisplayObject<RawImage>(px,py);
				img->changeTo(i);
				img->fix();
			}
			else {//editing an existing image
				popupConcerned->getImageConcerned()->changeTo(i);
			}
		}
		cury+=h+5;
		
	}
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
		toReturn = 0x01;
	}
	
    return toReturn;
}
