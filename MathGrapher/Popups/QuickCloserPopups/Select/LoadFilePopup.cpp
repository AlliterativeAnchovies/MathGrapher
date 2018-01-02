//
//  LoadFilePopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "LoadFilePopup.hpp"

Uint8 LoadFilePopup::handle(double mouseX,double mouseY,bool clicked) {
	Uint8 toReturn = 0x00;
    clicked = clicked&&successfulRaycast&&!locked;
	
	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	drawText("Choose File", 22, px, py, 0xff000000);
	double cury = py+30;
	for (int i = 0;i<loadableFiles.size();i++) {
		int w,h;
		auto temp = splitAt(loadableFiles[i], '/');
		std::string textToShow = temp[temp.size()-1];
		TTF_SizeUTF8((*fontgrab)(16),textToShow.c_str(),&w,&h);
		drawTextWithBackground(textToShow, 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
		if (clicked&&pointInBounds(mouseX, mouseY, px+5, px+5+w, cury, cury+h)) {
			clicked = false;
			toReturn = 0x02;
			load(loadableFiles[i]);
		}
		cury+=h+5;
		
	}
	if (clicked&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
		toReturn = 0x01;
	}
	
    return toReturn;
}
