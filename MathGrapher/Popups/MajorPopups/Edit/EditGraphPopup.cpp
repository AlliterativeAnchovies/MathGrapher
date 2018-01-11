//
//  EditGraphPopup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "EditGraphPopup.hpp"

Uint8 EditGraphPopup::handle(double mouseX,double mouseY) {
	Uint8 toReturn = 0x00;
    bool clicked = clickAllowed(leftMouseReleased);
	drawBorderedRect(px, py, sx, sy, 0xff9fc9f2, 0xff000000);
	
	int offy;
	int curx = px+5;
	int cury = py+5;
	
	Graph* graphConcerned = (Graph*)getConcernation<Graph*>();
	
	//edit fields
	bool clickedEdit = handleEditableInfo(curx, cury, graphConcerned, clicked, mouseX, mouseY, &offy);
	cury+=offy;

	int showgridy = cury;
	int gx,gy;
	drawTextWithBackground((graphConcerned->showingGrid())?"Showing Grid":"Not Showing Grid", 20, px+10, showgridy, 0xff000000, (graphConcerned->showingGrid())?0xffffcf9e:0xffbd854d, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(20), (graphConcerned->showingGrid())?"Showing Grid":"Not Showing Grid", &gx, &gy);
	if (clicked&&pointInBounds(mouseX, mouseY, px+10, px+10+gx, showgridy, showgridy+gy)) {
		clicked = false;
		toReturn = 0x01;
		graphConcerned->toggleGrid();
	}
	int ax,ay;
	drawTextWithBackground((graphConcerned->showingAxes())?"Showing Axes":"Not Showing Axes", 20, px+10+gx+10, showgridy, 0xff000000, (graphConcerned->showingAxes())?0xffffcf9e:0xffbd854d, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(20), (graphConcerned->showingAxes())?"Showing Grid":"Not Showing Grid", &ax, &ay);
	if (clicked&&pointInBounds(mouseX, mouseY, px+10+gx+10, px+10+gx+10+ax, showgridy, showgridy+ay)) {
		clicked = false;
		toReturn = 0x01;
		graphConcerned->toggleAxes();
	}


	int x_functionsy = showgridy+35;
	drawText("X Functions:", 24, px+5, x_functionsy, 0xff000000);
	auto xfunctionlist = graphConcerned->getXFunctions();
	x_functionsy+=30;
	if (xfunctionlist.empty()) {
		drawText("None", 20, px+10, x_functionsy, 0xff000000);
		x_functionsy+=25;
	}
	for (int i = 0;i<xfunctionlist.size();i++) {
		drawText(xfunctionlist[i]->getName(), 20, px+10, x_functionsy, 0xff000000);
		int tx,ty;
		TTF_SizeUTF8((*fontgrab)(20),xfunctionlist[i]->getName().c_str(),&tx,&ty);
		//draw edit button
		drawTextWithBackground("Edit", 16, px+15+tx, x_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
		int ex,ey;
		TTF_SizeUTF8((*fontgrab)(16),"Edit",&ex,&ey);
		if (clicked&&pointInBounds(mouseX, mouseY, px+15+tx, px+15+tx+ex, x_functionsy, x_functionsy+ey)) {
			clicked = false;
			toReturn = 0x01;
			createPopup(EDIT_FUNCTION_POPUP, mouseX, mouseY-200)
				->concernWith(graphConcerned)
				->concernWith(xfunctionlist[i]);
		}
		//draw remove button
		drawTextWithBackground("Remove", 16, px+15+tx+ex+10, x_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
		int rx,ry;
		TTF_SizeUTF8((*fontgrab)(16),"Remove",&rx,&ry);
		if (clicked&&pointInBounds(mouseX, mouseY, px+15+tx+ex+10, px+15+tx+rx+ex+10, x_functionsy, x_functionsy+ry)) {
			clicked = false;
			toReturn = 0x01;
			xfunctionlist[i]->tag();
		}
		x_functionsy+=25;
	}
	drawTextWithBackground("Add Function", 16, px+10, x_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
	int funcsx,funcsy;
	TTF_SizeUTF8((*fontgrab)(20),"Add Function",&funcsx,&funcsy);
	if (clicked&&pointInBounds(mouseX, mouseY, px+10, px+10+funcsx, x_functionsy, x_functionsy+funcsy)) {
		createPopup(CHOOSE_FUNCTION_POPUP, mouseX, mouseY-200)
			->concernWith(graphConcerned)
			->concernWith(X_AXIS);
	}

	int y_functionsy = x_functionsy+funcsy+5;
	drawText("Y Functions:", 24, px+5, y_functionsy, 0xff000000);
	auto yfunctionlist = graphConcerned->getYFunctions();
	y_functionsy+=30;
	if (yfunctionlist.empty()) {
		drawText("None", 20, px+10, y_functionsy, 0xff000000);
		y_functionsy+=25;
	}
	for (int i = 0;i<yfunctionlist.size();i++) {
		drawText(yfunctionlist[i]->getName(), 20, px+10, y_functionsy, 0xff000000);
		int tx,ty;
		TTF_SizeUTF8((*fontgrab)(20),yfunctionlist[i]->getName().c_str(),&tx,&ty);
		//draw edit button
		drawTextWithBackground("Edit", 16, px+15+tx, y_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
		int ex,ey;
		TTF_SizeUTF8((*fontgrab)(16),"Edit",&ex,&ey);
		if (clicked&&pointInBounds(mouseX, mouseY, px+15+tx, px+15+tx+ex, y_functionsy, y_functionsy+ey)) {
			clicked = false;
			toReturn = 0x01;
			createPopup(EDIT_FUNCTION_POPUP, mouseX, mouseY-200)
				->concernWith(graphConcerned)
				->concernWith(yfunctionlist[i]);
		}
		//draw remove button
		drawTextWithBackground("Remove", 16, px+15+tx+ex+10, y_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
		int rx,ry;
		TTF_SizeUTF8((*fontgrab)(16),"Remove",&rx,&ry);
		if (clicked&&pointInBounds(mouseX, mouseY, px+15+tx+ex+10, px+15+tx+rx+ex+10, y_functionsy, y_functionsy+ry)) {
			clicked = false;
			toReturn = 0x01;
			yfunctionlist[i]->tag();
		}
		y_functionsy+=25;
	}
	drawTextWithBackground("Add Function", 16, px+10, y_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(20),"Add Function",&funcsx,&funcsy);
	if (clicked&&pointInBounds(mouseX, mouseY, px+10, px+10+funcsx, y_functionsy, y_functionsy+funcsy)) {
		createPopup(CHOOSE_FUNCTION_POPUP, mouseX, mouseY-200)
			->concernWith(graphConcerned)
			->concernWith(Y_AXIS);
	}


	//now we'll do the Interpolations stuff
	bool clickedInterpol = drawInterpolationSidebar(px+5*sx/8,py,clicked,mouseX,mouseY,graphConcerned);
	if (clickedInterpol) {
		clicked = false;
		toReturn = 0x01;
	}

	drawBorderedRect(px+sx-20, py, 20, 20, 0xffff0000, 0xff000000);
	drawText("x", 20, px+sx-20+5, py-3, 0xff000000);
	if (clicked&&pointInBounds(mouseX, mouseY, px+sx-20, px+sx, py, py+20)) {
		toReturn = 0x02;
		clicked = false;
		thingForInString = NULL;
		instringswitch = -1;
	}

	if (clickedEdit) {
		toReturn = 0x01;
	}

	if (clicked&&pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy)) {
		toReturn = 0x01;
	}
	
	return toReturn;
}
