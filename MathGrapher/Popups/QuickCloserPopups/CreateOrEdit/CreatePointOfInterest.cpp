//
//  CreatePointOfInterest.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "CreatePointOfInterest.hpp"

Uint8 CreatePointOfInterest::handle(double mouseX,double mouseY) {
	Uint8 toReturn = 0x00;
    MouseClick clicked = prepareMouse(&leftMouseReleased);
	
	drawBorderedRect(px, py, sx, sy, 0xffaaf2aa, 0xff000000);
	drawText("Point of Interest", 24, px, py, 0xff000000);
	std::string header = " px: ";
	Function* functionConcerned = (Function*)getConcernation<Function*>();
	//std::string stringConcerned = (std::string)(*((DerivedData<std::string>*)getConcernation<std::string>()));
	PointOfInterest* pointConcerned = (PointOfInterest*)getConcernation<PointOfInterest*>();
	
	if (functionConcerned->isParametric()) {header = " t: ";}
	//if (stringConcerned==""&&instringswitch!=18) {
	//	header+="0";
	//}
	bool clickedEdit = false;
	int curx = px+5;
	int cury = py+30;
	int offy;
	//Edit field for position of point of interest
	clickedEdit = handleEditableInfo(curx, cury, pointConcerned, &clicked, mouseX, mouseY, &offy);
	cury+=offy;

	bool visibility = pointConcerned->isVisible();//(bool)(*((DerivedData<bool>*)getConcernation<bool>()));
	bool showslope = pointConcerned->isShowingSlope();
	Graph* graphConcerned = (Graph*)getConcernation<Graph*>();

	int visiblex,visibley;
	TTF_SizeUTF8((*fontgrab)(16),visibility?"Is Visible":"Is Hidden",&visiblex,&visibley);
	drawTextWithBackground(!visibility?"Is Visible":"Is Hidden", 16, px+5, py+50, 0xff000000, !visibility?0xffffcf9e:0xffbd854d, 0xff000000);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, curx, curx+visiblex, cury, cury+visibley)) {
		clicked.unclick();
		toReturn = 0x01;
		pointConcerned->toggleVisibility();
	}
	cury+=visibley;
	
	int showx,showy;
	TTF_SizeUTF8((*fontgrab)(16),showslope?"Shows Slope":"Does Not Show Slope",&showx,&showy);
	drawTextWithBackground(!showslope?"Shows Slope":"Does Not Show Slope", 16, px+5, cury, 0xff000000, !showslope?0xffffcf9e:0xffbd854d, 0xff000000);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, curx, curx+showx, cury, cury+showy)) {
		clicked.unclick();
		toReturn = 0x01;
		pointConcerned->toggleSlope();
	}
	cury+=showy;

	int addx,addy;
	TTF_SizeUTF8((*fontgrab)(16)," Add ",&addx,&addy);
	drawTextWithBackground(" Add ", 16, px+sx-40, py+5, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px+sx-40, px+sx-40+addx, py+5, py+5+addy)) {
		clicked.unclick();
		toReturn = 0x02;
		//stringConcerned = stringConcerned==""?"0":stringConcerned;
		//PointOfInterest* newpoint = new PointOfInterest(graphConcerned,functionConcerned,numberFromString(stringConcerned),!boolConcerned);
		pointConcerned->giveGraph(graphConcerned);
		pointConcerned->giveFunction(functionConcerned);
		pointsOfInterest.push_back(pointConcerned);
		functionConcerned->addPoint(pointConcerned);
		instring = "";
		thingForInString = NULL;
		instringswitch = -1;
	}

	if (clickedEdit) {
		toReturn = 0x01;
	}
	
	
    return toReturn;
}
