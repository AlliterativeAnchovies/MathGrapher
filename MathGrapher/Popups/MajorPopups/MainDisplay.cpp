//
//  MainDisplay.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 01/01/2018.
//  Copyright © 2018 Alliterative Anchovies. All rights reserved.
//

#include "MainDisplay.hpp"

Uint8 MainDisplay::handle(double mouseX,double mouseY) {
	Uint8 toReturn = 0x00;
    bool clicked = clickAllowed(leftMouseReleased);
	
	
	//Fill screen to background
    drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffe8e2);
	for (DisplayObject* d : selectedObjects) {d->highlight();}
	for (int i = 0;i<objects.size();i++) {
		if (clicked&&objects[i]->clickedIn(mouseX,mouseY)) {
			if (!shiftClicked) {
				selectedObjects = {objects[i]};
			}
			else {
				bool isInSelectedObjects = foldr([](bool a,bool b){return a||b;},map([&](DisplayObject* d){return d==objects[i];}, selectedObjects),false);
				if (!isInSelectedObjects) {
					selectedObjects.push_back(objects[i]);
				}
				else {
					decltype(selectedObjects) newselects = {};
					for (DisplayObject* d : selectedObjects) {
						if (d!=objects[i]) {
							newselects.push_back(d);
						}
					}
					selectedObjects = newselects;
				}
			}
			clicked = false;
		}
		std::string specificObject = objects[i]->getID();
		drawDisplayObject(objects[i]);
	}

	//draw control bar
	double controlBarY = RECORDABLE_HEIGHT;//SCREEN_HEIGHT-100;
	drawBorderedRect(0, controlBarY, SCREEN_WIDTH, 100, 0xffffcf9e, 0xff000000);
	if (runningVideo) {
		drawText("PRESS SPACE TO RETURN", 36, 10, controlBarY+10, 0xffff0000);
		drawText("    TO EDIT MODE     ", 36, 10, controlBarY+50, 0xffff0000);
	}
	else {
		double totoff = 10;
		decltype(selectedObjects) newselectedobjects = {};
		for (int i = 0;i<selectedObjects.size();i++) {
			int w,h,w2,h2,w3,h3,w4,h4;
			//draw name of graph
			std::string name = selectedObjects[i]->getName();
			drawText(name, 16, totoff, controlBarY+5, 0xff000000);
			TTF_SizeUTF8((*fontgrab)(16), name.c_str(), &w, &h);
			double newtotoff = totoff+5+w;
			//draw run button
			TTF_SizeUTF8((*fontgrab)(16), "Run", &w2, &h2);
			drawTextWithBackground("Run", 16, totoff, controlBarY+5+h, 0xff000000,0xff9fc9f2,0xff000000);
			if (clicked&&pointInBounds(mouseX, mouseY, totoff, totoff+w2, controlBarY+5+h, controlBarY+5+h+h2)) {
				selectedObjects[i]->run();
				runningVideo = true;
				selectedObjects = {};
				newselectedobjects = {};
				clicked = false;
				break;
			}
			//draw edit button
			TTF_SizeUTF8((*fontgrab)(16), "Edit", &w3, &h3);
			drawTextWithBackground("Edit", 16, totoff, controlBarY+5+h+h2, 0xff000000,0xff9fc9f2,0xff000000);
			if (clicked&&pointInBounds(mouseX, mouseY, totoff, totoff+w3, controlBarY+5+h+h2, controlBarY+5+h+h2+h3)) {
				if (selectedObjects[i]->getID()=="Graph") {
					Popup* blargh = createPopup(EDIT_GRAPH_POPUP, 10, 10);
					blargh->concernWith((Graph*)selectedObjects[i]);
					clicked = false;
				}
				else if (selectedObjects[i]->getID()=="Slider") {
					Popup* blargh = createPopup(EDIT_SLIDER_POPUP, 10, 10);
					blargh->concernWith((Slider*)selectedObjects[i]);
					clicked = false;
				}
				else if (selectedObjects[i]->getID()=="Image") {
					Popup* blargh = createPopup(EDIT_IMAGE_POPUP, 10, 10);
					blargh->concernWith((RawImage*)selectedObjects[i]);
					clicked = false;
				}
				else if (selectedObjects[i]->getID()=="Text") {
					Popup* blargh = createPopup(EDIT_TEXT_POPUP, 10, 10);
					blargh->concernWith((RawText*)selectedObjects[i]);
					clicked = false;
				}
				else if (selectedObjects[i]->getID()=="Arrow") {
					Popup* blargh = createPopup(EDIT_ARROW_POPUP, 10, 10);
					blargh->concernWith((Arrow*)selectedObjects[i]);
					clicked = false;
				}
			}
			//draw delete button
			TTF_SizeUTF8((*fontgrab)(16), "Delete", &w4, &h4);
			drawTextWithBackground("Delete", 16, totoff, controlBarY+5+h+h2+h3, 0xff000000,0xff9fc9f2,0xff000000);
			if (clicked&&pointInBounds(mouseX, mouseY, totoff, totoff+w4, controlBarY+5+h+h2+h3, controlBarY+5+h+h2+h3+h4)) {
				decltype(objects) newobjects = {};
				for (int j = 0;j<objects.size();j++) {
					if (objects[j]!=selectedObjects[i]) {newobjects.push_back(objects[j]);}
				}
				delete selectedObjects[i];
				selectedObjects[i] = NULL;
				objects = newobjects;
				clicked = false;
			}
			else {newselectedobjects.push_back(selectedObjects[i]);}
			totoff=newtotoff;
		}
		selectedObjects=newselectedobjects;
		
		//draw run all button
		int rax,ray;
		TTF_SizeUTF8((*fontgrab)(16), "Run All", &rax, &ray);
		drawTextWithBackground("Run All", 16, SCREEN_WIDTH-200, controlBarY+5, 0xff000000,0xff9fc9f2,0xff000000);
		if (clicked&&pointInBounds(mouseX, mouseY, SCREEN_WIDTH-200, SCREEN_WIDTH-200+rax,controlBarY+5,controlBarY+5+ray)) {
			for (DisplayObject* d : objects) {
				d->run();
			}
			runningVideo = true;
			selectedObjects = {};
			newselectedobjects = {};
			clicked = false;
		}
		//draw run selected button
		int rsx,rsy;
		TTF_SizeUTF8((*fontgrab)(16), "Run Selected", &rsx, &rsy);
		drawTextWithBackground("Run Selected", 16, SCREEN_WIDTH-200-5-rsx, controlBarY+5, 0xff000000,0xff9fc9f2,0xff000000);
		if (clicked&&pointInBounds(mouseX, mouseY, SCREEN_WIDTH-200-5-rsx, SCREEN_WIDTH-200-5-rsx+rsx,controlBarY+5,controlBarY+5+rsy)) {
			for (DisplayObject* d : selectedObjects) {
				d->run();
			}
			runningVideo = true;
			selectedObjects = {};
			newselectedobjects = {};
			clicked = false;
		}
		//draw save button
		int savex,savey;
		TTF_SizeUTF8((*fontgrab)(16), "Save", &savex, &savey);
		drawTextWithBackground("Save", 16, SCREEN_WIDTH-200, controlBarY+5+rsy+5, 0xff000000,0xff9fc9f2,0xff000000);
		if (clicked&&pointInBounds(mouseX, mouseY, SCREEN_WIDTH-200, SCREEN_WIDTH-200+savex,controlBarY+5+rsy+5,controlBarY+5+savey+rsy+5)) {
			//save();
			createPopup(SAVE_FILE_POPUP, mouseX-500, mouseY-100)
				->concernWith(std::string("Generic_Save"));
		}
		//draw load button
		int loadx,loady;
		TTF_SizeUTF8((*fontgrab)(16), "Load", &loadx, &loady);
		drawTextWithBackground("Load", 16, SCREEN_WIDTH-200-5-savex, controlBarY+5+rsy+5, 0xff000000,0xff9fc9f2,0xff000000);
		if (clicked&&pointInBounds(mouseX, mouseY, SCREEN_WIDTH-200-5-savex, SCREEN_WIDTH-200-5-savex+loadx,controlBarY+5+rsy+5,controlBarY+5+savey+rsy+5)) {
			createPopup(LOAD_FILE_POPUP, mouseX, mouseY-200);
		}
		//draw record button
		int recordx,recordy;
		TTF_SizeUTF8((*fontgrab)(16), "Record", &recordx, &recordy);
		drawTextWithBackground("Record", 16, SCREEN_WIDTH-200, controlBarY+5+rsy+5+loady+5, 0xff000000,0xff9fc9f2,0xff000000);
		if (clicked&&pointInBounds(mouseX, mouseY, SCREEN_WIDTH-200, SCREEN_WIDTH-200+savex,controlBarY+5+rsy+5+loady+5,controlBarY+5+savey+rsy+5+loady+5)) {
			std::cout << "Recording started...\n";
			runningVideo = true;
			recordingVideo = true;
			FRAME_NUM = 0;
			for (DisplayObject* d : objects) {
				d->run();
			}
			selectedObjects = {};
			newselectedobjects = {};
			clicked = false;
		}
	}


	//draw points of interest bar
	double interestBarX = RECORDABLE_WIDTH;//SCREEN_WIDTH-150;
	drawBorderedRect(interestBarX, 0, 150, SCREEN_HEIGHT, 0xff597bf5, 0xff000000);
	drawText("Points of Interest", 20, interestBarX+5, 5, 0xff000000);
	if (pointsOfInterest.empty()) {
		drawText("None", 16, interestBarX+5, 30, 0xff000000);
	}
	for (int i = 0;i<pointsOfInterest.size();i++) {
		if (pointsOfInterest[i]->shouldDelete()) {continue;}
		drawText(pointsOfInterest[i]->getDisplayLocation(), 16, interestBarX+5, 30+i*40, 0xff000000);
		drawText("  "+pointsOfInterest[i]->getDisplayPoint(), 12, interestBarX+5, 30+i*40+20, 0xff000000);
	}

	//update graphs
	for (DisplayObject* d : objects) {
		if (d->getID()=="Graph") {
			((Graph*)d)->cleanFunctions();
		}
		d->cleanInterpolations();
		if (d->isRunning()) {d->update();}
	}

	//delete points of interest
	std::vector<PointOfInterest*> newpoi = {};
	for (int i = 0;i<pointsOfInterest.size();i++) {
		if (pointsOfInterest[i]->shouldDelete()) {
			delete pointsOfInterest[i];
		}
		else {
			newpoi.push_back(pointsOfInterest[i]);
		}
	}
	pointsOfInterest=newpoi;
	
	if (clicked&&!runningVideo) {
        if (mouseY<SCREEN_HEIGHT-100&&mouseX<SCREEN_WIDTH-150) {
            createPopup(ADD_OBJECT_POPUP, mouseX, mouseY);
            selectedObjects = {};
        }
    }
	
    return toReturn;
}
