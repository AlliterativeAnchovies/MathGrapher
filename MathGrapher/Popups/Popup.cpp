//
//  Popup.cpp
//  MathGrapher
//
//  Created by Bailey Andrew on 14/12/2017.
//  Copyright © 2017 Alliterative Anchovies. All rights reserved.
//

#include "Popup.hpp"

//returns 0 if nothing happened,
//return  2 if should delete,
//return  1 if clicked in

void Popup::tag() {
    taggedForDeletion = true;
}

bool Popup::isTagged() {
    return taggedForDeletion;
}

Uint8 Popup::getID() {
    return popupID;
}

bool Popup::inBounds(double mouseX,double mouseY) {
    return pointInBounds(mouseX, mouseY, px, px+sx, py, py+sy);//||isMajor(popupID);
}

Popup* Popup::concernWith(Graph* g) {
    graphConcerned = g;
    return this;
}

Popup* Popup::concernWith(bool b) {
    boolConcerned = b;
    return this;
}

Popup* Popup::concernWith(std::string s) {
    stringConcerned = s;
    return this;
}

Popup* Popup::concernWith(Interpolation* i) {
    interpolationConcerned = i;
    return this;
}

Popup* Popup::concernWith(Popup* p) {
    popupConcerned = p;
    return this;
}

Popup* Popup::concernWith(Function* f) {
    functionConcerned = f;
    return this;
}


void Popup::setUpInterpolation() {
    //called when concerning with a string
    Uint8 interpolID = 0x00;
    if (stringConcerned=="Move") {
        interpolID = SMOOTH_TRANSLATE;
    }
    else if (stringConcerned=="Resize") {
        interpolID = SMOOTH_GRID_RESIZE_SMART_CENTER;
    }
    else if (stringConcerned=="Rescale") {
        interpolID = SMOOTH_GRID_SCALE;
    }
    else if (stringConcerned=="Rotate") {
        interpolID = SMOOTH_GRID_ROTATE;
    }
    else if (stringConcerned=="Re-Origin") {
        interpolID = SMOOTH_ORIGIN_TRANSLATE;
    }
    else if (stringConcerned=="Stretch") {
        interpolID = SMOOTH_FUNCTION_STRETCH;
    }
    else if (stringConcerned=="Run") {
        interpolID = SMOOTH_FUNCTION_RUN;
    }
    else if (stringConcerned=="Highlight") {
        interpolID = HIGHLIGHT_GRAPH;
    }
    else if (stringConcerned=="Fix") {
    	interpolID = FIX_THINGAMAJIG;
	}
	else if (stringConcerned=="Rescale Head") {
    	interpolID = RESCALE_HEAD;
	}
    else {
        throw std::runtime_error("Invalid Interpolation To Set Up");
    }
    if (graphConcerned!=NULL) {
    	interpolationConcerned = new Interpolation(interpolID,0,0,FRAME_RATE,graphConcerned);
    }
    else if (sliderConcerned!=NULL) {
		interpolationConcerned = new Interpolation(interpolID,0,0,FRAME_RATE,sliderConcerned);
	}
	else if (imageConcerned!=NULL) {
		interpolationConcerned = new Interpolation(interpolID,0,0,FRAME_RATE,imageConcerned);
	}
	else if (textConcerned!=NULL) {
		interpolationConcerned = new Interpolation(interpolID,0,0,FRAME_RATE,textConcerned);
	}
	else if (arrowConcerned!=NULL) {
		interpolationConcerned = new Interpolation(interpolID,0,0,FRAME_RATE,arrowConcerned);
	}
	else {
		throw std::runtime_error("Not set up adding interpolations to this type of display object!");
	}
}

void Popup::lock() {
    locked = true;
}

void Popup::unlock() {
    locked = false;
}

bool isStringTypeOfValueEditor(int instrswch) {
    switch (instrswch) {
        case 0:
        case 18:
        case 21:
        case 28:
        case 33:
        case 36:
        case 39:
        case 40:
            return true;
    }
    return false;
}

bool isIntTypeOfValueEditor(int instrswch) {
    switch (instrswch) {
        case 13:
        case 14:
        case 27:
        case 37:
            return true;
    }
    return false;
}

bool isDoubleTypeOfValueEditor(int instrswch) {
    switch (instrswch) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 15:
        case 16:
        case 17:
        case 19:
        case 20:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 29:
        case 30:
        case 31:
        case 32:
        case 34:
        case 35:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
            return true;
    }
    return false;
}

bool isHexadecimalTypeOfValueEditor(int instrswch) {
	switch (instrswch) {
		case 38:
		case 48:
			return true;
	}
	return false;
}

void deleteInStrings() {
    if (isStringTypeOfValueEditor(instringswitch)) {
        delete (ValueEditor<std::string>*)thingForInString;
    }
    else if (isDoubleTypeOfValueEditor(instringswitch)) {
        delete (ValueEditor<double>*)thingForInString;
    }
    else if (isIntTypeOfValueEditor(instringswitch)) {
        delete (ValueEditor<int>*)thingForInString;
    }
    else if (isHexadecimalTypeOfValueEditor(instringswitch)) {
    	delete (ValueEditor<Uint32>*)thingForInString;
	}
    else {
        throw std::runtime_error("Error!  Don't know type of thingForInString!");
    }
    thingForInString=NULL;
    instring="";
    instringswitch=-1;
}

Popup::~Popup() {
    deleteInStrings();
}




