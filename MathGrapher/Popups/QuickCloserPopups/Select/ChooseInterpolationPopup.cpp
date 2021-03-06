//
//  ChooseInterpolationPopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "ChooseInterpolationPopup.hpp"

Uint8 ChooseInterpolationPopup::handle(double mouseX,double mouseY) {
	Uint8 toReturn = 0x00;
    MouseClick clicked = prepareMouse(&leftMouseReleased);
	
	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	drawText("Interpolations", 22, px, py, 0xff000000);
	std::vector<std::string> validInterpols = getFirstDisplayObject()->getValidInterpolations();
	double cury = py+30;
	double curx = px+5;
	for (int i = 0;i<validInterpols.size();i++) {
		int buttonSX,buttonSY;
		TTF_SizeUTF8((*fontgrab)(16), validInterpols[i].c_str(), &buttonSX, &buttonSY);
		if (curx+buttonSX>px+sx-3) {
			//goes too far to the side, create new line of buttons
			curx = px+5;
			cury+=buttonSY+5;
		}
		drawTextWithBackground(validInterpols[i], 16, curx, cury, 0xff000000, 0xffffcf9e, 0xff000000);
		if (clicked.status()&&pointInBounds(mouseX, mouseY, curx, curx+buttonSX, cury, cury+buttonSY)) {
			(new CreateSimpleInterpolation(mouseX, mouseY))
				->concernWithAllDisplayObjects(this)
				->concernWith(validInterpols[i])
				->setUpInterpolation();
		}
		curx+=buttonSX+5;//put next button to the right of previous button
	}
	
    return toReturn;
}
