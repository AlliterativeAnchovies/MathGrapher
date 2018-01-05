//
//  AddObjectPopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "AddObjectPopup.hpp"

Uint8 AddObjectPopup::handle(double mouseX,double mouseY,bool clicked) {
	Uint8 toReturn = 0x00;
    clicked = clicked&&successfulRaycast&&!locked;

	//main background
	drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
	drawText("Add Object", 24, px+5, py+5, 0xff000000);
	drawTextWithBackground(" Graph ", 20, px+5, py+35, 0xff000000, 0xffffcf9e, 0xff000000);
	int w,h;
	TTF_SizeUTF8((*fontgrab)(20), " Graph ", &w, &h);
	if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+w, py+35, py+35+h)) {
		//add graph!
		addGraph(px, py);
		clicked = false;
		toReturn = 0x02;
	}
	if (clicked&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
		toReturn = 0x01;
	}
	int sliderw,sliderh;
	TTF_SizeUTF8((*fontgrab)(20), " Slider ", &sliderw, &sliderh);
	drawTextWithBackground(" Slider ", 20, px+5+w+5, py+35, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+5+w+5, px+5+w+5+sliderw, py+35, py+35+sliderh)) {
		//add slider!
		addSlider(px, py);
		clicked  = false;
		toReturn = 0x02;
	}
	double cury = py+35+25;
	int imagew,imageh;
	TTF_SizeUTF8((*fontgrab)(20), " Image ", &imagew, &imageh);
	drawTextWithBackground(" Image ", 20, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+imagew, cury, cury+imageh)) {
		//add image!
		createPopup(CHOOSE_WHICH_IMAGE_POPUP, px, py);
		clicked  = false;
		toReturn = 0x02;
	}

	int textw,texth;
	TTF_SizeUTF8((*fontgrab)(20), " Text ", &textw, &texth);
	drawTextWithBackground(" Text ", 20, px+5+imagew+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+5+imagew+5, px+5+textw+imagew+5, cury, cury+texth)) {
		//add text!
		addText(px, py);
		clicked  = false;
		toReturn = 0x02;
	}
	
	cury += 25;
	int arroww,arrowh;
	TTF_SizeUTF8((*fontgrab)(20), " Arrow ", &arroww, &arrowh);
	drawTextWithBackground(" Arrow ", 20, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+arroww, cury, cury+arrowh)) {
		//add arror!
		addArrow(px, py);
		clicked  = false;
		toReturn = 0x02;
	}
	
	return toReturn;
}
