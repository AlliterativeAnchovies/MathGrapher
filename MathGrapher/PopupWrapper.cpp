//
//  PopupWrapper.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "PopupWrapper.hpp"

void RawImage::handleExtraData(int* curx_,int* cury_,int mouseX,int mouseY,std::vector<MouseClick*> clicked_,
		Uint8* toReturn_) {
	int curx = *curx_;
	int cury = *cury_;
	MouseClick clicked = *(clicked_[0]);
	Uint8 toReturn = *toReturn_;

	//change the image
	int whichimgw,whichimgh;
	TTF_SizeUTF8((*fontgrab)(16),getOrigName().c_str(),&whichimgw,&whichimgh);
	drawTextWithBackground(getOrigName(), 16, curx, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, curx, curx+whichimgw, cury, cury+whichimgh)) {
		clicked.unclick();
		toReturn = 0x01;
		(new ChooseWhichImagePopup(mouseX, mouseY))
			->concernWith(this);
	}
	curx+=whichimgw+5;

	//rescale ("fix") to normal resolution so that its not weird
	//only useful if you've changed the image
	int fiximgw,fiximgh;
	TTF_SizeUTF8((*fontgrab)(16),"Fix", &fiximgw, &fiximgh);
	drawTextWithBackground("Fix", 16, curx, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, curx, curx+fiximgw, cury, cury+fiximgh)) {
		clicked.unclick();
		toReturn = 0x01;
		fix();
	}
	curx = px+5;
}

void Slider::handleExtraData(int* curx_,int* cury_,int mouseX,int mouseY,std::vector<MouseClick*> clicked_,
		Uint8* toReturn_) {
	int curx = *curx_;
	int cury = *cury_;
	MouseClick clicked = *(clicked_[0]);
	Uint8 toReturn = *toReturn_;

	//Select the function that will determine the spacing of the ticks
	int w9,h9;
	drawTextWithBackground("Tick Function: "+getFunction()->getName(), 20, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(20), ("Tick Function: "+getFunction()->getName()).c_str(), &w9, &h9);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px+5, px+5+w9, cury, cury+h9)) {
		(new ChooseFunctionPopup(mouseX, mouseY))
			->concernWith(this);
		clicked.unclick();
		toReturn = 0x01;
	}
	cury += h9;
	//Select a point to hook the slider up to
	int w10,h10;
	std::string pointConcernedString = "Point Concerned: None";
	if (getPointConcerned()!=NULL) {
		pointConcernedString = "Point Concerned: "+getPointConcerned()->getDisplayLocation();
	}
	drawTextWithBackground(pointConcernedString, 16, px+5, cury, 0xff000000, 0xffffcf9e, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(16), (pointConcernedString).c_str(), &w10, &h10);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px+5, px+5+w10, cury, cury+h10)) {
		(new ChoosePointConcernedForLinkingPopup(mouseX, mouseY))
			->concernWith(this);
		clicked.unclick();
		toReturn = 0x01;
	}
}

void Graph::handleExtraData(int* curx_,int* cury_,int mouseX,int mouseY,std::vector<MouseClick*> clicked_,
		Uint8* toReturn_) {
	int curx = *curx_;
	int cury = *cury_;
	MouseClick clicked = *(clicked_[0]);
	Uint8 toReturn = *toReturn_;
	
	int showgridy = cury;
	int gx,gy;
	drawTextWithBackground((showingGrid())?"Showing Grid":"Not Showing Grid", 20, px+10, showgridy, 0xff000000, (showingGrid())?0xffffcf9e:0xffbd854d, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(20), (showingGrid())?"Showing Grid":"Not Showing Grid", &gx, &gy);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px+10, px+10+gx, showgridy, showgridy+gy)) {
		clicked.unclick();
		toReturn = 0x01;
		toggleGrid();
	}
	int ax,ay;
	drawTextWithBackground((showingAxes())?"Showing Axes":"Not Showing Axes", 20, px+10+gx+10, showgridy, 0xff000000, (showingAxes())?0xffffcf9e:0xffbd854d, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(20), (showingAxes())?"Showing Grid":"Not Showing Grid", &ax, &ay);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px+10+gx+10, px+10+gx+10+ax, showgridy, showgridy+ay)) {
		clicked.unclick();
		toReturn = 0x01;
		toggleAxes();
	}


	int x_functionsy = showgridy+35;
	drawText("X Functions:", 24, px+5, x_functionsy, 0xff000000);
	auto xfunctionlist = getXFunctions();
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
		if (clicked.status()&&pointInBounds(mouseX, mouseY, px+15+tx, px+15+tx+ex, x_functionsy, x_functionsy+ey)) {
			clicked.unclick();
			toReturn = 0x01;
			(new EditFunctionPopup(mouseX, mouseY-200))
				->concernWith(this)
				->concernWith(xfunctionlist[i]);
		}
		//draw remove button
		drawTextWithBackground("Remove", 16, px+15+tx+ex+10, x_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
		int rx,ry;
		TTF_SizeUTF8((*fontgrab)(16),"Remove",&rx,&ry);
		if (clicked.status()&&pointInBounds(mouseX, mouseY, px+15+tx+ex+10, px+15+tx+rx+ex+10, x_functionsy, x_functionsy+ry)) {
			clicked.unclick();
			toReturn = 0x01;
			xfunctionlist[i]->tag();
		}
		x_functionsy+=25;
	}
	drawTextWithBackground("Add Function", 16, px+10, x_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
	int funcsx,funcsy;
	TTF_SizeUTF8((*fontgrab)(20),"Add Function",&funcsx,&funcsy);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px+10, px+10+funcsx, x_functionsy, x_functionsy+funcsy)) {
		(new ChooseFunctionPopup(mouseX, mouseY-200))
			->concernWith(this)
			->concernWith(X_AXIS);
	}

	int y_functionsy = x_functionsy+funcsy+5;
	drawText("Y Functions:", 24, px+5, y_functionsy, 0xff000000);
	auto yfunctionlist = getYFunctions();
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
		if (clicked.status()&&pointInBounds(mouseX, mouseY, px+15+tx, px+15+tx+ex, y_functionsy, y_functionsy+ey)) {
			clicked.unclick();
			toReturn = 0x01;
			(new EditFunctionPopup(mouseX, mouseY-200))
				->concernWith(this)
				->concernWith(yfunctionlist[i]);
		}
		//draw remove button
		drawTextWithBackground("Remove", 16, px+15+tx+ex+10, y_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
		int rx,ry;
		TTF_SizeUTF8((*fontgrab)(16),"Remove",&rx,&ry);
		if (clicked.status()&&pointInBounds(mouseX, mouseY, px+15+tx+ex+10, px+15+tx+rx+ex+10, y_functionsy, y_functionsy+ry)) {
			clicked.unclick();
			toReturn = 0x01;
			yfunctionlist[i]->tag();
		}
		y_functionsy+=25;
	}
	drawTextWithBackground("Add Function", 16, px+10, y_functionsy, 0xff000000, 0xffffcf9e, 0xff000000);
	TTF_SizeUTF8((*fontgrab)(20),"Add Function",&funcsx,&funcsy);
	if (clicked.status()&&pointInBounds(mouseX, mouseY, px+10, px+10+funcsx, y_functionsy, y_functionsy+funcsy)) {
		(new ChooseFunctionPopup(mouseX, mouseY-200))
			->concernWith(this)
			->concernWith(Y_AXIS);
	}
}


