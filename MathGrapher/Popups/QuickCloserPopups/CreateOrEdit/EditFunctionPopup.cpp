//
//  EditFunctionPopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "EditFunctionPopup.hpp"

Uint8 EditFunctionPopup::handle(double mouseX,double mouseY) {
	Uint8 toReturn = 0x00;
    MouseClick clicked = prepareMouse(&leftMouseReleased);
	
	Function* functionConcerned = (Function*)getConcernation<Function*>();
	Graph* graphConcerned = (Graph*)getConcernation<Graph*>();
	
	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	drawText(functionConcerned->getName(), 24, px+5, py+5, 0xff000000);
	int curx = px+10;
	int cury = py+35;
	int offy;
	bool clickedEdit = false;
	
	clickedEdit = handleEditableInfo(curx, cury, functionConcerned, &clicked, mouseX, mouseY, &offy);
	cury+=offy;
	//draw visibility
	int visx,visy;
	drawTextWithBackground((functionConcerned->isVisible())?"Is Visible":"Is Hidden", 16, curx+5, cury, 0xff000000, (functionConcerned->isVisible())?0xffffcf9e:0xffbd854d, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(16),((functionConcerned->isVisible())?"Is Visible":"Is Hidden"),&visx,&visy);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, curx+5, curx+5+visx, cury, cury+visy)) {
		functionConcerned->toggleVisibility();
		clicked.unclick();
		toReturn = 0x01;
	}
	cury+=visy;


	//draw add interpolations
	int intpx,intpy;
	drawTextWithBackground("Add Interpolation", 16, curx, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(16),"Add Interpolation",&intpx,&intpy);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, curx, curx+intpx, cury, cury+intpy)) {
		clicked.unclick();
		toReturn = 0x01;
		(new ChooseFunctionInterpolation(mouseX, mouseY))
			->concernWith(graphConcerned)
			->concernWith(functionConcerned);
	}

	//draw add points of interest
	int poix,poiy;
	drawTextWithBackground("Add Point of Interest", 16, curx+intpx+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(16),"Add Point of Interest",&poix,&poiy);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, curx+intpx+5, curx+intpx+5+poix, cury, cury+poiy)) {
		clicked.unclick();
		toReturn = 0x01;
		PointOfInterest* p = new PointOfInterest();
		p->giveFunction(functionConcerned);
		p->giveGraph(graphConcerned);
		(new CreatePointOfInterest(mouseX, mouseY))
			->concernWith(graphConcerned)
			->concernWith(functionConcerned)
			->concernWith(p);
	}

	if (clickedEdit) {
		toReturn = 0x01;
	}
	
    return toReturn;
}
