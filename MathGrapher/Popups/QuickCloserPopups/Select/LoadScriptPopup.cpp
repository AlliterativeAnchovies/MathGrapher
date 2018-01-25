//
//  LoadScriptPopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 25/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "LoadScriptPopup.hpp"

Uint8 LoadScriptPopup::handle(double mouseX,double mouseY) {
	Uint8 toReturn = 0x00;
    MouseClick clicked = prepareMouse(&leftMouseReleased);
	
	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	drawText("Choose File", 22, px, py, 0xff000000);
	double cury = py+30;
	for (int i = 0;i<sloadableFiles.size();i++) {
		int w,h;
		auto temp = splitAt(sloadableFiles[i], '/');
		std::string textToShow = temp[temp.size()-1];
		TTF_SizeUTF8((*fontgrab)(16),textToShow.c_str(),&w,&h);
		drawTextWithBackground(textToShow, 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
		if (clicked.status()&&pointInBounds(mouseX, mouseY, px+5, px+5+w, cury, cury+h)) {
			clicked.unclick();
			toReturn = 0x02;
			sload(sloadableFiles[i]);
		}
		cury+=h+5;
		
	}
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
		toReturn = 0x01;
	}
	
    return toReturn;
}
